#include "pack.h"

Pack::Pack(QString name)
{
    packName = name;
    oputputFormat = 0;
    inputDir = "";
    width = 1024;
    height = 1024;
}

void Pack::buildAtlas(QString outputFolder)
{
    try{
        this->clear();
        graphics.clear();

        QDir path(inputDir);
        QStringList types;
        types << "*.png";

        QStringList files = path.entryList(types, QDir::Files);

        this->setSceneRect(0, 0, width, height);

        // the fastest generator

        int actualX = 0, actualY = 0;
        int maxYPerLine = 0;
        int x, y;

        for(int i = 0; i < files.size(); i++)
        {
            QPixmap image(QString("%1/%2").arg(inputDir, files.at(i).toHtmlEscaped()));
            QGraphicsPixmapItem *item = this->addPixmap(image);

            if(actualY + image.height() > height)
            {
                QMessageBox::critical(0, tr("Błąd"), QString("Zbyt mały obszar na stworzenie atlasu tekstur!"));
                throw(QString("Zbyt mały obszar na stworzenie atlasu tekstur!"));
            }

            if(actualX + image.width() <= width)
            {
                x = actualX;
                y = actualY;
                actualX += image.width();
            } else {
                actualX = image.width();
                x = 0;
                actualY += maxYPerLine;
                y = actualY;
                maxYPerLine = 0;
            }

            if(image.height() > maxYPerLine) maxYPerLine = image.height();

            item->setOffset(x, y);
            //item->setFlag(QGraphicsItem::ItemIsSelectable);
            graphics.insert(files.at(i).toHtmlEscaped(), QRect(x, y, image.width(), image.height()));
        }

        QImage atlas(QSize(width, height), QImage::Format_ARGB32);
        QPainter painter(&atlas);
        this->render(&painter, QRect(0, 0, width, height));
        atlas.save(QString("%1/%2.png").arg(outputFolder, packName));
        this->saveToXML(outputFolder);

        // end generator

    } catch(QString e){
        qDebug() << "Error: " << e;
        this->clear();
    }
}

void Pack::saveToXML(QString outputFolder)
{
    QString xmlFile = QString("%1/%2.xml").arg(outputFolder, packName);
    QString xml;
    QFile file(xmlFile);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter *writer = new QXmlStreamWriter(&file);
    //writer->setDevice(&file);
    writer->setAutoFormatting(true);

    //writer->writeStartDocument();
    writer->writeStartElement("TextureAtlas");
    writer->writeAttribute("imagePath", QString("%1.png").arg(packName));

    QMap<QString, QRect>::Iterator it;
    for(it = graphics.begin(); it != graphics.end(); ++it)
    {
        QRect rect = it.value();
        writer->writeStartElement("Texture");
        writer->writeAttribute("name", it.key());
        writer->writeAttribute("x", QString("%1").arg(rect.x()));
        writer->writeAttribute("y", QString("%1").arg(rect.y()));
        writer->writeAttribute("width", QString("%1").arg(rect.width()));
        writer->writeAttribute("height", QString("%1").arg(rect.height()));
        writer->writeEndElement();
    }

    writer->writeEndElement();
    //writer->writeEndDocument();

    file.close();
}
