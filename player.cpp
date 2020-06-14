#include <QDebug>
#include <QTransform>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <ctime>
#include <QtMath>
#include <QString>
#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QGraphicsDropShadowEffect>

#include "player.h"
#include "mrhankey.h"
#include "platforms.h"
#include "view.h"
#include "props.h"
#include "eric.h"


Player::Player(QGraphicsScene * scene) : verticaltimer(new QTimer(this)), movetimer(new QTimer(this)), backgroundtimer(new QTimer(this)), fadetimer(new QTimer(this)), weedtimer(new QTimer(this)), angeltimer(new QTimer(this)), weedlasttimer(new QTimer(this)), butterflylasttimer(new QTimer(this)), angellasttimer(new QTimer(this)), decrouchtimer(new QTimer(this)), pauseandfliptimer(new QTimer(this)),
commonParent(new QGraphicsRectItem()), lastBounce(new QGraphicsRectItem(commonParent)), tree(new QGraphicsPixmapItem()), mountains(new QGraphicsPixmapItem()), randy(new QGraphicsPixmapItem(this)), smoke(new QGraphicsPixmapItem(this)), kenny(new QGraphicsPixmapItem(this)), darkness(new QGraphicsPathItem(this)), shadow(new QGraphicsDropShadowEffect(this)), darkWell(new QGraphicsPixmapItem(commonParent)),
score(new QGraphicsTextItem()), gameover(new QGraphicsTextItem(QString("Game Over!"))), gameoverwrapup(new QGraphicsTextItem()),
bgm(new QMediaPlayer(this, QMediaPlayer::LowLatency)), killedKenny(new QMediaPlayer(this, QMediaPlayer::LowLatency)), kennyTalk(new QMediaPlayer(this, QMediaPlayer::LowLatency)), angelSound(new QMediaPlayer(this, QMediaPlayer::LowLatency)){

    // basic configuration

    scene->addItem(this);
    setZValue(CHARACTER_LAYER);
    scene->addItem(commonParent);
    commonParent->setZValue(PLATFORM_LAYER);

    // all connects

    connect(movetimer,SIGNAL(timeout()),this,SLOT(checkHorizontalMovement()));
    connect(verticaltimer,SIGNAL(timeout()),this,SLOT(checkVerticalMovement()));
    connect(backgroundtimer,SIGNAL(timeout()), this, SLOT(checkBackground()));
    connect(weedlasttimer, SIGNAL(timeout()), this, SLOT(weedOut()));
    connect(weedtimer, SIGNAL(timeout()), this, SLOT(smokeFade()));
    connect(butterflylasttimer, SIGNAL(timeout()), this, SLOT(butterflyDead()));
    connect(angellasttimer,SIGNAL(timeout()),this,SLOT(angelOut()));
    connect(angeltimer, SIGNAL(timeout()), this, SLOT(angelFade()));
    connect(decrouchtimer, SIGNAL(timeout()), this, SLOT(decrouch()));
    connect(pauseandfliptimer, SIGNAL(timeout()), this, SLOT(flipping()));
    connect(this, SIGNAL(paused()), verticaltimer, SLOT(stop()));
    connect(this, SIGNAL(paused()), backgroundtimer, SLOT(stop()));
    connect(this, SIGNAL(paused()), fadetimer, SLOT(stop()));
    connect(this, SIGNAL(paused()), weedtimer, SLOT(stop()));
    connect(this, SIGNAL(paused()), angeltimer, SLOT(stop()));
    connect(this, SIGNAL(resumed()), verticaltimer, SLOT(start()));
    connect(this, SIGNAL(resumed()), backgroundtimer, SLOT(start()));
    connect(this, SIGNAL(resumed()), fadetimer, SLOT(start()));
    connect(this, SIGNAL(resumed()), weedtimer, SLOT(start()));
    connect(this, SIGNAL(resumed()), angeltimer, SLOT(start()));

    // background set up

    backgroundtimer->start(1000.0/FPS);
    mountains->setPixmap(QPixmap(":/Resource/Mountains.png").scaled(SCREEN_HEIGHT,(SCREEN_WIDTH-100)));
    mountains->setPos(-50,SCREEN_HEIGHT-300);
    mountains->setZValue(MOUNTAIN_LAYER);
    tree->setPixmap(QPixmap(":/Resource/Tree.png").scaled(200,650));
    tree->setPos(rand()%(SCREEN_WIDTH-100)+50, SCREEN_HEIGHT - 600);
    tree->setZValue(TREE_LAYER);
    QMediaPlayer *start = new QMediaPlayer(nullptr, QMediaPlayer::LowLatency);
    start->setMedia(QUrl("qrc:/Resource/Start.mp3"));
    start->setVolume(30);
    start->play();
    QTimer::singleShot(3000, this, SLOT(startBGM()));

    // sound setup

    killedKenny->setMedia(QUrl("qrc:/Resource/YouBastard.mp3"));
    killedKenny->setVolume(30);
    kennyTalk->setMedia(QUrl("qrc:/Resource/AngelKenny.mp3"));
    angelSound->setMedia(QUrl("qrc:/Resource/Heavens.mp3"));

    // player elements set up

    randy->setPixmap(QPixmap(":/Resource/RandyLeft.png").scaled(PLAYER_WIDTH, PLAYER_HEIGHT));
    smoke ->setPixmap(QPixmap(":/Resource/smoke.png").scaled(200,200));
    smoke -> setPos(-80,-70);
    smoke -> setZValue(-1);
    smoke -> setOpacity(0.0);
    kenny ->setPixmap(QPixmap(":/Resource/AngelKenny.png").scaled(200,150));
    kenny->setPos(-70, -40);
    kenny->setZValue(-11);
    kenny -> setOpacity(0.0);

    QPainterPath path;
    path.addRect(-SCREEN_WIDTH-100, -2000, SCREEN_WIDTH*2+300, 3000);
    QRadialGradient alphaGradient(22.5, 15, 150);
    alphaGradient.setColorAt(0.0, Qt::transparent);
    alphaGradient.setColorAt(0.1, Qt::transparent);
    alphaGradient.setColorAt(1.0, Qt::black);
    darkness->setBrush(alphaGradient);
    darkness->setPath(path);
    darkness->setOpacity(0);
    shadow->setOffset(10,10);
    shadow->setColor(QColor(255,255,255,0));
    //shadow->setBlurRadius(5);
    randy->setGraphicsEffect(shadow);


    // timer setup

    weedlasttimer->setSingleShot(true);
    butterflylasttimer->setSingleShot(true);
    angellasttimer->setSingleShot(true);
    decrouchtimer ->setSingleShot(true);
    verticaltimer->start(1000.0/FPS);

    //plaform arrangement

    platforms.push_back(new Normal(commonParent));
    int n = rand()%(SCREEN_WIDTH-100);
    platforms[0]->setPos(n, BOTTOM);
    setPos(n, BOTTOM - PLAYER_HEIGHT);  // player starts on top of the first platform

    for(int i = 1; i < SCREEN_HEIGHT/SPACING; ++i){
        if(rand()%ELASTIC_PROB == 1){
            platforms.push_back(new Elastic(commonParent));
            platforms[i]->setPos(rand()%(SCREEN_WIDTH-100), BOTTOM - SPACING * i);
            connect(platforms[i], SIGNAL(spring()), this, SLOT(setElasticMode()));
            connect(this, SIGNAL(paused()), platforms[i], SLOT(stopTimer()));
            connect(this, SIGNAL(resumed()), platforms[i], SLOT(resumeTimer()));
        }
        else{
            platforms.push_back(new Normal(commonParent));
            platforms[i]->setPos(rand()%(SCREEN_WIDTH-100), BOTTOM - SPACING * i);
            if(rand()%WING_PROB == 2){
                props.push_back(new Wings(platforms[platforms.size()-1]));
                props[props.size()-1]->setPos(0,-15);
            }
            else if(rand()%WEED_PROB == 3){
                props.push_back(new Weed(platforms[platforms.size()-1]));
                props[props.size()-1]->setPos(20,-50);
            }
        }
    }
    lastBounce->setX(0);
    lastBounce->setY(platforms[0]->y());

    // text configuration

    score->setPlainText(QString::number(0));
    score->setDefaultTextColor(Qt::blue);
    score->setFont(QFont("courier", 20));
    score->setZValue(6);
    gameover->setDefaultTextColor(Qt::magenta);
    gameover->setFont(QFont("impact", 40));
    gameover->setPos(100,1400);
    gameover->setZValue(6);
    gameoverwrapup->setDefaultTextColor(Qt::darkGreen);
    gameoverwrapup->setFont(QFont("comic sans ms", 20));
    gameoverwrapup->setPos(100,1480);
    gameoverwrapup->setZValue(6);
}

