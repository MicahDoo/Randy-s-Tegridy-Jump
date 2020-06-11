#include "monsters.h"
#include "numbers.h"
#include "view.h"
#include <QtMath>
#include <QDebug>
#include <QMediaPlayer>

Monster::Monster(QGraphicsItem *parent){
    setParentItem(parent);
    if(rand()%2 == 1){
        monstertype = monstertypes::satan;
        setPixmap(QPixmap(":/Resource/Satan.PNG").scaled(100,MONSTER_HEIGHT));
        QMediaPlayer *satanhowl = new QMediaPlayer;
        satanhowl->setMedia(QUrl("qrc:/Resource/Satan.mp3"));
        satanhowl->setVolume(30);
        satanhowl->play();
    }
    else{
        monstertype = monstertypes::manbearpig;
        setPixmap(QPixmap(":/Resource/ManBearPig.PNG").scaled(100,MONSTER_HEIGHT));
        QMediaPlayer *manbearpig = new QMediaPlayer;
        manbearpig ->setMedia(QUrl("qrc:/Resource/ManBearPig.mp3"));
        manbearpig->setVolume(30);
        manbearpig ->play();
    }
    setZValue(CHARACTER_LAYER);
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()),this,SLOT(move()));
    timer -> start(1000/FPS);
}

void Monster::move(){
    moveBy(2.0*qCos(t),0);
    //qDebug() << "moving";
    t = t + 0.02;

    if(scene() != nullptr){
        Player* player = dynamic_cast<Game*>(scene()->views()[0])->getPlayer();
        if(player->getRandy()->collidesWithItem(this)){
            if(mapFromItem(player,0,0).y() < MONSTER_HEIGHT - 5){
                if(!player->isProtectedMode())
                    player->setFail();
            }
            else{
                pixmap().scaled(100,MONSTER_HEIGHT/2.0);
                moveBy(0,50);
                timer->stop();
                return;
            }
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
