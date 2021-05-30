#!/usr/bin/env python
#

import argparse
import glob
import numpy as np
import os
import time
import cv2

# Stub to warn about opencv version.
if int(cv2.__version__[0]) < 3: # pragma: no cover
  print('Warning: OpenCV 3 is not installed')


class VideoStreamer(object):
  """ Class to help process image streams. Three types of possible inputs:"
    1.) USB Webcam.
    2.) A directory of images (files in directory matching 'img_glob').
    3.) A video file, such as an .mp4 or .avi file.
  """
  def __init__(self, basedir, camid, height, width, skip, img_glob):
    self.cap = []
    self.camera = False
    self.video_file = False
    self.listing = []
    self.sizer = [height, width]
    self.i = 0
    self.skip = skip
    self.maxlen = 1000000
    # If the "basedir" string is the word camera, then use a webcam.
    if basedir == "camera/" or basedir == "camera":
      print('==> Processing Webcam Input.')
      self.cap = cv2.VideoCapture(camid)
      self.listing = range(0, self.maxlen)
      self.camera = True
    else:
      # Try to open as a video.
      self.cap = cv2.VideoCapture(basedir)
      lastbit = basedir[-4:len(basedir)]
      if (type(self.cap) == list or not self.cap.isOpened()) and (lastbit == '.mp4'):
        raise IOError('Cannot open movie file')
      elif type(self.cap) != list and self.cap.isOpened() and (lastbit != '.txt'):
        print('==> Processing Video Input.')
        num_frames = int(self.cap.get(cv2.CAP_PROP_FRAME_COUNT))
        self.listing = range(0, num_frames)
        self.listing = self.listing[::self.skip]
        self.camera = True
        self.video_file = True
        self.maxlen = len(self.listing)
      else:
        print('==> Processing Image Directory Input.')
        search = os.path.join(basedir, img_glob)
        self.listing = glob.glob(search)
        self.listing.sort()
        self.listing = self.listing[::self.skip]
        self.maxlen = len(self.listing)
        if self.maxlen == 0:
          raise IOError('No images were found (maybe bad \'--img_glob\' parameter?)')

  def read_image(self, impath, img_size):
    """ Read image as grayscale and resize to img_size.
    Inputs
      impath: Path to input image.
      img_size: (W, H) tuple specifying resize size.
    Returns
      grayim: float32 numpy array sized H x W with values in range [0, 1].
    """
    grayim = cv2.imread(impath, 0)
    if grayim is None:
      raise Exception('Error reading image %s' % impath)
    # Image is resized via opencv.
    interp = cv2.INTER_AREA
    grayim = cv2.resize(grayim, (img_size[1], img_size[0]), interpolation=interp)
    grayim = (grayim.astype('float32') / 255.)
    return grayim

  def next_frame(self):
    """ Return the next frame, and increment internal counter.
    Returns
       image: Next H x W image.
       status: True or False depending whether image was loaded.
    """
    if self.i == self.maxlen:
      return (None, False)
    if self.camera:
      ret, input_image = self.cap.read()
      if ret is False:
        print('VideoStreamer: Cannot get image from camera (maybe bad --camid?)')
        return (None, False)
      if self.video_file:
        self.cap.set(cv2.CAP_PROP_POS_FRAMES, self.listing[self.i])
      input_image = cv2.resize(input_image, (self.sizer[1], self.sizer[0]),
                               interpolation=cv2.INTER_AREA)
      input_image = cv2.cvtColor(input_image, cv2.COLOR_RGB2GRAY)
      input_image = input_image.astype('float')/255.0
    else:
      image_file = self.listing[self.i]
      input_image = self.read_image(image_file, self.sizer)
    # Increment internal counter.
    self.i = self.i + 1
    input_image = input_image.astype('float32')
    return (input_image, True)


