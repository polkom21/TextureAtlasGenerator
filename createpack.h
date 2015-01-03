#ifndef CREATEPACK_H
#define CREATEPACK_H

#include <QDialog>
#include <QString>

namespace Ui {
class CreatePack;
}

class CreatePack : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePack(QWidget *parent = 0);
    ~CreatePack();
    QString packName;

private:
    Ui::CreatePack *ui;

private slots:
    void accept();
};

#endif // CREATEPACK_H
