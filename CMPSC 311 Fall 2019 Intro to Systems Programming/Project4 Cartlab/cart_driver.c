////////////////////////////////////////////////////////////////////////////////
//
//  File           : cart_driver.c
//  Description    : This is the implementation of the standardized IO functions
//                   for used to access the CART storage system.
//
//  Author         : Hongshuo Wang
//  PSU email      : hkw5146@psu.edu
//

// Includes
#include <stdlib.h>
#include <string.h>

// Project Includes
#include "cart_driver.h"
#include "cart_controller.h"


//
// Implementation
//


// keep track of the current cartirdge 
CartridgeIndex loaded_cart = -1;

// keep track of the number of file in the system
int file_counter;

// data structure Frame with cart_id and frame_id in order to locate it in the memory
struct Frame {
  CartridgeIndex cart_id;
  CartridgeIndex frame_id;
};


// the global frame that help us to location the next frame (using to allocate new frame)
struct Frame next_frame;

// File handler 
//  files_name: file's name (128 bytes)
//  size: file's size
//  position; file current position (relatively position in file)
//  FrameList: a list that 100 frame in order to help us to track the static location
//             in the system. Each frame object has static cart and frame indexs.
//  frame_counter: keep track of how many frame we have in the file
//  open_status: whether the file is open or not 1 is open 0 is not  
//
struct File {
  char file_names[CART_MAX_PATH_LENGTH];
  int size;
  int position;
  struct Frame FrameList[100];
  int frame_counter; 
  int open_status;
};

// An file array to keep track of all the files
struct File Files[CART_MAX_TOTAL_FILES];

///////////////////////////////////////////////////////////////////////////////
//
//Function    : set_opcode
//Description : helper function in order to set up the opcode 
//
//Inputs      : The opcode's register KY1, KY2, RT1, CT1, FM1
//Outputs     : opcode that combined KY1, KY2, RT1, CT1, FM1 and unused
//
//////////////////////////////////////////////////////////////////////////////
CartXferRegister set_opcode(CartXferRegister KY1, CartXferRegister KY2,
    CartXferRegister RT1, CartXferRegister CT1, CartXferRegister FM1) {

// combine KY1(8), KY2(8), RT1(1), CT1(16), FM1(16) and unused(15) to from the opcode (64 bytes)
  CartXferRegister regstate, ky1, ky2, rt1, ct1, fm1, unused;
  ky1 = KY1 << 56;
  ky2 = KY2 << 48;
  rt1 = RT1 << 47;
  ct1 = CT1 << 31;
  fm1 = FM1 << 15;
  unused = 0;
  regstate = ky1|ky2|rt1|ct1|fm1|unused;
  return regstate;
} 

