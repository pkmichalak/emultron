#include <QMessageBox>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLineEdit>
#include <QInputDialog>
#include <QStackedLayout>
#include <QMainWindow>
#include <QStatusBar>
#include <QLabel>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QSplitter>
#include <QPainter>
#include <QToolBox>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QCheckBox>
#include <QTimer>
#include <QComboBox>

#include "gigaemuldebugwidget.h"
#include "screendebugwidget.h"

#include "gigaemul.h"
#include "gigaemulf2.h"

constexpr char *GigaEmulDebugWidget::regName[];

/*
 *
 */
GigaEmulDebugWidget::GigaEmulDebugWidget(GigaEmul *gigaEmul, ScreenDebugWidget *screen, QWidget *parent)
	: QWidget(parent),
		tProcAuto(0)
{
	this->gigaEmul = gigaEmul;
	this->screen = screen;
//	setFocusPolicy(Qt::StrongFocus);
	
	createControls();
	configureControls();
	loadRom();
	loadRam();
	loadRegisters();
	updateRegs();
	updateAddressing();
	updateTickCount();
	screen->setImage(gigaEmul->getScreen());
	
	connect(pbReset, SIGNAL(clicked()), SLOT(onPbResetClicked()));
	connect(pbProcTick, SIGNAL(clicked()), SLOT(onPbProcTickClicked()));
	connect(pbProcTicks, SIGNAL(clicked()), SLOT(onPbProcTicksClicked()));
	
	connect(cbProcAuto, SIGNAL(stateChanged(int)), SLOT(onCbProcAutoStateChanged(int)));
	connect(cbFps, SIGNAL(currentIndexChanged(int)), SLOT(onCbFpsCurrentIndexChanged(int)));
	
	tProcAuto = new QTimer;
	connect(tProcAuto, SIGNAL(timeout()), SLOT(onProcAutoTimeout()));
}

/*
 *
 */
GigaEmulDebugWidget::~GigaEmulDebugWidget()
{
	delete gigaEmul;
}

/*
 *
 */
