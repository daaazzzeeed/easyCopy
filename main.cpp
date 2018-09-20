#include "copier.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Copier w;
    w.setWindowTitle("Утилита для копирования данных БД");
    w.show();

    return a.exec();
}