void Player::keyPressEvent(QKeyEvent *event){
    // qDebug() << "keyPress detected";

    switch(event->key()){
    case Qt::Key_Up:{
        if(pause == false){
            MrHankey *mrhankey = new MrHankey();
            mrhankey->setPos(x()-30,y());
            scene()->addItem(mrhankey);
            connect(this, SIGNAL(paused()), mrhankey, SLOT(stopTimer()));
            connect(this, SIGNAL(resumed()), mrhankey, SLOT(resumeTimer()));
        }
        break;
    }
    case Qt::Key_Down:
        break;
    case Qt::Key_Left:
        if(pause == false){
            if(leftKeyHeldDown == false){
                leftKeyHeldDown = true;
                if(!movetimer->isActive()){
                    movetimer->start(1000/FPS);
                }
                setOrientation(directions::left);
                direction = directions::left;
            }
        }
        break;
    case Qt::Key_Right:
        if(pause == false){
            if(rightKeyHeldDown == false){
                rightKeyHeldDown = true;
                if(!movetimer->isActive()){
                    movetimer->start(1000.0/FPS);
                }
                setOrientation(directions::right);
                //setTransform(QTransform::fromScale(-1, 1));
                direction = directions::right;
            }
        }
        break;
    case Qt::Key_Space:
        if(!flippingMode){
            setPause(!pause);
        }
        break;
    }
}

