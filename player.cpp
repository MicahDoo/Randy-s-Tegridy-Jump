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

#include "player.h"
#include "mrhankey.h"
#include "platforms.h"
#include "view.h"
#include "props.h"
#include "eric.h"


Player::Player(QGraphicsScene * scene){
    scene->addItem(this);
    setZValue(CHARACTER_LAYER);
    movetimer = new QTimer();
    connect(movetimer,SIGNAL(timeout()),this,SLOT(checkHorizontalMovement()));
    verticaltimer = new QTimer();
    connect(verticaltimer,SIGNAL(timeout()),this,SLOT(checkVerticalMovement()));
    backgroundtimer = new QTimer();
    //connect(backgroundtimer,SIGNAL(timeout()), this, SLOT(checkBackground()));
    backgroundtimer->start(1000.0/FPS);
    mountains = new QGraphicsPixmapItem();
    mountains->setPixmap(QPixmap(":/Resource/Mountains.png").scaled(700,400));
    mountains->pixmap().setDevicePixelRatio(-5);
    mountains->setPos(-50,400);
    mountains->setZValue(MOUNTAIN_LAYER);
    tree = new QGraphicsPixmapItem();
    tree->setPixmap(QPixmap(":/Resource/Tree.png").scaled(200,650));
    tree->pixmap().setDevicePixelRatio(-5);
    tree->setPos(rand()%400+50, 100);
    tree->setZValue(TREE_LAYER);

    smoke = new QGraphicsPixmapItem(this);
    smoke ->setPixmap(QPixmap(":/Resource/smoke.png").scaled(200,200));
    smoke -> setPos(-80,-70);
    smoke -> setZValue(-1);
    smoke -> setOpacity(0.0);
    kenny = new QGraphicsPixmapItem(this);
    kenny ->setPixmap(QPixmap(":/Resource/AngelKenny.png").scaled(200,150));
    kenny->setPos(-70, -40);
    kenny->setZValue(-1);
    kenny -> setOpacity(0.0);
    weedtimer= new QTimer();
    angeltimer = new QTimer();

    verticaltimer->start(1000.0/FPS);

    commonParent = new QGraphicsRectItem();
    scene->addItem(commonParent);
    //commonParent->setPos(0,0); //this line will make player it's child
    commonParent->setZValue(PLATFORM_LAYER);

    platforms.push_back(new Normal(commonParent));
    int n = rand()%400;
    platforms[0]->setPos(n, BOTTOM);
    setPos(n, BOTTOM - PLAYER_HEIGHT);

    for(int i = 1; i < 10; ++i){
        if(rand()%ELASTIC_PROB == 1){
            platforms.push_back(new Elastic(commonParent));
            platforms[i]->setPos(rand()%400, BOTTOM - SPACING * i);
        }
        else{
            platforms.push_back(new Normal(commonParent));
            platforms[i]->setPos(rand()%400, BOTTOM - SPACING * i);
            if(rand()%WING_PROB == 2){
                props.push_back(new Wings(commonParent));
                props[props.size()-1]->setPos(platforms[i]->x(),platforms[i]->y()-15);
            }
            else if(rand()%WEED_PROB == 3){
                props.push_back(new Weed(commonParent));
                props[props.size()-1]->setPos(platforms[i]->x()+20,platforms[i]->y()-50);
            }
        }
    }

    lastBounce = platforms[0];

    score = new QGraphicsTextItem();
    score->setPlainText(QString::number(0));
    score->setDefaultTextColor(Qt::black);
    score->setFont(QFont("times", 16));
    gameover = new QGraphicsTextItem();
    gameover->setPlainText(QString("Game Over!"));
    gameover->setDefaultTextColor(Qt::magenta);
    gameover->setFont(QFont("ariel", 40));
    gameover->setPos(100,1400);
    gameover->setZValue(MEMBERBERRY_LAYER);
    gameoverwrapup = new QGraphicsTextItem();
    gameoverwrapup->setDefaultTextColor(Qt::black);
    gameoverwrapup->setFont(QFont("times", 20));
    gameoverwrapup->setPos(100,1480);
    gameoverwrapup->setZValue(MEMBERBERRY_LAYER);

    QMediaPlayer *start = new QMediaPlayer();
    start->setMedia(QUrl("qrc:/Resource/Start.mp3"));
    start->setVolume(30);
    start->play();

    bgm = new QMediaPlayer();

    QTimer::singleShot(3000, this, SLOT(startBGM()));
}

