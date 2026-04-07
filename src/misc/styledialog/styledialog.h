#ifndef STYLE_DIALOG_H
#define STYLE_DIALOG_H

#include <QDialog>

class QVBoxLayout;
class QGroupBox;
class QCheckBox;
class QDialogButtonBox;
class QRadioButton;

class StyleDialog : public QDialog
{
	Q_OBJECT
	public:
		StyleDialog(QWidget *parent=0);
		~StyleDialog();
	private:
		void createControls();
	private slots:
		void onStyleChanged();
		void onUseStylePaletteChanged();
	private:
		QPalette originalPalette;
		QVBoxLayout *vblMain;
			QGroupBox *gbChangeStyle;
				QVBoxLayout *vblChangeStyle;
					QList<QRadioButton *> rbStyle;
			QGroupBox *gbOptions;
				QVBoxLayout *vblOptions;
					QCheckBox *cbUseStylePalette;
			QDialogButtonBox *dbbButtons;			
};

#endif // STYLE_DIALOG_H
