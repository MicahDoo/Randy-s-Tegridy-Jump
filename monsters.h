#ifndef MONSTERS_H
#define MONSTERS_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class Monster: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT;
public:
    Monster(QGraphicsItem *parent);
public slots:
    void move();
    void stopTimer();
    void resumeTimer();
private:
    enum monstertypes{satan, manbearpig};
    monstertypes monstertype;
    QTimer *timer;
    double t = 0.0;

public:
    ~Monster();
};

#endif // MONSTERS_H

