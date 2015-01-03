#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>
#include <QGraphicsPixmapItem>
#include "createpack.h"
#include "pack.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QVector<Pack*> packs;
    int activePack;
    QString outputDir;
    QString proFile;

private slots:
    void about();
    void newPackSlot();
    void removePackageSlot();
    void setPackage(const QModelIndex &index);
    void getOutputDir();
    void getInputDir();
    void buildAtlas();
    void saveSettingsPack();
    void saveProject();
};

#endif // MAINWINDOW_H
