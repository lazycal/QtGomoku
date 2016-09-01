#include "dialog.h"
#include "ui_dialog.h"
#include<QFont>
#include "ipdialog.h"

extern Chess cs;
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setStyleSheet("QLabel{font-size: 15px;}");
//    ui->label_2->setStyleSheet("font-size: 15px;");
    connect(ui->widget, &chBoard::ReadyEnabled, [=](){
        qDebug() << "ReadyEnabled";
        ui->pushButton_3->setEnabled(true);
    });
    connect(ui->widget, &chBoard::setButtonConnect, this, &Dialog::setButtonConnect);
    connect(ui->widget, &chBoard::updateUI, this, &Dialog::updateUI);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setButtonConnect(bool connected)
{
    if (connected) {
        ui->pushButton->setText("Disconnect");
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(true);
    }else {
        ui->pushButton->setText("Create");
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(false);
    }
}

void Dialog::updateUI()
{
    if (cs.curSide) {//Host
        ui->label->setStyleSheet("color: red; font-weight: bold;");
        ui->label_2->setStyleSheet("");
        ui->lcdLeft->display(qMax(0, cs.countDown));
        ui->lcdLeft_2->display(0);
        ui->lcdTotal->display(cs.timeUsage[cs.curSide].toString("mm:ss"));
        ui->lcdTotal_2->display(cs.timeUsage[cs.curSide^1].toString("mm:ss"));
    }else {
        ui->label_2->setStyleSheet("color: red; font-weight: bold;");
        ui->label->setStyleSheet("");
        ui->lcdLeft->display(0);
        ui->lcdLeft_2->display(qMax(0, cs.countDown));
        ui->lcdTotal_2->display(cs.timeUsage[cs.curSide].toString("mm:ss"));
        ui->lcdTotal->display(cs.timeUsage[cs.curSide^1].toString("mm:ss"));
    }
//    ui->pushButton_3->setEnabled(!ui->widget->ready[ui->widget->isHost]);
}

void Dialog::on_pushButton_clicked()
{
    if (ui->pushButton->text() == "Create") {
        IpDialog *dialog = new IpDialog(this);
        QString ip, port;
        if (dialog->exec(ip, port) == QDialog::Accepted) {
            ui->widget->init(true, this);
            ui->widget->setServer(ip, port);
            setWindowTitle("Host");
        }
    }else {
        ui->widget->disconnectServer();
        setButtonConnect(false);
    }
}

void Dialog::on_pushButton_2_clicked()
{
    IpDialog *dialog = new IpDialog(this);
    QString ip, port;
    if (dialog->exec(ip, port) == QDialog::Accepted) {
        ui->widget->init(false, this);
        ui->widget->setSocket(ip, port);
        setWindowTitle("Client");
    }
}

void Dialog::on_pushButton_3_clicked()
{
//    cs.init(qrand() % 2);
    ui->pushButton_3->setDisabled(true);
    ui->widget->toggleReady(true);
//    ui->widget->start();
}
