#ifndef PLATFORMS_H
#define PLATFORMS_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QTimer>

class Platform: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    //Platform();
    Platform(QGraphicsItem * parent = nullptr);
    ~Platform();
    virtual void response(); //some platforms respond by breaking or jiggling
    virtual void behavior(); //some platforms move constanly

public slots:
    void stopTimer();
    void resumeTimer();

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
    Q_OBJECT
public:
    Elastic(QGraphicsItem * parent = nullptr);
    ~Elastic();
    void response();
    void behavior();
public: signals:
    void spring();
private:

};

class Horizontal: public Platform{
    Q_OBJECT
public:
    Horizontal(QGraphicsItem * parent = nullptr);
    ~Horizontal();
    void response();
    void behavior();
public slots:
    void move();
private:
    double t = 0.0;
};

/*class Breakable: public Platform{
public:
    Breakable(QGraphicsItem * parent = nullptr);
    void response();
    void behavior();
};*/





#endif // PLATFORMS_H
