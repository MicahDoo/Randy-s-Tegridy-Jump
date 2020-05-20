#ifndef PLATFORMS_H
#define PLATFORMS_H

#include <QGraphicsPixmapItem>

class Platform: public QGraphicsPixmapItem{
public:
    //Platform();
    Platform(QGraphicsItem * parent = nullptr);
    virtual void response(); //some platforms respond by breaking or jiggling
    virtual void behavior(); //some platforms move constanly
private:
    QTimer *timer;
};

class Normal: public Platform{
public:
    Normal(QGraphicsItem * parent = nullptr);
    void response();
    void behavior();
private:
    QTimer *timer;
};

class Elastic: public Platform{
public:
    Elastic(QGraphicsItem * parent = nullptr);
    void response();
    void behavior();
private:
    QTimer *timer;
};

/*class Horizontal: public Platform{
public:
    Horizontal(QGraphicsItem * parent = nullptr);
    void response();
    void behavior();
};

class Breakable: public Platform{
public:
    Breakable(QGraphicsItem * parent = nullptr);
    void response();
    void behavior();
};*/





#endif // PLATFORMS_H
