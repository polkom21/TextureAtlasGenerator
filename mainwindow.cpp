#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    qDebug() << proFile;
}