//////////////////////////////////////////////////////////////////////////////////////////
//
//Function    : get_register
//Description : helper function to get opcode's register after go through the io_bus
//
//Inputs      : returned opcode and RT1 register's pointer
//Outputs     : return 0 if success
//
///////////////////////////////////////////////////////////////////////////////////////////
int get_register(CartXferRegister op, char *RT1) {
   
// CT1 is set at the specific location of opcode
// behind KY1(8) | KY2(8) (see reference picture)
  *RT1 = (char)((op << 16) >> 63);
  return 0;
} 

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//Function    : opcode_handler
//Description : helper function that help us to handle the opcode by sending the specific type
//              of them to the cart_io_bus function, and get the return_register value in order
//              to check whether the command is success or failed
//Inputs      : specific cart opcode type, and index (frame_id or cart_id) and buffer to hold
//              the information plan to read or wrote
//Outputs     : -1 if failed and 0 if success 
//
////////////////////////////////////////////////////////////////////////////////////////////////////
int opcode_handler(CartXferRegister cartOpType, uint16_t index, char *buf) {
  char RT1 = 0;
  CartXferRegister regstate = 0;
  CartXferRegister oregstate = 0;

// depends on each input opcode type we set up correponding register value and send
// it to the set_opcode to get the opcode then we send it to the cart_io_bus
// if it's write and read's we also need to send the buffer; otherwise send null
// (all the information is on the pdf)
  if (cartOpType == CART_OP_INITMS) {
    regstate = set_opcode(CART_OP_INITMS, 0, 0, 0, 0);
    oregstate = cart_io_bus(regstate, NULL);

  } else if (cartOpType == CART_OP_LDCART) {

// the difference between load opcode and the other is we also need to update 
// the current loaded cart index since we load a new cart
// The index here denotes the cart index
// If we see the current cart is hold as same as the input cart, we do not load twice
// just skip it return 0
    if (loaded_cart != index) {
      regstate = set_opcode(CART_OP_LDCART, 0, 0, index, 0);
      oregstate = cart_io_bus(regstate, NULL);
      get_register(oregstate, &RT1);
      if (RT1 == -1) {
        return -1;
      }
      loaded_cart = index;
    }
    return 0;
  } else if (cartOpType == CART_OP_BZERO) {
    regstate = set_opcode(CART_OP_BZERO, 0, 0, 0, 0);
    oregstate = cart_io_bus(regstate, NULL);
  
  } else if (cartOpType == CART_OP_POWOFF) {
    regstate = set_opcode(CART_OP_POWOFF, 0, 0, 0, 0);
    oregstate = cart_io_bus(regstate, NULL);
  
// The index here denotes frame_index since we need to read current frame
  } else if (cartOpType == CART_OP_RDFRME) {
    regstate = set_opcode(CART_OP_RDFRME, 0, 0, 0, index);
    oregstate = cart_io_bus(regstate, buf);

// The index here denotes frame_index since we need to write bytes
// in the current frame
  } else if (cartOpType == CART_OP_WRFRME) {
    regstate = set_opcode(CART_OP_WRFRME, 0, 0, 0, index);
    oregstate = cart_io_bus(regstate, buf);
     
  } else {

// undeclared opcode type should return -1
    return -1;
  }

// after we update all command to the cart_io_bus, we need to check whether
// the command is success or failed. To see that, we use get register to see
// the RT1 value. -1 if failed and 0 is success
  get_register(oregstate, &RT1);
  if (RT1 == -1) {
      return -1;
  }

// if the handler is correct return 0
  return 0;

}