if __name__ == '__main__':

  # Parse command line arguments.
  parser = argparse.ArgumentParser(description='Video Loop Demo')
  parser.add_argument('input', type=str, default='',
      help='Image directory or movie file or "camera" (for webcam).')
  parser.add_argument('--img_glob', type=str, default='*.png',
      help='Glob match if directory of images is specified (default: \'*.png\').')
  parser.add_argument('--skip', type=int, default=1,
      help='Images to skip if input is movie or directory (default: 1).')
  parser.add_argument('--H', type=int, default=480,
      help='Input image height (default: 480).')
  parser.add_argument('--W', type=int, default=640,
      help='Input image width (default:640).')
  parser.add_argument('--threshold', type=float, default=.2,
      help='Intensity threshold (default:.2).')
  parser.add_argument('--alpha', type=float, default=.5,
      help='Intensity threshold (default:.5).')
  parser.add_argument('--showbg', action='store_true',
      help='show bg image (default: False).')
  parser.add_argument('--display_scale', type=int, default=1,
      help='Factor to scale output visualization (default: 1).')
  parser.add_argument('--waitkey', type=int, default=1,
      help='OpenCV waitkey time in ms (default: 1).')
  parser.add_argument('--camid', type=int, default=0,
      help='OpenCV webcam video capture ID, usually 0 or 1 (default: 0).')
  parser.add_argument('--cuda', action='store_true',
      help='Use cuda GPU to speed up network processing speed (default: False)')
  parser.add_argument('--write', action='store_true',
      help='Save output frames to a directory (default: False)')
  parser.add_argument('--write_dir', type=str, default='capturedFrames/',
      help='Directory where to write output frames (default: capturedFrames/).')
  opt = parser.parse_args()
  print(opt)

  # This class helps load input images from different sources.
  vs = VideoStreamer(opt.input, opt.camid, opt.H, opt.W, opt.skip, opt.img_glob)

  # Create a window to display the demo.
  win = 'Lab 9'
  cv2.namedWindow(win)

  # Font parameters for visualizaton.
  font = cv2.FONT_HERSHEY_DUPLEX
  font_clr = (255, 0, 255)
  font_pt = (4, 12)
  font_sc = 0.4

  # Create output directory if desired.
  if opt.write:
    print('==> Will write outputs to %s' % opt.write_dir)
    if not os.path.exists(opt.write_dir):
      os.makedirs(opt.write_dir)

  print('==> Running Demo.')
  firstframe = True
  fps = 0.0
  while True:
    start = time.time()

    # Get a new image.
    img, status = vs.next_frame()
    if status is False:
      break
    if firstframe:
      bgimg = img
      firstframe = False

    # Compute absolute value of differences between current and background images
    # Alpha blend current and background images to become new background image
    ########################################

    old = np.abs(bgimg - img)
    new = (old > opt.threshold)


    bg = (opt.alpha)*img + (1.0 - opt.alpha) * bgimg


    ########################################
    result = new ### Replace this line
    bgimg = bg ### Replace this line


    # Format output images
    out = (np.dstack((img, img, img)) * 255.0).astype('uint8')
    ### Need another line for formatting the output of your result
    cv2.putText(out, '%.2f FPS - Press Q to quit'%fps, font_pt, font, font_sc, font_clr, lineType=16)

    # Resize final output
    out = cv2.resize(out, (opt.display_scale*opt.W, opt.display_scale*opt.H))
    ### Need another line for scaling the output of your result
    
    # Place two images next to each other
    ########################################
    result = (np.dstack((result, result, result)) * 255.0).astype('uint8')
    cv2.putText(result, '%.2f FPS - Press Q to quit' % fps, font_pt, font, font_sc, font_clr, lineType=16)

    result = cv2.resize(result, (opt.display_scale * opt.W, opt.display_scale * opt.H))

    f_out = np.hstack((out, result))


    ########################################
    final_out = f_out ### Replace this, placeholder to ensure starter code runs

    # Display visualization image to screen.
    cv2.imshow(win, final_out)

    # Get and process key presses
    key = cv2.waitKey(opt.waitkey) & 0xFF
    if key == ord('q'):
      print('Quitting, \'q\' pressed.')
      break

    # Compute FPS every so often
    if vs.i % 10 == 0:
      end = time.time()
      fps = (1.0 / float(end - start))

  # Close any remaining windows.
  cv2.destroyAllWindows()

  print('==> Finished Demo.')