void
GigaEmulDebugWidget::createControls()
{
	cbRegisters = new QCheckBox(tr("Registers"));
	twRegisters = new QTableWidget;
	glRegisters = new QGridLayout;
	glRegisters->addWidget(cbRegisters, 0, 0);
	glRegisters->addWidget(twRegisters, 1, 0);
	glRegisters->setMargin(0);
	glRegisters->setSpacing(0);
	wRegisters = new QWidget;
	wRegisters->setLayout(glRegisters);

	lButtons = new QLabel(tr("Input"));
	pbLeft = new QPushButton(tr("Left"));
	pbRight = new QPushButton(tr("Right"));
	pbUp = new QPushButton(tr("Up"));
	pbDown = new QPushButton(tr("Down"));
	pbA = new QPushButton(tr("A"));
	pbB = new QPushButton(tr("B"));
	pbSelect = new QPushButton(tr("Select"));
	pbStart = new QPushButton(tr("Start"));
	glButtons = new QGridLayout;
	glButtons->addWidget(lButtons, 0, 0);
	glButtons->addWidget(pbLeft, 1, 0);
	glButtons->addWidget(pbRight, 1, 2);
	glButtons->addWidget(pbUp, 0, 1);
	glButtons->addWidget(pbDown, 1, 1);
	glButtons->addWidget(pbSelect, 0, 3);
	glButtons->addWidget(pbStart, 1, 3);
	glButtons->addWidget(pbA, 0, 4);
	glButtons->addWidget(pbB, 1, 4);

	cbAddressing = new QCheckBox(tr("Addressing"));
	twAddressing = new QTableWidget;
	glAddressing = new QGridLayout;
	glAddressing->addWidget(cbAddressing, 0, 0);
	glAddressing->addWidget(twAddressing, 1, 0);
	glAddressing->addLayout(glButtons, 2, 0);
	glAddressing->setMargin(0);
	glAddressing->setSpacing(0);
	wAddressing = new QWidget;
	wAddressing->setLayout(glAddressing);
	
	sLeft = new QSplitter(Qt::Vertical);
	sLeft->addWidget(wRegisters);
	sLeft->addWidget(wAddressing);
	sLeft->setStretchFactor(0, 2);
	sLeft->setStretchFactor(1, 1);
	
	glLeft = new QGridLayout;
	glLeft->addWidget(sLeft, 0, 0);
	glLeft->setMargin(0);
	glLeft->setSpacing(0);
	wLeft = new QWidget;
	wLeft->setLayout(glLeft);
	
	cbRam = new QCheckBox(tr("RAM"));
	twRam = new QTableWidget;
	glRam = new QGridLayout;
	glRam->addWidget(cbRam, 0, 0);
	glRam->addWidget(twRam, 1, 0);
	glRam->setMargin(0);
	glRam->setSpacing(0);
	wRam = new QWidget;
	wRam->setLayout(glRam);
	
	cbRom = new QCheckBox(tr("ROM"));
	twRom = new QTableWidget;
	glRom = new QGridLayout;
	glRom->addWidget(cbRom, 0, 0);
	glRom->addWidget(twRom, 1, 0);
	glRom->setMargin(0);
	glRom->setSpacing(0);
	wRom = new QWidget;
	wRom->setLayout(glRom);
	
	sTop = new QSplitter(Qt::Horizontal);
	sTop->addWidget(wLeft);
	sTop->addWidget(wRam);
	sTop->addWidget(wRom);
	sTop->setStretchFactor(0, 5);
	sTop->setStretchFactor(1, 3);
	sTop->setStretchFactor(2, 16);
	
	pbReset = new QPushButton(tr("Reset"));
	lTicks = new QLabel(tr("Ticks"));
	leTickCount = new QLineEdit;
	pbProcTick = new QPushButton(tr("+1 Tick"));
	sbTicks = new QSpinBox;
	pbProcTicks = new QPushButton(tr("Proc ticks"));
	sbProcInterval = new QSpinBox;
	cbProcAuto = new QCheckBox(tr("Auto"));
	cbFps = new QComboBox;
	lMilliseconds = new QLabel(tr("0 ms"));

	glProc = new QGridLayout;
	glProc->addWidget(pbReset, 0, 0);
	glProc->addWidget(lTicks, 0, 1);
	glProc->addWidget(leTickCount, 0, 2);
	glProc->addWidget(pbProcTick, 0, 3);
	glProc->addWidget(sbTicks, 0, 4);
	glProc->addWidget(pbProcTicks, 0, 5);
	glProc->addWidget(sbProcInterval, 0, 6);
	glProc->addWidget(cbProcAuto, 0, 7);
	glProc->addWidget(cbFps, 0, 8);
	glProc->addWidget(lMilliseconds, 0, 9);
	glProc->setColumnStretch(10, 1);
	
	glMain = new QGridLayout;
	glMain->addWidget(sTop, 0, 0);
	glMain->addLayout(glProc, 2, 0, 1, 3);
	glMain->setMargin(0);
	
	setLayout(glMain);
}

/*
 *
 */
