#include <QFile>

#include "gigaemulf2.h"

/*
 *
 */
GigaEmulF2::GigaEmulF2()
	: romReadBytes(-1)
{
	rom = new uint16_t[romSizeWords];
	ram = new uint8_t[ramSizeBytes];
	ramSave = new uint8_t[ramSizeBytes];

	screen = QImage(vgaPixels, vgaLines, QImage::Format_RGB32);
	pixels = (QRgb *) screen.bits();
	d = pixels;
	x = 0;
	y = 0;
	
	init();
}

/*
 *
 */
GigaEmulF2::~GigaEmulF2()
{
	delete [] rom;
	delete [] ram;
	delete [] ramSave;
}

/*
 *
 */
void
GigaEmulF2::init()
{
	QFile romFile("./data/ROM.rom");
	if ( romFile.open(QIODevice::ReadOnly) )
	{
		romReadBytes = romFile.read((char *) rom, romSizeWords * 2);
		romFile.close();
	}
	else
		memset(rom, 0x00, romSizeWords * 2);
	
	tickCount = 0;
	
	memset(ram, 0x00, ramSizeBytes);
}

/*
 *
 */
void
GigaEmulF2::reset()
{
	tickCount = 0;
	
	memset(ram, 0x00, ramSizeBytes);
	memset(ramSave, 0x00, ramSizeBytes);
	state = GigaState();
}

/*
 *
 */
void
GigaEmulF2::save()
{
	memcpy(ramSave, ram, sizeof(ramSizeBytes));
}

/*
 *
 */
void
GigaEmulF2::setInput(uint8_t input)
{
	state.in = input;
}

/*
 *
 */
