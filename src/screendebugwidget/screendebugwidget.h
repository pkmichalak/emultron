#ifndef SCREEN_DEBUG_WIDGET_H
#define SCREEN_DEBUG_WIDGET_H

#include <QWidget>
#include <QImage>

class ScreenDebugWidget : public QWidget
{
	Q_OBJECT
	public:
		ScreenDebugWidget(QWidget *parent=0);
		~ScreenDebugWidget();
	public:
		void setImage(const QImage &image);
	private:
		void paintEvent(QPaintEvent *event);
		void keyPressEvent(QKeyEvent *event);
		void keyReleaseEvent(QKeyEvent *event);
	signals:
		void key(int keyDown, int keyUp);
	private:
		QImage image;
};

#endif // SCREEN_DEBUG_WIDGET_H