void
GigaEmulDebugWidget::configureControls()
{
	cbRegisters->setChecked(true);
	cbAddressing->setChecked(true);
	cbRam->setChecked(true);
	cbRom->setChecked(true);
	
	pbLeft->setCheckable(true);
	pbRight->setCheckable(true);
	pbUp->setCheckable(true);
	pbDown->setCheckable(true);
	pbA->setCheckable(true);
	pbB->setCheckable(true);
	pbSelect->setCheckable(true);
	pbStart->setCheckable(true);
	
	twRegisters->setRowCount(1 + regIdxLast);
	twRegisters->setColumnCount(4);
	
	twRegisters->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Reg")));
	twRegisters->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Hex")));
	twRegisters->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Dec")));
	twRegisters->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Decode")));
	
	for (int i = 0; i <= regIdxLast; ++i)
	{
		twRegisters->setItem(i, 0, new QTableWidgetItem(tr(regName[i])));
		twRegisters->setItem(i, 1, new QTableWidgetItem(tr("-")));
		twRegisters->setItem(i, 2, new QTableWidgetItem(tr("-")));
		twRegisters->setItem(i, 3, new QTableWidgetItem(tr("-")));
	}
	
	twAddressing->setRowCount(4);
	twAddressing->setColumnCount(2);
	twAddressing->setHorizontalHeaderItem(0, new QTableWidgetItem(QString("Hex")));
	twAddressing->setHorizontalHeaderItem(1, new QTableWidgetItem(QString("Dec")));
	twAddressing->setVerticalHeaderItem(0, new QTableWidgetItem(tr("[D]")));
	twAddressing->setVerticalHeaderItem(1, new QTableWidgetItem(tr("[X]")));
	twAddressing->setVerticalHeaderItem(2, new QTableWidgetItem(tr("[Y:D]")));
	twAddressing->setVerticalHeaderItem(3, new QTableWidgetItem(tr("[Y:X]")));
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 2; ++j)
			twAddressing->setItem(i, j, new QTableWidgetItem(QString("-")));
	
	twRam->setRowCount(GigaEmul::ramSizeBytes);
	twRam->setColumnCount(2);
	twRam->setHorizontalHeaderItem(0, new QTableWidgetItem(QString("Hex")));
	twRam->setHorizontalHeaderItem(1, new QTableWidgetItem(QString("Dec")));
	for (uint32_t i = 0; i < GigaEmul::ramSizeBytes; ++i)
	{
		twRam->setVerticalHeaderItem(i, new QTableWidgetItem(QString("%1").arg(i, 4, 16, QChar('0'))));
		for (int j = 0; j < 2; ++j)
			twRam->setItem(i, j, new QTableWidgetItem(QString("-")));
	}
	
	twRom->setRowCount(GigaEmul::romSizeWords);
	twRom->setColumnCount(7);
	twRom->setHorizontalHeaderItem(0, new QTableWidgetItem(QString("[0] Hex")));
	twRom->setHorizontalHeaderItem(1, new QTableWidgetItem(QString("[1] Hex")));
	twRom->setHorizontalHeaderItem(2, new QTableWidgetItem(QString("Inst")));
	twRom->setHorizontalHeaderItem(3, new QTableWidgetItem(QString("Addr")));
	twRom->setHorizontalHeaderItem(4, new QTableWidgetItem(QString("-> Bus")));
	twRom->setHorizontalHeaderItem(5, new QTableWidgetItem(QString("Asm")));
	twRom->setHorizontalHeaderItem(6, new QTableWidgetItem(QString("[1] Dec")));
	for (uint32_t i = 0; i < GigaEmul::romSizeWords; ++i)
	{
		twRom->setVerticalHeaderItem(i, new QTableWidgetItem(QString("%1").arg(i, 4, 16, QChar('0'))));
		for (int j = 0; j < 7; ++j)
			twRom->setItem(i, j, new QTableWidgetItem(QString("-")));
	}
	
	leTickCount->setReadOnly(true);
	sbTicks->setRange(1, 100000000);
//sbTicks->setValue(800 * 525 * 60 / 4);	// 800 pixels * 525 lines * 60 Hz / 4
	sbTicks->setValue(6250000);
	sbTicks->setPrefix(tr("+ "));
	
	sbProcInterval->setRange(1, 100000);
	sbProcInterval->setValue(1000);
	sbProcInterval->setSuffix(tr("ms"));
	
	cbFps->addItem(tr("1 FPS"),  QVariant(QList<QVariant>() << QVariant(6250000) << QVariant(1000)));
	cbFps->addItem(tr("2 FPS"),  QVariant(QList<QVariant>() << QVariant(3125000) << QVariant(500)));
	cbFps->addItem(tr("5 FPS"),  QVariant(QList<QVariant>() << QVariant(1250000) << QVariant(200)));
	cbFps->addItem(tr("10 FPS"), QVariant(QList<QVariant>() << QVariant(625000)  << QVariant(100)));
	cbFps->addItem(tr("20 FPS"), QVariant(QList<QVariant>() << QVariant(312500)  << QVariant(50)));
	cbFps->addItem(tr("30 FPS"), QVariant(QList<QVariant>() << QVariant(208333)  << QVariant(33)));
	cbFps->addItem(tr("60 FPS"), QVariant(QList<QVariant>() << QVariant(104167)  << QVariant(16)));
}

/*
 *
 */
