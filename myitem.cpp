#include "myitem.h"

myItem::myItem()
{
    this->itemName = "";
}

myItem::myItem(QString prefix, QString name)
{
    this->setPixmap(QPixmap(":/"+ prefix + "/resourses/" + name + ".png"));
    this->itemName = name;
}

void myItem::setPic(QString prefix, QString name)
{
    this->setPixmap(QPixmap(":/"+ prefix + "/resourses/" + name + ".png"));
    this->itemName = name;
}

void myItem::setRect(int x, int y, int width, int height)
{
    this->setPixmap(this->pixmap().copy(x, y, width, height));
}

QString myItem::getName()
{
    return this->itemName;
}