void Player::keyPressEvent(QKeyEvent *event){
    // qDebug() << "keyPress detected";

    switch(event->key()){
    case Qt::Key_Up:{
        if(pause == false){
            MrHankey *mrhankey = new MrHankey(this);
            mrhankey->setPos(-30,0);
            //mrhankey->setPos(x()-30,y());
            //scene()->addItem(memberberry);
        }
        break;
    }
    case Qt::Key_Down:
        break;
    case Qt::Key_Left:
        if(pause == false){
            if(leftKeyHeldDown == false){
                leftKeyHeldDown = true;
                //qDebug() << "left" << leftKeyHeldDown;
                if(!movetimer->isActive()){
                    movetimer->start(1000/FPS);
                }
                if(rocketMode == true){
                    setPixmap(QPixmap(":/Resource/ButterflyLeft.png").scaled(100,100));
                    pixmap().setDevicePixelRatio(-5);
                }
                else{
                    setPixmap(QPixmap(":/Resource/RandyLeft.png").scaled(PLAYER_WIDTH, PLAYER_HEIGHT));
                    pixmap().setDevicePixelRatio(-5);
                }
                //setTransform(QTransform::fromScale(1, 1));
                direction = directions::left;
            }
        }
        break;
    case Qt::Key_Right:
        if(pause == false){
            if(rightKeyHeldDown == false){
                rightKeyHeldDown = true;
                //qDebug() << "right" << rightKeyHeldDown;
                if(!movetimer->isActive()){
                    movetimer->start(1000.0/FPS);
                }
                if(rocketMode == true){
                    setPixmap(QPixmap(":/Resource/ButterflyRight.png").scaled(100,100));
                    pixmap().setDevicePixelRatio(-5);
                }
                else{
                    setPixmap(QPixmap(":/Resource/RandyRight.png").scaled(PLAYER_WIDTH, PLAYER_HEIGHT));
                    pixmap().setDevicePixelRatio(-5);
                }
                //setTransform(QTransform::fromScale(1, 1));
                direction = directions::right;
            }
        }
        break;
    case Qt::Key_Space:
        if(verticaltimer->isActive()){
            verticaltimer->stop();
            pause = true;
        }
        else{
            verticaltimer->start(1000.0/FPS);
            pause = false;
        }
        break;
    }
}

