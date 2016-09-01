#include "chboard.h"
#include<QPainter>
#include<QMouseEvent>
#include<QDebug>
#include<QMessageBox>
const int extent = 50, pointsz = 4, countDown = 10;

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
            if (cs.chessBoard[i][j] != -1)
                drawPiece(&p, i, j, (bool)cs.chessBoard[i][j]);
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
void chBoard::setServer(const QString &ip, const QString &port)
{
    tcpSocket->abort();
    tcpServer->close();
    tcpServer = new QTcpServer(this);
    cs.init(qrand() % 2);
    connect(tcpServer, &QTcpServer::acceptError, [=](){
        QMessageBox::critical(this, "Error", tcpServer->errorString());
    });
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setText("Waiting for connection...");
    msgBox->setStandardButtons(QMessageBox::Cancel);
    connect(tcpServer, &QTcpServer::newConnection, [=](){
        qDebug() << "newConnection.";
        tcpSocket = tcpServer->nextPendingConnection();
        QMessageBox::information(this, "newConnection", QString("hostname=%3\nip=%1\nport=%2")
                                 .arg(tcpSocket->peerAddress().toString(),QString(tcpSocket->peerPort()),tcpSocket->peerName()));
        msgBox->accept();
        initSocket();
        emit setButtonConnect(true);
        if (cs.getState() != -2) sendMessage();
    });
    tcpServer->listen(QHostAddress(ip), port.toInt());
    int ret = msgBox->exec();
    if (ret == QMessageBox::Cancel)
        tcpServer->close();
}

void chBoard::initSocket()
{
    connect(tcpSocket, static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), [=](){
        QMessageBox::critical(this, "tcpSocket->Error",tcpSocket->errorString());
    });
    connect(tcpSocket, &QTcpSocket::readyRead, this, &chBoard::readData);
    connect(tcpSocket, &QTcpSocket::connected, [=](){
        emit setButtonConnect(true);
    });
    connect(tcpSocket, &QTcpSocket::disconnected, [=](){
        emit setButtonConnect(false);
        if (!isHost) {
            timer->stop();
            cs.init(qrand() % 2);
        }
    });
}

void chBoard::setSocket(const QString &ip, const QString &port)
{
    delete tcpSocket;
    tcpSocket = new QTcpSocket(this);
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setText("Connecting...");
    msgBox->setStandardButtons(QMessageBox::Cancel);
    connect(tcpSocket, &QTcpSocket::disconnected, [=](){
        QTimer::singleShot(100, msgBox, SLOT(accept()));
    });
    connect(tcpSocket, &QTcpSocket::connected, [=](){
        QTimer::singleShot(100, msgBox, SLOT(accept()));
    });
    tcpSocket->connectToHost(ip, port.toInt());
    initSocket();
    int ret = msgBox->exec();
    if (ret == QMessageBox::Cancel) {
        qDebug() << "QMessageBox::Cancel";
        tcpSocket->abort();
    }
}

//================================================================================================
//Chess borad changes only at:
//step && readData
//step::my turn: timeout1 && mousePress; his/her turn: timeout2
//readData::
chBoard::chBoard(QWidget *parent) : QWidget(parent), timer(new QTimer(this)),wait_timer(new QTimer(this)), tcpServer(new QTcpServer(this)), tcpSocket(new QTcpSocket(this)), cs_lock(false)
{
    this->setFixedSize(540, 540);
    cellsz = (width() - extent) / 14.0;
    timer->setInterval(1000);
    wait_timer->setInterval(2000);
    connect(timer, &QTimer::timeout, this, &chBoard::timeout1);
    connect(wait_timer, &QTimer::timeout, this, &chBoard::timeout2);
}

void chBoard::result()
{
    QString res = "You %1!";
    if (cs.getState() == 2) res = "Draw!";
    else res = res.arg(isHost == cs.getState() ? "win :-)" : "lose :-<");
    end();
    QMessageBox::information(this, "Result", res);
}

void chBoard::disconnectServer()
{
    tcpServer->close();
    tcpSocket->abort();
    end();
    cs.init(qrand()%2);
}

