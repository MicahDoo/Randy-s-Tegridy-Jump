#include "props.h"
#include "view.h"
#include "player.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QMediaPlayer>

Prop::Prop(QGraphicsItem * parent){
    setParentItem(parent);
}

void Prop::response(){

}

Wings::Wings(QGraphicsItem * parent){
    setParentItem(parent);
    setPixmap(QPixmap(":/Resource/Wings.png").scaled(80,20));
    setZValue(PLATFORM_LAYER);
}

void Wings::response(){
    Game* game = dynamic_cast<Game*>(scene()->views()[0]);
    game->getPlayer()->setRocketMode(true);
    game->getPlayer()->setDownMode(false);
    game->getPlayer()->setVerticalSpeed(3.0);
    if(game->getPlayer()->getDirection() == directions::left){
        game->getPlayer()->setPixmap(QPixmap(":/Resource/ButterflyLeft.png").scaled(100,100));
    }
    else{
        game->getPlayer()->setPixmap(QPixmap(":/Resource/ButterflyRight.png").scaled(100,100));
    }
    QTimer::singleShot(PROPS_DURATION, game->getPlayer(), SLOT(butterflyDead()));
}

Weed::Weed(QGraphicsItem * parent){
    setParentItem(parent);
    setPixmap(QPixmap(":/Resource/TegridyWeed.png").scaled(40,50));
    setZValue(PLATFORM_LAYER);
}

void Weed::response(){
    QMediaPlayer *tegridyweed = new QMediaPlayer();
    tegridyweed->setMedia(QUrl("qrc:/Resource/TegridyWeed.mp3"));
    tegridyweed->setVolume(1000);
    tegridyweed->play();
    Game* game = dynamic_cast<Game*>(scene()->views()[0]);
    game->getPlayer()->setBOTTOM(1000.0);
    game->getPlayer()->setWeedMode();
    QTimer::singleShot(PROPS_DURATION,game->getPlayer(),SLOT(weedOut()));
}

Kenny::Kenny(QGraphicsItem * parent){
    setParentItem(parent);
    setPixmap(QPixmap(":/Resource/DeadKenny.png").scaled(80,50));
    setZValue(PLATFORM_LAYER);
}

void Kenny::response(){
    Game* game = dynamic_cast<Game*>(scene()->views()[0]);
    game->getPlayer()->setAngelMode();
    QTimer::singleShot(10000,game->getPlayer(),SLOT(angelOut()));
}
