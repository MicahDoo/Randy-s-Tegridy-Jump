#ifndef MEMBERBERRY_H
#define MEMBERBERRY_H

#include "numbers.h"
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QVector>
#include <QMediaPlayer>

class MemberBerry: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    MemberBerry(sides side, QGraphicsItem *parent);

public slots:
    void fly();
    void swell();
    void stopSound(QMediaPlayer* player);
    void flipping();
    void stopTimer();
    void resumeTimer();

private:
    QTimer *timer;
    directions direction;
    QTimer *swelltimer;
    QTimer *pauseandfliptimer;
    int flipangle;
    double scalefactor;
    QMediaPlayer *music;
    QMediaPlayer *nya;

public:
    ~MemberBerry();
};



#endif // MEMBERBERRY_H
