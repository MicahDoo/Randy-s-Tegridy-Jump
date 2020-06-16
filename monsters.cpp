#include "monsters.h"
#include "numbers.h"
#include "view.h"
#include <QtMath>
#include <QDebug>
#include <QMediaPlayer>

Monster::Monster(QGraphicsItem *parent) : QGraphicsPixmapItem(parent), timer(new QTimer(this)){
    if(rand()%2 == 1){
        monstertype = monstertypes::satan;
        setPixmap(QPixmap(":/Resource/Satan.PNG").scaled(100,MONSTER_HEIGHT));
        QMediaPlayer *satanhowl = new QMediaPlayer(this);
        satanhowl->setMedia(QUrl("qrc:/Resource/Satan.mp3"));
        satanhowl->setVolume(30);
        satanhowl->play();
    }
    else{
        monstertype = monstertypes::manbearpig;
        setPixmap(QPixmap(":/Resource/ManBearPig.PNG").scaled(100,MONSTER_HEIGHT));
        QMediaPlayer *manbearpig = new QMediaPlayer(this);
        manbearpig ->setMedia(QUrl("qrc:/Resource/ManBearPig.mp3"));
        manbearpig->setVolume(30);
        manbearpig ->play();
    }
    setZValue(CHARACTER_LAYER);
    connect(timer, SIGNAL(timeout()),this,SLOT(move()));
    timer -> start(1000/FPS);
}

void Monster::move(){
    moveBy(2.0*qCos(t) *48.0/FPS,0);
    //qDebug() << "moving";
    t = t + 0.02 * 48.0/FPS;

    if(scene() != nullptr){
        Player* player = dynamic_cast<Game*>(scene()->views()[0])->getPlayer();
        if(player->getRandy()->collidesWithItem(this)){
            if(!player->isProtectedMode())
                player->setFail();
        }
    }
}

void Monster::stopTimer(){
    timer->stop();
}

void Monster::resumeTimer(){
    timer->start(1000/FPS);
}

Monster::~Monster(){
    delete timer;
}
