#ifndef __CPMDN_H_INCLUED__
#define __CPMDN_H_INCLUED__

#include "Arduino.h"

#define CPMDN_BUFF_SIZE            50

typedef struct tagCPMDN {
  int8_t _buffer_ptr;
  int8_t _window_size;
  int8_t _median_ptr;
  int8_t _not_filled;
  int16_t _inbuffer[CPMDN_BUFF_SIZE];
  int16_t _sortbuffer[CPMDN_BUFF_SIZE];
  uint8_t _precise;
} CPMDN;

int8_t med_init(CPMDN *mdn, int8_t mdnsz);
int16_t med_getmedian(CPMDN *mdn);
int8_t med_getmedianex(CPMDN *mdn, int16_t *mv);
void med_addvalue(CPMDN *mdn, int16_t new_value);
void med_reset(CPMDN *mdn);

#endif /* __CPMDN_H_INCLUED__ */
