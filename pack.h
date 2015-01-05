#ifndef PACK_H
#define PACK_H

#include <QString>
#include <QGraphicsScene>
#include <QDir>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QMap>
#include <QDebug>
#include <QXmlStreamWriter>

class Pack: public QGraphicsScene
{
public:
    Pack(QString name);

    QString packName;
    int oputputFormat; // 0 = PNG
    QString inputDir;
    int width;
    int height;

    QMap<QString, QRect> graphics;

    void buildAtlas(QString outputFolder);

private:
    void saveToXML(QString outputFolder);
};

#endif // PACK_H
