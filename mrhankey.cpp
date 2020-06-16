#include "mrhankey.h"
#include "numbers.h"
#include "view.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QMediaPlayer>

MrHankey::MrHankey() : timer(new QTimer(this)){
    setPixmap(QPixmap(":/Resource/MrHankey.png").scaled(100,80));
    setZValue(4);
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()),this,SLOT(fly()));
    timer->start(1000/FPS);
    QMediaPlayer* howdyho = new QMediaPlayer();
    howdyho -> setMedia(QUrl("qrc:/Resource/HowdyHow.mp3"));
    howdyho->setVolume(30);
    howdyho->play();
}

void MrHankey::fly(){
    moveBy(0,-10 * 48.0/FPS);
    if(mapToScene(0,0).y() < 0-40){
        qDebug() << "hankey deleted";
        scene()->removeItem(this);
        timer->stop();
        delete this;
        return;
    }

    // Don't use "=" because it will created a copy and remove doesn't work on the original vector
    // When you return a vector it gets copied, try returning it's pointer instead
    QVector<Monster*> *monsters = dynamic_cast<Game*>(scene()->views()[0])->getPlayer()->getMonsters();
    for(int i = 0; i < monsters->size(); ++i){

        // if the berry collides with one monster
        if((*monsters)[i]->collidesWithItem(this)){

            qDebug() << "colliding with" << QString::pointer((*monsters)[i]);

            // stop the monster's timer
            (*monsters)[i]->stopTimer();

            // remove the monster from the scene
            scene()->removeItem((*monsters)[i]);

            // remove the monster from the vector
            (*monsters).remove(i);

            scene()->removeItem(this);
            timer->stop();
            delete this;
            return;
        }
    }
}

MrHankey::~MrHankey(){
    delete timer;
}

void MrHankey::stopTimer(){
    timer->stop();
}

void MrHankey::resumeTimer(){
    timer->start(1000/FPS);
}
