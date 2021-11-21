#include "confjasmine.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    confjasmine w;
    w.show();
    return a.exec();
}
