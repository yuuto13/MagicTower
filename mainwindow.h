#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "animateditem.h"
#include "objectclass.h"
#include "pathfinding.h"
#include <QtCore>
#include <QInputDialog>
#include <QActionGroup>
#include <QMessageBox>

const int MARGIN = 18;
const int H_MENU = 23;

const int x_pad[9] = {0, 1, 2, 0, 1, 2, 0, 1, 2};
const int y_pad[9] = {0, 0, 0, 1, 1, 1, 2, 2, 2};
const int x_token[4] = {0,  0, -1, 1};
const int y_token[4] = {1, -1,  0, 0};
//const int x_pad_[9] = {-1,  0,  1, -1, 0, 1, -1, 0, 1};
//const int y_pad_[9] = {-1, -1, -1,  0, 0, 0,  1, 1, 1};

void Delay_MSec(unsigned int msec);

struct Event
{
    int flag = 0;
    int type = 0;
    int x = 0, y = 0;
    int id = 0;
};

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setFloor(int floorNumber);
    void setPlayer(int x, int y);
    void setPlayer(int objectId = 206);
    void setRef();
    void setUI();
    void showInfo(int id);
    void showObject(AnimatedItem * item, int id);
    void showAnimation(AnimatedItem * item, int, int objId = 0, bool isReverse = false);

    void grabItem();
    void throwItem();
    void interact();
    void fight(int);
    void aiWalk(int, int);
    void playerWalk();
    void endDrag();
    void setNum(NumberItem *, QPoint, int);
    bool eventTrigger(int eventType);
    void scene(int sceneId);
    void objectMove(int objId, int startX, int startY, int endX, int endY);

public slots:
    void selectObject(int, int);
    void selectItem(int, int);
    void selectDiff(int, int);
    void gridDiaplay(int, int);

    void autoSave();
    void saveGame();
    void loadGame();
    void saveMap();
    void setMode(bool);
    void syncAnimations();
    void gotoFloor();
    void notice(QString);

protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);

private slots:
    void on_actionNew_Game_triggered();

private:
    Ui::MainWindow *ui;
    QActionGroup * saveActions;
    QActionGroup * loadActions;

    MyDialog * dialog;

    QPixmap * sourcePix[5];
    AnimatedItem * floor[13][13];
    AnimatedItem * object[13][13];
    AnimatedItem * ref[8][8];
    AnimatedItem * uiItem[25];
    AnimatedItem * inventory[15];
    AnimatedItem * frame[3];
    AnimatedItem * effect[4];
    AnimatedItem * playerItem;
    AnimatedItem * dragItem;
    AnimatedItem * hpBar;
    NumberItem * numberItem[12];
    NumberItem * dragNum;
    QMap<Grid, NumberItem *> numMap;

    QList<Event> eventList;

    Player player;

    short objectMap[13][13];
    short mapRef[8][8];
    short x = -1, y = -1;

    int key;
    short token = 0;
    short pace = 0;
    bool isPressed = false;
    bool isDragging = false;
    bool isWalking = false;
    bool isFighting = false;
    bool hasControl = true;

    short saveSlot = 0;
    short floorNumber = 0;
    short maxFloorNum = 50;
    QString floorName;
    bool isEditMode = false;

    short selectedItemId = 0;
    short selectedObjectId = 0;
};

#endif // MAINWINDOW_H
