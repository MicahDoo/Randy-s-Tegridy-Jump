#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include "platforms.h"
#include "player.h"
#include <QKeyEvent>

class Game: public QGraphicsView{
public:
    Game();
    Player *getPlayer();
    QGraphicsScene * getScene();
    void startGame();
public slots:
    void setGameOver();
    void keyPressEvent(QKeyEvent *event);
private:
    QGraphicsScene * scene;
    Player * player;
    bool gameover = false;
    QGraphicsItem * commonParent;
};

#endif // VIEW_H