void Player::keyReleaseEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Left || event->key() == Qt::Key_Right){
        if(leftKeyHeldDown == true && rightKeyHeldDown == true ){
            if(event->key() == Qt::Key_Left){
                direction = directions::right;
                setOrientation(directions::right);
                leftKeyHeldDown = false;
            }
            else{
                direction = directions::left;
                setOrientation(directions::left);
                rightKeyHeldDown = false;
            }
        }
        else if(movetimer->isActive()){
            movetimer->stop();
            if(event->key() == Qt::Key_Left){
                leftKeyHeldDown = false;
            }
            else{
                rightKeyHeldDown = false;
            }
        }

    }
}

void Player::checkHorizontalMovement(){
    if(direction == directions::left){
        setX(x() - STEP_SIZE);
    }
    else{
        setX(x() + STEP_SIZE);
    }
    if(x() < 0 - randy->pixmap().width()){
        setX(scene()->width()-pixmap().width()/2);
    }
    else if(x() >= scene()->width()){
        setX(0 - randy->pixmap().width()/2);
    }
}

void Player::checkVerticalMovement(){
    if(processing == false){
        processing = true;
        //qDebug() << "vertical";

        checkDarkWell();

        checkFlipMode();

        checkDelete();

        checkNewElements();

        if(rocketMode == true){
            rocketMove();
        }
        else{
            normalMove();
            checkFail();
            if(fail == true){
                BOTTOM = SCREEN_HEIGHT - 50.0;
                endGame();
            }
            else{
                checkBounce();
                checkProps();
                if(downMode == true){
                    pullBG();
                }
            }
        }

        processing = false;
    }
}

void Player::checkFlipMode(){
    if(flipMode == true){
        if(travelDistance >= flipEndDistance){
            flipangle = 0;
            scalefactor = 90.0;
            pauseandfliptimer->start(1000/FPS);
            setPause(true);
            flipMode = false;
            flippingMode = true;
        }
    }
}

void Player::checkDarkWell(){
    if(travelDistance >= nextDarkWellLocation){
        darkWell = new QGraphicsPixmapItem(commonParent);
        darkWell->setPixmap(QPixmap(":/Resource/Red Wall.jpg").scaled(SCREEN_WIDTH+10, WELL_LENGTH));
        darkWell->setPos(0,0 - commonParent->mapToScene(0,0).y()-WELL_LENGTH);
        nextDarkWellLocation = (travelDistance + WELL_LENGTH + WELL_INTERVAL + rand()%10000);
        darkWellMode = true;
    }
    if(darkWellMode == true){
        shadow->setXOffset((x()-250.0)/20.0);
        shadow->setYOffset((y()-600.0)/20.0);
        if(darkWell->mapToScene(0,0).y()<0.0){
            if(darkness->opacity()<1.0){
                darkness->setOpacity((darkWell->mapToScene(0,0).y()+WELL_LENGTH)/SCREEN_HEIGHT);
                //qDebug() << "darkwell depth" << darkWell->mapToScene(0,0).y()+WELL_LENGTH;
                shadow->setColor(QColor(0, 0, 0,255*(darkWell->mapToScene(0,0).y()+WELL_LENGTH-MAX_HEIGHT)/(SCREEN_HEIGHT+300.0)));
            }
        }
        else{
            if(darkness->opacity()>0.0){
                darkness->setOpacity((SCREEN_HEIGHT-darkWell->mapToScene(0,0).y())/SCREEN_HEIGHT);
                shadow->setColor(QColor(0, 0, 0,255*(SCREEN_HEIGHT-darkWell->mapToScene(0,0).y()-MAX_HEIGHT)/(SCREEN_HEIGHT+300.0)));
            }
            else{
                delete darkWell;
                darkWellMode = false;
            }
        }
    }
}

