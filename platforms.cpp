#include "platforms.h"
#include "numbers.h"
#include "view.h"
#include <QtMath>
#include <QBuffer>

Platform::Platform(QGraphicsItem * parent){
    setParentItem(parent);
    setZValue(PLATFORM_LAYER);
    timer = new QTimer();
}

void Platform::behavior(){
    // none
}

void Platform::response(){
    // none
}

void Platform::stopTimer(){
    timer->stop();
}

void Platform::resumeTimer(){
    timer->start();
}

Normal::Normal(QGraphicsItem * parent) : Platform(parent){
    setPixmap(QPixmap(":/Resource/Platform.png").scaled(PLATFORM_WIDTH,PLATFORM_HEIGHT));
    timer->start(1000/FPS);
    //sound = new QSoundEffect();
   // sound->setSource(QUrl("qrc:/Resource/Bounce.wav"));
}

void Normal::response(){
    //sound->play();
}

void Normal::behavior(){
    // none
}

Elastic::Elastic(QGraphicsItem * parent) : Platform(parent){
    setPixmap(QPixmap(":/Resource/ElasticStill.png").scaled(PLATFORM_WIDTH,PLATFORM_HEIGHT+40));
    timer->start(1000/FPS);
    //sound = new QSoundEffect();
    //sound->setSource(QUrl("qrc:/Resource/Spring.mp3"));
}

void Elastic::response(){
    setPixmap(QPixmap(":/Resource/ElasticSet.png").scaled(PLATFORM_WIDTH,PLATFORM_HEIGHT+40));
    moveBy(0, -20);
    Game* game = dynamic_cast<Game*>(scene()->views()[0]);
    game->getPlayer()->setElasticMode(true);
    game->getPlayer()->setBOTTOM(1000.0);
    //sound->play();
}

void Elastic::behavior(){
    // none
}

Elastic::~Elastic(){
    delete timer;
    //delete sound;
}

Horizontal::Horizontal(QGraphicsItem * parent) : Platform(parent){
    setPixmap(QPixmap(":/Resource/Platform.png").scaled(PLATFORM_WIDTH,PLATFORM_HEIGHT));
    behavior();
    //sound = new QSoundEffect();
    //sound->setSource(QUrl("qrc:/Resource/Bounce.wav"));
}

void Horizontal::response(){
    //sound->play();
}

void Horizontal::behavior(){
    connect(timer,SIGNAL(timeout()), this, SLOT(move()));
    timer->start(1000/FPS);
}

void Horizontal::move(){
    moveBy(3.0*qCos(t),0);
    t = t + 0.02;
}

Horizontal::~Horizontal(){
    delete timer;
    //delete sound;
}
