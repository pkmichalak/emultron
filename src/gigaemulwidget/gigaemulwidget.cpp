#include <QMessageBox>
#include <QGridLayout>
#include <QPushButton>
#include <QKeyEvent>
#include <QLineEdit>
#include <QInputDialog>
#include <QStackedLayout>
#include <QMainWindow>
#include <QStatusBar>
#include <QLabel>

#include "gigaemulwidget.h"

#include "gigaemulf2.h"

/*
 *
 */
GigaEmulWidget::GigaEmulWidget(QWidget *parent)
	: QWidget(parent)
{
	gigaEmul = new GigaEmulF2;
	
	setWindowTitle(tr("Emul"));
//	setFocusPolicy(Qt::StrongFocus);
	
	createControls();
	configureControls();
}

/*
 *
 */
GigaEmulWidget::~GigaEmulWidget()
{
	delete gigaEmul;
}

/*
 *
 */
void
GigaEmulWidget::createControls()
{
	glMain = new QGridLayout;
	
	setLayout(glMain);
}

/*
 *
 */
void
GigaEmulWidget::configureControls()
{
}