void Player::focusOutEvent(QFocusEvent *event){
    setFocus();
}

QVector<Platform*> Player::getPlatforms(){
    return platforms;
}


QGraphicsTextItem* Player::getScore(){
    return score;
}

void Player::decrouch(){
    setOrientation(direction);
}

void Player::checkBackground(){
    if(travelDistance - mountaincount * 100 > 0){
        mountains->moveBy(0,1);
        ++mountaincount;
    }
    if(int n = travelDistance - treecount * 10 > 0){
        tree->moveBy(0,(n+10)/10);
        ++treecount;
    }
}

QGraphicsPixmapItem * Player::getTree(){
    return tree;
}
QGraphicsPixmapItem *Player:: getMountains(){
    return mountains;
}

QVector<Prop*> Player::getProps(){
    return props;
}

void Player::setBOTTOM(double number){
    BOTTOM = number;
}

void Player::butterflyDead(){
    randy->setOffset(0,0);
    verticalSpeed = 2 * 50/(jumpTime/2.0);
    ACC_PLAYER = verticalSpeed/(jumpTime/2.0);
    rocketMode = false;
    setOrientation(direction);
}

directions Player::getDirection(){
    return direction;
}

void Player::setRocketMode(bool a){
    randy->setOffset(-30,-20);
    rocketMode = a;
    downMode = false;
    setOrientation(direction);
    butterflylasttimer->start(PROPS_DURATION);
    verticalSpeed = -(2.0*(y()-MAX_HEIGHT-50)/jumpTime);
    ACC_PLAYER = verticalSpeed/jumpTime;
}

void Player::setDownMode(bool a){
    downMode = a;
}

void Player::setWeedMode(){
    smoke->setOpacity(0.9);
    if(weedMode == true){
        weedtimer->stop(); //stop the smoke from fading;
        int remainingTime = weedlasttimer->remainingTime();
        weedlasttimer->start(remainingTime + PROPS_DURATION);
    }
    else{
        weedMode = true;
        weedlasttimer->start(PROPS_DURATION);
    }
}

void Player::weedOut(){
    BOTTOM = SCREEN_HEIGHT - 50.0;
    weedMode = false;
    //ACC_PLAYER = DEFAULT_GRAVITY;
    weedtimer->start(1000/FPS);
}

void Player::smokeFade(){
    if(smoke->opacity()>0.0){
         smoke->setOpacity(smoke->opacity()-0.04);
    }
    else{
        weedtimer->stop();
    }

}

void Player::setElasticMode(){
    elasticMode = true;
    BOTTOM = 1000;
}

void Player::setGRAVITY(double number){
    ACC_PLAYER = number;
}

QVector<Monster*> * Player::getMonsters(){
    return &monsters;
}

void Player::setFail(){
    if(fail == false){
        for(int i = 0; i < platforms.size(); ++i){
            if(platforms[i]->mapToScene(0,0).y() > SCREEN_HEIGHT){
                //platforms[i]->setScale(0);
            }
            else{
                break;
            }
        }
        fail = true;
        scene()->views()[0]->resetTransform();
        gameoverwrapup->setPlainText(QString("Your score: ")+QString::number(travelDistance)+QString("\nPress enter to restart"));
        QMediaPlayer *gameoversound = new QMediaPlayer();
        gameoversound->setMedia(QUrl("qrc:/Resource/Game Over.mp3"));
        gameoversound->setVolume(30);
        gameoversound->play();
        stopBGM();
    }
}

bool Player::isProtectedMode(){
    if(rocketMode == true){
        return true;
    }
    if(angelMode == true){
        return true;
    }
    return false;
}

QGraphicsTextItem * Player::getGameover(){
    return gameover;
}

QGraphicsTextItem * Player::getGameoverwrapup(){
    return gameoverwrapup;
}

void Player::startBGM(){
    playlist = new QMediaPlaylist();
    if(rand()%3 == 1){
        playlist->clear();
        playlist->addMedia(QUrl("qrc:/Resource/BakcgroundMusic.mp3"));
    }else{
        playlist->clear();
        playlist->addMedia(QUrl("qrc:/Resource/Chicken on the Rocks.mp3"));
    }
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    bgm -> setPlaylist(playlist);
    bgm -> setVolume(30);
    bgm -> play();
}

void Player::stopBGM(){
    connect(fadetimer, SIGNAL(timeout()), this, SLOT(fadeOutBGM()));
    fadetimer->start(150);
}

