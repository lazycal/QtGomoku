#include "dialog.h"
#include <QApplication>
#include"chess.h"
Chess cs;
int main(int argc, char *argv[])
{
    qsrand(time(0));
    cs.init(qrand() % 2);
    QApplication a(argc, argv);
    Dialog w;
    w.show();

    return a.exec();
}