void Player::keyReleaseEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Left || event->key() == Qt::Key_Right){
        if(leftKeyHeldDown == true && rightKeyHeldDown == true ){
            if(event->key() == Qt::Key_Left){
                direction = directions::right;
                if(rocketMode == true){
                    setPixmap(QPixmap(":/Resource/ButterflyLeft.png").scaled(100,100));
                    pixmap().setDevicePixelRatio(-5);
                }
                else{
                    setPixmap(QPixmap(":/Resource/RandyLeft.png").scaled(PLAYER_WIDTH, PLAYER_HEIGHT));
                    pixmap().setDevicePixelRatio(-5);
                }
                leftKeyHeldDown = false;
            }
            else{
                direction = directions::left;
                if(rocketMode == true){
                    setPixmap(QPixmap(":/Resource/ButterflyRight.png").scaled(100,100));
                    pixmap().setDevicePixelRatio(-5);
                }
                else{
                    setPixmap(QPixmap(":/Resource/RandyRight.png").scaled(PLAYER_WIDTH, PLAYER_HEIGHT));
                    pixmap().setDevicePixelRatio(-5);
                }
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
    if(x() < 0 - pixmap().width()/2){
        setX(scene()->width()-pixmap().width()/2);
    }
    else if(x() >= scene()->width()-pixmap().width()/2){
        setX(0 - pixmap().width()/2);
    }
}

void Player::checkVerticalMovement(){
    //if(processing == false){
        processing = true;
        //qDebug() << "vertical";

        checkDelete();

        checkNewElements();

        if(rocketMode == true){
            rocketMove();
        }
        else{
            normalMove();
            checkFail();
            if(fail == true){
                BOTTOM = 650.0;
                endGame();
            }
            else{
                checkBounce();
                checkProps();
            }
            if(downMode == true){
                pullBG();
            }
        }

        processing = false;
    //}
}

//Platform * getPlatform(){
//    return platformparent;
//}

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
    if(direction == directions::right){
        if(rocketMode == true){
            setPixmap(QPixmap(":/Resource/ButterflyRight.png").scaled(100,100));
            pixmap().setDevicePixelRatio(-5);
        }
        else{
            setPixmap(QPixmap(":/Resource/RandyRight.png").scaled(PLAYER_WIDTH, PLAYER_HEIGHT));
            pixmap().setDevicePixelRatio(-5);
        }
    }
    else{
        if(rocketMode == true){
            setPixmap(QPixmap(":/Resource/ButterflyLeft.png").scaled(100,100));
            pixmap().setDevicePixelRatio(-5);
        }
        else{
            setPixmap(QPixmap(":/Resource/RandyLeft.png").scaled(PLAYER_WIDTH, PLAYER_HEIGHT));
            pixmap().setDevicePixelRatio(-5);
        }
    }
}

void Player::checkBackground(){
    /*if(travelDistance - mountaincount * 100 > 0){
        mountains->moveBy(0,1);
        ++mountaincount;
    }
    if(int n = travelDistance - treecount * 10 > 0){
        tree->moveBy(0,(n+10)/10);
        ++treecount;
    }*/
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
    //qDebug()<<"butterfly dead";
    if(direction == directions::left){
        setPixmap(QPixmap(":/Resource/RandyLeft.png").scaled(PLAYER_WIDTH, PLAYER_HEIGHT));
        pixmap().setDevicePixelRatio(-5);
        rocketMode = false;
    }
    else{
        setPixmap(QPixmap(":/Resource/RandyRight.png").scaled(PLAYER_WIDTH, PLAYER_HEIGHT));
        pixmap().setDevicePixelRatio(-5);
        rocketMode = false;
    }
}

directions Player::getDirection(){
    return direction;
}

void Player::setRocketMode(bool a){
    rocketMode = a;
}

void Player::setDownMode(bool a){
    downMode = a;
}

void Player::setWeedMode(){
    smoke->setOpacity(0.9);
    ++weedMode;
}

void Player::setVerticalSpeed(double v){
    verticalSpeed = v;
}

void Player::weedOut(){
    if(weedMode == 1){
        BOTTOM = 650.0;
        --weedMode;
        connect(weedtimer, SIGNAL(timeout()), this, SLOT(smokeFade()));
        weedtimer->start(1000/FPS);
    }
    else{
        --weedMode;
    }
}

void Player::smokeFade(){
    if(smoke->opacity()>0.0){
         smoke->setOpacity(smoke->opacity()-0.04);
    }
    else{
        weedtimer->stop();
    }

}

void Player::setElasticMode(bool a){
    elasticMode = a;
}

void Player::setGRAVITY(double number){
    ACC_PLAYER = number;
}

QVector<Monster*> * Player::getMonsters(){
    return &monsters;
}

void Player::setFail(){
    if(fail == false){
        fail = true;
        gameoverwrapup->setPlainText(QString("Your score: ")+QString::number(travelDistance));
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
}

QGraphicsTextItem * Player::getGameover(){
    return gameover;
}

QGraphicsTextItem * Player::getGameoverwrapup(){
    return gameoverwrapup;
}

void Player::startBGM(){
    playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl("qrc:/Resource/BakcgroundMusic.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    qDebug()<<"playing bgm";
    bgm -> setPlaylist(playlist);
    bgm -> setVolume(30);
    bgm -> play();
}

void Player::stopBGM(){
    fadetimer = new QTimer();
    connect(fadetimer, SIGNAL(timeout()), this, SLOT(fadeOutBGM()));
    fadetimer->start(150);
}

void Player::fadeOutBGM(){
    if(bgm->volume() > 0){
        bgm->setVolume(bgm->volume()-1);
        //qDebug() << "volume =" << bgm->volume();
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
    // delete platforms
    for(int i = 0; i < platforms.size();++i){
        if(platforms[i]->mapToScene(0,0).y() > BOTTOM + 50){
            //qDebug() << "delete platform";
            scene()->removeItem(platforms[i]);
            platforms.removeFirst();
        }
        else{
            break;
        }
    }
    for(int i = 0; i < props.size(); ++i){
        if(props[i]->mapToScene(0,0).y() > BOTTOM + 50){
            scene()->removeItem(props[i]);
            props.removeFirst();
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
            if(monsters[i]->mapToScene(0,0).y() > BOTTOM + 50){
                //qDebug() << "monster out of bounds";
                monsters[i]->stopTimer();
                scene()->removeItem(monsters[i]);
                monsters.removeFirst();
            }
            else{
                break;
            }
        }
    }
    monsters.squeeze();
    for(int i = 0; i < erics.size(); ++i){
        if(erics[i]->mapToScene(0,0).y()> BOTTOM + 50){
            qDebug() << "stopping" << QString::pointer(erics[i]) << "timers";
            erics[i]->stopTimer();
            for(int j = 0; j < erics[i]->getMemberberries()->size(); ++j){
                qDebug() << "removing" << QString::pointer(erics[i]) << "memberberries["<<j<<"] timers";
                (*erics[i]->getMemberberries())[j]->stopTimer();
                scene()->removeItem((*erics[i]->getMemberberries())[j]);
            }
            (*erics[i]->getMemberberries()).clear();
            (*erics[i]->getMemberberries()).squeeze();
            scene()->removeItem(erics[i]);
            erics.remove(i);
        }
        else{
            break;
        }
    }

}

void Player::checkNewElements(){
    // add new platforms and monsters
    if(travelDistance-platformcount * SPACING > 0){
        qDebug() << "new item added";
        if(rand()%ELASTIC_PROB == 1){
            platforms.push_back(new Elastic(commonParent));
            platforms[platforms.size()-1]->setPos(rand()%400, 0 - 10 - commonParent->mapToScene(0,0).y());
        }
        else{
            platforms.push_back(new Normal(commonParent));
            platforms[platforms.size()-1]->setPos(rand()%400, 0 - 10 - commonParent->mapToScene(0,0).y());
            if(rand()%WING_PROB == 2){
                props.push_back(new Wings(platforms[platforms.size()-1]));
                props[props.size()-1]->setPos(platforms[platforms.size()-1]->x(),platforms[platforms.size()-1]->y()-15);
                //scene()->addItem(props[props.size()-1]);
            }
            else if(rand()%WEED_PROB == 3){
                props.push_back(new Weed(commonParent));
                props[props.size()-1]->setPos(platforms[platforms.size()-1]->x()+20,platforms[platforms.size()-1]->y()-50);
                //scene()->addItem(props[props.size()-1]);
            }
            else if(rand()%MONSTER_PROB == 0){
                //qDebug() << "monster created";
                monsters.push_back(new Monster(commonParent));
                monsters[monsters.size()-1]->setPos(rand()%400 ,platforms[platforms.size()-1]->y()-MONSTER_HEIGHT-10);
                //scene()->addItem(monsters[monsters.size()-1]);
            }
            else if(rand()%ERIC_PROB == 5){
                //qDebug() << "eric appeared";
                if(rand()%2 == 1){
                    erics.push_back(new Eric(leftSide, commonParent));
                    erics[erics.size()-1]->setPos(-10, platforms[platforms.size()-1]->y()-ERIC_SIZE);
                }
                else{
                    erics.push_back(new Eric(rightSide, commonParent));
                    erics[erics.size()-1]->setPos(410, platforms[platforms.size()-1]->y()-ERIC_SIZE);
                }
                //scene()->addItem(erics[erics.size()-1]);
            }
            else if(rand()%KENNY_PROB == 8){
                props.push_back(new Kenny(commonParent));
                props[props.size()-1]->setPos(platforms[platforms.size()-1]->x()+20 ,platforms[platforms.size()-1]->y()- 40);
            }
        }
        //scene()->addItem(platforms[platforms.size()-1]);
        ++platformcount;
    }

}

void Player::checkBounce(){

    // bounces off a platform and bring down all items if needed
    if(verticalSpeed < 0){ //don't do = 0 cause it will create infinity speed
        for(int i = 0; i < platforms.size(); ++i){
            if(mapFromItem(platforms[i], 0, 0).x()<PLAYER_WIDTH && mapFromItem(platforms[i], 0, 0).x() > -PLATFORM_WIDTH)
                if(mapFromItem(platforms[i], 0, 0).y() >= PLAYER_HEIGHT && mapFromItem(platforms[i], 0, 0).y() < PLAYER_HEIGHT - verticalSpeed){
                    //qDebug() << "jump!";
                    if(direction == directions::right){
                        setPixmap(QPixmap(":/Resource/RandyCrouchedRight.png").scaled(PLAYER_WIDTH, PLAYER_HEIGHT));
                    }
                    else{
                        setPixmap(QPixmap(":/Resource/RandyCrouchedLeft.png").scaled(PLAYER_WIDTH, PLAYER_HEIGHT));
                    }
                    QTimer::singleShot(200, this, SLOT(decrouch()));
                    platforms[i]->response();
                    setY(platforms[i]->mapToScene(0,0).y() - PLAYER_HEIGHT);
                    qDebug() << "height = " << y();
                    //if(y()-MAX_HEIGHT > 0){
                        verticalSpeed = (2.0*(y()-MAX_HEIGHT)/jumpTime);
                        ACC_PLAYER = verticalSpeed/jumpTime;
                    //}
                    //else{
                        //t = (sqrt(verticalSpeed*verticalSpeed-2*GRAVITY*(y()-MAX_HEIGHT))-verticalSpeed)/GRAVITY;
                    //}
                    //qDebug() << "s = "<< y()-MAX_HEIGHT;
                    lastBounce = platforms[i];
                    if(mapFromItem(lastBounce, 0, 0).y() < BOTTOM){
                        downMode = true;
                        verticalItemSpeed = (BOTTOM-lastBounce->mapToScene(0,0).y()) * 2.0/jumpTime;
                        ACC_ITEM = verticalItemSpeed/jumpTime;
                        qDebug() << "item speed = "<< verticalItemSpeed;

                        //verticalItemSpeed = (t>0)?((BOTTOM-lastBounce->y())/t):10;
                    }
                    break;
                }
            }
        }

 }

void Player::Bounce(){
    //qDebug() << "jump!";
    if(direction == directions::right){
        setPixmap(QPixmap(":/Resource/RandyCrouchedRight.png").scaled(PLAYER_WIDTH, PLAYER_HEIGHT));
    }
    else{
        setPixmap(QPixmap(":/Resource/RandyCrouchedLeft.png").scaled(PLAYER_WIDTH, PLAYER_HEIGHT));
    }
    QTimer::singleShot(200, this, SLOT(decrouch()));
    qDebug() << "height = " << y();
    //if(y()-MAX_HEIGHT > 0){
        verticalSpeed = (2.0*(y()-MAX_HEIGHT)/jumpTime);
        ACC_PLAYER = verticalSpeed/jumpTime;
    //}
    //else{
        //t = (sqrt(verticalSpeed*verticalSpeed-2*GRAVITY*(y()-MAX_HEIGHT))-verticalSpeed)/GRAVITY;
    //}
    //qDebug() << "s = "<< y()-MAX_HEIGHT;
    if(mapFromItem(lastBounce, 0, 0).y() < BOTTOM){
        downMode = true;
        verticalItemSpeed = (BOTTOM-lastBounce->mapToScene(0,0).y()) * 2.0/jumpTime;
        ACC_ITEM = verticalItemSpeed/jumpTime;
        qDebug() << "item speed = "<< verticalItemSpeed;
    }
}

void Player::checkProps(){
    //check props
    for(int i = 0; i < props.size(); ++i){
        if(props[i]->collidesWithItem(this)){
            props[i]->response();
            scene()->removeItem(props[i]);
            props.remove(i);
        }
    }
}

void Player::endGame(){

    for(int i = 0; i < platforms.size();++i){
        if(platforms[i]->y()>BOTTOM + 50){
            //qDebug() << "platform removed";
            scene()->removeItem(platforms[i]);
            platforms.removeFirst();
        }
        else{
            break;
        }
    }
    for(int i = 0; i < props.size(); ++i){
        if(props[i]->y()>BOTTOM + 50){
            scene()->removeItem(props[i]);
            props.removeFirst();
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
            if(monsters[i]->y() > BOTTOM + 50){
                scene()->removeItem(monsters[i]);
                monsters[i]->stopTimer();
                monsters.removeFirst();
            }
            else{
                break;
            }

        }
    }
    for(int i = 0; i < erics.size(); ++i){
        if(erics[i]->y()>BOTTOM + 50){
            scene()->removeItem(erics[i]);
            erics.removeFirst();
        }
        else{
            break;
        }
    }

    if(gameover->y() <= 100){
        verticaltimer->stop();
        dynamic_cast<Game*>(scene()->views()[0])->setGameOver();
        //qDebug() << "game over set";
        scene()->removeItem(this);
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
            ACC_PLAYER = (650.0 - MAX_HEIGHT - PLAYER_HEIGHT)*2.0 /(jumpTime*jumpTime);
            downMode = false;
            verticalItemSpeed = 0;
            travelDistance += BOTTOM-lastBounce->mapToScene(0,0).y();
            score->setPlainText(QString::number(travelDistance));
            if(elasticMode == true){
                BOTTOM = 650.0;
                ACC_PLAYER= (650.0 - MAX_HEIGHT - PLAYER_HEIGHT)*2.0 /(jumpTime*jumpTime);;
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
        ACC_PLAYER = 700.0/(jumpTime*jumpTime);
        downMode = false;
    }
}

void Player::rocketMove(){
    // move to a certain height and stop
    if(verticalSpeed > 0 && y() > 300){
        if(y() < MAX_HEIGHT + ROCKET_SPEED){
                setY(MAX_HEIGHT);
                verticalSpeed = 0;
        }
        else{
            moveBy(0,-ROCKET_SPEED);
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
        qDebug() << "height = " << y();
        if(fail == false){
            qDebug() << "you fail!";
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

    for (int i = 0; i < platforms.size(); i++)
        delete platforms[i];
    platforms.clear();

    for (int i = 0; i < props.size(); i++)
        delete props[i];
    props.clear();

    for (int i = 0; i < monsters.size(); i++)
        delete monsters[i];
    monsters.clear();

    for (int i = 0; i < erics.size(); i++)
        delete erics[i];
    erics.clear();
}

void Player::setLastBounce(QGraphicsItem * item){
    lastBounce = item;
}

void Player::setAngelMode(){
    kenny->setOpacity(0.5);
    ++angelMode;
    qDebug() << "angelMode = " << angelMode;
}

void Player::angelOut(){
    if(angelMode == 1){
        --angelMode;
        connect(angeltimer, SIGNAL(timeout()), this, SLOT(angelFade()));
        angeltimer->start(1000/FPS);
        qDebug() << "angelMode = " << angelMode;
    }
    else{
        --angelMode;
        qDebug() << "angelMode = " << angelMode;
    }
}


void Player::angelFade(){
    if(kenny->opacity()>0.0){
         kenny->setOpacity(kenny->opacity()-0.04);
    }
    else{
        angeltimer->stop();
    }
}