void
GigaEmulDebugWidget::updateRegs()
{
	const GigaState &state = gigaEmul->getState();
	
	// PC
	{
		const int idx = pcIdx;
		const uint16_t val = state.pc;
		twRegisters->item(idx, 1)->setText(QString("%1").arg((uint) val, 4, 16, QChar('0')));
		twRegisters->item(idx, 2)->setText(QString("%1").arg((uint) val));
	}
	
	QString asem;
	decodeAsem(state.ir, state.d, asem);
	// PC-1
	{
		const int idx = pc_1Idx;
		const uint16_t val = state.pc_1;
		twRegisters->item(idx, 1)->setText(QString("%1").arg((uint) val, 4, 16, QChar('0')));
		twRegisters->item(idx, 2)->setText(QString("%1").arg((uint) val));
		twRegisters->item(idx, 3)->setText(asem);
	}
	
	// IR
	{
		const int idx = irIdx;
		const uint8_t val = state.ir;
		twRegisters->item(idx, 1)->setText(QString("%1").arg((uint) val, 2, 16, QChar('0')));
		twRegisters->item(idx, 2)->setText(QString("%1").arg((uint) val));
	}

	// D
	{
		const int idx = dIdx;
		const uint8_t val = state.d;
		twRegisters->item(idx, 1)->setText(QString("%1").arg((uint) val, 2, 16, QChar('0')));
		twRegisters->item(idx, 2)->setText(QString("%1").arg((uint) val));
	}
	
	// AC
	{
		const int idx = acIdx;
		const uint8_t val = state.ac;
		twRegisters->item(idx, 1)->setText(QString("%1").arg((uint) val, 2, 16, QChar('0')));
		twRegisters->item(idx, 2)->setText(QString("%1").arg((uint) val));
	}
	
	// X
	{
		const int idx = xIdx;
		const uint8_t val = state.x;
		twRegisters->item(idx, 1)->setText(QString("%1").arg((uint) val, 2, 16, QChar('0')));
		twRegisters->item(idx, 2)->setText(QString("%1").arg((uint) val));
	}
	
	// Y
	{
		const int idx = yIdx;
		const uint8_t val = state.y;
		twRegisters->item(idx, 1)->setText(QString("%1").arg((uint) val, 2, 16, QChar('0')));
		twRegisters->item(idx, 2)->setText(QString("%1").arg((uint) val));
	}
	
	// OUT
	{
		const int idx = outIdx;
		const uint8_t val = state.out;
		twRegisters->item(idx, 1)->setText(QString("%1").arg((uint) val, 2, 16, QChar('0')));
		twRegisters->item(idx, 2)->setText(QString("%1").arg((uint) val));
	}
	
	// IN
	{
		const int idx = inIdx;
		const uint8_t val = state.in;
		twRegisters->item(idx, 1)->setText(QString("%1").arg((uint) val, 2, 16, QChar('0')));
		twRegisters->item(idx, 2)->setText(QString("%1").arg((uint) val));
	}

	// BUS
	{
		const int idx = busIdx;
		const uint8_t val = state.bus;
		twRegisters->item(idx, 1)->setText(QString("%1").arg((uint) val, 2, 16, QChar('0')));
		twRegisters->item(idx, 2)->setText(QString("%1").arg((uint) val));
	}
}

/*
 *
 */
void
GigaEmulDebugWidget::updateAddressing()
{
	const GigaState &state = gigaEmul->getState();
	const uint8_t *ram = gigaEmul->getRam();
	
	// [D]
	{
		const int idx = 0;
		const uint16_t val = ram[state.d];
		twAddressing->item(idx, 0)->setText(QString("%1").arg((uint) val, 2, 16, QChar('0')));
		twAddressing->item(idx, 1)->setText(QString("%1").arg((uint) val));
	}

	// [X]
	{
		const int idx = 1;
		const uint16_t val = ram[state.x];
		twAddressing->item(idx, 0)->setText(QString("%1").arg((uint) val, 2, 16, QChar('0')));
		twAddressing->item(idx, 1)->setText(QString("%1").arg((uint) val));
	}

	// [Y:D]
	{
		const int idx = 2;
		const uint16_t val = ram[(state.y << 8) | state.d];
		twAddressing->item(idx, 0)->setText(QString("%1").arg((uint) val, 2, 16, QChar('0')));
		twAddressing->item(idx, 1)->setText(QString("%1").arg((uint) val));
	}

	// [Y:X]
	{
		const int idx = 3;
		const uint16_t val = ram[(state.y << 8) | state.x];
		twAddressing->item(idx, 0)->setText(QString("%1").arg((uint) val, 2, 16, QChar('0')));
		twAddressing->item(idx, 1)->setText(QString("%1").arg((uint) val));
	}
}

