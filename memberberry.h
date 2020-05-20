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
    void stopTimer();

public slots:
    void fly();
    void swell();
    void stopSound(QMediaPlayer* player);

private:
    QTimer *timer;
    directions direction;
    QTimer *swelltimer;
    QMediaPlayer *music;

public:
    ~MemberBerry();
};



#endif // MEMBERBERRY_H