void chBoard::readData()
{
    qDebug() << "readyRead";
    if (tcpSocket->bytesAvailable() <= 0) return;
    QByteArray buffer = tcpSocket->readAll();
    m_buffer.append(buffer);
    QDataStream in(m_buffer);
    while (m_buffer.size() >= sizeof(int)) {
        int bsz, type_id; Chess tmp; bool side;
        in >> bsz;
        if (m_buffer.size() < bsz) break;
        in >> type_id;
        switch (type_id) {
        case 0:
            qDebug() << "0";
            in >> side >> tmp;
            if(tmp.curRound == cs. curRound + 1 || !isHost) {
                received = true;
                wait_timer->stop();
                timer->stop();
                cs = tmp;
                update();
                emit updateUI();
                startCountDown();
                if (cs.getState() != -1 && cs.getState() != -2) result();
            }
            break;
        case 1:
            qDebug() << "1";
            in >> ready[0] >> ready[1];
            if (ready[0] && ready[1] && isHost) {
                start(true);
                ready[0] = ready[1] = false;
                sendMessage2(ready);
                emit ReadyEnabled();
            }else if (!ready[0] && !ready[1]) emit ReadyEnabled();
            break;
        }
        m_buffer = m_buffer.right(m_buffer.size() - bsz);
    }
}

void chBoard::step(const QPoint&p)
{
    if (cs.step(p)) {
        cs.countDown = -1;
        update();
        emit updateUI();
        startCountDown();
        sendMessage();
        if (cs.getState() != -1 && cs.getState() != -2) result();
    }
}

void chBoard::toggleReady(bool flg)
{
    ready[isHost] = true;
    if (ready[0] && ready[1] && isHost) {
        start(true);
        ready[0] = ready[1] = false;
        emit ReadyEnabled();
    }
    sendMessage2(ready);
}

void chBoard::mousePressEvent(QMouseEvent *event)
{
    if (isLocked()) return;
    QPoint tmp = getNrstCor(event->pos());
    if (tmp.x() != -1) {timer->stop(); step(tmp);}
}

void chBoard::init(bool _isHost, Dialog *_dialog)
{
    isHost = _isHost;
    dialog = _dialog;
    ready[0] = ready[1] = false;
}

void chBoard::start(bool flg)
{
    timer->stop();
    if (flg) cs.init(qrand() % 2);
    cs.start();
    startCountDown();
    if (isHost) sendMessage();
}

bool chBoard::isLocked()
{
    return cs.getState() != -1 || cs.curSide != isHost;
}

void chBoard::sendMessage()
{
    QByteArray ba;
    QDataStream ds(&ba, QIODevice::WriteOnly);
    ds << int(0) << int(0);
    ds << isHost << cs;
    ds.device()->seek(0);
    ds << (int)ba.size();
    tcpSocket->write(ba);
}

void chBoard::sendMessage2(bool r[2])
{
    QByteArray ba;
    QDataStream ds(&ba, QIODevice::WriteOnly);
    ds << int(0) << int(1);
    ds << r[0] << r[1];
    ds.device()->seek(0);
    ds << (int)ba.size();
    tcpSocket->write(ba);
}

void chBoard::startCountDown()
{
    qDebug() << "startCountDown=" << cs.countDown;
    if (cs.countDown < 0) cs.countDown = countDown;
    update();
    emit updateUI();
    timer->start();
}

void chBoard::end()
{
    timer->stop();
    ready[0] = ready[1] = false;
    sendMessage2(ready);
    emit ReadyEnabled();
}

void chBoard::timeout1()
{
    --cs.countDown;
     cs.timeUsage[cs.curSide] = cs.timeUsage[cs.curSide].addSecs(1);
     if (cs.countDown < 0) {
         timer->stop();
         if (cs.curSide != isHost) {// not my turn
             received = false;
             wait_timer->start();
         }else step(cs.defaultMove());//my turn
     }
     emit updateUI();
}

void chBoard::timeout2()//not my turn
{
    wait_timer->stop();
    qDebug() << "reaceived=" << received;
    if (received) return;
    if (!isHost) {//lost connection
        QMessageBox::information(this, "Lost connection", "Lost connection.");
        cs.init(qrand()%2);
    }
    else step(cs.defaultMove());//he/she lost connection
}
