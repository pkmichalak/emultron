#include <QApplication>

#include "emultron/emultron.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	
	Emultron emultron;
	emultron.show();
	
	return app.exec();
}
