#ifndef GIGA_STATE_H
#define GIGA_STATE_H

#include <cinttypes>

struct GigaState
{
	GigaState()
	{
		pc = 0x0000;
		pc_1 = 0x0000;
		ir = 0x00;
		d = 0x00;
		ac = 0x00;
		x = 0x00;
		y = 0x00;
		out = 0x00;
		in = 0xff;
		bus = 0x00;
	};
	
	uint16_t pc;
	uint16_t pc_1;	// pc prefetched to ir, d
	uint8_t ir;
	uint8_t d;
	uint8_t ac;
	uint8_t x;
	uint8_t y;
	uint8_t out;
	uint8_t in;
	uint8_t bus;		// floating bus (random value)
};

#endif // GIGA_STATE_H
