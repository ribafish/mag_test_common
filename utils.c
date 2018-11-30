/*
 * utils.c
 *
 *  Created on: Jul 28, 2018
 *      Author: gasper
 */

#include "utils.h"
#include "debug.h"

/*
 * RINGBUFFER STUFF
 */
void ringbuf_u16_printout(ringbuf_u16_t * buf) {
	uint8_t head = (buf->tail + buf->length) % buf->maxsize;
	debug_line_global("Buf size %d head %d tail %d length %d available %d\n",
			buf->maxsize, head, buf->tail, buf->length, ringbuf_u16_space_available(buf));
	for (uint8_t i = 0; i < 16; i++) {
		debug_global(" %02d |", i);
	}
	for (uint16_t i = 0; i < buf->maxsize; i++) {
		if (i%16==0) { debug_global("\n"); }
		debug_global(" %02d |", buf->queue[i]);
	}
	debug_global("\n");
}


void ringbuf_u16_push(ringbuf_u16_t * buf, uint16_t val) {
	if (ringbuf_u16_space_available(buf) == 0) {
		uint16_t tmp = ringbuf_u16_pop(buf);
		debug_error_global("ERROR: no more space in queue, so we popped value 0x%04x\n", tmp);
	}
	uint8_t head = (buf->tail + buf->length) % buf->maxsize;
	buf->queue[head] = val;
	buf->length++;
}

// pushes to the first space (LIFO), instead of last one (FIFO)
void ringbuf_u16_push_first(ringbuf_u16_t * buf, uint16_t val) {
	if (ringbuf_u16_space_available(buf) == 0) {
		uint16_t tmp = ringbuf_u16_pop(buf);
		debug_error_global("ERROR: no more space in queue, so we popped value 0x%04x\n", tmp);
	}
	buf->tail = (buf->tail - 1) % buf->maxsize;
	buf->queue[buf->tail] = val;
	buf->length++;
}

uint16_t ringbuf_u16_peek(ringbuf_u16_t * buf) {
	return buf->queue[buf->tail];
}

uint16_t ringbuf_u16_pop(ringbuf_u16_t * buf) {
	if (buf->length == 0) {
		debug_error_global("ERROR: trying to pop a value out of empty buffer\n");
		return 0;
	}
	uint16_t val = buf->queue[buf->tail];
	buf->queue[buf->tail] = 0;
	buf->tail = (buf->tail + 1) % buf->maxsize;
	buf->length--;
	return val;
}

uint8_t ringbuf_u16_space_available(ringbuf_u16_t * buf) {
	return buf->maxsize - buf->length;
}

uint8_t ringbuf_u16_get_length(ringbuf_u16_t * buf) {
	return buf->length;
}

void ringbuf_u8_push(ringbuf_u8_t * buf, uint8_t val) {
	if (ringbuf_u8_space_available(buf) == 0) {
		uint8_t tmp = ringbuf_u8_pop(buf);
		debug_error_global("ERROR: no more space in queue, so we popped value 0x%04x\n", tmp);
	}
	uint8_t head = (buf->tail + buf->length) % buf->maxsize;
	buf->queue[head] = val;
	buf->length++;
}

uint8_t ringbuf_u8_peek(ringbuf_u8_t * buf) {
	return buf->queue[buf->tail];
}

uint8_t ringbuf_u8_pop(ringbuf_u8_t * buf) {
	if (buf->length == 0) {
		debug_error_global("ERROR: trying to pop a value out of empty buffer\n");
		return 0;
	}
	uint8_t val = buf->queue[buf->tail];
	buf->queue[buf->tail] = 0;
	buf->tail = (buf->tail + 1) % buf->maxsize;
	buf->length--;
	return val;
}

uint8_t ringbuf_u8_space_available(ringbuf_u8_t * buf) {
	return buf->maxsize - buf->length;
}

uint8_t ringbuf_u8_get_length(ringbuf_u8_t * buf) {
	return buf->length;
}
