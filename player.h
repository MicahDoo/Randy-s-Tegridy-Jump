#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QObject>
#include <QVector>
#include <QGraphicsTextItem>
#include <QMediaPlayer>
#include <QGraphicsDropShadowEffect>
#include "platforms.h"
#include "numbers.h"
#include "props.h"
#include "monsters.h"
#include "eric.h"

class Player: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Player(QGraphicsScene *scene);


    //get game items
    QVector<Platform*> getPlatforms();
    QGraphicsPixmapItem *getTree();
    QGraphicsPixmapItem *getMountains();
    QVector<Prop*>  getProps();
    QVector<Monster*> *getMonsters();
    QGraphicsItem * getCommonParent();
    QGraphicsPixmapItem* getRandy();

    //get text items
    QGraphicsTextItem *getScore();
    QGraphicsTextItem *getGameover();
    QGraphicsTextItem *getGameoverwrapup();

    //get attributes
    double getSpeed();
    directions getDirection();
    bool isProtectedMode();
    bool isFlipMode();
    int getTravelDistance();

    //set modes
    void setRocketMode(bool a);
    void setDownMode(bool a);
    void setVerticalSpeed(double v);
    void setBOTTOM(double number);
    void setWeedMode();
    void setFail();
    void setElasticMode(bool a);
    void setAngelMode();
    void setLastBounce(QGraphicsItem * item);
    void setFlipMode(bool a);
    void setFlipEndDistance(int num);
    void setPause(bool a);

    //set attributes
    void setGRAVITY(double number);

    void checkDelete();
    void checkNewElements();
    void checkBounce();
    void checkProps();
    void checkDarkWell();
    void checkFlipMode();
    void endGame();
    void pullBG();
    void rocketMove();
    void normalMove();
    void checkFail();

    void Bounce();

    void stopTimers();

    // key events
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    void focusOutEvent(QFocusEvent * event);


public slots:
    void checkHorizontalMovement();
    void checkVerticalMovement();
    void decrouch();
    void checkBackground();
    void butterflyDead();
    void weedOut();
    void startBGM();
    void fadeOutBGM();
    void stopBGM();
    void smokeFade();
    void angelOut();
    void angelFade();
    void flipping();

    //void fadeInKenny();
    //void fadeOutKenny();

private:

    //timers
    QTimer *verticaltimer;
    QTimer *movetimer;
    QTimer *backgroundtimer;
    QTimer *fadetimer;
    QTimer *weedtimer;
    QTimer *angeltimer;
    QTimer *weedlasttimer;
    QTimer *butterflylasttimer;
    QTimer *angellasttimer;
    QTimer *decrouchtimer;
    QTimer *pauseandfliptimer;

    double weedRemain = -1;
    double butterflyRemain = -1;
    double angelRemain = -1;
    double decrouchRemain = -1;

    //status
    bool leftKeyHeldDown = false;
    bool rightKeyHeldDown = false;
    bool processing = false;
    QGraphicsItem *lastBounce;

    //modes
    bool downMode = false;
    bool fail = false;
    bool pause = false;
    bool rocketMode = false;
    bool elasticMode = false;
    bool weedMode = false;
    bool angelMode = false;
    bool darkWellMode = false;
    bool flipMode = false;

    // player attributes
    double BOTTOM = 650.0; // don't let items be deleted above this height
    double ACC_ITEM;
    const double jumpTime = JUMP_TIME_SECS * 1000.0/FPS; //miliseconds/refresh rate
    double verticalSpeed = (650.0 - MAX_HEIGHT - PLAYER_HEIGHT)*2.0 /jumpTime;
    double ACC_PLAYER = (650.0 - MAX_HEIGHT - PLAYER_HEIGHT)*2.0 /(jumpTime*jumpTime);
    double verticalItemSpeed;
    //enum directions {left, right};
    directions direction;
    int travelDistance = 0;

    // scene attributes
    int platformcount = 0;
    int mountaincount = 0;
    int treecount = 0;
    int nextDarkWellLocation;
    int flipEndDistance;
    int flipangle;
    double scalefactor;

    // game elements
    QGraphicsRectItem *commonParent;
    QVector<Platform*> platforms; // normal, spring
    QVector<Prop*> props; // wings, weeds, kenny's
    QVector<Monster*> monsters; //satan, manbearpig
    QVector<Eric*> erics;
    QGraphicsPixmapItem *tree;
    QGraphicsPixmapItem *mountains;
    QGraphicsPixmapItem *randy;
    QGraphicsPixmapItem *smoke;
    QGraphicsPixmapItem *kenny;
    QGraphicsPathItem *darkness;
    QGraphicsDropShadowEffect *shadow;
    QGraphicsPixmapItem *darkWell;

    //texts
    QGraphicsTextItem *score;
    QGraphicsTextItem *gameover;
    QGraphicsTextItem *gameoverwrapup;

    // media players
    QMediaPlayer *bgm;
    QMediaPlaylist *playlist;
    QMediaPlayer *killedKenny;
    QMediaPlayer *kennyTalk;
    QMediaPlayer *angelSound;

public:
    ~Player();
};




#endif // PLAYER_H
