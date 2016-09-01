#ifndef CHBOARD_H
#define CHBOARD_H

#include<QWidget>
#include<QPoint>
#include<QPainter>
#include"chess.h"
#include"dialog.h"
#include<QTimer>
#include<QtNetwork>
#include<QMutex>


extern Chess cs;
class chBoard : public QWidget
{
    Q_OBJECT

    friend class Dialog;
public:
    double cellsz;
    explicit chBoard(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    QPoint getPos(int x, int y);
    QPoint getPos(const QPoint &p);
    void drawPiece(QPainter *painter, const QPoint &p, bool col);
    void drawPiece(QPainter *painter, int x, int y, bool col);
    QPoint getNrstCor(QPoint p);
    void mousePressEvent(QMouseEvent *event);
    void init(bool _isHost, Dialog *_dialog);
    void start(bool flg=false);
    void toggleReady(bool flg);
    void setServer(const QString&ip, const QString&port);
    void setSocket(const QString&ip, const QString&port);
protected:
    Dialog *dialog;
    QTimer *timer, *wait_timer;
    bool isHost, cs_lock, ready[2];
    QTcpSocket *tcpSocket;
    QTcpServer *tcpServer;
    QByteArray m_buffer;
    bool received;
    bool isLocked();
    void sendMessage();
    void sendMessage2(bool r[]);
    void startCountDown();
    void end();
    void step(const QPoint&p);
    void initSocket();
    void result();
    void disconnectServer();
signals:
    void setButtonConnect(bool);
    void updateUI();
    void ReadyEnabled();
protected slots:
    void readData();
    void timeout1();
    void timeout2();
};

#endif // CHBOARD_H
