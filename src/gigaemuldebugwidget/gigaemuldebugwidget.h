#ifndef GIGA_EMUL_DEBUG_WIDGET_H
#define GIGA_EMUL_DEBUG_WIDGET_H

class GigaEmul;
class ScreenDebugWidget;

#include <sys/time.h>

#include <QWidget>

class QGridLayout;
class QStackedLayout;
class QLabel;
class QTableWidget;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QSplitter;
class QCheckBox;
class QTimer;
class QComboBox;

class GigaEmulDebugWidget : public QWidget
{
	Q_OBJECT
	public:
		GigaEmulDebugWidget(GigaEmul *gigaEmul, ScreenDebugWidget *screen, QWidget *parent=0);
		~GigaEmulDebugWidget();
	private:
		void createControls();
		void configureControls();
	private:
		void updateRegs();
		void updateAddressing();
		void decodeOpcode(uint8_t opcode,
											QString &instruction,
											QString &addressing,
											QString &bus_mode);
		void decodeAsem(uint8_t opcode,
										uint8_t data,
										QString &asem);
		void loadRom();
		void loadRam();
		void loadRegisters();
		void updateRom();
		void updateRam();
		void updateTickCount();
		void procTicks(uint64_t ticks);
	private slots:
		void onPbResetClicked();
		void onPbProcTickClicked();
		void onPbProcTicksClicked();
		void onCbProcAutoStateChanged(int state);
		void onProcAutoTimeout();
		void onCbFpsCurrentIndexChanged(int index);
		void onKey(int keyDown, int keyUp);
	private:
		static constexpr int pcIdx = 0;
		static constexpr int pc_1Idx = 1;
		static constexpr int irIdx = 2;
		static constexpr int dIdx = 3;
		static constexpr int acIdx = 4;
		static constexpr int xIdx = 5;
		static constexpr int yIdx = 6;
		static constexpr int outIdx = 7;
		static constexpr int inIdx = 8;
		static constexpr int busIdx = 9;
		static constexpr int regIdxLast = 9;
		static constexpr char *regName[] = {
			(char *) "PC",
			(char *) "PC-1",
			(char *) "IR",
			(char *) "D",
			(char *) "AC",
			(char *) "X",
			(char *) "Y",
			(char *) "OUT",
			(char *) "IN",
			(char *) "BUS"
		};
	private:
		GigaEmul *gigaEmul;
	private:
		ScreenDebugWidget *screen;
	private:
		timeval startTime;
		timeval stopTime;
		timeval startTimeProc;
		timeval stopTimeProc;
	private:
		QTimer *tProcAuto;
	private:
		QGridLayout *glMain;
			QSplitter *sTop;
				QWidget *wLeft;
					QGridLayout *glLeft;
						QSplitter *sLeft;
							QWidget *wRegisters;
								QGridLayout *glRegisters;
									QCheckBox *cbRegisters;
									QTableWidget *twRegisters;
							QWidget *wAddressing;
								QGridLayout *glAddressing;
									QCheckBox *cbAddressing;
									QTableWidget *twAddressing;
									QGridLayout *glButtons;
										QLabel *lButtons;
										QPushButton *pbLeft;
										QPushButton *pbRight;
										QPushButton *pbUp;
										QPushButton *pbDown;
										QPushButton *pbA;
										QPushButton *pbB;
										QPushButton *pbSelect;
										QPushButton *pbStart;
				QWidget *wRam;
					QGridLayout *glRam;
						QCheckBox *cbRam;
						QTableWidget *twRam;
				QWidget *wRom;
					QGridLayout *glRom;
						QCheckBox *cbRom;
						QTableWidget *twRom;
			QGridLayout *glProc;
				QPushButton *pbReset;
				QLabel *lTicks;
				QLineEdit *leTickCount;
				QPushButton *pbProcTick;
				QSpinBox *sbTicks;
				QPushButton *pbProcTicks;
				QSpinBox *sbProcInterval;
				QCheckBox *cbProcAuto;
				QComboBox *cbFps;
				QLabel *lMilliseconds;
};

#endif // GIGA_EMUL_DEBUG_WIDGET_H
