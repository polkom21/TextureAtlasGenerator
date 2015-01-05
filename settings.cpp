#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    this->setWindowTitle("Ustawienia");
    QString configFile = QString("%1/config.ini").arg(QDir::currentPath());
    conf = new QSettings(configFile, QSettings::IniFormat);
    if(!QFile().exists(configFile))
    {
        conf->setValue("default/outputFolder", QString("%1/Desktop").arg(QDir::homePath()));
        conf->setValue("default/atlasWidth", 1024);
        conf->setValue("default/atlasHeight", 1024);
        conf->sync();
    }

    ui->atlasWidth->setValue(conf->value("default/atlasWidth").toInt());
    ui->atlasHeight->setValue(conf->value("default/atlasHeight").toInt());

    connect(ui->saveSettings, SIGNAL(clicked()), this, SLOT(saveSettings()));
}

Settings::~Settings()
{
    delete ui;
}

// slots
void Settings::saveSettings()
{
    conf->setValue("default/atlasWidth", ui->atlasWidth->value());
    conf->setValue("default/atlasHeight", ui->atlasHeight->value());

    conf->sync();
    qDebug() << "Settings saved";
}
