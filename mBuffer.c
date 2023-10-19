// written by Ben Viall 
// licensed for use by currently enrolled ECE263 students 
// for class projects only.

#include "mBuffer.h"

void bufInit(MBUFFER *m,uint8_t *buf,uint8_t size)
{
	m->size=size;
	m->rxBuffer=buf;
	m->tail=m->head=m->bufferOverflow=0;
}

//free space available in buffer
uint8_t available(MBUFFER* m)
{
	if (m->bufferOverflow) return -1;
	return (m->tail+m->size-m->head)%m->size;
}

uint8_t isEmpty(MBUFFER* m)
{
	return (m->head==m->tail);	
}

uint8_t peek(MBUFFER* m)
{
	if (m->head==m->tail) return -1;
	return m->rxBuffer[m->head];	
}

void flush(MBUFFER *m)
{
	m->head=m->tail=m->bufferOverflow=0;	
}

uint8_t dequeue(MBUFFER *m)
{
	m->bufferOverflow=0;
	if (m->head==m->tail) return -1;
	uint8_t nextChar = m->rxBuffer[m->head];
	m->head=(m->head+1)%m->size;
	return nextChar;
}

void enqueue(MBUFFER *m,uint8_t newChar)
{
	 if ((m->tail + 1) % m->size == m->head)
	 { 
		 m->bufferOverflow = 1;
	 } 
	 else 
	 {							
		 m->rxBuffer[m->tail] = newChar;
		 m->tail = (m->tail + 1) % m->size;
	 }
}