/*
 * 
 */
void
GigaEmulDebugWidget::decodeOpcode(uint8_t opcode,
																	QString &instruction,
																	QString &addressing,
																	QString &bus_mode)
{
	static const QString instructions[] = {
		tr("LD"), tr("AND"), tr("OR"), tr("XOR"), tr("ADD"), tr("SUB"), tr("ST"), tr("JMP")
	};
		
	static const QString addressings[] = {
		tr("[0:D]->BUS, ALU->AC"),
		tr("[0:X]->BUS, ALU->AC"),
		tr("[Y:D]->BUS, ALU->AC"),
		tr("[Y:X]->BUS, ALU->AC"),
		tr("[0:D]->BUS, ALU->X"),
		tr("[0:D]->BUS, ALU->Y"),
		tr("[0:D]->BUS, ALU->OUT"),
		tr("[Y:X++]->BUS, ALU->OUT")
	};
	
	static const QString st_addressings[] = {
		tr("BUS->[0:D]"),
		tr("BUS->[0:X]"),
		tr("BUS->[Y:D]"),
		tr("BUS->[Y:X]"),
		tr("BUS->[0:D], ALU->X"),
		tr("BUS->[0:D], ALU->Y"),
		tr("BUS->[0:D]"),
		tr("BUS->[Y:X++]")
	};
	
	static const QString jumps[] = {
		tr("FAR Y:BUS"),
		tr("NEAR GT PC:BUS"),
		tr("NEAR LT PC:BUS"),
		tr("NEAR NE PC:BUS"),
		tr("NEAR EQ PC:BUS"),
		tr("NEAR GE PC:BUS"),
		tr("NEAR LE PC:BUS"),
		tr("NEAR PC:BUS")
	};
	
	static const QString bus_modes[] = {
		tr("D"), tr("[RAM]"), tr("AC"), tr("IN")
	};

	// instruction
	instruction = instructions[(opcode & GigaEmul::ir_instruction_mask) >> GigaEmul::ir_instruction_shift];

	if ( (opcode & GigaEmul::ir_instruction_mask) == GigaEmul::ir_instruction_st )
	{
		addressing = st_addressings[(opcode & GigaEmul::ir_addressing_mask) >> GigaEmul::ir_addressing_shift];
	}
	else if ( (opcode & GigaEmul::ir_instruction_mask) == GigaEmul::ir_instruction_jmp )
	{
		addressing = jumps[(opcode & GigaEmul::ir_addressing_mask) >> GigaEmul::ir_addressing_shift];
	}
	else
	{
		addressing = addressings[(opcode & GigaEmul::ir_addressing_mask) >> GigaEmul::ir_addressing_shift];
	}
		
	bus_mode = bus_modes[(opcode & GigaEmul::ir_bus_mode_mask) >> GigaEmul::ir_bus_mode_shift];
}

/*
 * 
 */