void Player::fadeOutBGM(){
    if(bgm->volume() > 0){
        bgm->setVolume(bgm->volume()-1);
    }
    else{
        bgm->stop();
        fadetimer->disconnect(this);
        fadetimer->stop();
    }
}

double Player::getSpeed(){
    return verticalSpeed;
}

QGraphicsItem * Player::getCommonParent(){
    return commonParent;
}

void Player::checkDelete(){
    for(int i = 0; i < platforms.size();++i){
            if(platforms[i]->mapToScene(0,0).y() > SCREEN_HEIGHT+50){
                platforms[i]->stopTimer();
                scene()->removeItem(platforms[i]);
                platforms.removeFirst();
            }
            else{
                break;
            }
    }
    for(int i = 0; i < monsters.size(); ++i){
        if(monsters[i] == nullptr){
            monsters.remove(i);
        }
        else{
            if(monsters[i]->mapToScene(0,0).y() > SCREEN_HEIGHT){
                monsters[i]->stopTimer();
                scene()->removeItem(monsters[i]);
                monsters.removeFirst();
            }
            else{
                //break;
            }
        }
    }
    monsters.squeeze();
    for(int i = 0; i < erics.size(); ++i){
        if(erics[i]->mapToScene(0,0).y()> SCREEN_HEIGHT){
            erics[i]->stopTimer();
            for(int j = 0; j < erics[i]->getMemberberries()->size(); ++j){
                (*erics[i]->getMemberberries())[j]->stopTimer();
                scene()->removeItem((*erics[i]->getMemberberries())[j]);
            }
            (*erics[i]->getMemberberries()).clear();
            (*erics[i]->getMemberberries()).squeeze();
            scene()->removeItem(erics[i]);
            erics.remove(i);
        }
        else{
            //break;
        }
    }

}

void Player::checkNewElements(){
    // add new platforms and monsters
    if(travelDistance - platformcount * SPACING > 0){
        //qDebug() << "new item added";
        if(rand()%ELASTIC_PROB == 1){
            platforms.push_back(new Elastic(commonParent));
            platforms[platforms.size()-1]->setPos(rand()%(SCREEN_WIDTH-100), 0 - 10 - commonParent->mapToScene(0,0).y());
            connect(platforms[platforms.size()-1], SIGNAL(spring()), this, SLOT(setElasticMode()));
            connect(this, SIGNAL(paused()), platforms[platforms.size()-1], SLOT(stopTimer()));
            connect(this, SIGNAL(resumed()), platforms[platforms.size()-1], SLOT(resumeTimer()));

        }
        else{
            if(rand()%HORI_PROB == 1){
                platforms.push_back(new Horizontal(commonParent));
                platforms[platforms.size()-1]->setPos(rand()%(SCREEN_WIDTH-100), 0 - 10 - commonParent->mapToScene(0,0).y());
                connect(this, SIGNAL(paused()), platforms[platforms.size()-1], SLOT(stopTimer()));
                connect(this, SIGNAL(resumed()), platforms[platforms.size()-1], SLOT(resumeTimer()));
            }
            else{
                platforms.push_back(new Normal(commonParent));
                platforms[platforms.size()-1]->setPos(rand()%(SCREEN_WIDTH-100), 0 - 10 - commonParent->mapToScene(0,0).y());
            }
            if(rand()%WING_PROB == 2){
                props.push_back(new Wings(platforms[platforms.size()-1]));
                props[props.size()-1]->setPos(0,-15);
                //scene()->addItem(props[props.size()-1]);
            }
            else if(rand()%WEED_PROB == 3){
                props.push_back(new Weed(platforms[platforms.size()-1]));
                props[props.size()-1]->setPos(20,-50);
                //scene()->addItem(props[props.size()-1]);
            }
            else if(rand()%MONSTER_PROB == 0){
                //qDebug() << "monster created";
                monsters.push_back(new Monster(commonParent));
                monsters[monsters.size()-1]->setPos(rand()%(SCREEN_WIDTH-100) ,platforms[platforms.size()-1]->y()-MONSTER_HEIGHT-10);
                connect(this, SIGNAL(paused()), monsters[monsters.size()-1], SLOT(stopTimer()));
                connect(this, SIGNAL(resumed()), monsters[monsters.size()-1], SLOT(resumeTimer()));
            }
            else if(rand()%ERIC_PROB == 5){
                //qDebug() << "eric appeared";
                if(rand()%2 == 1){
                    erics.push_back(new Eric(leftSide, commonParent));
                    erics[erics.size()-1]->setPos(-10, platforms[platforms.size()-1]->y()-ERIC_SIZE);
                }
                else{
                    erics.push_back(new Eric(rightSide, commonParent));
                    erics[erics.size()-1]->setPos(SCREEN_WIDTH+10-ERIC_SIZE, platforms[platforms.size()-1]->y()-ERIC_SIZE);
                }
                connect(this, SIGNAL(paused()), erics[erics.size()-1], SLOT(stopTimer()));
                connect(this, SIGNAL(resumed()), erics[erics.size()-1], SLOT(resumeTimer()));
                //scene()->addItem(erics[erics.size()-1]);
            }
            else if(rand()%KENNY_PROB == 8){
                props.push_back(new Kenny(platforms[platforms.size()-1]));
                props[props.size()-1]->setPos(20 ,-40);
                if(killedKenny->state() != QMediaPlayer::State::PlayingState){
                    killedKenny->play();
                }
            }
        }
        //scene()->addItem(platforms[platforms.size()-1]);
        ++platformcount;
    }

}

