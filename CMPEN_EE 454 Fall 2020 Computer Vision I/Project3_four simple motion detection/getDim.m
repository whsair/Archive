function [height,width] = getDim(picture)
%This function returns the height and width of an image
image = imread(picture); %load image
[height, width, colors]  = size(image); %get size of image for each color
%NOTE: we only care about height and width.
%colors is only there becase images consist of RGB colors
end

