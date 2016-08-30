#include "chess.h"
#include<cstring>

Chess::Chess() : chess(15, QVector<int>(15, -1))
{
    init(qrand() % 2);
}

void Chess::init(bool _cur_side)
{
    curSide = _cur_side;
    curRound = 0;
}

bool Chess::step(const QPoint &p)
{
    if (chess[p.x()][p.y()] == -1) {
        chess[p.x()][p.y()] = curSide;
        curSide ^= 1;
        ++curRound;
        return true;
    }else return false;
}