void
GigaEmulDebugWidget::decodeAsem(uint8_t opcode, uint8_t data, QString &asem)
{
							 QString addressing_in_d_ac(tr("[%1]").arg(data));
	static const QString addressing_in_x_ac(tr("[X]"));
							 QString addressing_in_yd_ac(tr("[Y:%1]").arg(data));
	static const QString addressing_in_yx_ac(tr("[Y:X]"));
							 QString addressing_in_d_x(tr("[%1]").arg(data));
							 QString addressing_in_d_y(tr("[%1]").arg(data));
							 QString addressing_in_d_out(tr("[%1]").arg(data));
	static const QString addressing_in_yx_out(tr("[Y:X++]"));

	static const QString addressing_out_d_ac(tr("AC"));
	static const QString addressing_out_x_ac(tr("AC"));
	static const QString addressing_out_yd_ac(tr("AC"));
	static const QString addressing_out_yx_ac(tr("AC"));
	static const QString addressing_out_d_x(tr("X"));
	static const QString addressing_out_d_y(tr("Y"));
	static const QString addressing_out_d_out(tr("OUT"));
	static const QString addressing_out_yx_out(tr("OUT"));
	
	       const QString bus_mode_d_bus(tr("%1").arg(data));
	static const QString bus_mode_ram_bus(tr("RAM"));
	static const QString bus_mode_ac_bus(tr("AC"));
	static const QString bus_mode_in_bus(tr("IN"));

	// addressing
	QString addressing_in;
	switch ( opcode & GigaEmul::ir_addressing_mask )
	{
		case GigaEmul::ir_addressing_d_ac:		addressing_in = addressing_in_d_ac;		break;
		case GigaEmul::ir_addressing_x_ac:		addressing_in = addressing_in_x_ac;		break;
		case GigaEmul::ir_addressing_yd_ac:		addressing_in = addressing_in_yd_ac;	break;
		case GigaEmul::ir_addressing_yx_ac:		addressing_in = addressing_in_yx_ac;	break;
		case GigaEmul::ir_addressing_d_x:			addressing_in = addressing_in_d_x;		break;
		case GigaEmul::ir_addressing_d_y:			addressing_in = addressing_in_d_y;		break;
		case GigaEmul::ir_addressing_d_out:		addressing_in = addressing_in_d_out;	break;
		case GigaEmul::ir_addressing_yx_out:	addressing_in = addressing_in_yx_out;	break;
	};
	
	// addressing
	QString addressing_out;
	switch ( opcode & GigaEmul::ir_addressing_mask )
	{
		case GigaEmul::ir_addressing_d_ac:		addressing_out = addressing_out_d_ac;		break;
		case GigaEmul::ir_addressing_x_ac:		addressing_out = addressing_out_x_ac;		break;
		case GigaEmul::ir_addressing_yd_ac:		addressing_out = addressing_out_yd_ac;	break;
		case GigaEmul::ir_addressing_yx_ac:		addressing_out = addressing_out_yx_ac;	break;
		case GigaEmul::ir_addressing_d_x:			addressing_out = addressing_out_d_x;		break;
		case GigaEmul::ir_addressing_d_y:			addressing_out = addressing_out_d_y;		break;
		case GigaEmul::ir_addressing_d_out:		addressing_out = addressing_out_d_out;	break;
		case GigaEmul::ir_addressing_yx_out:	addressing_out = addressing_out_yx_out;	break;
	};
		
	// bus
	QString bus_mode;
	QString addressing_bus;
	switch ( opcode & GigaEmul::ir_bus_mode_mask )
	{
		case GigaEmul::ir_bus_mode_d_bus:		addressing_bus = bus_mode = bus_mode_d_bus;		break;
		case GigaEmul::ir_bus_mode_ram_bus:	addressing_bus = bus_mode = addressing_in;		break; // TODO
		case GigaEmul::ir_bus_mode_ac_bus:	addressing_bus = bus_mode = bus_mode_ac_bus;	break;
		case GigaEmul::ir_bus_mode_in_bus:	addressing_bus = bus_mode = bus_mode_in_bus;	break;
	}

	// instruction
	switch ( opcode & GigaEmul::ir_instruction_mask )
	{
		case GigaEmul::ir_instruction_ld:		asem = tr("LD %2 <- %1")  .arg(addressing_bus).arg(addressing_out);	break;
		case GigaEmul::ir_instruction_and:	asem = tr("%2 <- AC & %1").arg(addressing_bus).arg(addressing_out);	break;
		case GigaEmul::ir_instruction_or:		asem = tr("%2 <- AC | %1").arg(addressing_bus).arg(addressing_out);	break;
		case GigaEmul::ir_instruction_xor:	asem = tr("%2 <- AC ^ %1").arg(addressing_bus).arg(addressing_out);	break;
		case GigaEmul::ir_instruction_add:	asem = tr("%2 <- AC + %1").arg(addressing_bus).arg(addressing_out);	break;
		case GigaEmul::ir_instruction_sub:	asem = tr("%2 <- AC - %1").arg(addressing_bus).arg(addressing_out);	break;
		case GigaEmul::ir_instruction_st:		asem = tr("ST %2 <- %1")  .arg(addressing_bus).arg(addressing_in);	break;
		case GigaEmul::ir_instruction_jmp:
			if ( (opcode & GigaEmul::ir_addressing_mask) == GigaEmul::ir_addressing_d_ac)
			{
				asem = tr("JMP Y:%1").arg(addressing_bus);
			}
			else
			{
				asem = tr("BRANCH %1").arg(addressing_bus);
			}
			break;
	};
}

/*
 *
 */
