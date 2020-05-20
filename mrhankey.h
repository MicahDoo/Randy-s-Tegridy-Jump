#ifndef MRHANKEY_H
#define MRHANKEY_H
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>

class MrHankey: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT;
public:
    MrHankey(QGraphicsItem *parent);
public slots:
    void fly();
private:
    QTimer *timer;

public:
    ~MrHankey();
};

#endif // MRHANKEY_H