void Player::checkBounce(){

    // bounces off a platform and brings down all items if needed

    if(verticalSpeed < 0){ //don't do = 0 cause it will create infinity speed
        for(int i = 0; i < platforms.size(); ++i){
            if(mapFromItem(platforms[i], 0, 0).x()<PLAYER_WIDTH && mapFromItem(platforms[i], 0, 0).x() > -PLATFORM_WIDTH)
                if(mapFromItem(platforms[i], 0, 0).y() >= PLAYER_HEIGHT && mapFromItem(platforms[i], 0, 0).y() <= PLAYER_HEIGHT - verticalSpeed){
                    //qDebug() << "jump!";
                    if(direction == directions::right){
                        randy->setPixmap(QPixmap(":/Resource/RandyCrouchedRight.png").scaled(PLAYER_WIDTH, PLAYER_HEIGHT));
                    }
                    else{
                        randy->setPixmap(QPixmap(":/Resource/RandyCrouchedLeft.png").scaled(PLAYER_WIDTH, PLAYER_HEIGHT));
                    }
                    decrouchtimer->start(200);
                    platforms[i]->response();
                    setY(platforms[i]->mapToScene(0,0).y() - PLAYER_HEIGHT);
                    lastBounce->setY(platforms[i]->y());
                    if(elasticMode || weedMode){
                        verticalSpeed = (2.0*(y()-MAX_HEIGHT)/springTime);
                        ACC_PLAYER = verticalSpeed/springTime;
                        if(mapFromItem(lastBounce, 0, 0).y() < BOTTOM){
                            downMode = true;
                            verticalItemSpeed = (BOTTOM-lastBounce->mapToScene(0,0).y()) * 2.0/springTime;
                            ACC_ITEM = verticalItemSpeed/springTime;
                        }
                    }else{
                        //qDebug() << "jump starts from y() = " << y();
                        verticalSpeed = (2.0*(y()-MAX_HEIGHT)/jumpTime);
                        ACC_PLAYER = verticalSpeed/jumpTime;
                        if(mapFromItem(lastBounce, 0, 0).y() < BOTTOM){
                            downMode = true;
                            verticalItemSpeed = (BOTTOM-lastBounce->mapToScene(0,0).y()) * 2.0/jumpTime;
                            ACC_ITEM = verticalItemSpeed/jumpTime;
                        }
                    }
                    break;
                }
            }
        }

 }


void Player::checkProps(){
    //check props
    for(int i = 0; i < props.size(); ++i){
        if(props[i]->collidesWithItem(randy)){
            props[i]->response();
            scene()->removeItem(props[i]);
            props.remove(i);
        }
    }
}

