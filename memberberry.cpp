#include "memberberry.h"
#include "player.h"
#include "view.h"
#include <QDebug>
#include <QMediaPlayer>

MemberBerry::MemberBerry(sides side, QGraphicsItem * parent){
    setParentItem(parent);
    if(side == leftSide){
        direction = directions::right;
    }
    else{
        direction = directions::left;
    }
    setPixmap(QPixmap(":/Resource/MemberBerry.png").scaled(1,1));
    setZValue(MEMBERBERRY_LAYER);
    timer = new QTimer();
    timer->start(1000/FPS);
    swelltimer = new QTimer();
    swelltimer->start(1000/FPS);
    connect(timer, SIGNAL(timeout()), this, SLOT(fly()));
    connect(swelltimer, SIGNAL(timeout()), this, SLOT(swell()));
    music = new QMediaPlayer();
    int n = rand()%3;
    if(n == 1){
        qDebug() << "n = 1";
        music->setMedia(QUrl("qrc:/Resource/OohIMember.mp3"));
        music->setVolume(1000);
        music-> play();
    }
    else if(n == 0){
        qDebug() << "n = 0";
        music->setMedia(QUrl("qrc:/Resource/MemberF.mp3"));
        music->setVolume(1000);
        music-> play();
    }
    else{
        qDebug() << "n = 2";
       music->setMedia(QUrl("qrc:/Resource/MemberFeelingSafe.mp3"));
       music->setVolume(1000);
       music->play();
    }
}

void MemberBerry::fly(){
    /*if(mapFromScene(0,0).y() <-80){
        //scene()->removeItem(this);  //-->how do i delete the vector pointer too?
        timer->stop();
        swelltimer->stop();
        return;
    //}*/
    moveBy(direction, 0);
    Player* player = dynamic_cast<Game*>(scene()->views()[0])->getPlayer();
    if(player->getRandy()->collidesWithItem(this)){
        if(!player->isProtectedMode()){
            if(!player->isFlipMode()){
                scene()->views()[0]->scale(1,-1);
                scene()->views()[0]->translate(0, 700);
                player->setFlipEndDistance(player->getTravelDistance()+FLIP_DISTANCE);
                player->setFlipMode(true);
            }
        }
    }
}

void MemberBerry::swell(){
    if(pixmap().rect().height() < 30){
        setPixmap(QPixmap(":/Resource/MemberBerry.png").scaled(pixmap().rect().height()+1,pixmap().rect().width()+1));
        moveBy(0, -0.5);
    }
    else{
        swelltimer->stop();
    }
}

void MemberBerry::stopTimer(){
    timer->stop();
    swelltimer->stop();
    qDebug() << "timers stopped";
}

void MemberBerry::stopSound(QMediaPlayer* player){
    player->stop();
}

void MemberBerry::resumeTimer(){
    timer->start();
    swelltimer->start(1000/FPS);
}

MemberBerry::~MemberBerry(){
    delete music;
    delete timer;
    delete swelltimer;
}
