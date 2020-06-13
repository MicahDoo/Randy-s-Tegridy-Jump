#ifndef PLATFORMS_H
#define PLATFORMS_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QMediaPlayer>
#include <QSoundEffect>

class Platform: public QGraphicsPixmapItem{
public:
    //Platform();
    Platform(QGraphicsItem * parent = nullptr);
    void stopTimer();
    void resumeTimer();
    virtual void response(); //some platforms respond by breaking or jiggling
    virtual void behavior(); //some platforms move constanly
protected:
    QTimer *timer;
    //QSoundEffect *sound;
};

class Normal: public Platform{
public:
    Normal(QGraphicsItem * parent = nullptr);
    void response();
    void behavior();
private:
    //QTimer *timer;
};

class Elastic: public Platform{
public:
    Elastic(QGraphicsItem * parent = nullptr);
    ~Elastic();
    void response();
    void behavior();
private:
    //QTimer *timer;
};

class Horizontal: public QObject, public Platform{
    Q_OBJECT
public:
    Horizontal(QGraphicsItem * parent = nullptr);
    ~Horizontal();
    void response();
    void behavior();
public slots:
    void move();
private:
    //QTimer *timer;
    double t = 0.0;
};

/*class Breakable: public Platform{
public:
    Breakable(QGraphicsItem * parent = nullptr);
    void response();
    void behavior();
};*/





#endif // PLATFORMS_H
