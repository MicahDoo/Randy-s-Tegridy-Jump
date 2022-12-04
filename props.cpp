#include "props.h"
#include "view.h"
#include "player.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QMediaPlayer>

Prop::Prop(QGraphicsItem * parent) : QGraphicsPixmapItem(parent){
    setZValue(PLATFORM_LAYER);
}

void Prop::response(){

}

Wings::Wings(QGraphicsItem * parent){
    setParentItem(parent);
    setPixmap(QPixmap(":/Resource/Wings.png").scaled(80,20));
    setZValue(PLATFORM_LAYER);
}

void Wings::response(){
    if(scene() != nullptr){
        Game* game = dynamic_cast<Game*>(scene()->views()[0]);
        game->getPlayer()->setRocketMode(true);
    }
}

Weed::Weed(QGraphicsItem * parent) : Prop(parent){
    setPixmap(QPixmap(":/Resource/TegridyWeed.png").scaled(40,50));
}

void Weed::response(){
    if(scene() != nullptr){
        QMediaPlayer *tegridyweed = new QMediaPlayer();
        tegridyweed->setMedia(QUrl("qrc:/Resource/TegridyWeed.mp3"));
        tegridyweed->setVolume(1000);
        tegridyweed->play();
        Game* game = dynamic_cast<Game*>(scene()->views()[0]);
        game->getPlayer()->setBOTTOM(SCREEN_HEIGHT+300);
        game->getPlayer()->setWeedMode();
    }
}

Kenny::Kenny(QGraphicsItem * parent) : Prop(parent){
    setPixmap(QPixmap(":/Resource/DeadKenny.png").scaled(80,50));
}

void Kenny::response(){
    if(scene() != nullptr){
        Game* game = dynamic_cast<Game*>(scene()->views()[0]);
        game->getPlayer()->setAngelMode();
    }
}

Kenny::~Kenny(){

}