void
GigaEmulDebugWidget::loadRom()
{
	const uint16_t *rom = gigaEmul->getRom();
	
	for (uint32_t romAddr = 0; romAddr < GigaEmul::romSizeWords; ++romAddr)
	{
		const uint8_t opcode = ((uint8_t *) &rom[romAddr])[0];
		const uint8_t data = ((uint8_t *) &rom[romAddr])[1];
		twRom->item(romAddr, 0)->setText(QString("%1").arg(opcode, 2, 16, QChar('0')));
		
		QString ins_dec;
		QString addr_dec;
		QString bus_dec;
		decodeOpcode(opcode, ins_dec, addr_dec, bus_dec);
		twRom->item(romAddr, 2)->setText(ins_dec);
		twRom->item(romAddr, 3)->setText(addr_dec);
		twRom->item(romAddr, 4)->setText(bus_dec);
		
		QString asem_dec;
		decodeAsem(opcode, data, asem_dec);
		twRom->item(romAddr, 5)->setText(asem_dec);
		
		twRom->item(romAddr, 1)->setText(QString("%1").arg(data, 2, 16, QChar('0')));
		twRom->item(romAddr, 6)->setText(QString("%1").arg(data));
	}

	for (int i = 0; i < twRom->columnCount(); ++i)
		twRom->resizeColumnToContents(i);
}

/*
 *
 */
void
GigaEmulDebugWidget::loadRam()
{
	const uint8_t *ram = gigaEmul->getRam();
	
	for (uint32_t ramAddr = 0; ramAddr < GigaEmul::ramSizeBytes; ++ramAddr)
	{
		const uint8_t data = ram[ramAddr];
		twRam->item(ramAddr, 0)->setText(QString("%1").arg(data, 2, 16, QChar('0')));
		twRam->item(ramAddr, 1)->setText(QString("%1").arg(data));
	}
	
	twRam->resizeColumnToContents(0);
 	twRam->resizeColumnToContents(1);
}

/*
 *
 */
void
GigaEmulDebugWidget::loadRegisters()
{
	twRegisters->item(1, 1)->setText(tr("00000"));
	twRegisters->item(1, 2)->setText(tr("000"));
	twRegisters->item(1, 3)->setText(tr("[Y:X++]->BUS, ALU->OUT"));
	
	for (int i = 0; i < twRegisters->columnCount(); ++i)
		twRegisters->resizeColumnToContents(i);
}

/*
 *
 */
void
GigaEmulDebugWidget::updateRom()
{
	twRom->selectRow(gigaEmul->getState().pc_1);
}

/*
 *
 */
void
GigaEmulDebugWidget::updateRam()
{
	const uint8_t *ram = gigaEmul->getRam();
	const uint8_t *ramSave = gigaEmul->getRamSave();
	
	for (uint32_t ramAddr = 0; ramAddr < GigaEmul::ramSizeBytes; ++ramAddr)
	{
		if (ram[ramAddr] == ramSave[ramAddr])
			continue;
		
		const uint8_t data = ram[ramAddr];
		twRam->item(ramAddr, 0)->setText(QString("%1").arg(data, 2, 16, QChar('0')));
		twRam->item(ramAddr, 1)->setText(QString("%1").arg(data));
	}
}

/*
 *
 */
void
GigaEmulDebugWidget::updateTickCount()
{
	leTickCount->setText(tr("%1").arg(gigaEmul->getTickCount()));
}

/*
 *
 */
