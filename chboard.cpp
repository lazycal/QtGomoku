#include "chboard.h"
#include<QPainter>
#include<QMouseEvent>
#include<QDebug>
const int extent = 50, pointsz = 4;
chBoard::chBoard(QWidget *parent) : QWidget(parent), lock(false), isHost(false)
{
    this->setFixedSize(540, 540);
    cellsz = (width() - extent) / 14.0;
}



void chBoard::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(Qt::black);
    p.setBrush(Qt::yellow);
    p.drawRect(1, 1, width() - 2, height() - 2);
    p.translate(extent / 2, extent / 2);
    cellsz = (width() - extent) / 14.0;
    for (int i = 0; i < 15; ++i) {
        p.drawLine(i * cellsz, 0, i * cellsz, width() - extent);
        p.drawLine(0, i * cellsz, width() - extent,  i * cellsz);
    }
    p.setBrush(Qt::black);
    p.drawEllipse(getPos(QPoint(3, 3)), pointsz, pointsz);
    p.drawEllipse(getPos(QPoint(11, 3)), pointsz, pointsz);
    p.drawEllipse(getPos(QPoint(3, 11)), pointsz, pointsz);
    p.drawEllipse(getPos(QPoint(11, 11)), pointsz, pointsz);
    p.drawEllipse(getPos(QPoint(7, 7)), pointsz, pointsz);

    for (int i = 0; i < 15; ++i)
        for (int j = 0; j < 15; ++j)
            if (cs.chess[i][j] != -1)
                drawPiece(&p, i, j, (bool)cs.chess[i][j]);
}

inline QPoint chBoard::getPos(int x, int y)
{
    return getPos(QPoint(x, y));
}

inline QPoint chBoard::getPos(const QPoint &p)
{
    return QPoint(cellsz*p.x(), cellsz*p.y());
}

void chBoard::drawPiece(QPainter *painter, int x, int y, bool col)
{
    drawPiece(painter, QPoint(x, y), col);
}

void chBoard::mousePressEvent(QMouseEvent *event)
{
    if (lock) return;
    QPoint tmp = getNrstCor(event->pos());
    if (tmp.x() != -1)
        if (cs.step(tmp)) {
            update();
            lock = true;
        }
}

void chBoard::init(bool _isHost, Dialog *_dialog)
{
    isHost = _isHost;
    dialog = _dialog;
}

QPoint chBoard::getNrstCor(QPoint p)
{
    p -= QPoint(extent / 2, extent / 2);
    for (int i = 0; i < 15; ++i)
        for (int j = 0; j < 15; ++j)
            if (qAbs(p.x() - getPos(i, j).x()) < cellsz * 0.5 && qAbs(p.y() - getPos(i, j).y()) < cellsz * 0.5)
                return QPoint(i, j);
    return QPoint(-1, -1);
}

void chBoard::drawPiece(QPainter *painter, const QPoint &p, bool col)
{
    painter->setPen(Qt::NoPen);
    if (col) painter->setBrush(Qt::black);
    else painter->setBrush(Qt::white);
    painter->drawEllipse(getPos(p), cellsz * 0.4, cellsz * 0.4);
    update();
}
