#ifndef CHBOARD_H
#define CHBOARD_H

#include<QWidget>
#include<QPoint>
#include<QPainter>
#include"chess.h"
#include"dialog.h"

class chBoard : public QWidget
{
    Q_OBJECT

public:
    explicit chBoard(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    QPoint getPos(int x, int y);
    QPoint getPos(const QPoint &p);
    void drawPiece(QPainter *painter, const QPoint &p, bool col);
    void drawPiece(QPainter *painter, int x, int y, bool col);
    QPoint getNrstCor(QPoint p);
    double cellsz;
    void mousePressEvent(QMouseEvent *event);
    void init(bool _isHost, Dialog *_dialog);
protected:
    Dialog *dialog;
    bool lock, isHost;
    Chess cs;
};

#endif // CHBOARD_H
