#ifndef GIGA_EMUL_WIDGET_H
#define GIGA_EMUL_WIDGET_H

class GigaEmul;

#include <QWidget>

class QGridLayout;
class QStackedLayout;
class QLabel;

class GigaEmulWidget : public QWidget
{
	Q_OBJECT
	public:
		GigaEmulWidget(QWidget *parent=0);
		~GigaEmulWidget();
	private:
		void createControls();
		void configureControls();
	private:
		GigaEmul *gigaEmul;
	private:
		QGridLayout *glMain;
};

#endif // GIGA_EMUL_WIDGET_H
