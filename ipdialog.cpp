#include "ipdialog.h"
#include<QDialogButtonBox>
#include<QGridLayout>
#include<QPushButton>
#include<QHostAddress>
#include<QPointer>
#include<QKeyEvent>
#include<QHostInfo>
#include<QNetworkInterface>
#include<QApplication>

IpDialog::IpDialog(QWidget *parent) : QDialog(parent), hostCombo(new QComboBox)
{
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |     QDialogButtonBox::Cancel);
    QGridLayout *la = new QGridLayout(this);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    hostCombo->setEditable(true);
    portLineEdit = new QLineEdit("8888", this);
    portLineEdit->setValidator(new QIntValidator(1, 65535, this));
    // find out name of this machine
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        hostCombo->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
            hostCombo->addItem(name + QChar('.') + domain);
    }
    if (name != QLatin1String("localhost"))
        hostCombo->addItem(QString("localhost"));
    // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }
    // add localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }
    QPushButton *buttons[12];
    QGridLayout *numBox = new QGridLayout;
    for (qint8 i = 0; i < 11; ++i) {
        buttons[i] = new QPushButton(QString::number(i), this);
        buttons[i]->setFocusPolicy(Qt::NoFocus);
        connect(buttons[i], &QPushButton::clicked, [=](){
            QPointer<QWidget> w = focusWidget();
            if (!w) return;
            QChar character = buttons[i]->text()[0];
            QKeyEvent keyPress(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString(character));
            QApplication::sendEvent(w, &keyPress);
            QKeyEvent keyRelease(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString());
            QApplication::sendEvent(w, &keyRelease);
        });
    }
    buttons[11] = new QPushButton(this);
    buttons[11]->setFocusPolicy(Qt::NoFocus);
    connect(buttons[11], &QPushButton::clicked, [=](){
        QPointer<QWidget> w = focusWidget();
        if (!w) return;
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        QApplication::sendEvent(w, &keyPress);
    });
    buttons[10]->setText(".");
    buttons[11]->setText("Del");
    numBox->addWidget(buttons[0], 3, 0);
    numBox->addWidget(buttons[10], 3, 1);
    numBox->addWidget(buttons[11], 3, 2);
    for (int i = 1; i <= 9; ++i)
        numBox->addWidget(buttons[i], 2 - (i - 1) / 3, (i - 1) % 3);
    la->addWidget(buttonBox, 4, 1);
    la->addWidget(hostCombo, 1, 1, 1, 2);
    la->addWidget(portLineEdit, 2, 1, 1, 2);
    la->addLayout(numBox, 3 ,1);
}

int IpDialog::exec(QString &ip, QString &port)
{
    int res = QDialog::exec();
    ip = hostCombo->currentText();
    port =portLineEdit->text();
    return res;
}
