#ifndef GIGA_EMUL_H
#define GIGA_EMUL_H

#include <QImage>

#include "gigastate.h"

class GigaEmul
{
	public:
		virtual ~GigaEmul() {}
		
	public:
		virtual void reset() = 0;
		virtual void save() =0;
		virtual void setInput(uint8_t input) =0;
		virtual void proc(uint64_t ticks) =0;
		virtual uint64_t getTickCount() const =0;
		virtual const uint16_t *getRom() const=0;
		virtual const uint8_t *getRam() const=0;
		virtual const uint8_t *getRamSave() const=0;
		virtual const GigaState &getState() const =0;
		virtual const QImage &getScreen() const =0;
		
	public:
		static const uint32_t romSizeWords = 1 << 16;
		static const uint32_t ramSizeBytes = 1 << 16;
		
	public: // CPU
		static const uint8_t ir_instruction_shift	= 5;
		static const uint8_t ir_instruction_mask	= (0x7 << 5);
		static const uint8_t ir_instruction_ld		= (0x0 << 5);
		static const uint8_t ir_instruction_and		= (0x1 << 5);
		static const uint8_t ir_instruction_or		= (0x2 << 5);
		static const uint8_t ir_instruction_xor		= (0x3 << 5);
		static const uint8_t ir_instruction_add		= (0x4 << 5);
		static const uint8_t ir_instruction_sub		= (0x5 << 5);
		static const uint8_t ir_instruction_st		= (0x6 << 5);
		static const uint8_t ir_instruction_jmp		= (0x7 << 5);
		
		static const uint8_t ir_addressing_shift	= 2;
		static const uint8_t ir_addressing_mask		= (0x7 << 2);
		static const uint8_t ir_addressing_d_ac		= (0x0 << 2);
		static const uint8_t ir_addressing_x_ac		= (0x1 << 2);
		static const uint8_t ir_addressing_yd_ac	= (0x2 << 2);
		static const uint8_t ir_addressing_yx_ac	= (0x3 << 2);
		static const uint8_t ir_addressing_d_x		= (0x4 << 2);
		static const uint8_t ir_addressing_d_y		= (0x5 << 2);
		static const uint8_t ir_addressing_d_out	= (0x6 << 2);
		static const uint8_t ir_addressing_yx_out	= (0x7 << 2);
		
		static const uint8_t ir_jump_shifr				= 2;
		static const uint8_t ir_jump_mask					= (0x7 << 2);
		static const uint8_t ir_jump_far					= (0x0 << 2);
		static const uint8_t ir_jump_near_gt			= (0x1 << 2);
		static const uint8_t ir_jump_near_lt			= (0x2 << 2);
		static const uint8_t ir_jump_near_ne			= (0x3 << 2);
		static const uint8_t ir_jump_near_eq			= (0x4 << 2);
		static const uint8_t ir_jump_near_ge			= (0x5 << 2);
		static const uint8_t ir_jump_near_le			= (0x6 << 2);
		static const uint8_t ir_jump_near					= (0x7 << 2);
		
		static const uint8_t ir_bus_mode_shift		= 0;
		static const uint8_t ir_bus_mode_mask			= 0x3;
		static const uint8_t ir_bus_mode_d_bus		= 0x0;
		static const uint8_t ir_bus_mode_ram_bus	= 0x1;
		static const uint8_t ir_bus_mode_ac_bus		= 0x2;
		static const uint8_t ir_bus_mode_in_bus		= 0x3;

	public: // VGA
		static constexpr int vgaLinesBackPorch		= 33; 	// top border
		static constexpr int vgaLinesVisible			= 480;	// scan lines
		static constexpr int vgaLinesSyncPulse		= 2;		// retrace
		static constexpr int vgaLinesFrontPorch		= 10;		// bottom border
		static constexpr int vgaLines							= 525;
		
		static constexpr int vgaPixelsBackPorch		= 48;		// left
		static constexpr int vgaPixelsVisible			= 640;	// visible area
		static constexpr int vgaPixelsSyncPulse		= 96;		// retrace
		static constexpr int vgaPixelsFrontPorch	= 26;		// right
		static constexpr int vgaPixels						= 800;
		
		static constexpr uint8_t vSyncBitmask = 0x80;
		static constexpr uint8_t hSyncBitmask = 0x40;
		
		static constexpr QRgb vSyncColor = qRgb(0, 255, 0);
		static constexpr QRgb hSyncColor = qRgb(0, 0, 255);
		static constexpr QRgb vhSyncColor = qRgb(0, 255, 255);
};

#endif // GIGA_EMUL_H
