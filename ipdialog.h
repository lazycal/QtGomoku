#ifndef IPDIALOG_H
#define IPDIALOG_H

#include<QDialog>
#include<QLineEdit>
#include<QComboBox>

class IpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IpDialog(QWidget *parent = 0);
    int exec(QString &ip, QString &port);
    QComboBox *hostCombo;
    QLineEdit *portLineEdit;
};

#endif // IPDIALOG_H
