#include "chess.h"
#include<cstring>
#include<QDebug>
const int dir[4][2] = {{1,0},{1,1},{0,1},{1,-1}};

Chess::Chess()
{
    init(qrand() % 2 == 1 ? true : false);
}

void Chess::init(bool _cur_side)
{
    qDebug() << _cur_side;
    chessBoard = QVector<QVector<short> >(15, QVector<short>(15, -1));
    state = -2;
    curSide = _cur_side;
    curRound = 0;
    countDown = 0;
    timeUsage[0].setHMS(0, 0, 0);
    timeUsage[1].setHMS(0, 0, 0);
}

void Chess::start()
{
    state = -1;
}

bool Chess::step(const QPoint &p)
{
    if (chessBoard[p.x()][p.y()] == -1) {
        chessBoard[p.x()][p.y()] = curSide;
        bool find = false, empty = false;
        for (int i = 0; i < 15 && !find; ++i)
            for (int j = 0; j < 15 && !find; ++j) {
                empty |= (chessBoard[i][j] == -1);
                for (int k = 0; k < 4; ++k)
                    find |= exist(i, j, k, 5, curSide);
            }
        if (find) state = curSide;
        else if (!empty) state = 2;
        curSide ^= 1;
        ++curRound;
        return true;
    }else return false;
}

bool Chess::exist(int i, int j, int k, int num, int side)
{
    while (num--) {
        if (i < 0 || i >= 15 || j < 0 || j >= 15) return false;
        if (chessBoard[i][j] != side) return false;
        i += dir[k][0];
        j += dir[k][1];
    }
    return true;
}

int Chess::getState() const
{
    return state;
}

QPoint Chess::defaultMove()
{
    for (int i = 0; i < 15; ++i)
        for (int j = 0; j < 15; ++j)
            if (chessBoard[i][j] == -1)
                return QPoint(i,j );
}

QDataStream &operator<<(QDataStream &ds, const Chess &chess)
{
    ds << chess.chessBoard << chess.curRound << chess.curSide << chess.state << chess.timeUsage[0] << chess.timeUsage[1];
    return ds;
}

QDataStream &operator>>(QDataStream &ds, Chess &chess)
{
    ds >> chess.chessBoard >> chess.curRound >> chess.curSide >> chess.state >> chess.timeUsage[0] >> chess.timeUsage[1];
    return ds;
}
