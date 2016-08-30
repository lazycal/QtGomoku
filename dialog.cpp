#include "dialog.h"
#include "ui_dialog.h"
#include<QFont>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setStyleSheet("QLabel{font-size: 15px;}");
//    ui->label_2->setStyleSheet("font-size: 15px;");
}

Dialog::~Dialog()
{
    delete ui;
}
