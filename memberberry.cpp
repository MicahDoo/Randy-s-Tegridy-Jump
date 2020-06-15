#include "memberberry.h"
#include "player.h"
#include "view.h"
#include <QDebug>
#include <QMediaPlayer>

MemberBerry::MemberBerry (sides side, QGraphicsItem * parent) : QGraphicsPixmapItem(parent), timer(new QTimer(this)), swelltimer(new QTimer(this)), pauseandfliptimer(new QTimer(this)), music(new QMediaPlayer(this)), nya(new QMediaPlayer(this)){
    if(side == leftSide){
        direction = myDirections::directions::right;
    }
    else{
        direction = myDirections::directions::left;
    }
    setPixmap(QPixmap(":/Resource/MemberBerry.png").scaled(1,1));
    setZValue(MEMBERBERRY_LAYER);
    timer->start(1000/FPS);
    swelltimer->start(1000/FPS);
    connect(timer, SIGNAL(timeout()), this, SLOT(fly()));
    connect(swelltimer, SIGNAL(timeout()), this, SLOT(swell()));
    int n = rand()%3;
    music->setVolume(1000);
    if(n == 1){
        music->setMedia(QUrl("qrc:/Resource/OohIMember.mp3"));
        music-> play();
    }
    else if(n == 0){
        music->setMedia(QUrl("qrc:/Resource/MemberF.mp3"));
        music-> play();
    }
    else{
       music->setMedia(QUrl("qrc:/Resource/MemberFeelingSafe.mp3"));
       music->play();
    }
    connect(pauseandfliptimer, SIGNAL(timeout()), this, SLOT(flipping()));
}

void MemberBerry::fly(){
    moveBy(direction*48.0/FPS, 0);
    if(scene() != nullptr){
        Player* player = dynamic_cast<Game*>(scene()->views()[0])->getPlayer();
        if(!player->isFail()){
            if(!player->isProtectedMode()){
                if(!player->isFlipMode()){
                    if(player->getRandy()->collidesWithItem(this)){
                        nya->setMedia(QUrl("qrc:/Resource/CartmanNya.mp3"));
                        nya->setVolume(100);
                        nya-> play();
                        flipangle = 0;
                        scalefactor = 90.0;
                        pauseandfliptimer->start(1000/FPS);
                        player->setPause(true);
                        player->setFlippingMode(true);
                        player->setFlipEndDistance(player->getTravelDistance()+FLIP_DISTANCE);
                        player->setFlipMode(true);
                    }
                }
            }
        }
    }else{
        timer->stop();
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
        player->setFlippingMode(false);
    }
}

MemberBerry::~MemberBerry(){
    delete music;
    delete timer;
    delete swelltimer;
    delete nya;
}
