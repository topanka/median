//
// Released to the public domain
//
// Remarks:
// This is a lean but fast version.
// Initially, the buffer is filled with a "default_value". To get real CPMDN values
// you have to fill the object with N values, where N is the size of the sliding window.
// For example: for(int i=0; i < 32; i++) myCPMDN.addValue(readSensor());
//
// Constructor:
// FastRunningCPMDN<datatype_of_content, size_of_sliding_window, default_value>
// maximim size_of_sliding_window is 255
// Methods:
// addValue(val) adds a new value to the buffers (and kicks the oldest)
// getCPMDN() returns the current CPMDN value
//
//
// Usage:
// #include "FastRunningCPMDN.h"
// FastRunningCPMDN<unsigned int,32, 0> myCPMDN;
// ....
// myCPMDN.addValue(value); // adds a value
// m = myCPMDN.getCPMDN(); // retieves the CPMDN
//


#include "Arduino.h"
#include "cpmdn.h"

/************************************************************************/
void setprec(CPMDN *mdn)
{
  if(mdn->_not_filled) return;
  if((mdn->_sortbuffer[mdn->_window_size-1]-mdn->_sortbuffer[0]) < 30) mdn->_precise=1;
  else mdn->_precise=0;
}
/************************************************************************/
int8_t med_init(CPMDN *mdn, int8_t mdnsz)
{
  if(mdnsz > CPMDN_BUFF_SIZE) return(-1);
  mdn->_buffer_ptr=mdnsz;
  mdn->_window_size=mdnsz;
  mdn->_median_ptr=mdnsz/2;
  mdn->_not_filled=1;
  mdn->_precise=0;
  return(0);
}
/************************************************************************/
int16_t med_getmedian(CPMDN *mdn)
{
  return(mdn->_sortbuffer[mdn->_median_ptr]);
}
/************************************************************************/
int8_t med_getmedianex(CPMDN *mdn, int16_t *mv)
{
  if(mdn->_not_filled) return(-1);
  if(mdn->_precise == 0) return(-1);
  *mv=mdn->_sortbuffer[mdn->_median_ptr];
  return(0);
}
/************************************************************************/
void med_addvalue(CPMDN *mdn, int16_t new_value)
{
  int16_t old_value;
  uint8_t i=mdn->_window_size;
  int16_t p,q;
  int16_t tmp;
  
  // comparision with 0 is fast, so we decrement _buffer_ptr
  if (mdn->_buffer_ptr == 0) {
    mdn->_buffer_ptr=mdn->_window_size;
  }
  
  mdn->_buffer_ptr--;

  old_value = mdn->_inbuffer[mdn->_buffer_ptr]; // retrieve the old value to be replaced
  if ((new_value == old_value) && (mdn->_not_filled == 0)) {      // if the value is unchanged, do nothing
    return;
  }
  
  if (mdn->_buffer_ptr == 0) {
    mdn->_not_filled=0;
  }

  mdn->_inbuffer[mdn->_buffer_ptr] = new_value;  // fill the new value in the cyclic buffer
    
  // search the old_value in the sorted buffer
  while(i > 0) {
    i--;
    if (old_value == mdn->_sortbuffer[i])
      break;
  }
  
  // i is the index of the old_value in the sorted buffer
  mdn->_sortbuffer[i] = new_value; // replace the value

  // the sortbuffer is always sorted, except the [i]-element..
  if (new_value > old_value) {
    //  if the new value is bigger than the old one, make a bubble sort upwards
    for(p=i, q=i+1; q < mdn->_window_size; p++, q++) {
      // bubble sort step
      if (mdn->_sortbuffer[p] > mdn->_sortbuffer[q]) {
        tmp = mdn->_sortbuffer[p];
        mdn->_sortbuffer[p] = mdn->_sortbuffer[q];
        mdn->_sortbuffer[q] = tmp;
      } else {
        // done ! - found the right place
        setprec(mdn);
        return;
      }
    }
  } else {
    // else new_value is smaller than the old one, bubble downwards
    for(p=i-1, q=i; q > 0; p--, q--) {
      if (mdn->_sortbuffer[p] > mdn->_sortbuffer[q]) {
        tmp = mdn->_sortbuffer[p];
        mdn->_sortbuffer[p] = mdn->_sortbuffer[q];
        mdn->_sortbuffer[q] = tmp;
      } else {
        // done !
        setprec(mdn);
        return;
      }
    }
  }
}
/************************************************************************/
void med_reset(CPMDN *mdn)
{
  mdn->_buffer_ptr=mdn->_window_size;
  mdn->_not_filled=1;
  mdn->_precise=0;
}
/************************************************************************/

