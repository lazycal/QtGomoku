#include "ipdialog.h"
#include<QDialogButtonBox>
#include<QGridLayout>

IpDialog::IpDialog(QWidget *parent) : QDialog(parent)
{
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |     QDialogButtonBox::Cancel);
    QGridLayout *la = new QGridLayout(this);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    la->addWidget(buttonBox, 3, 1);
    lineEdit = new QLineEdit("127.0.0.1", this);
    portLineEdit = new QLineEdit("8888", this);
    la->addWidget(lineEdit, 1, 1, 1, 2);
    la->addWidget(portLineEdit, 2, 1, 1, 2);
}

int IpDialog::exec(QString &ip, QString &port)
{
    int res = QDialog::exec();
    ip = lineEdit->text();
    port =portLineEdit->text();
    return res;
}
