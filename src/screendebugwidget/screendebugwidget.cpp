#include <QPainter>
#include <QKeyEvent>

#include "screendebugwidget.h"

/*
 *
 */
ScreenDebugWidget::ScreenDebugWidget(QWidget *parent)
	: QWidget(parent)
{
	setWindowTitle(tr("Screen"));
//	setFocusPolicy(Qt::StrongFocus);
	
}

/*
 *
 */
ScreenDebugWidget::~ScreenDebugWidget()
{
}

/*
 *
 */
void
ScreenDebugWidget::setImage(const QImage &image)
{
	this->image = image;
	update();
}

/*
 *
 */
void
ScreenDebugWidget::paintEvent(QPaintEvent *event)
{
	QPainter p(this);
	const int w = width();
	const int h = height();
	const int iw = image.width();
	const int ih = image.height();
	const int wr = w / iw;
	const int hr = h / ih;
	const int r = wr <= hr ? wr : hr;
	
	if ( 1 < r )
		p.drawImage(0, 0, image.scaled(r * iw, r * ih));
	else	
		p.drawImage(0, 0, image);
}

/*
 *
 */
void
ScreenDebugWidget::keyPressEvent(QKeyEvent *event)
{
	event->accept();
	emit key(event->key(), 0);
}

/*
 *
 */
void
ScreenDebugWidget::keyReleaseEvent(QKeyEvent *event)
{
	event->accept();
	emit key(0, event->key());
}