//////////////////////////////////////////////////////////////////////////////////
//
//Function    : init_data_structure
//Description : init or clear up all the internal data in the function
//
//Inputs      : none
//output      : none
//
///////////////////////////////////////////////////////////////////////////////////
void init_data_structure() {


// we need to set up all the internal data(in Files object) to zero all one
// and the tracking data which is load_cart, next_frame's id to be 0
  int i;   
  for (i = 0; i < CART_MAX_TOTAL_FILES; i++) {
    Files[i].file_names[0] = '\0';
    Files[i].size = 0;
    Files[i].position = 0;
    Files[i].open_status = 0;
    Files[i].frame_counter = -1; 
  }
  loaded_cart = 0;
  next_frame.cart_id = 0;
  next_frame.frame_id = 0;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function     : cart_poweron
// Description  : Startup up the CART interface, initialize filesystem
//
// Inputs       : none
// Outputs      : 0 if successful, -1 if failure

int32_t cart_poweron(void) {

// First, we need to send the init command to the controller
  CartXferRegister i = 0;
  if (opcode_handler(CART_OP_INITMS, 0, NULL) == -1) {
    return -1;
  }

  
// Next, we need t set up all cart's memony to 0
// By using load command to locate cart_id then use bzero command to zero the
// current cart
  for (i = 0; i < CART_MAX_CARTRIDGES; i++) {
    if (opcode_handler(CART_OP_LDCART, i, NULL) == -1) {
      return -1;
    }
    if (opcode_handler(CART_OP_BZERO, 0, NULL) == -1) {
      return -1;
    }
  }

// after we init the memory, we need to init the internal data (all file_handler and tracking variables)
  init_data_structure();

// return 0 if success
  return(0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : cart_poweroff
// Description  : Shut down the CART interface, close all files
//
// Inputs       : none
// Outputs      : 0 if successful, -1 if failure

int32_t cart_poweroff(void) {

// use the opcode_hander to send the powoff command
  if (opcode_handler(CART_OP_POWOFF, 0, NULL) == -1) {
    return -1;
  }

// Finally, we need to clear up the internal structure (all files handler and tracking variables)
  init_data_structure();

// return 0 if success
  return(0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : cart_open
// Description  : This function opens the file and returns a file handle
//
// Inputs       : path - filename of the file to open
// Outputs      : file handle if successful, -1 if failure

int16_t cart_open(char *path) {

  int i;
// we need to go through the all the file's created handler to check whether the file is open or not
// if it has already opened before return -1 else return the file handle's index and set open_status 
// to be open(1)
  for (i = 0; i < file_counter; i++) {
    if (strcmp(path, Files[i].file_names) == 0) { 
      if (Files[i].open_status == 1) {
        return -1;
      } else {
        Files[i].open_status = 1;
        Files[i].position = 0;
        return i;
      }
    }
  }


// if we cannot find the files_handler depend on the input path(filename) that means we have to 
// set up a new handler by copying the input path to new files as the file's name and initialize
// all internal data. At last, update the number of file's handler
  strncpy(Files[i+1].file_names, path, strlen(path) + 1);
  Files[i+1].position = 0;
  Files[i+1].size = 0;
  Files[i+1].frame_counter = -1;
  Files[i+1].open_status = 1;
  file_counter++;

  return file_counter;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : cart_close
// Description  : This function closes the file
//
// Inputs       : fd - the file descriptor
// Outputs      : 0 if successful, -1 if failure

int16_t cart_close(int16_t fd) {

// If the the decriptor id is invalid or the filer descriptor has already closed
// return -1 else set the open status to close (0)
  if ((fd < 0 || CART_MAX_TOTAL_FILES <= fd) || (Files[fd].open_status == 0)) {
    return -1;
  }
  
  Files[fd].open_status = 0;

// return 0 if success
  return (0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : cart_read
// Description  : Reads "count" bytes from the file handle "fh" into the 
//                buffer "buf"
//
// Inputs       : fd - filename of the file to read from
//                buf - pointer to buffer to read into
//                count - number of bytes to read
// Outputs      : bytes read if successful, -1 if failure

int32_t cart_read(int16_t fd, void *buf, int32_t count) {

// create the temp frame buffer since we read the bytes frame by frame
  char* frame_buf = malloc(CART_FRAME_SIZE);

// set up the reading length by input count variable
// set up teh remaining rest of bytes we needs to read to be count
// the initial buf's location is always start at 0 index
  int length = count;
  int rest = count;
  int buf_location = 0;

// check the files' name whether is valid and file whether is open
// if closed or the name is valid return -1
  if ((fd < 0 || CART_MAX_TOTAL_FILES <= fd) || (Files[fd].open_status == 0)) {
    free(frame_buf);
    return -1;

// we also need to check whether the files has the enough bytes for us to read
// if not just set reading length and rest bytes we need to read 
// as the rest bytes that the files has
  } else if (count > (Files[fd].size - Files[fd].position)) {
    length = Files[fd].size - Files[fd].position;
    rest = length;
  }
   
// We need to handle when our current position locate in the specific frame instead of the
// beginning of the frame. To check this, we / currentPosition by the frame's size to get
// the relatively frame_index and use % current position by the frame's size to get
// the offset of bytes which is the number of bytes we current at in the speific frame with 
// the frame_index we calculate before. we also initialize a integer variable next_bytes to
// hold the number of bytes we need to read next.
  int currentPosition = Files[fd].position;
  int offset = currentPosition % CART_FRAME_SIZE;
  int frame_index = currentPosition / CART_FRAME_SIZE;
  int next_bytes;

// if there is no offset or the current position is start at the begining of the frame[frame_index]
// we just skip it
  if (offset != 0) {   

// if the offset is not zero, we need to read the offset's amount of bytes first 
// To do this, we first need to send the Load command to locate the cart_id
// then send the read command to read bytes at the index of frame_index    
    if (opcode_handler(CART_OP_LDCART, Files[fd].FrameList[frame_index].cart_id, NULL) == -1) {
      free(frame_buf);
      return -1;
    }
    
    if (opcode_handler(CART_OP_RDFRME, Files[fd].FrameList[frame_index].frame_id, frame_buf) == -1) {
      free(frame_buf);
      return -1;
    }
    
// if the reading length is greater than the rest bytes in the current frame
// then read out the rest bytes of the current frame
// Else the length is even smaller than the rest of bytes in the current frame
// we just read the input count's amount bytes since there is enought bytes in the current frame.
    if (length > (CART_FRAME_SIZE - offset)) {
      next_bytes = CART_FRAME_SIZE - offset;
    } else {
      next_bytes = length;
    }  

// copy the bytes to our output buffer
    memcpy(buf, &frame_buf[offset], next_bytes);
    
// update the buffer location which is the current position in the buffer
// extract the bytes we have read from the remaining bytes we need to read
// update the relatively location in the file 
    buf_location += next_bytes;
    rest -= next_bytes;
    Files[fd].position += next_bytes;

  }

// Now, it much simple, we can start use the whole frame depending on how many remaining bytes we have
  while (rest > 0) {
    frame_index = (Files[fd].position) / CART_FRAME_SIZE;

// Same as before, send the load command and read command to get specific frame read bytes in frame buffer
// since we use malloc, any return will need to happen after we free the memory
    if (opcode_handler(CART_OP_LDCART, Files[fd].FrameList[frame_index].cart_id, NULL) == -1) {
      free(frame_buf);
      return -1;
    }
    
    if (opcode_handler(CART_OP_RDFRME, Files[fd].FrameList[frame_index].frame_id, frame_buf) == -1) {
      free(frame_buf);
      return -1;
    }

// At last, when the rest bytes is not enough for our to read the whole frame, just read the rest of them
// instead read the whole frame each time.
    if (rest < CART_FRAME_SIZE) {
      next_bytes = rest;
    } else {
      next_bytes = CART_FRAME_SIZE;
    }

// Same as perivous, copy the temp frame buf data to the output buf frame then update current
// location and buf location. at last extract the read bytes
    memcpy(buf + buf_location, frame_buf, next_bytes);
    Files[fd].position += next_bytes;
    rest -= next_bytes;
    buf_location += next_bytes;
  }

// free the temp buffer before return
// return 0 if success 
  free(frame_buf);
  return length;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//Function    : allocate_frame
//Description : allocate new frame depending on the constructive cart_id and frame_id
//              update the frame_counter
//
//Inputs      : frame_list_location - frame list's location 
//              fd - filename of the file
//Outputs     : None
//
////////////////////////////////////////////////////////////////////////////////////////
void allocate_frame(int frame_list_location, int16_t fd) {

// we can use next_frame's cart and frame id to allocate the next frame 
// since those are static id, all we need is to update the frame list we defined in
// the file stucture.The frame list contain all the frame's information that belong to
// this files.
  Files[fd].FrameList[frame_list_location].cart_id = next_frame.cart_id;
  Files[fd].FrameList[frame_list_location].frame_id = next_frame.frame_id;

// update cart_id if go through the last frame of the current cart
// else just update the frame id each time
  if (next_frame.frame_id >= CART_CARTRIDGE_SIZE - 1) {
    next_frame.cart_id += 1;
    next_frame.frame_id = 0;
  } else {
    next_frame.frame_id++;
  }

// update the number of frame that current filer handler contains
  Files[fd].frame_counter++;   
}  
////////////////////////////////////////////////////////////////////////////////
//
// Function     : cart_write
// Description  : Writes "count" bytes to the file handle "fh" from the 
//                buffer  "buf"
//
// Inputs       : fd - filename of the file to write to
//                buf - pointer to buffer to write from
//                count - number of bytes to write
// Outputs      : bytes written if successful, -1 if failure

int32_t cart_write(int16_t fd, void *buf, int32_t count) {

// just like in the read function
// set up the rest as the remaining bytes we need to write
// set up buffer location at the beginning of the buffer
// defined frame index to hold the relatively frame index in specific file
// defind next bytes to hold next bytes we need to write
  int rest = count;
  int buf_location = 0;
  int frame_index;
  int next_bytes;

// define a buffer with frame size since we write frame by frame
// bv the way, I will free each buffer before the function return
  char* frame_buf = malloc(CART_FRAME_SIZE);


// check whether the file name is valid or the file is closed or not
  if ((fd < 0 || CART_MAX_TOTAL_FILES <= fd) || (Files[fd].open_status == 0)) {
    free(frame_buf);
    return -1;
  }

// we also need to handle when the bytes we want to write is beyond the frame size
// or smaller then frame's size
  while (rest > 0) {
  
// To check this, we need to look at the offset of the specific frame just like we
// did in the reading
// offset donotes the current relatively position in a specific frame
// frame_index denotes which frame we are in 
// temp_bytes is defined for holding the sum of remaining bytes and the offset bytes
    int currentPosition = Files[fd].position;
    int offset = currentPosition % CART_FRAME_SIZE;
    frame_index = currentPosition / CART_FRAME_SIZE;
    int temp_bytes = rest + offset;
  
// if the current relatively index is larger than the number of frame we have in this file
// we need to allocate new frame to this file handler
    if (frame_index > Files[fd].frame_counter) {
      
      allocate_frame(frame_index, fd);
      
    } else {  

// Else we have enough frame space, we will need to read out the frame first
// To do this, we send the load and read command if success, we will have a frame_buf containing all
// the information in the current frame
        if (opcode_handler(CART_OP_LDCART, Files[fd].FrameList[frame_index].cart_id, NULL) == -1) {
          free(frame_buf);
          return -1;
        }
    
        if (opcode_handler(CART_OP_RDFRME, Files[fd].FrameList[frame_index].frame_id, frame_buf) == -1) {
          free(frame_buf);
          return -1;
        }
    }
     
// Next we need to figure out how many bytes we need to write next
// if we want to write more any one frame bytes in current relatively location
// we just write the rest of bytes of the current frame then next frame we will writing at the beginning of 
// the frame instead of in the frame
// If the remaining bytes we want to write is smaller that frame size that means we have enough space to
// write the bytes in the current frame.
// Else we write frame by frame 
    if (temp_bytes >= CART_FRAME_SIZE) {
      next_bytes = CART_FRAME_SIZE - offset;
    } else if (temp_bytes < CART_FRAME_SIZE) {
      next_bytes = rest;
    } else {
      next_bytes = CART_FRAME_SIZE;
    }
   
     
// After we decided how many bytes we can write next, we need copy the number of those bytes from 
// buffer to the temp frame buf
    memcpy(&frame_buf[offset], buf + buf_location, next_bytes);

// After that, we need to write it back to the memory by sending the load and write command
    if (opcode_handler(CART_OP_LDCART, Files[fd].FrameList[frame_index].cart_id, NULL) == -1) {
      free(frame_buf);
      return -1;
    }
    
    if (opcode_handler(CART_OP_WRFRME, Files[fd].FrameList[frame_index].frame_id, frame_buf) == -1) {
      free(frame_buf);
      return -1;
    }

// After we write each frame, we need to update the current position in the file-handler
// if we allocated any new frame, we also need to update the size of file
    Files[fd].position += next_bytes;

    if (Files[fd].size < Files[fd].position) {
      Files[fd].size += next_bytes;
    }

// extract writted bytes from the remaining bytes
// update the current buffer's location
    rest -= next_bytes;
    buf_location += next_bytes;

  }

// free the temp buffer and return 0 if success 
  free(frame_buf); 
  return (count);
}
 

////////////////////////////////////////////////////////////////////////////////
//
// Function     : cart_seek
// Description  : Seek to specific point in the file
//
// Inputs       : fd - filename of the file to write to
//                loc - offfset of file in relation to beginning of file
// Outputs      : 0 if successful, -1 if failure

int32_t cart_seek(int16_t fd, uint32_t loc) {

// First, check whether the fd is valid and files is closed
// if it is invalid and closed return -1
  if ((fd < 0 || CART_MAX_TOTAL_FILES <= fd) || (Files[fd].open_status == 0)) {
    return -1;
  }

// check whether the location is beyond the file's size return -1 if it exceeds
  if ((Files[fd].size) < loc) {
    return -1;
  }

// Else set the current position to the input location
  Files[fd].position = loc;

// return 0 if success
  return (0);
}
