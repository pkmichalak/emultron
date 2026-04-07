#include <QFile>

#include "gigaemulf3.h"

/*
 *
 */
GigaEmulF3::GigaEmulF3()
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
GigaEmulF3::~GigaEmulF3()
{
	delete [] rom;
	delete [] ram;
	delete [] ramSave;
}

/*
 *
 */
void
GigaEmulF3::init()
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
GigaEmulF3::reset()
{
	tickCount = 0;
	
	memset(ram, 0x00, ramSizeBytes);
	memset(ramSave, 0x00, ramSizeBytes);
	s = GigaState();
}

/*
 *
 */
void
GigaEmulF3::save()
{
	memcpy(ramSave, ram, sizeof(ramSizeBytes));
}

/*
 *
 */
void
GigaEmulF3::setInput(uint8_t input)
{
	s.in = input;
}

/*
 *
 */
void
GigaEmulF3::proc(uint64_t ticks)
{
	static const void *opcode_labels[256] = {
/* 00 */ &&  ld__d__ac____d,
/* 01 */ &&  ld__d__ac__ram,
/* 02 */ &&  ld__d__ac___ac,
/* 03 */ &&  ld__d__ac___in,
/* 04 */ &&  ld__x__ac____d,
/* 05 */ &&  ld__x__ac__ram,
/* 06 */ &&  ld__x__ac___ac,
/* 07 */ &&  ld__x__ac___in,
/* 08 */ &&  ld_yd__ac____d,
/* 09 */ &&  ld_yd__ac__ram,
/* 0A */ &&  ld_yd__ac___ac,
/* 0B */ &&  ld_yd__ac___in,
/* 0C */ &&  ld_yx__ac____d,
/* 0D */ &&  ld_yx__ac__ram,
/* 0E */ &&  ld_yx__ac___ac,
/* 0F */ &&  ld_yx__ac___in,
/* 10 */ &&  ld__d___x____d,
/* 11 */ &&  ld__d___x__ram,
/* 12 */ &&  ld__d___x___ac,
/* 13 */ &&  ld__d___x___in,
/* 14 */ &&  ld__d___y____d,
/* 15 */ &&  ld__d___y__ram,
/* 16 */ &&  ld__d___y___ac,
/* 17 */ &&  ld__d___y___in,
/* 18 */ &&  ld__d_out____d,
/* 19 */ &&  ld__d_out__ram,
/* 1A */ &&  ld__d_out___ac,
/* 1B */ &&  ld__d_out___in,
/* 1C */ &&  ld_yx_out____d,
/* 1D */ &&  ld_yx_out__ram,
/* 1E */ &&  ld_yx_out___ac,
/* 1F */ &&  ld_yx_out___in,
/* 20 */ && and__d__ac____d,
/* 21 */ && and__d__ac__ram,
/* 22 */ && and__d__ac___ac,
/* 23 */ && and__d__ac___in,
/* 24 */ && and__x__ac____d,
/* 25 */ && and__x__ac__ram,
/* 26 */ && and__x__ac___ac,
/* 27 */ && and__x__ac___in,
/* 28 */ && and_yd__ac____d,
/* 29 */ && and_yd__ac__ram,
/* 2A */ && and_yd__ac___ac,
/* 2B */ && and_yd__ac___in,
/* 2C */ && and_yx__ac____d,
/* 2D */ && and_yx__ac__ram,
/* 2E */ && and_yx__ac___ac,
/* 2F */ && and_yx__ac___in,
/* 30 */ && and__d___x____d,
/* 31 */ && and__d___x__ram,
/* 32 */ && and__d___x___ac,
/* 33 */ && and__d___x___in,
/* 34 */ && and__d___y____d,
/* 35 */ && and__d___y__ram,
/* 36 */ && and__d___y___ac,
/* 37 */ && and__d___y___in,
/* 38 */ && and__d_out____d,
/* 39 */ && and__d_out__ram,
/* 3A */ && and__d_out___ac,
/* 3B */ && and__d_out___in,
/* 3C */ && and_yx_out____d,
/* 3D */ && and_yx_out__ram,
/* 3E */ && and_yx_out___ac,
/* 3F */ && and_yx_out___in,
/* 40 */ &&  or__d__ac____d,
/* 41 */ &&  or__d__ac__ram,
/* 42 */ &&  or__d__ac___ac,
/* 43 */ &&  or__d__ac___in,
/* 44 */ &&  or__x__ac____d,
/* 45 */ &&  or__x__ac__ram,
/* 46 */ &&  or__x__ac___ac,
/* 47 */ &&  or__x__ac___in,
/* 48 */ &&  or_yd__ac____d,
/* 49 */ &&  or_yd__ac__ram,
/* 4A */ &&  or_yd__ac___ac,
/* 4B */ &&  or_yd__ac___in,
/* 4C */ &&  or_yx__ac____d,
/* 4D */ &&  or_yx__ac__ram,
/* 4E */ &&  or_yx__ac___ac,
/* 4F */ &&  or_yx__ac___in,
/* 50 */ &&  or__d___x____d,
/* 51 */ &&  or__d___x__ram,
/* 52 */ &&  or__d___x___ac,
/* 53 */ &&  or__d___x___in,
/* 54 */ &&  or__d___y____d,
/* 55 */ &&  or__d___y__ram,
/* 56 */ &&  or__d___y___ac,
/* 57 */ &&  or__d___y___in,
/* 58 */ &&  or__d_out____d,
/* 59 */ &&  or__d_out__ram,
/* 5A */ &&  or__d_out___ac,
/* 5B */ &&  or__d_out___in,
/* 5C */ &&  or_yx_out____d,
/* 5D */ &&  or_yx_out__ram,
/* 5E */ &&  or_yx_out___ac,
/* 5F */ &&  or_yx_out___in,
/* 60 */ && xor__d__ac____d,
/* 61 */ && xor__d__ac__ram,
/* 62 */ && xor__d__ac___ac,
/* 63 */ && xor__d__ac___in,
/* 64 */ && xor__x__ac____d,
/* 65 */ && xor__x__ac__ram,
/* 66 */ && xor__x__ac___ac,
/* 67 */ && xor__x__ac___in,
/* 68 */ && xor_yd__ac____d,
/* 69 */ && xor_yd__ac__ram,
/* 6A */ && xor_yd__ac___ac,
/* 6B */ && xor_yd__ac___in,
/* 6C */ && xor_yx__ac____d,
/* 6D */ && xor_yx__ac__ram,
/* 6E */ && xor_yx__ac___ac,
/* 6F */ && xor_yx__ac___in,
/* 70 */ && xor__d___x____d,
/* 71 */ && xor__d___x__ram,
/* 72 */ && xor__d___x___ac,
/* 73 */ && xor__d___x___in,
/* 74 */ && xor__d___y____d,
/* 75 */ && xor__d___y__ram,
/* 76 */ && xor__d___y___ac,
/* 77 */ && xor__d___y___in,
/* 78 */ && xor__d_out____d,
/* 79 */ && xor__d_out__ram,
/* 7A */ && xor__d_out___ac,
/* 7B */ && xor__d_out___in,
/* 7C */ && xor_yx_out____d,
/* 7D */ && xor_yx_out__ram,
/* 7E */ && xor_yx_out___ac,
/* 7F */ && xor_yx_out___in,
/* 80 */ && add__d__ac____d,
/* 81 */ && add__d__ac__ram,
/* 82 */ && add__d__ac___ac,
/* 83 */ && add__d__ac___in,
/* 84 */ && add__x__ac____d,
/* 85 */ && add__x__ac__ram,
/* 86 */ && add__x__ac___ac,
/* 87 */ && add__x__ac___in,
/* 88 */ && add_yd__ac____d,
/* 89 */ && add_yd__ac__ram,
/* 8A */ && add_yd__ac___ac,
/* 8B */ && add_yd__ac___in,
/* 8C */ && add_yx__ac____d,
/* 8D */ && add_yx__ac__ram,
/* 8E */ && add_yx__ac___ac,
/* 8F */ && add_yx__ac___in,
/* 90 */ && add__d___x____d,
/* 91 */ && add__d___x__ram,
/* 92 */ && add__d___x___ac,
/* 93 */ && add__d___x___in,
/* 94 */ && add__d___y____d,
/* 95 */ && add__d___y__ram,
/* 96 */ && add__d___y___ac,
/* 97 */ && add__d___y___in,
/* 98 */ && add__d_out____d,
/* 99 */ && add__d_out__ram,
/* 9A */ && add__d_out___ac,
/* 9B */ && add__d_out___in,
/* 9C */ && add_yx_out____d,
/* 9D */ && add_yx_out__ram,
/* 9E */ && add_yx_out___ac,
/* 9F */ && add_yx_out___in,
/* A0 */ && sub__d__ac____d,
/* A1 */ && sub__d__ac__ram,
/* A2 */ && sub__d__ac___ac,
/* A3 */ && sub__d__ac___in,
/* A4 */ && sub__x__ac____d,
/* A5 */ && sub__x__ac__ram,
/* A6 */ && sub__x__ac___ac,
/* A7 */ && sub__x__ac___in,
/* A8 */ && sub_yd__ac____d,
/* A9 */ && sub_yd__ac__ram,
/* AA */ && sub_yd__ac___ac,
/* AB */ && sub_yd__ac___in,
/* AC */ && sub_yx__ac____d,
/* AD */ && sub_yx__ac__ram,
/* AE */ && sub_yx__ac___ac,
/* AF */ && sub_yx__ac___in,
/* B0 */ && sub__d___x____d,
/* B1 */ && sub__d___x__ram,
/* B2 */ && sub__d___x___ac,
/* B3 */ && sub__d___x___in,
/* B4 */ && sub__d___y____d,
/* B5 */ && sub__d___y__ram,
/* B6 */ && sub__d___y___ac,
/* B7 */ && sub__d___y___in,
/* B8 */ && sub__d_out____d,
/* B9 */ && sub__d_out__ram,
/* BA */ && sub__d_out___ac,
/* BB */ && sub__d_out___in,
/* BC */ && sub_yx_out____d,
/* BD */ && sub_yx_out__ram,
/* BE */ && sub_yx_out___ac,
/* BF */ && sub_yx_out___in,
/* C0 */ &&  st__d________d,
/* C1 */ &&  st__d______ram,
/* C2 */ &&  st__d_______ac,
/* C3 */ &&  st__d_______in,
/* C4 */ &&  st__x________d,
/* C5 */ &&  st__x______ram,
/* C6 */ &&  st__x_______ac,
/* C7 */ &&  st__x_______in,
/* C8 */ &&  st_yd________d,
/* C9 */ &&  st_yd______ram,
/* CA */ &&  st_yd_______ac,
/* CB */ &&  st_yd_______in,
/* CC */ &&  st_yx________d,
/* CD */ &&  st_yx______ram,
/* CE */ &&  st_yx_______ac,
/* CF */ &&  st_yx_______in,
/* D0 */ &&  st__d___x____d,
/* D1 */ &&  st__d___x__ram,
/* D2 */ &&  st__d___x___ac,
/* D3 */ &&  st__d___x___in,
/* D4 */ &&  st__d___y____d,
/* D5 */ &&  st__d___y__ram,
/* D6 */ &&  st__d___y___ac,
/* D7 */ &&  st__d___y___in,
/* D8 */ &&  st__d___2____d,
/* D9 */ &&  st__d___2__ram,
/* DA */ &&  st__d___2___ac,
/* DB */ &&  st__d___2___in,
/* DC */ &&  st_yx___2____d,
/* DD */ &&  st_yx___2__ram,
/* DE */ &&  st_yx___2___ac,
/* DF */ &&  st_yx___2___in,
/* E0 */ && jmp__far______d,
/* E1 */ && jmp__far____ram,
/* E2 */ && jmp__far_____ac,
/* E3 */ && jmp__far_____in,
/* E4 */ && jmp_near_gt___d,
/* E5 */ && jmp_near_gt_ram,
/* E6 */ && jmp_near_gt__ac,
/* E7 */ && jmp_near_gt__in,
/* E8 */ && jmp_near_lt___d,
/* E9 */ && jmp_near_lt_ram,
/* EA */ && jmp_near_lt__ac,
/* EB */ && jmp_near_lt__in,
/* EC */ && jmp_near_ne___d,
/* ED */ && jmp_near_ne_ram,
/* EE */ && jmp_near_ne__ac,
/* EF */ && jmp_near_ne__in,
/* F0 */ && jmp_near_eq___d,
/* F1 */ && jmp_near_eq_ram,
/* F2 */ && jmp_near_eq__ac,
/* F3 */ && jmp_near_eq__in,
/* F4 */ && jmp_near_ge___d,
/* F5 */ && jmp_near_ge_ram,
/* F6 */ && jmp_near_ge__ac,
/* F7 */ && jmp_near_ge__in,
/* F8 */ && jmp_near_le___d,
/* F9 */ && jmp_near_le_ram,
/* FA */ && jmp_near_le__ac,
/* FB */ && jmp_near_le__in,
/* FC */ && jmp_near______d,
/* FD */ && jmp_near____ram,
/* FE */ && jmp_near_____ac,
/* FF */ && jmp_near_____in
	};
	
	for (uint64_t i = 0; i < ticks; ++i)
	{
		GigaState n = s;
		
		// Fetch instruction to be executed next time
		n.ir = ((uint8_t *) &rom[s.pc])[0];
		n.d  = ((uint8_t *) &rom[s.pc])[1];
		n.pc_1 = s.pc;
		
		goto *opcode_labels[s.ir];
	
		while (1)
		{

/* 00 */ ld__d__ac____d: n.ac                  =        s.d;                                                             ++n.pc; break;
/* 01 */ ld__d__ac__ram: n.ac                  =        ram[s.d];                                                        ++n.pc; break;
/* 02 */ ld__d__ac___ac: n.ac                  =        s.ac;                                                            ++n.pc; break;
/* 03 */ ld__d__ac___in: n.ac                  =        s.in;                                                            ++n.pc; break;
/* 04 */ ld__x__ac____d: n.ac                  =        s.d;                                                             ++n.pc; break;
/* 05 */ ld__x__ac__ram: n.ac                  =        ram[s.x];                                                        ++n.pc; break;
/* 06 */ ld__x__ac___ac: n.ac                  =        s.ac;                                                            ++n.pc; break;
/* 07 */ ld__x__ac___in: n.ac                  =        s.in;                                                            ++n.pc; break;
/* 08 */ ld_yd__ac____d: n.ac                  =        s.d;                                                             ++n.pc; break;
/* 09 */ ld_yd__ac__ram: n.ac                  =        ram[(s.y << 8) | s.d];                                           ++n.pc; break;
/* 0A */ ld_yd__ac___ac: n.ac                  =        s.ac;                                                            ++n.pc; break;
/* 0B */ ld_yd__ac___in: n.ac                  =        s.in;                                                            ++n.pc; break;
/* 0C */ ld_yx__ac____d: n.ac                  =        s.d;                                                             ++n.pc; break;
/* 0D */ ld_yx__ac__ram: n.ac                  =        ram[(s.y << 8) | s.x];                                           ++n.pc; break;
/* 0E */ ld_yx__ac___ac: n.ac                  =        s.ac;                                                            ++n.pc; break;
/* 0F */ ld_yx__ac___in: n.ac                  =        s.in;                                                            ++n.pc; break;
/* 10 */ ld__d___x____d: n.x                   =        s.d;                                                             ++n.pc; break;
/* 11 */ ld__d___x__ram: n.x                   =        ram[s.d];                                                        ++n.pc; break;
/* 12 */ ld__d___x___ac: n.x                   =        s.ac;                                                            ++n.pc; break;
/* 13 */ ld__d___x___in: n.x                   =        s.in;                                                            ++n.pc; break;
/* 14 */ ld__d___y____d: n.y                   =        s.d;                                                             ++n.pc; break;
/* 15 */ ld__d___y__ram: n.y                   =        ram[s.d];                                                        ++n.pc; break;
/* 16 */ ld__d___y___ac: n.y                   =        s.ac;                                                            ++n.pc; break;
/* 17 */ ld__d___y___in: n.y                   =        s.in;                                                            ++n.pc; break;
/* 18 */ ld__d_out____d: n.out                 =        s.d;                                                             ++n.pc; break;
/* 19 */ ld__d_out__ram: n.out                 =        ram[s.d];                                                        ++n.pc; break;
/* 1A */ ld__d_out___ac: n.out                 =        s.ac;                                                            ++n.pc; break;
/* 1B */ ld__d_out___in: n.out                 =        s.in;                                                            ++n.pc; break;
/* 1C */ ld_yx_out____d: n.out                 =        s.d;                                                      ++n.x; ++n.pc; break;
/* 1D */ ld_yx_out__ram: n.out                 =        ram[(s.y << 8) | s.x];                                    ++n.x; ++n.pc; break;
/* 1E */ ld_yx_out___ac: n.out                 =        s.ac;                                                     ++n.x; ++n.pc; break;
/* 1F */ ld_yx_out___in: n.out                 =        s.in;                                                     ++n.x; ++n.pc; break;
/* 20 */and__d__ac____d: n.ac                  = s.ac & s.d;                                                             ++n.pc; break;
/* 21 */and__d__ac__ram: n.ac                  = s.ac & ram[s.d];                                                        ++n.pc; break;
/* 22 */and__d__ac___ac: n.ac                  = s.ac & s.ac;                                                            ++n.pc; break;
/* 23 */and__d__ac___in: n.ac                  = s.ac & s.in;                                                            ++n.pc; break;
/* 24 */and__x__ac____d: n.ac                  = s.ac & s.d;                                                             ++n.pc; break;
/* 25 */and__x__ac__ram: n.ac                  = s.ac & ram[s.x];                                                        ++n.pc; break;
/* 26 */and__x__ac___ac: n.ac                  = s.ac & s.ac;                                                            ++n.pc; break;
/* 27 */and__x__ac___in: n.ac                  = s.ac & s.in;                                                            ++n.pc; break;
/* 28 */and_yd__ac____d: n.ac                  = s.ac & s.d;                                                             ++n.pc; break;
/* 29 */and_yd__ac__ram: n.ac                  = s.ac & ram[(s.y << 8) | s.d];                                           ++n.pc; break;
/* 2A */and_yd__ac___ac: n.ac                  = s.ac & s.ac;                                                            ++n.pc; break;
/* 2B */and_yd__ac___in: n.ac                  = s.ac & s.in;                                                            ++n.pc; break;
/* 2C */and_yx__ac____d: n.ac                  = s.ac & s.d;                                                             ++n.pc; break;
/* 2D */and_yx__ac__ram: n.ac                  = s.ac & ram[(s.y << 8) | s.x];                                           ++n.pc; break;
/* 2E */and_yx__ac___ac: n.ac                  = s.ac & s.ac;                                                            ++n.pc; break;
/* 2F */and_yx__ac___in: n.ac                  = s.ac & s.in;                                                            ++n.pc; break;
/* 30 */and__d___x____d: n.x                   = s.ac & s.d;                                                             ++n.pc; break;
/* 31 */and__d___x__ram: n.x                   = s.ac & ram[s.d];                                                        ++n.pc; break;
/* 32 */and__d___x___ac: n.x                   = s.ac & s.ac;                                                            ++n.pc; break;
/* 33 */and__d___x___in: n.x                   = s.ac & s.in;                                                            ++n.pc; break;
/* 34 */and__d___y____d: n.y                   = s.ac & s.d;                                                             ++n.pc; break;
/* 35 */and__d___y__ram: n.y                   = s.ac & ram[s.d];                                                        ++n.pc; break;
/* 36 */and__d___y___ac: n.y                   = s.ac & s.ac;                                                            ++n.pc; break;
/* 37 */and__d___y___in: n.y                   = s.ac & s.in;                                                            ++n.pc; break;
/* 38 */and__d_out____d: n.out                 = s.ac & s.d;                                                             ++n.pc; break;
/* 39 */and__d_out__ram: n.out                 = s.ac & ram[s.d];                                                        ++n.pc; break;
/* 3A */and__d_out___ac: n.out                 = s.ac & s.ac;                                                            ++n.pc; break;
/* 3B */and__d_out___in: n.out                 = s.ac & s.in;                                                            ++n.pc; break;
/* 3C */and_yx_out____d: n.out                 = s.ac & s.d;                                                      ++n.x; ++n.pc; break;
/* 3D */and_yx_out__ram: n.out                 = s.ac & ram[(s.y << 8) | s.x];                                    ++n.x; ++n.pc; break;
/* 3E */and_yx_out___ac: n.out                 = s.ac & s.ac;                                                     ++n.x; ++n.pc; break;
/* 3F */and_yx_out___in: n.out                 = s.ac & s.in;                                                     ++n.x; ++n.pc; break;
/* 40 */ or__d__ac____d: n.ac                  = s.ac | s.d;                                                             ++n.pc; break;
/* 41 */ or__d__ac__ram: n.ac                  = s.ac | ram[s.d];                                                        ++n.pc; break;
/* 42 */ or__d__ac___ac: n.ac                  = s.ac | s.ac;                                                            ++n.pc; break;
/* 43 */ or__d__ac___in: n.ac                  = s.ac | s.in;                                                            ++n.pc; break;
/* 44 */ or__x__ac____d: n.ac                  = s.ac | s.d;                                                             ++n.pc; break;
/* 45 */ or__x__ac__ram: n.ac                  = s.ac | ram[s.x];                                                        ++n.pc; break;
/* 46 */ or__x__ac___ac: n.ac                  = s.ac | s.ac;                                                            ++n.pc; break;
/* 47 */ or__x__ac___in: n.ac                  = s.ac | s.in;                                                            ++n.pc; break;
/* 48 */ or_yd__ac____d: n.ac                  = s.ac | s.d;                                                             ++n.pc; break;
/* 49 */ or_yd__ac__ram: n.ac                  = s.ac | ram[(s.y << 8) | s.d];                                           ++n.pc; break;
/* 4A */ or_yd__ac___ac: n.ac                  = s.ac | s.ac;                                                            ++n.pc; break;
/* 4B */ or_yd__ac___in: n.ac                  = s.ac | s.in;                                                            ++n.pc; break;
/* 4C */ or_yx__ac____d: n.ac                  = s.ac | s.d;                                                             ++n.pc; break;
/* 4D */ or_yx__ac__ram: n.ac                  = s.ac | ram[(s.y << 8) | s.x];                                           ++n.pc; break;
/* 4E */ or_yx__ac___ac: n.ac                  = s.ac | s.ac;                                                            ++n.pc; break;
/* 4F */ or_yx__ac___in: n.ac                  = s.ac | s.in;                                                            ++n.pc; break;
/* 50 */ or__d___x____d: n.x                   = s.ac | s.d;                                                             ++n.pc; break;
/* 51 */ or__d___x__ram: n.x                   = s.ac | ram[s.d];                                                        ++n.pc; break;
/* 52 */ or__d___x___ac: n.x                   = s.ac | s.ac;                                                            ++n.pc; break;
/* 53 */ or__d___x___in: n.x                   = s.ac | s.in;                                                            ++n.pc; break;
/* 54 */ or__d___y____d: n.y                   = s.ac | s.d;                                                             ++n.pc; break;
/* 55 */ or__d___y__ram: n.y                   = s.ac | ram[s.d];                                                        ++n.pc; break;
/* 56 */ or__d___y___ac: n.y                   = s.ac | s.ac;                                                            ++n.pc; break;
/* 57 */ or__d___y___in: n.y                   = s.ac | s.in;                                                            ++n.pc; break;
/* 58 */ or__d_out____d: n.out                 = s.ac | s.d;                                                             ++n.pc; break;
/* 59 */ or__d_out__ram: n.out                 = s.ac | ram[s.d];                                                        ++n.pc; break;
/* 5A */ or__d_out___ac: n.out                 = s.ac | s.ac;                                                            ++n.pc; break;
/* 5B */ or__d_out___in: n.out                 = s.ac | s.in;                                                            ++n.pc; break;
/* 5C */ or_yx_out____d: n.out                 = s.ac | s.d;                                                      ++n.x; ++n.pc; break;
/* 5D */ or_yx_out__ram: n.out                 = s.ac | ram[(s.y << 8) | s.x];                                    ++n.x; ++n.pc; break;
/* 5E */ or_yx_out___ac: n.out                 = s.ac | s.ac;                                                     ++n.x; ++n.pc; break;
/* 5F */ or_yx_out___in: n.out                 = s.ac | s.in;                                                     ++n.x; ++n.pc; break;
/* 60 */xor__d__ac____d: n.ac                  = s.ac ^ s.d;                                                             ++n.pc; break;
/* 61 */xor__d__ac__ram: n.ac                  = s.ac ^ ram[s.d];                                                        ++n.pc; break;
/* 62 */xor__d__ac___ac: n.ac                  = s.ac ^ s.ac;                                                            ++n.pc; break;
/* 63 */xor__d__ac___in: n.ac                  = s.ac ^ s.in;                                                            ++n.pc; break;
/* 64 */xor__x__ac____d: n.ac                  = s.ac ^ s.d;                                                             ++n.pc; break;
/* 65 */xor__x__ac__ram: n.ac                  = s.ac ^ ram[s.x];                                                        ++n.pc; break;
/* 66 */xor__x__ac___ac: n.ac                  = s.ac ^ s.ac;                                                            ++n.pc; break;
/* 67 */xor__x__ac___in: n.ac                  = s.ac ^ s.in;                                                            ++n.pc; break;
/* 68 */xor_yd__ac____d: n.ac                  = s.ac ^ s.d;                                                             ++n.pc; break;
/* 69 */xor_yd__ac__ram: n.ac                  = s.ac ^ ram[(s.y << 8) | s.d];                                           ++n.pc; break;
/* 6A */xor_yd__ac___ac: n.ac                  = s.ac ^ s.ac;                                                            ++n.pc; break;
/* 6B */xor_yd__ac___in: n.ac                  = s.ac ^ s.in;                                                            ++n.pc; break;
/* 6C */xor_yx__ac____d: n.ac                  = s.ac ^ s.d;                                                             ++n.pc; break;
/* 6D */xor_yx__ac__ram: n.ac                  = s.ac ^ ram[(s.y << 8) | s.x];                                           ++n.pc; break;
/* 6E */xor_yx__ac___ac: n.ac                  = s.ac ^ s.ac;                                                            ++n.pc; break;
/* 6F */xor_yx__ac___in: n.ac                  = s.ac ^ s.in;                                                            ++n.pc; break;
/* 70 */xor__d___x____d: n.x                   = s.ac ^ s.d;                                                             ++n.pc; break;
/* 71 */xor__d___x__ram: n.x                   = s.ac ^ ram[s.d];                                                        ++n.pc; break;
/* 72 */xor__d___x___ac: n.x                   = s.ac ^ s.ac;                                                            ++n.pc; break;
/* 73 */xor__d___x___in: n.x                   = s.ac ^ s.in;                                                            ++n.pc; break;
/* 74 */xor__d___y____d: n.y                   = s.ac ^ s.d;                                                             ++n.pc; break;
/* 75 */xor__d___y__ram: n.y                   = s.ac ^ ram[s.d];                                                        ++n.pc; break;
/* 76 */xor__d___y___ac: n.y                   = s.ac ^ s.ac;                                                            ++n.pc; break;
/* 77 */xor__d___y___in: n.y                   = s.ac ^ s.in;                                                            ++n.pc; break;
/* 78 */xor__d_out____d: n.out                 = s.ac ^ s.d;                                                             ++n.pc; break;
/* 79 */xor__d_out__ram: n.out                 = s.ac ^ ram[s.d];                                                        ++n.pc; break;
/* 7A */xor__d_out___ac: n.out                 = s.ac ^ s.ac;                                                            ++n.pc; break;
/* 7B */xor__d_out___in: n.out                 = s.ac ^ s.in;                                                            ++n.pc; break;
/* 7C */xor_yx_out____d: n.out                 = s.ac ^ s.d;                                                      ++n.x; ++n.pc; break;
/* 7D */xor_yx_out__ram: n.out                 = s.ac ^ ram[(s.y << 8) | s.x];                                    ++n.x; ++n.pc; break;
/* 7E */xor_yx_out___ac: n.out                 = s.ac ^ s.ac;                                                     ++n.x; ++n.pc; break;
/* 7F */xor_yx_out___in: n.out                 = s.ac ^ s.in;                                                     ++n.x; ++n.pc; break;
/* 80 */add__d__ac____d: n.ac                  = s.ac + s.d;                                                             ++n.pc; break;
/* 81 */add__d__ac__ram: n.ac                  = s.ac + ram[s.d];                                                        ++n.pc; break;
/* 82 */add__d__ac___ac: n.ac                  = s.ac + s.ac;                                                            ++n.pc; break;
/* 83 */add__d__ac___in: n.ac                  = s.ac + s.in;                                                            ++n.pc; break;
/* 84 */add__x__ac____d: n.ac                  = s.ac + s.d;                                                             ++n.pc; break;
/* 85 */add__x__ac__ram: n.ac                  = s.ac + ram[s.x];                                                        ++n.pc; break;
/* 86 */add__x__ac___ac: n.ac                  = s.ac + s.ac;                                                            ++n.pc; break;
/* 87 */add__x__ac___in: n.ac                  = s.ac + s.in;                                                            ++n.pc; break;
/* 88 */add_yd__ac____d: n.ac                  = s.ac + s.d;                                                             ++n.pc; break;
/* 89 */add_yd__ac__ram: n.ac                  = s.ac + ram[(s.y << 8) | s.d];                                           ++n.pc; break;
/* 8A */add_yd__ac___ac: n.ac                  = s.ac + s.ac;                                                            ++n.pc; break;
/* 8B */add_yd__ac___in: n.ac                  = s.ac + s.in;                                                            ++n.pc; break;
/* 8C */add_yx__ac____d: n.ac                  = s.ac + s.d;                                                             ++n.pc; break;
/* 8D */add_yx__ac__ram: n.ac                  = s.ac + ram[(s.y << 8) | s.x];                                           ++n.pc; break;
/* 8E */add_yx__ac___ac: n.ac                  = s.ac + s.ac;                                                            ++n.pc; break;
/* 8F */add_yx__ac___in: n.ac                  = s.ac + s.in;                                                            ++n.pc; break;
/* 90 */add__d___x____d: n.x                   = s.ac + s.d;                                                             ++n.pc; break;
/* 91 */add__d___x__ram: n.x                   = s.ac + ram[s.d];                                                        ++n.pc; break;
/* 92 */add__d___x___ac: n.x                   = s.ac + s.ac;                                                            ++n.pc; break;
/* 93 */add__d___x___in: n.x                   = s.ac + s.in;                                                            ++n.pc; break;
/* 94 */add__d___y____d: n.y                   = s.ac + s.d;                                                             ++n.pc; break;
/* 95 */add__d___y__ram: n.y                   = s.ac + ram[s.d];                                                        ++n.pc; break;
/* 96 */add__d___y___ac: n.y                   = s.ac + s.ac;                                                            ++n.pc; break;
/* 97 */add__d___y___in: n.y                   = s.ac + s.in;                                                            ++n.pc; break;
/* 98 */add__d_out____d: n.out                 = s.ac + s.d;                                                             ++n.pc; break;
/* 99 */add__d_out__ram: n.out                 = s.ac + ram[s.d];                                                        ++n.pc; break;
/* 9A */add__d_out___ac: n.out                 = s.ac + s.ac;                                                            ++n.pc; break;
/* 9B */add__d_out___in: n.out                 = s.ac + s.in;                                                            ++n.pc; break;
/* 9C */add_yx_out____d: n.out                 = s.ac + s.d;                                                      ++n.x; ++n.pc; break;
/* 9D */add_yx_out__ram: n.out                 = s.ac + ram[(s.y << 8) | s.x];                                    ++n.x; ++n.pc; break;
/* 9E */add_yx_out___ac: n.out                 = s.ac + s.ac;                                                     ++n.x; ++n.pc; break;
/* 9F */add_yx_out___in: n.out                 = s.ac + s.in;                                                     ++n.x; ++n.pc; break;
/* A0 */sub__d__ac____d: n.ac                  = s.ac - s.d;                                                             ++n.pc; break;
/* A1 */sub__d__ac__ram: n.ac                  = s.ac - ram[s.d];                                                        ++n.pc; break;
/* A2 */sub__d__ac___ac: n.ac                  = s.ac - s.ac;                                                            ++n.pc; break;
/* A3 */sub__d__ac___in: n.ac                  = s.ac - s.in;                                                            ++n.pc; break;
/* A4 */sub__x__ac____d: n.ac                  = s.ac - s.d;                                                             ++n.pc; break;
/* A5 */sub__x__ac__ram: n.ac                  = s.ac - ram[s.x];                                                        ++n.pc; break;
/* A6 */sub__x__ac___ac: n.ac                  = s.ac - s.ac;                                                            ++n.pc; break;
/* A7 */sub__x__ac___in: n.ac                  = s.ac - s.in;                                                            ++n.pc; break;
/* A8 */sub_yd__ac____d: n.ac                  = s.ac - s.d;                                                             ++n.pc; break;
/* A9 */sub_yd__ac__ram: n.ac                  = s.ac - ram[(s.y << 8) | s.d];                                           ++n.pc; break;
/* AA */sub_yd__ac___ac: n.ac                  = s.ac - s.ac;                                                            ++n.pc; break;
/* AB */sub_yd__ac___in: n.ac                  = s.ac - s.in;                                                            ++n.pc; break;
/* AC */sub_yx__ac____d: n.ac                  = s.ac - s.d;                                                             ++n.pc; break;
/* AD */sub_yx__ac__ram: n.ac                  = s.ac - ram[(s.y << 8) | s.x];                                           ++n.pc; break;
/* AE */sub_yx__ac___ac: n.ac                  = s.ac - s.ac;                                                            ++n.pc; break;
/* AF */sub_yx__ac___in: n.ac                  = s.ac - s.in;                                                            ++n.pc; break;
/* B0 */sub__d___x____d: n.x                   = s.ac - s.d;                                                             ++n.pc; break;
/* B1 */sub__d___x__ram: n.x                   = s.ac - ram[s.d];                                                        ++n.pc; break;
/* B2 */sub__d___x___ac: n.x                   = s.ac - s.ac;                                                            ++n.pc; break;
/* B3 */sub__d___x___in: n.x                   = s.ac - s.in;                                                            ++n.pc; break;
/* B4 */sub__d___y____d: n.y                   = s.ac - s.d;                                                             ++n.pc; break;
/* B5 */sub__d___y__ram: n.y                   = s.ac - ram[s.d];                                                        ++n.pc; break;
/* B6 */sub__d___y___ac: n.y                   = s.ac - s.ac;                                                            ++n.pc; break;
/* B7 */sub__d___y___in: n.y                   = s.ac - s.in;                                                            ++n.pc; break;
/* B8 */sub__d_out____d: n.out                 = s.ac - s.d;                                                             ++n.pc; break;
/* B9 */sub__d_out__ram: n.out                 = s.ac - ram[s.d];                                                        ++n.pc; break;
/* BA */sub__d_out___ac: n.out                 = s.ac - s.ac;                                                            ++n.pc; break;
/* BB */sub__d_out___in: n.out                 = s.ac - s.in;                                                            ++n.pc; break;
/* BC */sub_yx_out____d: n.out                 = s.ac - s.d;                                                      ++n.x; ++n.pc; break;
/* BD */sub_yx_out__ram: n.out                 = s.ac - ram[(s.y << 8) | s.x];                                    ++n.x; ++n.pc; break;
/* BE */sub_yx_out___ac: n.out                 = s.ac - s.ac;                                                     ++n.x; ++n.pc; break;
/* BF */sub_yx_out___in: n.out                 = s.ac - s.in;                                                     ++n.x; ++n.pc; break;
/* C0 */ st__d________d: ram[s.d]              =        s.d;                                                             ++n.pc; break;
/* C1 */ st__d______ram: ram[s.d]              =        s.bus; /* floating bus (random value) */                         ++n.pc; break;
/* C2 */ st__d_______ac: ram[s.d]              =        s.ac;                                                            ++n.pc; break;
/* C3 */ st__d_______in: ram[s.d]              =        s.in;                                                            ++n.pc; break;
/* C4 */ st__x________d: ram[s.x]              =        s.d;                                                             ++n.pc; break;
/* C5 */ st__x______ram: ram[s.x]              =        s.bus; /* floating bus (random value) */                         ++n.pc; break;
/* C6 */ st__x_______ac: ram[s.x]              =        s.ac;                                                            ++n.pc; break;
/* C7 */ st__x_______in: ram[s.x]              =        s.in;                                                            ++n.pc; break;
/* C8 */ st_yd________d: ram[(s.y << 8) | s.d] =        s.d;                                                             ++n.pc; break;
/* C9 */ st_yd______ram: ram[(s.y << 8) | s.d] =        s.bus; /* floating bus (random value) */                         ++n.pc; break;
/* CA */ st_yd_______ac: ram[(s.y << 8) | s.d] =        s.ac;                                                            ++n.pc; break;
/* CB */ st_yd_______in: ram[(s.y << 8) | s.d] =        s.in;                                                            ++n.pc; break;
/* CC */ st_yx________d: ram[(s.y << 8) | s.x] =        s.d;                                                             ++n.pc; break;
/* CD */ st_yx______ram: ram[(s.y << 8) | s.x] =        s.bus; /* floating bus (random value) */                         ++n.pc; break;
/* CE */ st_yx_______ac: ram[(s.y << 8) | s.x] =        s.ac;                                                            ++n.pc; break;
/* CF */ st_yx_______in: ram[(s.y << 8) | s.x] =        s.in;                                                            ++n.pc; break;
/* D0 */ st__d___x____d: ram[s.d]              =        s.d;                   n.x = s.d;                                ++n.pc; break;
/* D1 */ st__d___x__ram: ram[s.d]              =        0;                     n.x = 0;                                  ++n.pc; break;
/* D2 */ st__d___x___ac: ram[s.d]              =        s.ac;                  n.x = s.ac;                               ++n.pc; break;
/* D3 */ st__d___x___in: ram[s.d]              =        s.in;                  n.x = s.in;                               ++n.pc; break;
/* D4 */ st__d___y____d: ram[s.d]              =        s.d;                   n.y = s.d;                                ++n.pc; break;
/* D5 */ st__d___y__ram: ram[s.d]              =        0;                     n.y = 0;                                  ++n.pc; break;
/* D6 */ st__d___y___ac: ram[s.d]              =        s.ac;                  n.y = s.ac;                               ++n.pc; break;
/* D7 */ st__d___y___in: ram[s.d]              =        s.in;                  n.y = s.in;                               ++n.pc; break;
/* D8 */ st__d___2____d: ram[s.d]              =        s.d;                                                             ++n.pc; break;
/* D9 */ st__d___2__ram: ram[s.d]              =        0;                                                               ++n.pc; break;
/* DA */ st__d___2___ac: ram[s.d]              =        s.ac;                                                            ++n.pc; break;
/* DB */ st__d___2___in: ram[s.d]              =        s.in;                                                            ++n.pc; break;
/* DC */ st_yx___2____d: ram[(s.y << 8) | s.x] =        s.d;                                                      ++n.x; ++n.pc; break;
/* DD */ st_yx___2__ram: ram[(s.y << 8) | s.x] =        0;                                                        ++n.x; ++n.pc; break;
/* DE */ st_yx___2___ac: ram[(s.y << 8) | s.x] =        s.ac;                                                     ++n.x; ++n.pc; break;
/* DF */ st_yx___2___in: ram[(s.y << 8) | s.x] =        s.in;                                                     ++n.x; ++n.pc; break;
/* E0 */jmp__far______d:                                                     n.pc = (s.y << 8)      | s.d;                       break;
/* E1 */jmp__far____ram:                                                     n.pc = (s.y << 8)      | ram[s.d];                  break;
/* E2 */jmp__far_____ac:                                                     n.pc = (s.y << 8)      | s.ac;                      break;
/* E3 */jmp__far_____in:                                                     n.pc = (s.y << 8)      | s.in;                      break;
/* E4 */jmp_near_gt___d: if (1 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | s.d;      else     ++n.pc; break;
/* E5 */jmp_near_gt_ram: if (1 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | ram[s.d]; else     ++n.pc; break;
/* E6 */jmp_near_gt__ac: if (1 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | s.ac;     else     ++n.pc; break;
/* E7 */jmp_near_gt__in: if (1 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | s.in;     else     ++n.pc; break;
/* E8 */jmp_near_lt___d: if (2 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | s.d;      else     ++n.pc; break;
/* E9 */jmp_near_lt_ram: if (2 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | ram[s.d]; else     ++n.pc; break;
/* EA */jmp_near_lt__ac: if (2 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | s.ac;     else     ++n.pc; break;
/* EB */jmp_near_lt__in: if (2 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | s.in;     else     ++n.pc; break;
/* EC */jmp_near_ne___d: if (3 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | s.d;      else     ++n.pc; break;
/* ED */jmp_near_ne_ram: if (3 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | ram[s.d]; else     ++n.pc; break;
/* EE */jmp_near_ne__ac: if (3 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | s.ac;     else     ++n.pc; break;
/* EF */jmp_near_ne__in: if (3 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | s.in;     else     ++n.pc; break;
/* F0 */jmp_near_eq___d: if (4 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | s.d;      else     ++n.pc; break;
/* F1 */jmp_near_eq_ram: if (4 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | ram[s.d]; else     ++n.pc; break;
/* F2 */jmp_near_eq__ac: if (4 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | s.ac;     else     ++n.pc; break;
/* F3 */jmp_near_eq__in: if (4 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | s.in;     else     ++n.pc; break;
/* F4 */jmp_near_ge___d: if (5 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | s.d;      else     ++n.pc; break;
/* F5 */jmp_near_ge_ram: if (5 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | ram[s.d]; else     ++n.pc; break;
/* F6 */jmp_near_ge__ac: if (5 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | s.ac;     else     ++n.pc; break;
/* F7 */jmp_near_ge__in: if (5 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | s.in;     else     ++n.pc; break;
/* F8 */jmp_near_le___d: if (6 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | s.d;      else     ++n.pc; break;
/* F9 */jmp_near_le_ram: if (6 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | ram[s.d]; else     ++n.pc; break;
/* FA */jmp_near_le__ac: if (6 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | s.ac;     else     ++n.pc; break;
/* FB */jmp_near_le__in: if (6 & (1 << ((s.ac >> 7) + ( ! s.ac << 1)) ) )    n.pc = (s.pc & 0xff00) | s.in;     else     ++n.pc; break;
/* FC */jmp_near______d:                                                     n.pc = (s.pc & 0xff00) | s.d;                       break;
/* FD */jmp_near____ram:                                                     n.pc = (s.pc & 0xff00) | ram[s.d];                  break;
/* FE */jmp_near_____ac:                                                     n.pc = (s.pc & 0xff00) | s.ac;                      break;
/* FF */jmp_near_____in:                                                     n.pc = (s.pc & 0xff00) | s.in;                      break;

		}

		// Screen
		{
			const uint8_t out = n.out;
			const uint8_t outPrev = s.out;
			
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
				n.bus = rand() & 0xff;
			}

			QRgb c;
		//QRgb c = 0;
		//QRgb c = qRgb(((out & 0x3) << 6), ((out & (0x3 << 2)) << 4), ((out & (0x3 << 4)) << 2));
			
			if ( ! (vSync || hSync) )
			{
				c = ((out & 0x3) << 22) | ((out & (0x3 << 2)) << 12) | ((out & (0x3 << 4)) << 2);
/*
				if (out & 0x3)
					c |= 0x004f0000;
				if (out & (0x3 << 2))
					c |= 0x00004f00;
				if (out & (0x3 << 4))
					c |= 0x0000004f;
*/
			}
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
		
		s = n;
		
		++tickCount;
	}
}
