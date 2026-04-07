#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QApplication>
#include <QStyleFactory>
#include <QDialogButtonBox>
#include <QRadioButton>
#include <QStyle>

#include "styledialog.h"

/*
 *
 */
StyleDialog::StyleDialog(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(tr("Style"));
	originalPalette = QApplication::palette();
	createControls();
}

/*
 *
 */
StyleDialog::~StyleDialog()
{
	delete vblMain;
}

/*
 *
 */
void StyleDialog::createControls()
{
	vblChangeStyle = new QVBoxLayout;

	QStringList styles = QStyleFactory::keys();
	for (int i = 0; i < styles.size(); ++i)
	{
		QString style = styles[i];
		QRadioButton *rb = new QRadioButton(style, this);
		rbStyle << rb;
		vblChangeStyle->addWidget(rb);
		connect(rb, SIGNAL(clicked()), this, SLOT(onStyleChanged()));
	}

	gbChangeStyle = new QGroupBox(tr("Change style"), this);
	gbChangeStyle->setLayout(vblChangeStyle);

	cbUseStylePalette = new QCheckBox(tr("Use style palette"), this);
	connect(cbUseStylePalette, SIGNAL(toggled(bool)),
					SLOT(onUseStylePaletteChanged()));
	vblOptions = new QVBoxLayout;
	vblOptions->addWidget(cbUseStylePalette);
	gbOptions = new QGroupBox(tr("Options"), this);
	gbOptions->setLayout(vblOptions);

	dbbButtons = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, this);
	connect(dbbButtons, SIGNAL(accepted()), SLOT(accept()));

	vblMain = new QVBoxLayout;
	vblMain->addWidget(gbChangeStyle);
	vblMain->addWidget(gbOptions);
	vblMain->addWidget(dbbButtons);

	setLayout(vblMain);
}

/*
 *
 */
void StyleDialog::onStyleChanged()
{
	for (int i = 0; i < rbStyle.size(); ++i)
		if ( rbStyle[i]->isChecked() )
			QApplication::setStyle(QStyleFactory::create(rbStyle[i]->text()));
	onUseStylePaletteChanged();
}

/*
 *
 */
void StyleDialog::onUseStylePaletteChanged()
{
	if ( cbUseStylePalette->isChecked() )
		QApplication::setPalette(QApplication::style()->standardPalette());
	else
		QApplication::setPalette(originalPalette);
}
