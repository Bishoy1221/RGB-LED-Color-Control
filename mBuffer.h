//written by Ben Viall 
// liceneced for use by currently enrolled ECE263 students 
// for class projects only.


#ifndef MBUFFER_H
#define MBUFFER_H
#include <stdint.h>
// buffer for at most 255 bytes.
typedef struct _mbuffer
{
	uint8_t size;
	uint8_t *rxBuffer;
	uint8_t tail;
	uint8_t head;
	uint8_t bufferOverflow;
} MBUFFER;


void bufInit(MBUFFER *m,uint8_t *buf,uint8_t size);
uint8_t available(MBUFFER* m);
uint8_t isEmpty(MBUFFER* m);
uint8_t peek(MBUFFER* m);
void flush(MBUFFER *m);
uint8_t dequeue(MBUFFER *m);
void enqueue(MBUFFER *m,uint8_t newChar);

#endif