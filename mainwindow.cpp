#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = new Settings(this);

    // set elements to default values
    ui->outputFolder->setText(settings->conf->value("default/outputFolder").toString());
    ui->widthPack->setValue(settings->conf->value("default/atlasWidth").toInt());
    ui->heightPack->setValue(settings->conf->value("default/atlasHeight").toInt());


    this->setWindowTitle("Texture Atlas Generator");
    connect(ui->about, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->newPack, SIGNAL(clicked()), this, SLOT(newPackSlot()));
    connect(ui->packs, SIGNAL(clicked(QModelIndex)), this, SLOT(setPackage(QModelIndex)));
    connect(ui->removePack, SIGNAL(clicked()), this, SLOT(removePackageSlot()));
    connect(ui->outputFolderBtn, SIGNAL(clicked()), this, SLOT(getOutputDir()));
    connect(ui->inputFolderBtn, SIGNAL(clicked()), this, SLOT(getInputDir()));
    connect(ui->packAtlas, SIGNAL(clicked()), this, SLOT(buildAtlas()));
    connect(ui->saveSettings, SIGNAL(clicked()), this, SLOT(saveSettingsPack()));
    connect(ui->actionZapisz, SIGNAL(triggered()), this, SLOT(saveProject()));
    connect(ui->actionUstawienia, SIGNAL(triggered()), settings, SLOT(open()));
    connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(openProject()));
    connect(ui->actionNowy, SIGNAL(triggered()), this, SLOT(newProject()));

    QLabel *statusLabel = new QLabel(this);
    ui->statusBar->addPermanentWidget(statusLabel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("O programie"), QString("<p>Jakiś tekst</p>"));
}

void MainWindow::newPackSlot()
{
    CreatePack *pack = new CreatePack(this);
    QListWidgetItem *item = new QListWidgetItem;
    //qDebug() << "Pack name is: " << pack->packName;
    item->setText(pack->packName);
    ui->inputFolderDir->setText("");
    ui->packs->addItem(item);
    packs.push_back(new Pack(pack->packName));
    ui->removePack->setDisabled(false);
    ui->statusBar->showMessage(QString("Pack \"" + pack->packName + "\" created."), 2000);
}

void MainWindow::removePackageSlot()
{
    //ui->packs->removeItemWidget(ui->packs->currentItem());
    packs.remove(ui->packs->currentIndex().row());
    delete ui->packs->item(ui->packs->currentIndex().row());
    ui->packSettings->setDisabled(true);
    if(ui->packs->currentRow() < 0) ui->removePack->setDisabled(true);
    //qDebug() << "removePackage" << ui->packs->currentIndex().row();
    ui->statusBar->showMessage("Package removed.", 2000);
}

void MainWindow::setPackage(const QModelIndex &index)
{
    //qDebug() << "Package setted " << index.row() << "  " << activePack;

    ui->activePackName->setText(ui->packs->item(index.row())->text());
    activePack = index.row();
    ui->packSettings->setDisabled(false);

    ui->inputFolderDir->setText(packs[activePack]->inputDir);
    ui->widthPack->setValue(packs[activePack]->width);
    ui->heightPack->setValue(packs[activePack]->height);

    ui->graphicsView->setScene(packs[activePack]);
}

void MainWindow::getOutputDir()
{
    outputDir = QFileDialog::getExistingDirectory(this, tr("Wybierz folder"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->outputFolder->setText(outputDir);
}

void MainWindow::getInputDir()
{
    QString inputDir = QFileDialog::getExistingDirectory(this, tr("Wybierz folder"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->inputFolderDir->setText(inputDir);
    packs[activePack]->inputDir = inputDir;
}

void MainWindow::buildAtlas()
{
    this->saveSettingsPack();
    packs[activePack]->buildAtlas(ui->outputFolder->text());
    ui->graphicsView->setScene(packs[activePack]);
}

void MainWindow::saveSettingsPack()
{
    // save settings
    packs[activePack]->inputDir = ui->inputFolderDir->text();
    packs[activePack]->width = ui->widthPack->value();
    packs[activePack]->height = ui->heightPack->value();

    ui->statusBar->showMessage(QString("Settings this package saved."), 2000);
}

void MainWindow::saveProject()
{
    if(proFile == "")
    {
        proFile = QFileDialog::getSaveFileName(0, tr("Zapisz jako"), QString("%1/Desktop").arg(QDir::homePath()), QString("*.ta"));
    }

    QString project;
    QFile file(QString("%1.ta").arg(proFile));
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter *writer = new QXmlStreamWriter(&file);

    writer->setAutoFormatting(true);

    writer->writeStartElement("atlases");
    writer->writeAttribute("outputFolder", ui->outputFolder->text());

    for(int i = 0; i < packs.size(); i++)
    {
        writer->writeStartElement("Texture");
        writer->writeAttribute("name", packs[i]->packName);
        writer->writeAttribute("width", QString("%1").arg(packs[i]->width));
        writer->writeAttribute("height", QString("%1").arg(packs[i]->height));
        writer->writeAttribute("inputDir", packs[i]->inputDir);
        writer->writeEndElement();
    }

    writer->writeEndElement();

    file.close();

    ui->statusBar->showMessage(QString("Projekt został zapisany."), 2000);
}

void MainWindow::openProject()
{
    proFile = QFileDialog::getOpenFileName(0, tr("Otwórz projekt"), QString("%1/Desktop").arg(QDir::homePath()), QString("*.ta"));

    QFile file(proFile);
    file.open(QIODevice::ReadOnly);
    QXmlStreamReader xmlStream(&file);

    this->newProject();

    while(!xmlStream.atEnd())
    {
        if(xmlStream.isStartElement())
        {
            if(xmlStream.name() == QString("atlases")){
                outputDir = xmlStream.attributes().first().value().toString();
                ui->outputFolder->setText(outputDir);
            } else if(xmlStream.name() == QString("Texture")){
                QXmlStreamAttributes attributes = xmlStream.attributes();
                QString textureName = attributes.value("name").toString();
                QListWidgetItem *item = new QListWidgetItem;
                item->setText(textureName);
                ui->packs->addItem(item);
                packs.push_back(new Pack(textureName));
                packs.last()->inputDir = attributes.value("inputDir").toString();
                packs.last()->width = attributes.value("width").toInt();
                packs.last()->height = attributes.value("height").toInt();
                packs.last()->buildAtlas(outputDir);
            }
        }
        xmlStream.readNext();
    }

    file.close();
}

void MainWindow::newProject()
{
    // clear all inputs
    ui->packs->clear();
    this->packs.clear();
    proFile.clear();
    ui->packSettings->setDisabled(true);
    ui->removePack->setDisabled(true);
    ui->outputFolder->setText(settings->conf->value("default/outputFolder").toString());
    ui->graphicsView->setScene(new QGraphicsScene());
}
