#ifndef CHESS_H
#define CHESS_H

#include<QPoint>
#include<QVector>
#include<QDataStream>
#include<QTime>
class Chess
{
public:
    Chess();
    QVector<QVector<short> > chessBoard;
    int curSide, curRound, countDown;
    QTime timeUsage[2];
    void init(bool _cur_side);
    void start();
    void setTipOn(int side);
    bool step(const QPoint &p);
    bool exist(int x, int y, int k, int num, int side);
    int getState() const;
    QPoint defaultMove();
    friend QDataStream &operator<<(QDataStream& ds, const Chess &chess);
    friend QDataStream &operator>>(QDataStream& ds, Chess &chess);

private:
    int piece(int i, int j, int k, int num);
    int state;
};

#endif // CHESS_H