void Player::endGame(){
    for(int i = 0; i < platforms.size(); ++i){
        if(platforms[i]->scene() != nullptr){
            if(platforms[i]->mapToScene(0,0).y() < 0-10){
                platforms[i]->stopTimer();
                scene()->removeItem(platforms[i]);
                platforms.removeFirst();
            }
            else{
                break;
            }
        }
    }
    for(int i = 0; i < monsters.size(); ++i){
        if(monsters[i] == nullptr){
            qDebug() << "monster nullptr";
            monsters.remove(i);
        }
        else{
            if(monsters[i]->scene() != nullptr){
                if(monsters[i]->mapToScene(0,0).y() < 0-MONSTER_HEIGHT){
                    qDebug() << "monster"<< QString::pointer(monsters[i]) <<"removed";
                    scene()->removeItem(monsters[i]);
                    monsters[i]->stopTimer();
                    monsters.removeFirst();
                }
                else{
                    break;
                }
            }
        }
    }
    for(int i = 0; i < erics.size(); ++i){
        if(erics[i]->scene() != nullptr){
            if(erics[i]->mapToScene(0,0).y()< 0-100){
                erics[i]->stopTimer();
                for(int j = 0; j < erics[i]->getMemberberries()->size(); ++j){
                    (*erics[i]->getMemberberries())[j]->stopTimer();
                    scene()->removeItem((*erics[i]->getMemberberries())[j]);
                }
                (*erics[i]->getMemberberries()).clear();
                (*erics[i]->getMemberberries()).squeeze();
                qDebug() << "eric removed";
                scene()->removeItem(erics[i]);
                erics.remove(i);
            }
            else{
                //break;
            }
        }
    }

    if(gameover->y() <= 100){
        verticaltimer->stop();
        dynamic_cast<Game*>(scene()->views()[0])->setGameOver();
        //qDebug() << "game over set";
        //delete this;
        return;
    }

    commonParent->moveBy(0, -INITIAL_JUMP_SPEED);

    gameover->moveBy(0, -INITIAL_JUMP_SPEED);
    gameoverwrapup->moveBy(0, -INITIAL_JUMP_SPEED);
}

void Player::pullBG(){
    if(lastBounce->mapToScene(0,0).y() <= BOTTOM){
        if(lastBounce->mapToScene(0,0).y() >= BOTTOM - verticalItemSpeed){
            commonParent->moveBy(0, BOTTOM-lastBounce->mapToScene(0,0).y());
            tree->moveBy(0,(BOTTOM-lastBounce->mapToScene(0,0).y())/10.0);
            mountains->moveBy(0,(BOTTOM-lastBounce->mapToScene(0,0).y())/100.0);
            downMode = false;
            verticalItemSpeed = 0;
            travelDistance += BOTTOM-lastBounce->mapToScene(0,0).y();
            score->setPlainText(QString::number(travelDistance));
            if(elasticMode == true){
                if(weedMode == false){
                    BOTTOM = SCREEN_HEIGHT - 50.0;
                }
                ACC_PLAYER= (BOTTOM - MAX_HEIGHT - PLAYER_HEIGHT)*2.0 /(jumpTime*jumpTime);;
                elasticMode = false;
            }
        }

        else{

            commonParent->moveBy(0, verticalItemSpeed);

            tree->moveBy(0,(verticalItemSpeed)/10.0);
            if(verticalItemSpeed-ACC_ITEM>0){
                verticalItemSpeed -= ACC_ITEM;
            }
            travelDistance += verticalItemSpeed;
            score->setPlainText(QString::number(travelDistance));
            //qDebug() << travelDistance;
        }
    }
    else{
        downMode = false;
    }
}

void Player::rocketMove(){
    // move to a certain height and stop
    if(verticalSpeed > 0 && y() > MAX_HEIGHT + 50){
        if(y() < MAX_HEIGHT + 50 + verticalSpeed){
                setY(MAX_HEIGHT + 50);
                verticalSpeed -= ACC_PLAYER;
        }
        else{
            moveBy(0, -verticalSpeed);
        }
    }

    commonParent->moveBy(0, ROCKET_SPEED);

    tree->moveBy(0,(ROCKET_SPEED/10.0));
    travelDistance += ROCKET_SPEED;
    score->setPlainText(QString::number(travelDistance));

    downMode = false;
}

void Player::normalMove(){
    // moving normally
    if(verticalSpeed > 0 && y() > MAX_HEIGHT){
        if(y() < MAX_HEIGHT + verticalSpeed){
                setY(MAX_HEIGHT);
                verticalSpeed = 0;
                ACC_PLAYER = (BOTTOM - MAX_HEIGHT - PLAYER_HEIGHT)*2.0 /(jumpTime*jumpTime);
        }
        else{
            moveBy(0,-verticalSpeed);
            verticalSpeed = verticalSpeed - ACC_PLAYER;
        }
    }
    else{
        setY(y() - verticalSpeed);
        verticalSpeed = verticalSpeed - ACC_PLAYER;
    }
}

void Player::checkFail(){
    // fail
    if(SCREEN_HEIGHT-y()<PLAYER_HEIGHT){
        //qDebug() << "height = " << y();
        if(fail == false){
            qDebug() << "you fail!";
            qDebug() << "platforms.size() =" << platforms.size();
            verticalSpeed = INITIAL_JUMP_SPEED;
            setFail();
        }
    }
}

/*void Player::stopTimers(){
    verticaltimer->stop();
    movetimer->stop();
    backgroundtimer->stop();
    fadetimer->stop();
    weedtimer->stop();
    bgm->stop();
}*/

