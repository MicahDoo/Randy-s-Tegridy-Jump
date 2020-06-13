#include "eric.h"
#include "numbers.h"
#include "memberberry.h"
#include <QDebug>
#include <QMediaPlayer>

Eric::Eric(sides s, QGraphicsItem *parent) : QGraphicsPixmapItem(parent){
    side = s;
    timer = new QTimer();
    singleshottimer = new QTimer();
    backtimer = new QTimer();
    singleshottimer->setSingleShot(true);
    backtimer->setSingleShot(true);
    if(side == leftSide){
        setPixmap(QPixmap(":/Resource/Eric Cartman Left.png").scaled(ERIC_SIZE,ERIC_SIZE));
    }
    else{
        setPixmap(QPixmap(":/Resource/Eric Cartman Right.png").scaled(ERIC_SIZE,ERIC_SIZE));
    }
    connect(timer, SIGNAL(timeout()), this, SLOT(ready()));
    connect(singleshottimer, SIGNAL(timeout()), this, SLOT(shoot()));
    connect(backtimer,SIGNAL(timeout()), this, SLOT(back()));
    timer -> start(1000/FPS);
    setZValue(CHARACTER_LAYER);
    QMediaPlayer *respectmah = new QMediaPlayer();
    respectmah -> setMedia(QUrl("qrc:/Resource/Respect My Authority.mp3"));
    respectmah -> setVolume(30);
    respectmah -> play();
}

void Eric::ready(){
    backtimer->stop();
    if(mapToScene(0,0).y() < 0 - 150 || mapToScene(0,0).y() > SCREEN_HEIGHT){
        //scene()->removeItem(this);  //-->how do i delete the vector pointer too?
        timer->stop();
        singleshottimer->stop();
        backtimer->stop();
        //delete this;
        return;
    }
    if(rand()%100 == 1){
        qDebug() << "rand()%100 == 1";
        timer->stop();
        QMediaPlayer *cool = new QMediaPlayer();
        cool -> setMedia(QUrl("qrc:/Resource/Cool.mp3"));
        cool -> setVolume(30);
        cool -> play();
        if(side == leftSide){
            setPixmap(QPixmap(":/Resource/EricLeft.png").scaled(ERIC_SIZE*1.2,ERIC_SIZE));
        }
        else{
            setPixmap(QPixmap(":/Resource/EricRight.png").scaled(ERIC_SIZE*1.2,ERIC_SIZE));
        }
        singleshottimer->start(1000);
    }
}

void Eric::shoot(){
    memberberries.push_back(new MemberBerry(side, this));
    qDebug() << "new memberberry" << memberberries.size()-1 << "from" << QString::pointer(this);
    if(side == leftSide){
        setPixmap(QPixmap(":/Resource/EricShootingLeft.png").scaled(ERIC_SIZE*1.2,ERIC_SIZE));
        memberberries[memberberries.size()-1]->setPos(ERIC_SIZE*1.18,ERIC_SIZE*0.45);
    }
    else{
        setPixmap(QPixmap(":/Resource/EricShootingRight.png").scaled(ERIC_SIZE*1.2,ERIC_SIZE));
        memberberries[memberberries.size()-1]->setPos(0,ERIC_SIZE*0.45);
    }
    backtimer->stop();
    backtimer->start(1000);
}

void Eric::back(){
    qDebug() << "turning back";
    if(side == leftSide){
        setPixmap(QPixmap(":/Resource/Eric Cartman Left.png").scaled(ERIC_SIZE,ERIC_SIZE));
    }
    else{
        setPixmap(QPixmap(":/Resource/Eric Cartman Right.png").scaled(ERIC_SIZE,ERIC_SIZE));
    }
    timer->start(1000/FPS);
    singleshottimer->stop();

}

QVector<MemberBerry*> * Eric::getMemberberries(){
    return &memberberries;
}

void Eric::stopTimer(){
    timer->stop();
    singleshottimerRemain = singleshottimer->remainingTime();
    qDebug() << "singleshottimerRemain = " << singleshottimerRemain;
    singleshottimer->stop();
    backtimerRemain = backtimer->remainingTime();
    qDebug() << "backtimerRemain = " << backtimerRemain;
    backtimer->stop();
    for (int i = 0; i < memberberries.size(); i++){
        memberberries[i]->stopTimer();
    }
}

void Eric::resumeTimer(){
    if(singleshottimerRemain >= 0){
        singleshottimer->start(singleshottimerRemain);
        qDebug() << "singleshottimerRemain = " << singleshottimerRemain;
        singleshottimerRemain = -1.0;
    }
    else if(backtimerRemain >= 0){
        backtimer->start(backtimerRemain);
        qDebug() << "backtimerRemain = " << backtimerRemain;
        backtimerRemain = -1.0;
    }
    else{
        timer->start(1000/FPS);
    }
    for (int i = 0; i < memberberries.size(); i++){
        memberberries[i]->resumeTimer();
    }
}

Eric::~Eric(){
    delete timer;
    delete singleshottimer;
    delete backtimer;

    for (int i = 0; i < memberberries.size(); i++)
        delete memberberries[i];
    memberberries.clear();
}
