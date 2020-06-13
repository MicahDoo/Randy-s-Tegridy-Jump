#ifndef PROPS_H
#define PROPS_H
#include <QGraphicsPixmapItem>
#include <QMediaPlayer>

class Prop: public QGraphicsPixmapItem{
public:
    Prop(QGraphicsItem * parent = nullptr);
    virtual void response();
};

class Wings: public Prop{
public:
    Wings(QGraphicsItem * parent = nullptr);
    void response();
};

class Weed: public Prop{
public:
    Weed(QGraphicsItem * parent = nullptr);
    void response();
};

class Kenny: public Prop{
public:
    Kenny(QGraphicsItem * parent = nullptr);
    void response();
public:
    ~Kenny();
};

#endif // PROPS_H
