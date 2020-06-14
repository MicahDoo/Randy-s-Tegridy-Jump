#ifndef ERIC_H
#define ERIC_H
#include "memberberry.h"
#include "numbers.h"
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QObject>
#include <QVector>

class Eric: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Eric(sides s, QGraphicsItem *parent);
    QVector<MemberBerry*> * getMemberberries();

public slots:
    void ready();
    void shoot();
    void back();
    void stopTimer();
    void resumeTimer();
signals:
    void paused();
    void resumed();
private:
    QTimer *timer;
    QTimer *singleshottimer;
    QTimer *backtimer;
    sides side;
    QVector<MemberBerry*> memberberries;
    double singleshottimerRemain;
    double backtimerRemain;
public:
    ~Eric();
};

#endif // ERIC_H
