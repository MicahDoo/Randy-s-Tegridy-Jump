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
    nya = new QMediaPlayer();
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
    pauseandfliptimer = new QTimer();
    connect(pauseandfliptimer, SIGNAL(timeout()), this, SLOT(flipping()));
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
                nya->setMedia(QUrl("qrc:/Resource/CartmanNya.mp3"));
                nya->setVolume(100);
                nya-> play();
                flipangle = 0;
                scalefactor = 90.0;
                pauseandfliptimer->start(1000/FPS);
                player->setPause(true);
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

void MemberBerry::flipping(){
    if(flipangle<90){
        scene()->views()[0]->rotate(2);
        ++flipangle;
    }
    else if(scalefactor>-90){
        if(scalefactor != 2){
            scene()->views()[0]->scale((scalefactor-2)/scalefactor,1);
            scalefactor = scalefactor - 2;
        }
        else{
            scene()->views()[0]->scale(-1, 1);
            scalefactor= -2;
        }
    }else{
        scene()->views()[0]->resetTransform();
        scene()->views()[0]->scale(1,-1);
        pauseandfliptimer->stop();
        Player* player = dynamic_cast<Game*>(scene()->views()[0])->getPlayer();
        player->setPause(false);
    }
}

MemberBerry::~MemberBerry(){
    delete music;
    delete timer;
    delete swelltimer;
    delete nya;
}
