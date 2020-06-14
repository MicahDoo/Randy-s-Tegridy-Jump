#ifndef MRHANKEY_H
#define MRHANKEY_H
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>

class MrHankey: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT;
public:
    MrHankey();
public slots:
    void fly();
    void stopTimer();
    void resumeTimer();
private:
    QTimer *timer;

public:
    ~MrHankey();
};

#endif // MRHANKEY_H
