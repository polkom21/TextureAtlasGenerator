#include "createpack.h"
#include "ui_createpack.h"

CreatePack::CreatePack(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreatePack)
{
    ui->setupUi(this);
    this->setWindowTitle("Nowa paczka");
    this->exec();
}

CreatePack::~CreatePack()
{
    delete ui;
}

void CreatePack::accept()
{
    packName = ui->packNameInput->text();
    this->done(0);
}
