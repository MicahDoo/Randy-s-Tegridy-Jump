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
    void stopTimer();
    void resumeTimer();

public slots:
    void ready();
    void shoot();
    void back();
private:
    QTimer *timer;
    QTimer *singleshottimer;
    QTimer *backtimer;
    sides side;
    QVector<MemberBerry*> memberberries;
    double singleshottimerRemain = -1.0;
    double backtimerRemain = -1.0;
public:
    ~Eric();
};

#endif // ERIC_H
