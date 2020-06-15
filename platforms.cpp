#include "platforms.h"
#include "numbers.h"
#include "view.h"
#include <QtMath>
#include <QBuffer>

Platform::Platform(QGraphicsItem * parent) : timer(new QTimer(this)){
    setParentItem(parent);
    setZValue(PLATFORM_LAYER);
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

Platform::~Platform(){
    //delete timer;
}

Normal::Normal(QGraphicsItem * parent) : Platform(parent){
    setPixmap(QPixmap(":/Resource/Platform.jpg").scaled(PLATFORM_WIDTH,PLATFORM_HEIGHT));
}

void Normal::response(){
    //sound->play();
}

void Normal::behavior(){
    // none
}

Elastic::Elastic(QGraphicsItem * parent) : Platform(parent){
    setPixmap(QPixmap(":/Resource/ElasticStill.png").scaled(PLATFORM_WIDTH,PLATFORM_HEIGHT+30));
    setOffset(0, -30);
}

void Elastic::response(){
    setPixmap(QPixmap(":/Resource/ElasticSet.png").scaled(PLATFORM_WIDTH,PLATFORM_HEIGHT+30));
    moveBy(0, -20);
    emit spring();
}

void Elastic::behavior(){
    // none
}

Elastic::~Elastic(){
    //delete sound;
}

Horizontal::Horizontal(QGraphicsItem * parent) : Platform(parent){
    setPixmap(QPixmap(":/Resource/Platform.jpg").scaled(PLATFORM_WIDTH,PLATFORM_HEIGHT));
    behavior();

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
    t = t + 0.02*48.0/FPS;
}

Horizontal::~Horizontal(){
    delete timer;
    //delete sound;
}
