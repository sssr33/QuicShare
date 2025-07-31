#include "QuicShare.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QuicShare window;
    window.show();
    return app.exec();
}