void
GigaEmulDebugWidget::procTicks(uint64_t ticks)
{
/*
	GigaEmul gigaEmul2 = new GigaEmulF2;
	while (1)
	{
		gigaEmul->proc(1);
		gigaEmul2->proc(1);
		GigaState s1 = gigaEmul->getState();
		GigaState s2 = gigaEmul2->getState();
		if ( memcmp((const void *) &s1,(const void *) &s2, sizeof(s1)) != 0 )
			break;
	}
*/
	gettimeofday(&startTime, 0);
	gigaEmul->save();
	
	gigaEmul->setInput(
		0xff
			& (pbLeft->isChecked()   ? 0xfd : 0xff)
 			& (pbRight->isChecked()  ? 0xfe : 0xff)
			& (pbUp->isChecked()     ? 0xf7 : 0xff)
			& (pbDown->isChecked()   ? 0xfb : 0xff)
			& (pbA->isChecked()      ? 0x7f : 0xff)
			& (pbB->isChecked()      ? 0xbf : 0xff)
			& (pbSelect->isChecked() ? 0xdf : 0xff)
			& (pbStart->isChecked()  ? 0xef : 0xff)
	);
	
	gettimeofday(&startTimeProc, 0);
	gigaEmul->proc(ticks);
	gettimeofday(&stopTimeProc, 0);
	
	if ( cbRegisters->isChecked() )
		updateRegs();
	
	if ( cbAddressing->isChecked() )
		updateAddressing();
	
	if ( cbRom->isChecked() )
		updateRom();
	
	if ( cbRam->isChecked() )
		updateRam();
	
	updateTickCount();
	screen->setImage(gigaEmul->getScreen());
	gettimeofday(&stopTime, 0);

	long usec = (stopTime.tv_sec - startTime.tv_sec) * 1000000;
	usec += stopTime.tv_usec - startTime.tv_usec;
	double mSec = (double) usec / 1000.0;

	long usecProc = (stopTimeProc.tv_sec - startTimeProc.tv_sec) * 1000000;
	usecProc += stopTimeProc.tv_usec - startTimeProc.tv_usec;
	double mSecProc = (double) usecProc / 1000.0;
	lMilliseconds->setText(tr("%1 (%2) ms").arg(mSecProc, 0, 'f', 0).arg(mSec, 0, 'f', 0));
	
}

/*
 *
 */
void
GigaEmulDebugWidget::onPbResetClicked()
{
	gigaEmul->reset();
	
	updateRegs();
	updateAddressing();
	updateRom();
	updateRam();
	updateTickCount();
	screen->setImage(gigaEmul->getScreen());
}

/*
 *
 */
void
GigaEmulDebugWidget::onPbProcTickClicked()
{
	procTicks(1);
}

/*
 *
 */
void
GigaEmulDebugWidget::onPbProcTicksClicked()
{
	procTicks(sbTicks->value());
}

/*
 *
 */
void
GigaEmulDebugWidget::onCbProcAutoStateChanged(int state)
{
	if ( state == Qt::Checked )
	{
		if ( tProcAuto->isActive() )
			return;
		
		tProcAuto->start(sbProcInterval->value());
		return;
	}
	
	tProcAuto->stop();
}

/*
 *
 */
void
GigaEmulDebugWidget::onProcAutoTimeout()
{
	procTicks(sbTicks->value());
}

/*
 * 
 */
void
GigaEmulDebugWidget::onCbFpsCurrentIndexChanged(int index)
{
	const QVariant v = cbFps->itemData(index);
	const QList<QVariant> l = v.toList();
	const int ticks = l[0].toInt();
	const int ms = l[1].toInt();
	
	sbTicks->setValue(ticks);
	sbProcInterval->setValue(ms);
}

/*
 *
 */
void
GigaEmulDebugWidget::onKey(int keyDown, int keyUp)
{
/*
	uint8_t in = 0xff;
	switch ( keyDown )
	{
		case Qt::Key_Left:	in &= 0xfd break;
		case Qt::Key_Right:	in &= 0xfe break;
		case Qt::Key_Up:		in &= 0xf7 break;
		case Qt::Key_Down:	in &= 0xfb break;
		case Qt::Key_A:			in &= 0x7f break;
		case Qt::Key_B:			in &= 0xbf break;
		case Qt::Key_Space:	in &= 0xdf break;
		case Qt::Key_Enter:	in &= 0xef break;
	}
*/
	pbLeft->setChecked(false);
	pbRight->setChecked(false);
	pbUp->setChecked(false);
	pbDown->setChecked(false);
	pbA->setChecked(false);
	pbB->setChecked(false);
	pbSelect->setChecked(false);
	pbStart->setChecked(false);

	switch ( keyDown )
	{
		case Qt::Key_Left:	pbLeft->setChecked(true);		break;
		case Qt::Key_Right:	pbRight->setChecked(true);	break;
		case Qt::Key_Up:		pbUp->setChecked(true);			break;
		case Qt::Key_Down:	pbDown->setChecked(true);		break;
		case Qt::Key_A:			pbA->setChecked(true);			break;
		case Qt::Key_B:			pbB->setChecked(true);			break;
		case Qt::Key_Space:	pbSelect->setChecked(true);	break;
		case Qt::Key_Enter:	pbStart->setChecked(true);	break;
	}

}
