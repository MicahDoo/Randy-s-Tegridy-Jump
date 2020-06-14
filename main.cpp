#include <QApplication>
#include <QGraphicsView>
#include <QObject>
#include <ctime>
#include "player.h"
#include "platforms.h"
#include "view.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    srand(time(NULL));


    // create a view

    Game * view = new Game();

    view->setFixedSize(SCREEN_WIDTH,SCREEN_HEIGHT);
    view->show();
    view->fitInView(view->getScene()->sceneRect(), Qt::IgnoreAspectRatio);

    return a.exec();
}
