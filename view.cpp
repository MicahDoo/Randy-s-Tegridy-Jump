#include "view.h"
#include "player.h"
#include "numbers.h"
#include <QDebug>

Game::Game(){
    startGame();
}


QGraphicsScene * Game::getScene(){
    return scene;
}

Player* Game::getPlayer(){
    return player;
}

void Game::setGameOver(){
    gameover = true;
    //scene->removeItem(player);
}

void Game::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter){
        if(gameover == true){
            qDebug() << "game restart";
            gameover = false;
            //player->stopTimers();
            scene->clear();
            //delete player;
            startGame();
        }
    }
    else{
        if(gameover == false){
            player->keyPressEvent(event);
        }
    }
}

void Game::startGame(){
    // create a scene
    scene = new QGraphicsScene();
    scene -> setSceneRect(2,0, 500, SCREEN_HEIGHT);
    scene -> setBackgroundBrush(QBrush(QImage(":/Resource/Background.jpeg").scaled(500, SCREEN_HEIGHT)));
    setScene(scene);

    // create an item
    player = new Player(scene);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    player->getCommonParent()->setPos(0,0);

    // add the item to the scene
    scene->addItem(player->getTree());
    scene->addItem(player->getMountains());
    scene->addItem(player->getScore());
    scene->addItem(player->getGameover());
    scene->addItem(player->getGameoverwrapup());
}
