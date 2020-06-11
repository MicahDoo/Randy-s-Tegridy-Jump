#include "platforms.h"
#include "numbers.h"
#include "view.h"
#include <QtMath>

Platform::Platform(QGraphicsItem * parent){
    setParentItem(parent);
    setPixmap(QPixmap(":/Resource/Platform.png").scaled(PLATFORM_WIDTH,PLATFORM_HEIGHT));
    setZValue(3);
    timer = new QTimer();
}

void Platform::response(){
    // none
}

void Platform::behavior(){
    // none
}

void Platform::stopTimer(){
    timer->stop();
}

void Platform::resumeTimer(){
    timer->start();
}

/*void Platform::check(){
    //if(player->getSpeed() < 0){
        if(mapToItem(player, 0, 0).x()<PLAYER_WIDTH && mapToItem(player, 0, 0).x() > -PLATFORM_WIDTH){
            if(mapFromItem(player, 0, 0).y() >= PLAYER_HEIGHT && mapFromItem(player, 0, 0).y() < PLAYER_HEIGHT - player->getSpeed());
        }
        player->setY(y() - PLAYER_HEIGHT);
        player->setLastBounce(this);
        player->Bounce();
        response();
    //}
}*/

Normal::Normal(QGraphicsItem * parent){
    setParentItem(parent);
    setPixmap(QPixmap(":/Resource/Platform.png").scaled(PLATFORM_WIDTH,PLATFORM_HEIGHT));
    setZValue(PLATFORM_LAYER);
    timer = new QTimer();
    //connect(timer,SIGNAL(timeout()), this, SLOT(check()));
    timer->start(1000/FPS);

    //player = dynamic_cast<Game*>(scene()->views()[0])->getPlayer();
}

void Normal::response(){
    // none
}

void Normal::behavior(){
    // none
}

/*void Normal::check(){
    if(player->getSpeed()<0){
        if(mapToItem(player, 0, 0).x()<PLAYER_WIDTH && mapToItem(player, 0, 0).x() > -PLATFORM_WIDTH){
            if(mapFromItem(player, 0, 0).y() >= PLAYER_HEIGHT && mapFromItem(player, 0, 0).y() < PLAYER_HEIGHT - player->getSpeed());
        }
        player->setY(y() - PLAYER_HEIGHT);
        player->setLastBounce(this);
        player->Bounce();
        response();
    }
}*/

Elastic::Elastic(QGraphicsItem * parent){
    setParentItem(parent);
    setPixmap(QPixmap(":/Resource/ElasticStill.png").scaled(PLATFORM_WIDTH,PLATFORM_HEIGHT+40));
    setZValue(PLATFORM_LAYER);
    timer = new QTimer();
    //connect(timer,SIGNAL(timeout()), this, SLOT(check()));
    timer->start(1000/FPS);

    //player = dynamic_cast<Game*>(scene()->views()[0])->getPlayer();
}

void Elastic::response(){
    setPixmap(QPixmap(":/Resource/ElasticSet.png").scaled(PLATFORM_WIDTH,PLATFORM_HEIGHT+40));
    moveBy(0, -20);
    Game* game = dynamic_cast<Game*>(scene()->views()[0]);
    game->getPlayer()->setElasticMode(true);
    game->getPlayer()->setBOTTOM(1000.0);
}

void Elastic::behavior(){
    // none
}

Elastic::~Elastic(){
    delete timer;
}

/*void Elastic::check(){
    if(player->getSpeed()<0){
        if(mapToItem(player, 0, 0).x()<PLAYER_WIDTH && mapToItem(player, 0, 0).x() > -PLATFORM_WIDTH){
            if(mapFromItem(player, 0, 0).y() >= PLAYER_HEIGHT && mapFromItem(player, 0, 0).y() < PLAYER_HEIGHT - player->getSpeed());
        }
        player->setY(y() - PLAYER_HEIGHT);
        player->setLastBounce(this);
        player->Bounce();
        response();
    }
}*/

Horizontal::Horizontal(QGraphicsItem * parent){
    setParentItem(parent);
    timer = new QTimer();
    setPixmap(QPixmap(":/Resource/Platform.png").scaled(PLATFORM_WIDTH,PLATFORM_HEIGHT));
    setZValue(PLATFORM_LAYER);
    behavior();
}

void Horizontal::response(){
    //none
}

void Horizontal::behavior(){
    connect(timer,SIGNAL(timeout()), this, SLOT(move()));
    timer->start(1000/FPS);
}

void Horizontal::move(){
    //qDebug() << "moving platform";
    moveBy(3.0*qCos(t),0);
    //qDebug() << "moving";
    t = t + 0.02;
}

Horizontal::~Horizontal(){
    delete timer;
}
