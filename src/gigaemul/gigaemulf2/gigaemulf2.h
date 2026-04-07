#ifndef GIGA_EMUL_F2_H
#define GIGA_EMUL_F2_H

#include <QImage>

#include "gigaemul.h"

class GigaEmulF2 : public GigaEmul
{
	public:
		GigaEmulF2();
		virtual ~GigaEmulF2();
	private:
		void init();
		virtual void reset();
		virtual void save();
		virtual void setInput(uint8_t input);
		virtual void proc(uint64_t ticks);
		virtual uint64_t getTickCount() const { return tickCount; }
		virtual const uint16_t *getRom() const { return rom; }
		virtual const uint8_t *getRam() const { return ram; }
		virtual const uint8_t *getRamSave() const { return ramSave; }
		virtual const GigaState &getState() const { return state; }
		virtual const QImage &getScreen() const { return screen; }
		void drawScreen(GigaState &next, GigaState &state);
	private: // CPU
		uint16_t *rom;
		uint8_t *ram;
		uint8_t *ramSave;
		GigaState state;
		
	private: // VGA
		QImage screen;
		QRgb *pixels;
		QRgb *d;
		int x;
		int y;
		
	private:
		int32_t romReadBytes;
		uint64_t tickCount;
};

#endif // GIGA_EMUL_H
