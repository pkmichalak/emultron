#ifndef EMULTRON_H
#define EMULTRON_H

#include <QMainWindow>

class QMdiArea;
class QAction;
class QMenuBar;
class QMenu;
class QToolBar;

class StyleDialog;

class Emultron : public QMainWindow
{
	Q_OBJECT
	public:
		Emultron(QWidget *parent=0);
		~Emultron();
	private:
		void createControls();
		void createActions();
		void createToolBars();
		void createMenus();
		void createMenuBar();
	public slots:
		void onStyleDialog();
		void onFontDialog();
		void onFullScreen();
		void onGigaEmulF2DebugNew();
		void onGigaEmulF3DebugNew();
		void onUpdateStatusBar(const QString &msg);
	private:
		int emulCount;
	private:
		StyleDialog *styleDialog;
		QMdiArea *mdiArea;
		QMenuBar *mbMain;
			QMenu *mFile;
				QAction *aFileExit;
			QMenu *mView;
				QAction *aStyleDialog;
				QAction *aFontDialog;
				QAction *aViewFullScreen;
			QMenu *mEmul;
				QAction *aGigaEmulF2DebugNew;
				QAction *aGigaEmulF3DebugNew;
		QToolBar *tbFile;
		QToolBar *tbView;
		QToolBar *tbEmul;
		
};

#endif // EMULTRON_H
