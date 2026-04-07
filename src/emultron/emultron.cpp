#include <QApplication>
#include <QMdiArea>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QFontDialog>
#include <QMdiSubWindow>
#include <QStatusBar>

#include "emultron.h"
#include "gigaemulwidget.h"
#include "gigaemuldebugwidget.h"
#include "gigaemulf2.h"
#include "gigaemulf3.h"
#include "screendebugwidget.h"
#include "styledialog.h"


/*
 *
 */
Emultron::Emultron(QWidget *parent)
	: QMainWindow(parent),
		emulCount(0),
		styleDialog(0),
		aGigaEmulF2DebugNew(0),
		aGigaEmulF3DebugNew(0)
{
	setWindowTitle(tr("Emultron"));
	mdiArea = new QMdiArea(this);
	setCentralWidget(mdiArea);
	
	createControls();
	createActions();
	createToolBars();
	createMenus();
	createMenuBar();
	
	statusBar()->showMessage("");
}

/*
 *
 */
Emultron::~Emultron()
{
}

/*
 *
 */
void Emultron::createControls()
{
	styleDialog = new StyleDialog(this);
}

/*
 *
 */
void Emultron::createActions()
{
	// mFile
	aFileExit = new QAction(QIcon("./data/icons/png/exit.png"), tr("Exit"), this);
	aFileExit->setToolTip(tr("Exit"));
	connect(aFileExit, SIGNAL(triggered()), SLOT(close()));
	
	// mView
	aStyleDialog = new QAction(tr("Style..."), this);
	aStyleDialog->setToolTip(tr("Change style"));
	connect(aStyleDialog, SIGNAL(triggered()), SLOT(onStyleDialog()));
	
	aFontDialog = new QAction(tr("Font..."), this);
	aFontDialog->setToolTip(tr("Change font"));
	connect(aFontDialog, SIGNAL(triggered()), SLOT(onFontDialog()));
	
	aViewFullScreen = new QAction(QIcon("./data/icons/png/window_fullscreen.png"),
																tr("Full screen"), this);
	aViewFullScreen->setShortcut(QKeySequence("F11"));
	aViewFullScreen->setToolTip(tr("Full screen"));
	aViewFullScreen->setCheckable(true);
	connect(aViewFullScreen, SIGNAL(triggered()), SLOT(onFullScreen()));
	
	aGigaEmulF2DebugNew = new QAction(QIcon("./data/icons/png/filenew.png"),
															 tr("Debug F2..."), this);
	aGigaEmulF2DebugNew->setToolTip(tr("New debug F2"));
	connect(aGigaEmulF2DebugNew, SIGNAL(triggered()), SLOT(onGigaEmulF2DebugNew()));

	aGigaEmulF3DebugNew = new QAction(QIcon("./data/icons/png/filenew.png"),
															 tr("Debug F3..."), this);
	aGigaEmulF3DebugNew->setToolTip(tr("New debug F3"));
	connect(aGigaEmulF3DebugNew, SIGNAL(triggered()), SLOT(onGigaEmulF3DebugNew()));
}

/*
 *
 */
void Emultron::createToolBars()
{
	tbFile = new QToolBar(tr("File"), this);
	tbFile->setObjectName("File");
	tbFile->addAction(aFileExit);
	
	tbView = new QToolBar(tr("View"), this);
	tbView->setObjectName("View");
	tbView->addAction(aViewFullScreen);
	
	tbEmul = new QToolBar(tr("Emul"), this);
	tbEmul->setObjectName("Emul");
	tbEmul->addAction(aGigaEmulF2DebugNew);
	tbEmul->addAction(aGigaEmulF3DebugNew);
	
	addToolBar(tbFile);
	addToolBar(tbView);
	addToolBar(tbEmul);
}

/*
 *
 */
void Emultron::createMenus()
{
	// mFile
	mFile = new QMenu(tr("File"), this);
	mFile->addAction(aFileExit);
	
	// mView
	mView = new QMenu(tr("View"), this);
	mView->addAction(aStyleDialog);
	mView->addAction(aFontDialog);
	mView->addAction(aViewFullScreen);
	
	// mEmul
	mEmul = new QMenu(tr("Emul"), this);
	mEmul->addAction(aGigaEmulF2DebugNew);
	mEmul->addAction(aGigaEmulF3DebugNew);
}

/*
 *
 */
void Emultron::createMenuBar()
{
	mbMain = new QMenuBar(this);
	mbMain->addMenu(mFile);
	mbMain->addMenu(mView);
	mbMain->addMenu(mEmul);
	
	setMenuBar(mbMain);
}

/*
 *
 */
void Emultron::onStyleDialog()
{
	styleDialog->exec();
}

/*
 *
 */
void Emultron::onFontDialog()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, qApp->font(), this);
	if ( ok )
		qApp->setFont(font);
}

/*
 *
 */
void Emultron::onFullScreen()
{
	if ( aViewFullScreen->isChecked() )
		showFullScreen();
	else
		showNormal();
}


/*
 *
 */
void Emultron::onGigaEmulF2DebugNew()
{
	GigaEmul *gigaEmul = new GigaEmulF2;
	
	ScreenDebugWidget *screen = new ScreenDebugWidget(this);
	screen->setWindowTitle(tr("Screen F2 %1").arg(emulCount));
	
	QMdiSubWindow *mdiSubWindowScreen = mdiArea->addSubWindow(screen);
	mdiSubWindowScreen->resize(QSize(800 + 10, 525 + 40));
	mdiSubWindowScreen->show();

	GigaEmulDebugWidget *gigaEmulWidget = new GigaEmulDebugWidget(gigaEmul, screen, this);
	gigaEmulWidget->setWindowTitle(tr("Debug F2 %1").arg(emulCount));
	
	QMdiSubWindow *mdiSubWindow = mdiArea->addSubWindow(gigaEmulWidget);
	mdiSubWindow->resize(QSize(1200, 700));
	mdiSubWindow->show();
	
	++emulCount;
}

/*
 *
 */
void Emultron::onGigaEmulF3DebugNew()
{
	GigaEmul *gigaEmul = new GigaEmulF3;
	
	ScreenDebugWidget *screen = new ScreenDebugWidget(this);
	screen->setWindowTitle(tr("Screen F3 %1").arg(emulCount));
	
	QMdiSubWindow *mdiSubWindowScreen = mdiArea->addSubWindow(screen);
	mdiSubWindowScreen->resize(QSize(800 + 10, 525 + 40));
	screen->setFocusPolicy(Qt::StrongFocus);
	mdiSubWindowScreen->show();

	GigaEmulDebugWidget *gigaEmulWidget = new GigaEmulDebugWidget(gigaEmul, screen, this);
	gigaEmulWidget->setWindowTitle(tr("Debug F3 %1").arg(emulCount));
	
	QMdiSubWindow *mdiSubWindow = mdiArea->addSubWindow(gigaEmulWidget);
	mdiSubWindow->resize(QSize(1200, 700));
	mdiSubWindow->show();
	
	connect(screen, SIGNAL(key(int, int)), gigaEmulWidget, SLOT(onKey(int, int)));
	
	++emulCount;
}

/*
 *
 */
void
Emultron::onUpdateStatusBar(const QString &msg)
{
	statusBar()->showMessage(msg);
}
