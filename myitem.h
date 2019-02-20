#ifndef MYITEM_H
#define MYITEM_H

#include <QGraphicsPixmapItem>
#include <QString>
#include <QPixmap>

class myItem : public QGraphicsPixmapItem
{
public:
    myItem();
    myItem(QString prefix, QString name);
    void setPic(QString prefix, QString name);
    void setRect(int x, int y, int width, int height);
    QString getName();

private:
    QString itemName;
};

#endif // MYITEM_H
