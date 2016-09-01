#include "chess.h"
#include<cstring>
#include<QDebug>
#include<QString>
const int dir[8][2] = {{1,0},{1,1},{0,1},{1,-1}, {-1, 0}, {-1, -1}, {0, -1}, {-1, 1}};

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
    countDown = -1;
    timeUsage[0].setHMS(0, 0, 0);
    timeUsage[1].setHMS(0, 0, 0);
}

void Chess::start()
{
    state = -1;
}

void Chess::setTipOn(int side)
{
    side ^= 1;
    for (int i = 0; i < 15; ++i)
        for (int j = 0; j < 15; ++j)
            if (chessBoard[i][j] == -1) {
                chessBoard[i][j] = side;
                int cnt = 0;
                for (int x = 0; x < 15 && cnt < 2; ++x)
                    for (int y = 0; y < 15 && cnt < 2; ++y) {
                        if (chessBoard[x][y] < 0)
                            for (int k = 0; k < 4; ++k)
                                if (exist(x + dir[k][0], y + dir[k][1], k, 3, side) && piece(x, y, k, 4) < 0) {
                                    ++cnt; // 3
                                    qDebug() << QString("find 3 on (%1,%2) of (%3,%4)").arg(QString::number(x), QString::number(y), QString::number(i), QString::number(j));
                                }
                        if (chessBoard[x][y] == side)
                            for (int k = 0; k < 4; ++k)
                                if (exist(x, y, k, 4, side) && (piece(x, y, k, 4) < 0 || piece(x, y, k, -1) < 0)) {
                                    ++cnt; // 4
                                    qDebug() << QString("find 4 on (%1,%2) of (%3,%4)").arg(QString::number(x), QString::number(y), QString::number(i), QString::number(j));
                                }
                    }
                if (cnt >= 2) chessBoard[i][j] = -2;
                else chessBoard[i][j] = -1;
            }
}

bool Chess::step(const QPoint &p)
{
    if (chessBoard[p.x()][p.y()] < 0) {
        chessBoard[p.x()][p.y()] = curSide;
        bool find = false, empty = false;
        for (int i = 0; i < 15 && !find; ++i)
            for (int j = 0; j < 15 && !find; ++j) {
                if (chessBoard[i][j] == -2) chessBoard[i][j] = -1;
                empty |= (chessBoard[i][j] < 0);
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

int Chess::piece(int i, int j, int k, int num)
{
    int sgn = 1;
    if (num < 0) num = -num, sgn = -1;
    while (num--) {
        if (i < 0 || i >= 15 || j < 0 || j >= 15) return 100;
        i += dir[k][0] * sgn;
        j += dir[k][1] * sgn;
    }
    if (i < 0 || i >= 15 || j < 0 || j >= 15) return 100;
    return chessBoard[i][j];
}

int Chess::getState() const
{
    return state;
}

QPoint Chess::defaultMove()
{
    for (int i = 0; i < 15; ++i)
        for (int j = 0; j < 15; ++j)
            if (chessBoard[i][j] < 0)
                return QPoint(i,j );
}

QDataStream &operator<<(QDataStream &ds, const Chess &chess)
{
    ds << chess.chessBoard << chess.curRound << chess.curSide << chess.state << chess.timeUsage[0] << chess.timeUsage[1] << chess.countDown;
    return ds;
}

QDataStream &operator>>(QDataStream &ds, Chess &chess)
{
    ds >> chess.chessBoard >> chess.curRound >> chess.curSide >> chess.state >> chess.timeUsage[0] >> chess.timeUsage[1] >> chess.countDown;
    return ds;
}
