#ifndef CHESS_H
#define CHESS_H

#include<QPoint>
#include<QVector>

class Chess
{
public:
    Chess();
    QVector<QVector<int> > chess;
    int curSide, curRound;
    void init(bool _cur_side);
    bool step(const QPoint &p);
};

#endif // CHESS_H