void
GigaEmulF2::proc(uint64_t ticks)
{
	static const void *addressing_labels[8] = {
		&&label_ir_addressing_d_ac,
		&&label_ir_addressing_x_ac,
		&&label_ir_addressing_yd_ac,
		&&label_ir_addressing_yx_ac,
		&&label_ir_addressing_d_x,
		&&label_ir_addressing_d_y,
		&&label_ir_addressing_d_out,
		&&label_ir_addressing_yx_out
	};
			
	static const void *bus_labels[4] = {
		&&label_ir_bus_mode_d_bus,
		&&label_ir_bus_mode_ram_bus,
		&&label_ir_bus_mode_ac_bus,
		&&label_ir_bus_mode_in_bus
	};

	static const void *instruction_labels[8] = {
		&&label_ir_instruction_ld,
		&&label_ir_instruction_and,
		&&label_ir_instruction_or,
		&&label_ir_instruction_xor,
		&&label_ir_instruction_add,
		&&label_ir_instruction_sub,
		&&label_ir_instruction_st,
		&&label_ir_instruction_jmp
	};

	static const void *result_labels[8] = {
		&&result_ir_addressing_d_ac,
		&&result_ir_addressing_x_ac,
		&&result_ir_addressing_yd_ac,
		&&result_ir_addressing_yx_ac,
		&&result_ir_addressing_d_x,
		&&result_ir_addressing_d_y,
		&&result_ir_addressing_d_out,
		&&result_ir_addressing_yx_out
	};
	
	for (uint64_t i = 0; i < ticks; ++i)
	{
		GigaState next = state;
		
		// Fetch instruction to be executed next time
		next.ir = ((uint8_t *) &rom[state.pc])[0];
		next.d  = ((uint8_t *) &rom[state.pc])[1];
		next.pc_1 = state.pc;
		
		const uint8_t instruction	= state.ir & ir_instruction_mask;
		const uint8_t addressing	= state.ir & ir_addressing_mask;
		const uint8_t bus_mode		= state.ir & ir_bus_mode_mask;

		const uint8_t instructionIdx	= instruction >> ir_instruction_shift;
		const uint8_t addressingIdx		= addressing >> ir_addressing_shift;
		const uint8_t bus_modeIdx			= bus_mode >> ir_bus_mode_shift;
		
		// In RAM address calculation
		uint16_t ram_addr = 0;
		if ( instruction == ir_instruction_jmp )
		{
																		ram_addr = (      0 << 8) | state.d;
		}
		else
		{
			goto *addressing_labels[addressingIdx];
			while ( 1 )
			{
				label_ir_addressing_d_ac:		ram_addr = (      0 << 8) | state.d;	break;
				label_ir_addressing_x_ac:		ram_addr = (      0 << 8) | state.x;	break;
				label_ir_addressing_yd_ac:	ram_addr = (state.y << 8) | state.d;	break;
				label_ir_addressing_yx_ac:	ram_addr = (state.y << 8) | state.x;	break;
				label_ir_addressing_d_x:		ram_addr = (      0 << 8) | state.d;	break;
				label_ir_addressing_d_y:		ram_addr = (      0 << 8) | state.d;	break;
				label_ir_addressing_d_out:	ram_addr = (      0 << 8) | state.d;	break;
				label_ir_addressing_yx_out:	ram_addr = (state.y << 8) | state.x;	break;
			}
		}
		
		// Load BUS value
		uint8_t bus = state.bus;	// floating bus (random value)
		goto *bus_labels[bus_modeIdx];
		while ( 1 )
		{
			label_ir_bus_mode_d_bus:	bus = state.d;				break;
			label_ir_bus_mode_ram_bus:
				if ( instruction == ir_instruction_st )				break;	// floating bus (random value)
																bus = ram[ram_addr];	break;
			label_ir_bus_mode_ac_bus:	bus = state.ac;				break;
			label_ir_bus_mode_in_bus:	bus = state.in;				break;
		}
		
		// Store BUS to RAM
		if ( instruction == ir_instruction_st )
			ram[ram_addr] = bus;
		
		// ALU operation
		uint8_t alu = 0;
		goto *instruction_labels[instructionIdx];
		while ( 1 )
		{
			label_ir_instruction_ld:	alu =  bus;							break;
			label_ir_instruction_and:	alu =  state.ac & bus;	break;
			label_ir_instruction_or:	alu =  state.ac | bus;	break;
			label_ir_instruction_xor:	alu =  state.ac ^ bus;	break;
			label_ir_instruction_add:	alu =  state.ac + bus;	break;
			label_ir_instruction_sub:	alu =  state.ac - bus;	break;
			label_ir_instruction_st:	alu =  state.ac;				break;
			label_ir_instruction_jmp:	alu = -state.ac;				break;
		}

		// Store ALU result
		if ( instruction == ir_instruction_jmp )
		{
		}
		else if ( instruction == ir_instruction_st )
		{
			switch ( addressing )
			{
			//case ir_addressing_d_ac:
			//case ir_addressing_x_ac:
			//case ir_addressing_yd_ac:
			//case ir_addressing_yx_ac:		next.ac = alu;	break;
				case ir_addressing_d_x:			next.x = alu;		break;
				case ir_addressing_d_y:			next.y = alu;		break;
			//case ir_addressing_d_out:
			//case ir_addressing_yx_out:	next.out = alu; break;
			}
		}
		else
		{
			goto *result_labels[addressingIdx];
			while ( 1 )
			{
				result_ir_addressing_d_ac:
				result_ir_addressing_x_ac:
				result_ir_addressing_yd_ac:
				result_ir_addressing_yx_ac:		next.ac = alu;	break;
				result_ir_addressing_d_x:			next.x = alu;		break;
				result_ir_addressing_d_y:			next.y = alu;		break;
				result_ir_addressing_d_out:
				result_ir_addressing_yx_out:	next.out = alu; break;
			}
		}
		
		// Increase X++ for [Y:X++] addressing
		if ( instruction == ir_instruction_jmp )
		{
		}
		else
		{
			if ( addressing == ir_addressing_yx_out )
				++next.x;
		}
		
		// Increase PC
		++next.pc;
		
		// Jump or branch
		if ( instruction == ir_instruction_jmp )
		{
			if ( addressing == ir_jump_far )
			{
				// jmp
				next.pc = (state.y << 8) | bus;
			}
			else
			{
				// branch
				if ( addressing & (( 1 << ((state.ac >> 7) + ( ! state.ac << 1)) ) << 2) )
					next.pc = (state.pc & 0xff00) | bus;
			}
		}

		// Screen
		{
			const uint8_t out = next.out;
			const uint8_t outPrev = state.out;
			
			const bool vSync = ! (out & vSyncBitmask);
		//const bool vSyncStart = ! (out & vSyncBitmask) && (outPrev & vSyncBitmask);
			const bool vSyncEnd = (out & vSyncBitmask) && ! (outPrev & vSyncBitmask);
			
		//if ( vSyncStart )
			if ( vSyncEnd )
			{
				y = 0;
				d = pixels + y * vgaPixels + x;
			}
			
			const bool hSync = ! (out & hSyncBitmask);
		//const bool hSyncStart = ! (out & hSyncBitmask) && (outPrev & hSyncBitmask);
			const bool hSyncEnd = (out & hSyncBitmask) && ! (outPrev & hSyncBitmask);
			
		//if ( hSyncStart )
			if ( hSyncEnd )
			{
				x = 0;
				++y;
				d = pixels + y * vgaPixels + x;
				next.bus = rand() & 0xff;
			}

			QRgb c;
		//QRgb c = qRgb(((out & 0x3) << 6), ((out & (0x3 << 2)) << 4), ((out & (0x3 << 4)) << 2));
			
			if ( ! (vSync || hSync) )
				c = ((out & 0x3) << 22) | ((out & (0x3 << 2)) << 12) | ((out & (0x3 << 4)) << 2);
			else if ( vSync && hSync )
				c = vhSyncColor;
			else if ( vSync )
				c = vSyncColor;
			else if ( hSync )
				c = hSyncColor;
			
			if (x < vgaPixels)
			{
				*d++ = c;
				*d++ = c;
				*d++ = c;
				*d++ = c;
				x += 4;
			}
		}
		
		state = next;
		
		++tickCount;
	}
}
