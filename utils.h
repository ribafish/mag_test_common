/*
 * utils.h
 *
 *  Created on: Jul 28, 2018
 *      Author: gasper
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>

/*
 * RINGBUFFER STUFF
 */
typedef struct {
	uint16_t * const queue; 	// constant pointer to normal data - pointer can't be changed once initialized
	uint8_t tail;
	uint8_t length;
	const uint8_t maxsize;		// constant maxsize - can't be changed once initialized
} ringbuf_u16_t;

typedef struct {
	uint8_t * const queue; 	// constant pointer to normal data - pointer can't be changed once initialized
	uint8_t tail;
	uint8_t length;
	const uint8_t maxsize;		// constant maxsize - can't be changed once initialized
} ringbuf_u8_t;

#define RINGBUF_U16_DECLARE_INIT(__ringbuf_name__, __ringbuf_size__) \
		static uint16_t __ringbuf_name__ ## _queue[__ringbuf_size__]; 	\
		static ringbuf_u16_t __ringbuf_name__ = {						\
				.queue = __ringbuf_name__ ## _queue, 				\
				.maxsize = __ringbuf_size__, 						\
				.tail = 0, 											\
				.length = 0};

#define RINGBUF_U8_DECLARE_INIT(__ringbuf_name__, __ringbuf_size__) \
		static uint8_t __ringbuf_name__ ## _queue[__ringbuf_size__]; 	\
		static ringbuf_u8_t __ringbuf_name__ = {						\
				.queue = __ringbuf_name__ ## _queue, 				\
				.maxsize = __ringbuf_size__, 						\
				.tail = 0, 											\
				.length = 0};


void ringbuf_u16_printout(ringbuf_u16_t * buf);
void ringbuf_u16_push(ringbuf_u16_t * buf, uint16_t val);
void ringbuf_u16_push_first(ringbuf_u16_t * buf, uint16_t val);
uint16_t ringbuf_u16_peek(ringbuf_u16_t * buf);
uint16_t ringbuf_u16_pop(ringbuf_u16_t * buf);
uint8_t ringbuf_u16_space_available(ringbuf_u16_t * buf);
uint8_t ringbuf_u16_get_length(ringbuf_u16_t * buf);

void ringbuf_u8_push(ringbuf_u8_t * buf, uint8_t val);
uint8_t ringbuf_u8_peek(ringbuf_u8_t * buf);
uint8_t ringbuf_u8_pop(ringbuf_u8_t * buf);
uint8_t ringbuf_u8_space_available(ringbuf_u8_t * buf);
uint8_t ringbuf_u8_get_length(ringbuf_u8_t * buf);


#define  W_LE32(addr, val)		((uint8_t *)(addr))[0] = (uint8_t) (((uint32_t) val >> 24) & 0x000000FF); \
								((uint8_t *)(addr))[1] = (uint8_t) (((uint32_t) val >> 16) & 0x000000FF); \
								((uint8_t *)(addr))[2] = (uint8_t) (((uint32_t) val >> 8)  & 0x000000FF); \
								((uint8_t *)(addr))[3] = (uint8_t) (((uint32_t) val) 	      & 0x000000FF);

#define  R_LE32(addr)           	( (((uint32_t)((uint8_t *)(addr))[0]) << 24) + \
								  (((uint32_t)((uint8_t *)(addr))[1]) << 16) + \
								  (((uint32_t)((uint8_t *)(addr))[2]) <<  8) + \
								  (((uint32_t)((uint8_t *)(addr))[3]) )        )

#define  W_LE16(addr, val)		((uint8_t *)(addr))[0] = (uint8_t) (((uint16_t) val >> 8)  & 0x00FF); \
								((uint8_t *)(addr))[1] = (uint8_t) (((uint16_t) val) 	      & 0x00FF);

#define  R_LE16(addr)           	( ((uint16_t) (((uint8_t *)(addr))[0] ) <<  8) + \
								  ((uint16_t) (((uint8_t *)(addr))[1]))        )

#define  W_LE32S(addr, val)         W_LE32(addr, (uint32_t)(val))
#define  R_LE32S(addr)              (int32_t)(R_LE32((addr)))

#define  W_LE16S(addr, val)         W_LE16(addr, (uint16_t)(val))
#define  R_LE16S(addr)              (int16_t)(R_LE16((addr)))


#endif /* UTILS_H_ */