Player::~Player(){

    delete verticaltimer;
    delete movetimer;
    delete backgroundtimer;
    delete fadetimer;
    delete weedtimer;
    delete bgm;
    delete commonParent;
    delete tree;
    delete mountains;
    delete smoke;
    delete kenny;
    delete killedKenny;
    delete angelSound;
    delete kennyTalk;

    /*for (int i = 0; i < platforms.size(); i++){
        platforms.erase(i);
    }
    platforms.clear();

    for (int i = 0; i < props.size(); i++)
        delete props[i];
    props.clear();

    for (int i = 0; i < monsters.size(); i++)
        delete monsters[i];
    monsters.clear();

    for (int i = 0; i < erics.size(); i++)
        delete erics[i];
    erics.clear();*/
}

void Player::setLastBounce(QGraphicsItem * item){
    lastBounce->setY(item->y());
}

void Player::setAngelMode(){
    kenny->setOpacity(0.9);
    if(angelMode){
        angeltimer->stop(); //stop angel from fading
        int remainingTime = angellasttimer->remainingTime();
        angellasttimer->start(remainingTime+PROPS_DURATION);
    }
    else{
        kennyTalk->setVolume(30);
        kennyTalk->play();
        angelSound->setVolume(30);
        angelSound->play();
        angelMode = true;
        angellasttimer->start(18000);
    }
}

void Player::angelOut(){
    angeltimer->start(1000/FPS);
}


void Player::angelFade(){
    if(kenny->opacity()>0.0){
         kenny->setOpacity(kenny->opacity()-0.04);
    }
    else{
        angelMode = false;
        angeltimer->stop();
    }
}

QGraphicsPixmapItem* Player:: getRandy(){
    return randy;
}

bool Player::isFlipMode(){
    return flipMode;
}

void Player::setFlipMode(bool a){
    flipMode = a;
}

int Player::getTravelDistance(){
    return travelDistance;
}

void Player::setFlipEndDistance(int num){
    flipEndDistance = num;
}

void Player::setPause(bool a){
    pause = a;
    if(a){
        emit paused();
        weedRemain = weedlasttimer->remainingTime();
        weedlasttimer->stop();
        butterflyRemain = butterflylasttimer->remainingTime();
        butterflylasttimer->stop();
        angelRemain = angellasttimer->remainingTime();
        angellasttimer->stop();
        decrouchRemain = decrouchtimer->remainingTime();
        decrouchtimer->stop();
        pause = true;
    }
    else{
        emit resumed();
        if(weedRemain >= 0){
            weedlasttimer -> start(weedRemain);
            weedRemain = -1;
        }
        if(butterflyRemain >= 0){
            butterflylasttimer -> start(butterflyRemain);
            butterflyRemain = -1;
        }
        if(angelRemain >= 0){
            angellasttimer -> start(angelRemain);
            angelRemain = -1;
        }
        if(decrouchRemain >= 0){
            decrouchtimer -> start(decrouchRemain);
            decrouchRemain = -1;
        }
        pause = false;
    }
}

void Player::flipping(){
    //qDebug() << "flipping back!!";
    if(flipangle<90){
        scene()->views()[0]->rotate(2);
        ++flipangle;
    }
    else if(scalefactor>-90){
        if(scalefactor != 2){
            scene()->views()[0]->scale((scalefactor-2.0)/scalefactor,1);
            scalefactor = scalefactor - 2;
        }
        else{
            scene()->views()[0]->scale(-1, 1);
            scalefactor= -2.0;
        }
    }else{
        scene()->views()[0]->resetTransform();
        pauseandfliptimer->stop();
        setPause(false);
        flippingMode = false;
    }
}

bool Player::isFail(){
    return fail;
}

void Player::setOrientation(directions d){
    if(d == directions::left){
        if(rocketMode == true){
            randy->setPixmap(QPixmap(":/Resource/ButterflyLeft.png").scaled(100,100));
        }
        else{
            randy->setPixmap(QPixmap(":/Resource/RandyLeft.png").scaled(PLAYER_WIDTH, PLAYER_HEIGHT));
        }
    }else{
        if(rocketMode == true){
            randy->setPixmap(QPixmap(":/Resource/ButterflyRight.png").scaled(100,100));
        }
        else{
            randy->setPixmap(QPixmap(":/Resource/RandyRight.png").scaled(PLAYER_WIDTH, PLAYER_HEIGHT));
        }
    }
}

void Player::setFlippingMode(bool a){
    flippingMode = a;
}
