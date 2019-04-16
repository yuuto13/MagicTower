#include "mainwindow.h"
#include "ui_mainwindow.h"

short mapRef1[8][8] = {
    {  1,   2,   3,   4,   5,   6,   7,   8},
    {  9,  10,  11,  12,  13,  14,  15,  16},
    { 17,  18,  19,  20,  21,  22,  23,  24},
    { 25,  26,  27,  28,  29,  30,  33,  34},
    { 35,  36,  37,  38, 101, 102, 103, 104},
    {105, 106, 107, 108, 109, 110, 111, 112},
    {113, 114, 115, 116, 117, 118, 119, 120},
    {  0,   0,   0,   0,   0,   0,   0, 306}
};
short mapRef2[8][8] = {
    {121, 122, 123, 124, 125, 126, 127, 128},
    {129, 130, 131, 132, 133, 134, 135, 136},
    {201, 202, 203, 204, 205, 206, 207, 208},
    {209, 210, 211, 212, 213, 215, 216, 217},
    {218, 251, 252, 308, 309, 310, 214, 256},
    {257,   0,   0,   0,   0,   0,   0,   0},
    {  0,   0,   0,   0,   0,   0,   0,   0},
    {305,   0,   0,   0,   0,   0,   0,   0}
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(738, 510);
    this->setMouseTracking(true);
    sourcePix[0] = new QPixmap("./resources/pic/model.png");
    sourcePix[1] = new QPixmap("./resources/pic/bonus_picture.png");
    sourcePix[2] = new QPixmap("./resources/pic/bonus_animation.png");
    sourcePix[3] = new QPixmap("./resources/pic/button.png");
    sourcePix[4] = new QPixmap("./resources/pic/number_4.png");
    for(int i = 0; i < 13; i++){
        for(int j = 0; j < 13; j++){
            objectMap[i][j] = 0;
        }
    }
    for(int i = 0; i < 3; i++){
        frame[i] = new AnimatedItem(this);
        frame[i]->hide();frame[i]->setEnabled(false);
        frame[i]->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    }
    for(int i = 0; i < 13; i++){
        for(int j = 0; j < 13; j++){
            floor[i][j] = new AnimatedItem(this);
            floor[i][j]->setPix(sourcePix[0]->copy(11*32, 15*32, 32, 32));
            floor[i][j]->move(MARGIN + i*32, MARGIN + H_MENU + j*32);
            floor[i][j]->setEnabled(false);
        }
    }
    for(int i = 0; i < 13; i++){
        for(int j = 0; j < 13; j++){
            object[i][j] = new AnimatedItem(this);
            object[i][j]->setCoordinates(i, j);
            this->showObject(object[i][j], 0);
            object[i][j]->move(MARGIN + i*32, MARGIN + H_MENU + j*32);
            connect(object[i][j], SIGNAL(mouseEntered(int, int)), this, SLOT(gridDiaplay(int, int)));
            connect(object[i][j], SIGNAL(mouseLeaved()), this->frame[0], SLOT(hide()));
            connect(object[i][j], SIGNAL(mousePressed(int, int)), this, SLOT(selectObject(int,int)));
            connect(object[i][j], SIGNAL(rightPressed(int, int)), this, SLOT(selectDiff(int,int)));
        }
    }
    for(int i = 0; i < 25; i++){
        uiItem[i] = new AnimatedItem(this);
        if(i < 12){
            uiItem[i]->move(MARGIN*3 + 13*32 + (i%6)*43, H_MENU + 230 + (i/6)*43);
            uiItem[i]->setPix(QPixmap("./resources/pic/slot.png"));
        }else if(i < 15){
            uiItem[i]->move(642, (i-12)*40+105);
            uiItem[i]->setPix(QPixmap("./resources/pic/slot.png"));
        }else {
            uiItem[i]->setEnabled(false);
        }
    }
    uiItem[15]->move(486, 58);
    uiItem[15]->setAnimation(QPixmap(sourcePix[0]->copy(1*32, 10*32, 64, 32)), 2, 300);
    uiItem[16]->setPix(QPixmap("./resources/pic/frame_2.png"));
    uiItem[17]->setPix(QPixmap("./resources/pic/frame_2.png"));
    uiItem[16]->move(484, 56);
    uiItem[17]->move(484, 341);
    uiItem[18]->move(486, 343);//Info Object Icon
    this->showObject(uiItem[18], 0);
    uiItem[19]->setPix(QPixmap("./resources/pic/value_icons.png").copy(0, 0, 20, 20));uiItem[19]->hide();uiItem[19]->move(472, 395);
    uiItem[20]->setPix(QPixmap("./resources/pic/value_icons.png").copy(20, 0, 20, 20));uiItem[20]->hide();uiItem[20]->move(472, 415);
    uiItem[21]->setPix(QPixmap("./resources/pic/value_icons.png").copy(40, 0, 20, 20));uiItem[21]->hide();uiItem[21]->move(472, 435);
    uiItem[22]->setPix(QPixmap("./resources/pic/value_icons.png").copy(60, 0, 20, 20));uiItem[22]->hide();uiItem[22]->move(472, 455);
    uiItem[23]->setPix(QPixmap("./resources/pic/value_icons.png").copy(20, 0, 20, 20));uiItem[23]->move(472, 110);
    uiItem[24]->setPix(QPixmap("./resources/pic/value_icons.png").copy(40, 0, 20, 20));uiItem[24]->move(472, 130);
//    uiItem[25]->setPix(QPixmap("./resources/pic/button_use_off.png"));
//    uiItem[25]->move(686, 333);//##
    for(int i = 0; i < 15; i++) {
        inventory[i] = new AnimatedItem(this);
        inventory[i]->setCoordinates(i, 13);
        inventory[i]->setDragable(true);
        this->showObject(inventory[i], 0);
        connect(inventory[i], SIGNAL(mouseEntered(int, int)), this, SLOT(gridDiaplay(int, int)));
        connect(inventory[i], SIGNAL(mouseLeaved()), this->frame[0], SLOT(hide()));
        connect(inventory[i], SIGNAL(mousePressed(int,int)), this, SLOT(selectItem(int,int)));
        connect(inventory[i], SIGNAL(rightPressed(int,int)), this, SLOT(selectDiff(int,int)));
        if(i < 12) {
            inventory[i]->move(MARGIN*3 + 13*32 + (i%6)*43, H_MENU + 230 + (i/6)*43);
            numberItem[i] = new NumberItem(this);
            numberItem[i]->setAttribute(Qt::WA_TransparentForMouseEvents, true);
            numberItem[i]->setPix(QPixmap("./resources/pic/number_4.png"));
            this->setNum(numberItem[i], inventory[i]->pos(), 0);
        }else{
            inventory[i]->move(642, (i-12)*40+105);
        }
    }
    this->ui->label_Object_Name->setText("Nothing");
    this->ui->label_Object_Name->move(479, 355);
    this->ui->label_Object_Description->setText("None.");
    this->ui->label_Object_Value_1->setText("");
    this->ui->label_Object_Value_2->setText("");
    this->ui->label_Object_Value_3->setText("");
    this->ui->label_Object_Value_4->setText("");
    this->ui->label_Enchantment->hide();
    this->ui->label_Object_Enchantment->hide();
    playerItem = new AnimatedItem(this);
    playerItem->setPix(QPixmap(sourcePix[0]->copy(0*32, 10*32, 32, 32)));
    playerItem->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    for(int i = 0; i < 4; i++){
        effect[i] = new AnimatedItem(this);
        effect[i]->setFlexablePix(sourcePix[0]->copy(9*32, 15*32, 32, 32));
        effect[i]->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        effect[i]->setEnabled(false);
        effect[i]->hide();
    }
    hpBar = new AnimatedItem(this);
    hpBar->setFlexablePix(QPixmap("./resources/pic/hp_bar.png"));hpBar->hide();
    hpBar->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    hpBar->setEnabled(false);
    frame[0]->setFlexablePix(QPixmap("./resources/pic/frame.png").copy(0*32, 0, 32, 32));
    frame[1]->setFlexablePix(QPixmap("./resources/pic/frame.png").copy(1*32, 0, 32, 32));
    frame[2]->setFlexablePix(QPixmap("./resources/pic/frame.png").copy(1*32, 0, 32, 32));
    frame[0]->raise();frame[1]->raise();
    dragItem = new AnimatedItem(this);
    dragItem->hide();dragItem->setEnabled(false);
    dragItem->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    dragNum = new NumberItem(this);
    dragNum->setEnabled(false);
    dragNum->setPix(QPixmap("./resources/pic/number_4.png"));
    this->setNum(dragNum, dragItem->pos(), 0);
    dragNum->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            mapRef[j][i] = mapRef1[j][i];
            ref[i][j] = new AnimatedItem(this);
            ref[i][j]->setCoordinates(i+13, j);
            ref[i][j]->move(MARGIN*3 + (i+13)*32, MARGIN + H_MENU + j*32);
            connect(ref[i][j], SIGNAL(mouseEntered(int, int)), this, SLOT(gridDiaplay(int, int)));
            connect(ref[i][j], SIGNAL(mouseLeaved()), this->frame[0], SLOT(hide()));
            connect(ref[i][j], SIGNAL(mousePressed(int,int)), this, SLOT(selectItem(int,int)));
        }
    }
    this->setRef();
    dialog = new MyDialog(this);dialog->hide();

    loadActions = new QActionGroup(this);saveActions = new QActionGroup(this);
    loadActions->addAction(this->ui->actionLoad_Slot_1);loadActions->addAction(this->ui->actionLoad_Slot_2);
    loadActions->addAction(this->ui->actionLoad_Slot_3);loadActions->addAction(this->ui->actionLoad_Slot_4);
    loadActions->addAction(this->ui->actionLoad_Slot_5);loadActions->addAction(this->ui->actionLoad_Slot_6);
    loadActions->addAction(this->ui->actionLoad_Slot_7);loadActions->addAction(this->ui->actionLoad_Slot_8);
    loadActions->addAction(this->ui->actionLoad_Slot_9);loadActions->addAction(this->ui->actionLoad_Slot_10);
    saveActions->addAction(this->ui->actionSave_Slot_1);saveActions->addAction(this->ui->actionSave_Slot_2);
    saveActions->addAction(this->ui->actionSave_Slot_3);saveActions->addAction(this->ui->actionSave_Slot_4);
    saveActions->addAction(this->ui->actionSave_Slot_5);saveActions->addAction(this->ui->actionSave_Slot_6);
    saveActions->addAction(this->ui->actionSave_Slot_7);saveActions->addAction(this->ui->actionSave_Slot_8);
    saveActions->addAction(this->ui->actionSave_Slot_9);saveActions->addAction(this->ui->actionSave_Slot_10);
    for(int i = 0; i < 10; ++i) {
        connect(loadActions->actions().at(i), SIGNAL(triggered()), this, SLOT(loadGame()));
        connect(saveActions->actions().at(i), SIGNAL(triggered()), this, SLOT(saveGame()));
        if(QFile::exists(QString("./resources/save/%1/0.floor").arg(i+1, 2, 10, QChar('0')))) {
            loadActions->actions().at(i)->setEnabled(true);
        }
    }
    QFile f("./resources/save/save.ini");
    if(f.open(QFile::ReadOnly)){
        QTextStream ts(&f);
        ts >> saveSlot;
        f.close();
    }
    if(saveSlot != 0) loadActions->actions().at(saveSlot-1)->setChecked(true);
    for(int i = 0; i < maxFloorNum; ++i) {
        QFile f(QString("./resources/map/%1.autosave").arg(i));
        if(f.exists()) f.remove();
    }
    this->loadGame();
    this->ui->actionEdit_Mode->setChecked(isEditMode);
    connect(this->ui->actionSave_Map, SIGNAL(triggered()), this, SLOT(saveMap()));
    connect(this->ui->actionSync_Animation, SIGNAL(triggered()), this, SLOT(syncAnimations()));
    connect(this->ui->actionEdit_Mode, SIGNAL(toggled(bool)), this, SLOT(setMode(bool)));
    connect(this->ui->actionGo_to_Floor, SIGNAL(triggered()), this, SLOT(gotoFloor()));
    this->setFocus();
}

MainWindow::~MainWindow()
{
    int i, j;
    delete ui;
    delete loadActions;
    delete saveActions;
    delete dialog;
    for(i = 0; i < 5; i++){
        delete sourcePix[i];
    }
    delete playerItem;
    delete hpBar;
    delete dragItem;
    delete dragNum;
    for(i = 0; i < 3; i++){
        delete frame[i];
    }
    for(int i = 0; i < 4; i++){
        delete effect[i];
    }
    for(i = 0; i < 13; i++){
        for(j = 0; j < 13; j++){
            delete floor[i][j];
            delete object[i][j];
        }
    }
    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
            delete ref[i][j];
        }
    }
    for(int i = 0; i < 25; i++){
        delete uiItem[i];
    }
    for(int i = 0; i < 15; ++i) {
        delete inventory[i];
        if(i < 12) {
            delete numberItem[i];
        }
    }
}

void MainWindow::setFloor(int floorNumber)
{
    this->floorNumber = floorNumber;
    foreach (NumberItem * numItem, numMap) {
        delete numItem;
    }
    numMap.clear();
    eventList.clear();
    QFile f;
    QString name1 = QString("./resources/save/%1/%2.floor").arg(saveSlot, 2, 10, QChar('0')).arg(floorNumber);
    QString name2 = QString("./resources/map/%1.autosave").arg(floorNumber);
    if(QFile::exists(name2)) {
        f.setFileName(name2);
    }else if(QFile::exists(name1)){
        f.setFileName(name1);
    }else f.setFileName(QString("./resources/map/%1.floor").arg(floorNumber));
    if(f.open(QFile::ReadOnly)){
        QTextStream ts(&f);
        for(int i = 0; i < 13; i++){
            for(int j = 0; j < 13; j++){
                ts >> objectMap[j][i];
            }
        }
        ts.readLine();
        floorName = ts.readLine();
        while(!ts.atEnd()) {
            Event event;
            ts >> event.flag >> event.type >> event.x >> event.y >> event.id;
            if(event.type != 0) eventList.append(event);
        }
        f.close();
    }
    this->ui->label_Floor->setText("The " + QString("%1").arg(floorNumber, 2, 10, QChar(' ')) + "th Floor: " + QString("%1").arg(floorName, 15, QChar(' ')));
    for(int i = 0; i < 13; i++){
        for(int j = 0; j < 13; j++){
            this->showObject(object[i][j], 0);
            object[i][j]->show();
        }
    }
    for(int i = 0; i < 13; i++){
        for(int j = 0; j < 13; j++){
            if(objectMap[i][j] == 31 || objectMap[i][j] == 32 || objectMap[i][j] == 39){
                if(objectMap[i][j] == 39){
                    if(objectMap[i+1][j]==-1&&objectMap[i+2][j]==-2){
                        object[i][j]->setAnimation(sourcePix[0]->copy(0*32, 9*32, 192, 32), 2, 300);
                        object[i+1][j]->hide();object[i+2][j]->hide();
                    }
                }else{
                    if(objectMap[i+1][j]==-1&&objectMap[i+2][j]==-2&&objectMap[i][j+1]==-3&&objectMap[i+1][j+1]==-4
                            &&objectMap[i+2][j+1]==-5&&objectMap[i][j+2]==-6&&objectMap[i+1][j+2]==-7&&objectMap[i+2][j+2]==-8){
                        object[i][j]->setAnimation(sourcePix[0]->copy((objectMap[i][j]-31)*6*32, 5*32, 192, 96), 2, 300);
                        for(int n = 1; n < 9; n++){
                            object[i+x_pad[n]][j+y_pad[n]]->hide();
                        }
                    }
                }
            }else this->showObject(object[i][j], objectMap[i][j]);
        }
    }
}

void MainWindow::setRef()
{
    if(isEditMode){
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                if(mapRef[j][i] == 0){
                    ref[i][j]->hide();
                }else{
                    ref[i][j]->show();
                    ref[i][j]->setDragable(true);
                    this->showObject(ref[i][j], mapRef[j][i]);
                }
            }
        }
    }else{
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                ref[i][j]->hide();
            }
        }
    }
}

void MainWindow::setUI()
{
    if(isEditMode){
        this->ui->label_Floor->hide();
        this->ui->label_HP->hide();
        this->ui->widget_HP->hide();
        this->ui->widget_MP->hide();
        this->ui->label_Player_Name->hide();
        uiItem[23]->hide();uiItem[24]->hide();
        this->ui->label_ATK->hide();
        this->ui->label_DEF->hide();
        this->ui->label_Weapon->hide();
        this->ui->label_Shield->hide();
        this->ui->label_Accessory->hide();
        this->ui->label_Items->hide();
        for(int i = 0; i < 25; i++){
            if(i != 17 && i != 18){
               uiItem[i]->hide();
            }
        }
        for(int i = 0; i < 15; i++){
            inventory[i]->hide();
            if(i < 12){
               numberItem[i]->hide();
            }
        }
    }else{
        this->ui->label_Floor->setText(QString("The %1th Floor:            ").arg(floorNumber, 2, 10, QChar(' ')) + floorName);
        this->ui->label_Floor->show();
        this->ui->label_HP->setText(QString("%1").arg(player.hp, 10, 10, QChar(' ')));
        this->ui->label_HP->show();
        float f = player.hp * 184.0 / player.max_hp;
        this->ui->widget_HP->resize(f, 5);
        this->ui->widget_HP->show();
        this->ui->widget_MP->show();
        this->ui->label_Player_Name->setText(player.name);
        this->ui->label_Player_Name->show();
        uiItem[23]->show();uiItem[24]->show();
        this->showObject(inventory[12], player.item[12].id);
        this->showObject(inventory[13], player.item[13].id);
        this->showObject(inventory[14], player.item[14].id);
        this->ui->label_ATK->setText(QString("%1").arg(player.atk()));//, 10, 10, QChar(' ')
        this->ui->label_DEF->setText(QString("%1").arg(player.def()));
        this->ui->label_ATK->show();
        this->ui->label_DEF->show();
        this->ui->label_Weapon->show();
        this->ui->label_Shield->show();
        this->ui->label_Accessory->show();
        this->ui->label_Items->show();
        for(int i = 0; i < 17; i++){
            uiItem[i]->show();
            if(i < 15) {
                inventory[i]->show();
                this->showObject(inventory[i], player.item[i].id);
                if(i < 12) this->setNum(numberItem[i], inventory[i]->pos(), player.item_amount[i]);
            }
        }
    }
}

void MainWindow::setPlayer(int x, int y)
{
    player.x = x, player.y = y;token = 0;
    playerItem->move(object[x][y]->pos());
    playerItem->setCoordinates(x, y);
    playerItem->show();
}

void MainWindow::setPlayer(int objectId)
{
    for(int i = 1; i < 12; i++) {
        for(int j = 1; j < 12; j++) {
            if(objectMap[i][j] == objectId) {
                player.x = i, player.y = j;token = 0;
                playerItem->move(object[i][j]->pos());
                playerItem->setCoordinates(i, j);
                return;
            }
        }
    }
}

void MainWindow::selectItem(int x, int y)
{
    if(!hasControl) return;
    if(isEditMode) {
        if(mapRef[y][x-13] == 302){
            this->syncAnimations();
        }else if(mapRef[y][x-13] == 303){
            this->saveMap();
        }else if(mapRef[y][x-13] == 304){
            if(isEditMode){
                frame[2]->hide();
                frame[1]->hide();
                isEditMode = false;
            }else{
                frame[2]->show();
                frame[1]->move(ref[x-13][y]->pos());
                frame[1]->show();
                isEditMode = true;
            }
        }else if(mapRef[y][x-13] == 305){
            for(int i = 0; i < 8; i++){
                for(int j = 0; j < 8; j++){
                    mapRef[j][i] = mapRef1[j][i];
                }
            }
            this->setRef();
            frame[0]->hide();
            frame[1]->hide();
        }else if(mapRef[y][x-13] == 306){
            for(int i = 0; i < 8; i++){
                for(int j = 0; j < 8; j++){
                    mapRef[j][i] = mapRef2[j][i];
                }
            }
            this->setRef();
            frame[0]->hide();
            frame[1]->hide();
        }else if(mapRef[y][x-13] != selectedItemId){
            selectedItemId = mapRef[y][x-13];
            frame[1]->move(ref[x-13][y]->pos());
            frame[1]->show();
            frame[0]->show();
            if(selectedItemId == 307) { selectedItemId = 0; }
            else if(selectedItemId == 308) { selectedItemId = 31; frame[0]->hide(); }
            else if(selectedItemId == 309) { selectedItemId = 32; frame[0]->hide(); }
            else if(selectedItemId == 310) { selectedItemId = 39; frame[0]->hide(); }
        } else {
            dragItem->hide();
            this->releaseMouse();
            isDragging = false;
            selectedItemId = 0;
            frame[1]->hide();
            return;
        }
        if(ref[x-13][y]->isDragable()) {
            if(selectedItemId == 31 || selectedItemId == 32 || selectedItemId == 39){
                if(selectedItemId == 39){
                    dragItem->setAnimation(sourcePix[0]->copy(0*32, 9*32, 192, 32), 2, 300);
                }else if(selectedItemId == 31){
                    dragItem->setAnimation(sourcePix[0]->copy(0*32, 5*32, 192, 96), 2, 300);
                }else if(selectedItemId == 32){
                    dragItem->setAnimation(sourcePix[0]->copy(6*32, 5*32, 192, 96), 2, 300);
                }
            }else this->showObject(dragItem, selectedItemId);
            if(!isDragging) {
                isDragging = true;
                dragItem->move(mapFromGlobal(QPoint(cursor().pos().x()-16, cursor().pos().y()-16)));
                dragItem->show();
                this->grabMouse();
            }
        }
    }else {
        if(!isDragging && player.item[x].id != 0) {
            isDragging = true;
            this->grabMouse();
            dragItem->move(mapFromGlobal(QPoint(cursor().pos().x()-16, cursor().pos().y()-16)));
            this->showObject(dragItem, player.item[x].id);dragItem->show();
            selectedItemId = player.item[x].id;
            if(x == 14) {
                this->ui->label_HP->setText(QString("%1").arg(player.modHp(false), 10, 10, QChar(' ')));
                float f = player.hp * 184.0 / player.max_hp;
                this->ui->widget_HP->resize(f, 5);
            }
            player.item[x] = Items(0);
            this->showObject(inventory[x], 0);
            if(x < 12) {
                this->setNum(dragNum, dragItem->pos(), player.item_amount[x]);
                dragNum->show();
                player.item_amount[x] = 0;
                this->setNum(numberItem[x], inventory[x]->pos(), 0);
            } else {
                this->setNum(dragNum, dragItem->pos(), 1);
                this->ui->label_ATK->setText(QString("%1").arg(player.atk()));
                this->ui->label_DEF->setText(QString("%1").arg(player.def()));
            }
        } else {
            if(selectedItemId == player.item[x].id) {
                if(player.item[x].type == 0 || player.item[x].type == 4) {
                    player.item_amount[x] += dragNum->number();
                    if(player.item_amount[x] > 999) {
                        this->setNum(dragNum, dragItem->pos(), player.item_amount[x] - 999);
                        this->setNum(numberItem[x], inventory[x]->pos(), 999);
                        player.item_amount[x] = 999;
                        return;
                    }
                    this->setNum(numberItem[x], inventory[x]->pos(), player.item_amount[x]);
                    numberItem[x]->show();
                    dragItem->hide();this->showObject(dragItem, 0);
                    this->setNum(dragNum, dragItem->pos(), 0);
                    selectedItemId = 0;
                    this->releaseMouse();
                    isDragging = false;
                }
            } else {
                if(x < 12) {
                    int temp1 = selectedItemId;
                    int temp2 = dragNum->number();
                    if(player.item[x].id == 0) {
                        dragItem->hide();this->showObject(dragItem, 0);
                        this->setNum(dragNum, dragItem->pos(), 0);
                        selectedItemId = 0;
                        this->releaseMouse();
                        isDragging = false;
                    } else {
                        this->showObject(dragItem, player.item[x].id);
                        selectedItemId = player.item[x].id;
                        this->setNum(dragNum, dragItem->pos(), player.item_amount[x]);
                    }
                    player.item[x] = Items(temp1);
                    player.item_amount[x] = temp2;
                    this->showObject(inventory[x], temp1);
                    this->setNum(numberItem[x], inventory[x]->pos(), temp2);
                    numberItem[x]->show();
                } else if(x == 12 || x == 13 || x == 14) {
                    if(Items(selectedItemId).type == x-11) {
                        int temp = selectedItemId;
                        if(player.item[x].id == 0) {
                            dragItem->hide();this->showObject(dragItem, 0);
                            this->setNum(dragNum, dragItem->pos(), 0);
                            selectedItemId = 0;
                            this->releaseMouse();
                            isDragging = false;
                        } else {
                            this->showObject(dragItem, player.item[x].id);
                            selectedItemId = player.item[x].id;
                            this->setNum(dragNum, dragItem->pos(), 1);
                        }
                        player.item[x] = Items(temp);
                        this->showObject(inventory[x], temp);
                        this->ui->label_HP->setText(QString("%1").arg(player.modHp(true), 10, 10, QChar(' ')));
                        float f = player.hp * 184.0 / player.max_hp;
                        this->ui->widget_HP->resize(f, 5);
                        this->ui->label_ATK->setText(QString("%1").arg(player.atk()));
                        this->ui->label_DEF->setText(QString("%1").arg(player.def()));
                    }
                }
            }
        }
        this->x = x, this->y = y;
        this->showInfo(selectedItemId);
    }
}

void MainWindow::selectObject(int x, int y)
{
    notice(QString("(%1, %2)").arg(x).arg(y));

    if(!hasControl || isFighting) return;
    if(isEditMode && selectedItemId != 0){
        if(selectedItemId != objectMap[x][y] && selectedItemId > 0){
            if(objectMap[x][y] < 0 || objectMap[x][y] == 31 || objectMap[x][y] == 32 || objectMap[x][y] == 39){
                return;
            }
            if(selectedItemId == 39){
                if(objectMap[x+1][y]==0&&objectMap[x+2][y]==0){
                    object[x][y]->setAnimation(sourcePix[0]->copy(0*32, 9*32, 192, 32), 2, 300);
                    object[x+1][y]->hide();object[x+2][y]->hide();
                    objectMap[x+1][y]=-1;objectMap[x+2][y]=-2;
                }objectMap[x][y] = selectedItemId;
                return;
            }else if(selectedItemId == 31 || selectedItemId == 32){
                if(objectMap[x+1][y]==0&&objectMap[x+2][y]==0&&objectMap[x][y+1]==0&&objectMap[x+1][y+1]==0
                        &&objectMap[x+2][y+1]==0&&objectMap[x][y+2]==0&&objectMap[x+1][y+2]==0&&objectMap[x+2][y+2]==0){
                    object[x][y]->setAnimation(sourcePix[0]->copy((selectedItemId-31)*6*32, 5*32, 192, 96), 2, 300);
                    for(int n = 1; n < 9; n++){
                        object[x+x_pad[n]][y+y_pad[n]]->hide();
                        objectMap[x+x_pad[n]][y+y_pad[n]] = -n;
                    }
                }objectMap[x][y] = selectedItemId;
                return;
            }else if(selectedItemId == 206 && floorNumber == 0){
                return;
            }else if(selectedItemId == 207){
                if(!QFile::exists(QString("./resources/map/%1.floor").arg(floorNumber+1))){
                    QFile::copy("./resources/map/.map.floor", QString("./resources/map/%1.floor").arg(floorNumber+1));
                    maxFloorNum = floorNumber + 1;
                }
            }else if(selectedItemId == 301){
                if(objectMap[x][y] == 39){
                    object[x+1][y]->show();object[x+2][y]->show();
                    objectMap[x+1][y]=0;objectMap[x+2][y]=0;
                }else if(objectMap[x][y] == 31 || objectMap[x][y] == 32){
                    for(int n = 1; n < 9; n++){
                        object[x+x_pad[n]][y+y_pad[n]]->show();
                        objectMap[x+x_pad[n]][y+y_pad[n]] = 0;
                    }
                }
                this->showObject(object[x][y], 0);
                objectMap[x][y] = 0;
                return;
            }
            this->showObject(object[x][y], selectedItemId);
            objectMap[x][y] = selectedItemId;
        }
        return;
    }
    if(abs(x - player.x) > abs(y - player.y)) {
        if(x - player.x > 0) { token = 3; }
        else { token = 2; }
    } else {
        if(y - player.y < 0) { token = 1; }
        else { token = 0; }
    }
    if(objectMap[x][y] < 0){
        selectedObjectId = objectMap[x-x_pad[-objectMap[x][y]]][y-y_pad[-objectMap[x][y]]];
    }else selectedObjectId = objectMap[x][y];
    if(selectedObjectId == 0 || selectedObjectId == 206 || selectedObjectId == 207){
        if(player.x == x && player.y == y) {
            if(!isDragging) { this->showInfo(401);return; }
            int amount = dragNum->number();int i;
            for(i = 0; i < 12; ++i) {
                if(player.item[i].id == selectedItemId) {
                    if(player.item[i].type == 0 || player.item[i].type == 4) {
                        if(player.item_amount[i] != 999){
                            if(player.item_amount[i] + amount > 999) {
                                player.item_amount[i] = 999;
                                amount = player.item_amount[i] + amount - 999;
                                this->setNum(numberItem[i], inventory[i]->pos(), 999);
                            }else {
                                player.item_amount[i] += amount;
                                this->setNum(numberItem[i], inventory[i]->pos(), player.item_amount[i]);
                                break;
                            }
                        }
                    }
                }else if(player.item[i].id == 0) {
                    player.item[i] = Items(selectedItemId);
                    this->showObject(inventory[i], selectedItemId);
                    player.item_amount[i] = amount;
                    this->setNum(numberItem[i], inventory[i]->pos(), player.item_amount[i]);
                    break;
                }
            }
            if(i == 12) {
                this->notice("Your Inventory is full! Try drop something on the Ground.");
                this->setNum(dragNum, dragItem->pos(), amount);
            }else {
                dragItem->hide();this->showObject(dragItem, 0);
                this->setNum(dragNum, dragItem->pos(), 0);
                selectedItemId = 0;
                this->releaseMouse();
                isDragging = false;
            }
        }else {
            isWalking = false;
            this->aiWalk(x, y);
        }
    }else if(selectedObjectId > 100 && selectedObjectId < 200) {
        if((abs(player.x - x) + abs(player.y - y)) == 1) { this->grabItem(); }
    }else {
        this->showInfo(selectedObjectId);
        playerItem->setPix(QPixmap(sourcePix[0]->copy(token*3*32, 10*32, 32, 32)));
    }
}

void MainWindow::selectDiff(int x, int y)
{
    if(!hasControl) return;
    if(isEditMode){
        if(isDragging) {
            dragItem->hide();this->showObject(dragItem, 0);
            selectedItemId = 0;
            this->releaseMouse();
            isDragging = false;
            frame[1]->hide();
        } else {
            if(objectMap[x][y] == 39){
                object[x+1][y]->show();object[x+2][y]->show();
                objectMap[x+1][y]=0;objectMap[x+2][y]=0;
            }else if(objectMap[x][y] == 31 || objectMap[x][y] == 32){
                for(int n = 1; n < 9; n++){
                    object[x+x_pad[n]][y+y_pad[n]]->show();
                    objectMap[x+x_pad[n]][y+y_pad[n]] = 0;
                }
            }
            this->showObject(object[x][y], 0);
            objectMap[x][y] = 0;
        }
        return;
    }
    if(y < 13) {
        if(isWalking || isFighting) return;
        if(abs(x - player.x) > abs(y - player.y)) {
            if(x - player.x > 0) { token = 3; }
            else { token = 2; }
        } else {
            if(y - player.y < 0) { token = 1; }
            else { token = 0; }
        } playerItem->setPix(QPixmap(sourcePix[0]->copy(token*3*32, 10*32, 32, 32)));
        if(objectMap[x][y] < 0){
            selectedObjectId = objectMap[x-x_pad[-objectMap[x][y]]][y-y_pad[-objectMap[x][y]]];
        }else selectedObjectId = objectMap[x][y];
        if((abs(player.x - x) + abs(player.y - y)) == 1) {
            if(selectedObjectId == 0) {
                this->throwItem();
            }else if(selectedObjectId > 0 && selectedObjectId < 100) {
                if(isDragging) {
                    this->interact();return;
                }
                if(eventTrigger(2)) return;
                this->fight(selectedObjectId);
            }else if(selectedObjectId > 100 && selectedObjectId < 200) {
                this->grabItem();
            }else if(selectedObjectId > 200 && selectedObjectId < 300) {
                this->interact();
            }
        }else if(player.x == x && player.y == y) {
            if(Items(selectedItemId).type == 4) {//player use item
                Items item = Items(selectedItemId);
                if(item.buff.hp != 0) player.setHp(item.buff.hp);
                else if(item.buff.atk != 0) player.base_atk += item.buff.atk;
                else if(item.buff.def != 0) player.base_def += item.buff.def;
                else return;
                this->setNum(dragNum, dragItem->pos(), dragNum->number()-1);
                if(dragNum->number() == 0) {
                    dragItem->hide();this->showObject(dragItem, 0);
                    selectedItemId = 0;
                    this->releaseMouse();
                    isDragging = false;
                }
                this->ui->label_HP->setText(QString("%1").arg(player.hp, 10, 10, QChar(' ')));
                float f = player.hp * 184.0 / player.max_hp;
                this->ui->widget_HP->resize(f, 5);
                this->ui->label_ATK->setText(QString("%1").arg(player.atk()));
                this->ui->label_DEF->setText(QString("%1").arg(player.def()));
            }
        }
    } else {
        if(!isDragging && player.item[x].id != 0) {
            isDragging = true;
            this->grabMouse();
            dragItem->move(mapFromGlobal(QPoint(cursor().pos().x()-16, cursor().pos().y()-16)));
            this->showObject(dragItem, player.item[x].id);dragItem->show();
            selectedItemId = player.item[x].id;
            int num;
            if(selectedItemId == 137) {
                num = player.item_amount[x] - 10 > 0 ? 10 : player.item_amount[x];
            }else num = 1;
            this->setNum(dragNum, dragItem->pos(), num);
            if(x < 12) {
                player.item_amount[x] -= num;
                if(player.item_amount[x] == 0) {
                    this->setNum(numberItem[x], inventory[x]->pos(), 0);
                } else {
                    this->setNum(numberItem[x], inventory[x]->pos(), player.item_amount[x]);
                    return;
                }
            }
            player.item[x] = Items(0);
            this->showObject(inventory[x], 0);
        } else {
            if(selectedItemId == player.item[x].id && x < 12) {
                int num;
                if(selectedItemId == 137) {
                    num = player.item_amount[x] - 10 > 0 ? 10 : player.item_amount[x];
                }else num = 1;
                this->setNum(dragNum, dragItem->pos(), dragNum->number()+num);
                player.item_amount[x] -= num;
                if(player.item_amount[x] == 0) {
                    this->setNum(numberItem[x], inventory[x]->pos(), 0);
                }else {
                    this->setNum(numberItem[x], inventory[x]->pos(), player.item_amount[x]);
                    return;
                }
                player.item[x] = Items(0);
                this->showObject(inventory[x], 0);
            } else this->selectItem(x, y);
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(!hasControl) return;
    if(isDragging && QRect(1, 24, 736, 456).contains(event->pos())) {
        QPoint pt = mapTo(this, event->pos());
        QWidget * w = this->childAt(pt);
        if(w->isEnabled()) {
            pt = w->mapFrom(this, pt);
            QMouseEvent * event_ = new QMouseEvent(event->type(), pt, event->button(), event->buttons(), event->modifiers());
            QApplication::postEvent(w, event_);
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent * event)
{
    if(isDragging && QRect(1, 24, 736, 456).contains(event->pos())){
        dragItem->move(event->pos().x()-16, event->pos().y()-16);
        dragNum->move(dragItem->pos().x()+dragNum->dx, dragItem->pos().y()+24);
        QWidget * w = this->childAt(event->pos());
        if(w->isEnabled()) {
            frame[0]->setGeometry(w->geometry());
            frame[0]->show();
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    if(!hasControl) return;
    if(!event->isAutoRepeat()){
        key = event->key();
        int nx = player.x+x_token[token], ny = player.y+y_token[token];
        int objectId = objectMap[nx][ny];
        if(objectId < 0) { objectId = objectMap[nx-x_pad[-objectId]][ny-y_pad[-objectId]]; }
        if(isWalking || isFighting) return;
        if(key == Qt::Key_Up || key == Qt::Key_W){
            token=1;this->playerWalk();
        }else if(key == Qt::Key_Left || key == Qt::Key_A){
            token=2;this->playerWalk();
        }else if(key == Qt::Key_Down || key == Qt::Key_S){
            token=0;this->playerWalk();
        }else if(key == Qt::Key_Right || key == Qt::Key_D){
            token=3;this->playerWalk();
        }else if(key == Qt::Key_E || key == Qt::Key_0){// show hint ##
            if(!isPressed) {
                isPressed = true;
                if(objectId > 0 && objectId < 100){
                    this->notice(Monsters(objectId).desc);
                }else if(objectId > 100 && objectId < 200) {
                    int amount = 1;
                    bool isCoin = false;
                    if(Items(objectId).type == 0) {
                        amount = numMap.value(Grid{nx, ny})->number();
                        isCoin = true;
                    }
                    int i;
                    for(i = 0; i < 12; ++i) {
                        if(player.item[i].id == objectId) {
                            if(player.item[i].type == 0 || player.item[i].type == 4) {
                                if(player.item_amount[i] != 999){
                                    if(player.item_amount[i] + amount > 999) {
                                        player.item_amount[i] = 999;
                                        amount = player.item_amount[i] + amount - 999;
                                        this->setNum(numberItem[i], inventory[i]->pos(), 999);
                                    }else {
                                        player.item_amount[i] += amount;
                                        this->setNum(numberItem[i], inventory[i]->pos(), player.item_amount[i]);
                                        break;
                                    }
                                }
                            }
                        }else if(player.item[i].id == 0) {
                            player.item[i] = Items(objectId);
                            this->showObject(inventory[i], objectId);
                            player.item_amount[i] = amount;
                            this->setNum(numberItem[i], inventory[i]->pos(), player.item_amount[i]);
                            break;
                        }
                    }
                    if(i == 12) {
                        this->notice("Your Inventory is full! Try drop something on the Ground.");
                        if(isCoin) {
                            this->setNum(numMap.value(Grid{nx, ny}), object[nx][ny]->pos(), amount);
                        }
                    }else {
                        this->showObject(object[nx][ny], 0);
                        objectMap[nx][ny] = 0;
                        if(isCoin) {
                            delete numMap.value(Grid{nx, ny});
                            numMap.remove(Grid{nx, ny});
                        }
                    }
                } else if(objectId > 200 && objectId < 300) {
                    this->notice(Object(objectId).desc);
                }
                isPressed = false;
            }
        }else if(key == Qt::Key_F) {
            if(isFighting) {
                isFighting = false;
                hpBar->hide();
            }else if(objectId > 0 && objectId < 100) { this->fight(objectId); }
        }else if(key == Qt::Key_N) {
            notice(QString("(%1, %2)").arg(eventList.front().x).arg(eventList.front().y));
            this->grabMouse();this->setFocus();
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat() && event->key() == key
            && event->key() != Qt::Key_0 && event->key() != Qt::Key_E){
        isPressed = false;
    }
}

void MainWindow::grabItem()
{
    int nx = player.x+x_token[token], ny = player.y+y_token[token];
    if(selectedObjectId == 0 || (selectedItemId != 0 && selectedObjectId != selectedItemId)) return;
    int num = 1;
    if(selectedObjectId == 137) {
        num = numMap.value(Grid{nx, ny})->number();
        delete numMap.value(Grid{nx, ny});
        numMap.remove(Grid{nx, ny});
    }
    if(isDragging) {
        int type = Items(selectedItemId).type;
        if(type == 0 || type == 4) {
            this->setNum(dragNum, dragItem->pos(), dragNum->number()+num);
        }else return;
    }
    else {
        isDragging = true;
        this->grabMouse();
        dragItem->move(mapFromGlobal(QPoint(cursor().pos().x()-16, cursor().pos().y()-16)));
        this->showObject(dragItem, selectedObjectId);dragItem->show();
        selectedItemId = selectedObjectId;
        this->setNum(dragNum, dragItem->pos(), num);dragNum->show();
    }
    this->showObject(object[nx][ny], 0);
    objectMap[nx][ny] = 0;
    this->showInfo(selectedObjectId);
}

void MainWindow::throwItem()
{
    int nx = player.x+x_token[token], ny = player.y+y_token[token];
    if(selectedObjectId != 0 && selectedObjectId != 137) return;
    if(selectedObjectId == selectedItemId && selectedItemId == 137) {//add to the PILE
        int total = numMap.value(Grid{nx, ny})->number() + dragNum->number();
        if(total > 999) {
            this->setNum(dragNum, dragItem->pos(), 999 - numMap.value(Grid{nx, ny})->number());
            this->setNum(numMap.value(Grid{nx, ny}), object[nx][ny]->pos(), 999);
            return;
        }else this->setNum(numMap.value(Grid{nx, ny}), object[nx][ny]->pos(), total);
    }else if(selectedObjectId == 0){
        this->showObject(object[nx][ny], selectedItemId);
        objectMap[nx][ny] = selectedItemId;
        if(selectedItemId == 137) {
            NumberItem * numItem = new NumberItem(this);
            numMap.insert(Grid{nx, ny}, numItem);
            numItem->setAttribute(Qt::WA_TransparentForMouseEvents, true);
            numItem->setPix(QPixmap("./resources/pic/number_4.png"));
            this->setNum(numItem, object[nx][ny]->pos(), dragNum->number());numItem->show();
        }else {
            this->setNum(dragNum, dragItem->pos(), dragNum->number()-1);
            if(dragNum->number() != 0) return;
        }
    }
    this->setNum(dragNum, dragItem->pos(), 0);
    dragItem->hide();this->showObject(dragItem, 0);
    selectedItemId = 0;
    this->releaseMouse();
    isDragging = false;
    this->showInfo(0);
}

void MainWindow::interact()
{
    int nx = player.x+x_token[token], ny = player.y+y_token[token];
    int id = selectedObjectId;
    if(id < 100){ if(selectedItemId == 120) {
        notice(player.damage(id));}
    }else if(id < 204) { if(selectedItemId == id-76 || selectedItemId == 130) {
        this->endDrag();
        this->showAnimation(object[nx][ny], id-201);
        objectMap[nx][ny] = 0;}
    }else if(id == 208) { if(selectedItemId == 113 || selectedItemId == 114) {
        this->showAnimation(object[nx][ny], 5);
        objectMap[nx][ny] = 0;}
        if(selectedItemId == 116) {
            this->endDrag();
            this->showAnimation(object[nx][ny], 5);
            objectMap[nx][ny] = 0;}
    }else if(id == 209) {if(selectedItemId == 114) {
        this->showAnimation(object[nx][ny], 6);
        objectMap[nx][ny] = 0;}
        if(selectedItemId == 116) {
            this->endDrag();
            this->showAnimation(object[nx][ny], 6);
            objectMap[nx][ny] = 0;}
    }else if(id == 210) {if(selectedItemId == 116) {
        this->endDrag();
        this->showAnimation(object[nx][ny], 7);
        objectMap[nx][ny] = 0;}
    }else if(id == 211) {if(selectedItemId == 119) {
        this->showAnimation(object[nx][ny], 8);
        objectMap[nx][ny] = 0;}
    }else if(id == 212){
        //box
    }else if(id == 213){if(selectedItemId == 128 || selectedItemId == 130) {
        this->endDrag();
        this->showAnimation(object[nx][ny], 10);
        objectMap[nx][ny] = 0;}
    }else if(id == 214){if(selectedItemId == 129) {
        this->endDrag();
        this->showAnimation(object[nx][ny], 11);
        objectMap[nx][ny] = 0;
        }else if(selectedItemId == 130) {
            this->showAnimation(object[nx][ny], 11);
            objectMap[nx][ny] = 0;}
    }else if(id == 215){if(selectedItemId == 125 || selectedItemId == 130) {
        this->endDrag();
        this->showAnimation(object[nx][ny], 12);
        objectMap[nx][ny] = 0;}
    }else if(id == 216){if(selectedItemId == 126 || selectedItemId == 130) {
        this->endDrag();
        this->showAnimation(object[nx][ny], 13);
        objectMap[nx][ny] = 0;}
    }else if(id == 217){if(selectedItemId == 127 || selectedItemId == 130) {
        this->endDrag();
        this->showAnimation(object[nx][ny], 14);
        objectMap[nx][ny] = 0;}
    }else if(id == 218){if(selectedItemId == 128 || selectedItemId == 130) {
        this->endDrag();
        this->showAnimation(object[nx][ny], 15);
        objectMap[nx][ny] = 0;}
    }else if(id == 251) {
        if(selectedItemId == 117) {
            this->showAnimation(object[nx][ny], 9);
            this->showAnimation(object[nx][ny], 8, 211, true);
            objectMap[nx][ny] = 0;
        }else if(selectedItemId == 120) {
            this->showAnimation(object[nx][ny], 9);
            objectMap[nx][ny] = 0;}
    }else if(id == 256) {
        this->showAnimation(object[nx][ny], 5);
        objectMap[nx][ny] = 0;
    }else if(id == 257) {if(selectedItemId == 119) {
            this->showAnimation(object[nx][ny], 5, 208, true);
            objectMap[nx][ny] = 208;}
        }else return;
}

void MainWindow::endDrag()
{
    this->setNum(dragNum, dragItem->pos(), dragNum->number()-1);
    if(dragNum->number() == 0) {
        dragItem->hide();this->showObject(dragItem, 0);dragItem->move(0, 0);
        selectedItemId = 0;
        this->releaseMouse();
        isDragging = false;
    }
}

void MainWindow::fight(int id)
{
    int nx = player.x+x_token[token], ny = player.y+y_token[token];
    if(id==31||id==32||id==39) {return;}
    isFighting = true;
    QPoint pt = object[nx][ny]->pos();
    Monsters mon = Monsters(id);
    hpBar->resize(32, 3);
    hpBar->move(pt.x(), pt.y()-3);hpBar->show();
    effect[1]->setFlexablePix(QPixmap("./resources/pic/effect.png").copy(0, 0, 32, 32));
    while(mon.hp > 0 && player.hp > 0 && isFighting) {
        effect[1]->move(pt);effect[1]->show();
        mon.hp -= player.atk() > mon.def ? player.atk() - mon.def : 0;
        if(mon.hp < 0) { mon.hp = 0; }
        double f = mon.hp * 32.0 / mon.max_hp;
        hpBar->resize(f, 3);
        Delay_MSec(200);
        effect[1]->hide();
        if(mon.hp > 0) {
            Delay_MSec(200);
            effect[1]->move(playerItem->pos());effect[1]->show();
            player.setHp(mon.atk > player.def() ? player.def() - mon.atk : 0);
            if(player.hp < 0) { player.hp = 0; }
            this->ui->label_HP->setText(QString("%1").arg(player.hp, 10, 10, QChar(' ')));
            f = player.hp * 184.0 / player.max_hp;
            this->ui->widget_HP->resize(f, 5);
            Delay_MSec(200);
            effect[1]->hide();
        }else break;
        Delay_MSec(200);
    }
    effect[1]->setFlexablePix(sourcePix[0]->copy(9*32, 15*32, 32, 32));
    hpBar->hide();
    if(mon.hp <= 0) {
        this->showAnimation(object[nx][ny], 16, 137);
        objectMap[nx][ny] = 137;
        NumberItem * numItem = new NumberItem(this);
        numMap.insert(Grid{nx, ny}, numItem);
        numItem->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        numItem->setPix(QPixmap("./resources/pic/number_4.png"));
        this->setNum(numItem, object[nx][ny]->pos(), mon.coin);numItem->show();
        isFighting = false;
    }else if(player.hp == 0){
        showAnimation(playerItem, 16, 0);
        QMessageBox::information(this, tr("You Lose..."),
                                        tr("You are Dead. Click Ok to start a new game."),
                                        QMessageBox::Ok);
        on_actionNew_Game_triggered();
        isFighting = false;
    }
}

void MainWindow::gridDiaplay(int x, int y)
{
    if(y == 13) {
        frame[0]->move(inventory[x]->pos());
        frame[0]->show();
    }else if(x < 13){
        frame[0]->setGeometry(object[x][y]->pos().x(), object[x][y]->pos().y(), object[x][y]->getWidth(), object[x][y]->getHeight());
        frame[0]->show();
    }else if(x >= 13){
        if(isEditMode){
            frame[0]->move(ref[x-13][y]->pos());
            frame[0]->show();
        }
    }
}

void MainWindow::showObject(AnimatedItem *item, int id)
{
    if(id == 0) {
        item->setPix(sourcePix[0]->copy(9*32, 15*32, 32, 32));
    }else if(id < 100 && id > 0){
        if(id <= 30){
            item->setAnimation(sourcePix[0]->copy((id-1)*2%12*32, (id-1)/6*32, 64, 32), 2, 300);
        }else if(id > 32 && id != 39){
            item->setAnimation(sourcePix[0]->copy((id-33)*2*32, 8*32, 64, 32), 2, 300);
        }
    }else if(id > 100 && id < 200){
        item->setPix(sourcePix[0]->copy((id-101)%12*32, (id+31)/12*32, 32, 32));
        if(id == 137) {
            item->setPix(QPixmap("./resources/pic/coin.png"));
        }
    }else if(id > 200 && id < 300){
        if(id <= 212){
            item->setPix(sourcePix[0]->copy((id-201)*32, 14*32, 32, 32));
        }else if(id > 212 && id < 250){
            item->setPix(sourcePix[1]->copy((id-213)*32, 0*32, 32, 32));
        }else if(id > 250 && id < 256){
            item->setAnimation(sourcePix[0]->copy((id-251)*2*32, 15*32, 64, 32), 2, 900);
        }else if(id == 256) {
            item->setPix(sourcePix[0]->copy(7*32, 14*32, 32, 32));
        }
    }else if(id > 300 && id < 308){
        item->setPix(sourcePix[3]->copy((id-301)*32, 0*32, 32, 32));
        item->setDragable(false);return;
    }else if(id == 310){
        item->setPix(sourcePix[0]->copy(1*32, 9*32, 32, 32));return;
    }else if(id == 308){
        item->setPix(sourcePix[0]->copy(1*32, 7*32, 32, 32));return;
    }else if(id == 309){
        item->setPix(sourcePix[0]->copy(7*32, 7*32, 32, 32));return;
    }else if(id ==401) {
        item->setAnimation(QPixmap(sourcePix[0]->copy(1*32, 10*32, 64, 32)), 2, 300);return;
    }
    else{
        item->setPix(sourcePix[0]->copy(9*32, 15*32, 32, 32));return;
    }
}

void MainWindow::showAnimation(AnimatedItem *item, int AnimeId, int objId, bool isReverse)
{
    if(AnimeId < 16) {
        item->setAnimation(sourcePix[2]->copy(0, AnimeId*32, 288, 32), 9, 40, false, isReverse);
        Delay_MSec(350);
        this->showObject(item, objId);
        objectMap[item->getX()][item->getY()] = objId;
    }else if(AnimeId == 16) {
        effect[0]->move(item->pos());effect[0]->show();
        if(isReverse) {
            this->showObject(item, objId);
            objectMap[item->getX()][item->getY()] = objId;
        }
        effect[0]->setAnimation(sourcePix[2]->copy(0, 16*32, 288, 32), 9, 80, false, isReverse);
        Delay_MSec(720);
        if(!isReverse) {
            this->showObject(item, objId);
            objectMap[item->getX()][item->getY()] = objId;
        }
        effect[0]->setFlexablePix(sourcePix[0]->copy(9*32, 15*32, 32, 32));effect[0]->hide();
    }
}

void MainWindow::showInfo(int id)
{
    this->ui->label_Object_Name->setText("Anonymous");
    this->ui->label_Object_Name->move(473, 355);
    this->ui->label_Object_Description->setText("");
    uiItem[19]->hide();uiItem[20]->hide();uiItem[21]->hide();uiItem[22]->hide();
    uiItem[19]->setPix(QPixmap("./resources/pic/value_icons.png").copy(0, 0, 20, 20));
    uiItem[20]->setPix(QPixmap("./resources/pic/value_icons.png").copy(20, 0, 20, 20));
    uiItem[21]->setPix(QPixmap("./resources/pic/value_icons.png").copy(40, 0, 20, 20));
    uiItem[22]->setPix(QPixmap("./resources/pic/value_icons.png").copy(60, 0, 20, 20));
    this->ui->label_Object_Value_1->setText("");
    this->ui->label_Object_Value_2->setText("");
    this->ui->label_Object_Value_3->setText("");
    this->ui->label_Object_Value_4->setText("");
    this->ui->label_Enchantment->hide();
    this->ui->label_Object_Enchantment->hide();
    if(id == 0) {
        this->ui->label_Object_Name->setText("Nothing");
        this->ui->label_Object_Name->move(479, 355);
        this->ui->label_Object_Description->setText("None.");
    }else if(id < 100 && id > 0){
        Monsters m(id);
        this->ui->label_Object_Name->setText(m.name);
        this->ui->label_Object_Name->move(500 - m.name.length()*3, 355);
        uiItem[19]->show();uiItem[20]->show();uiItem[21]->show();uiItem[22]->show();
        this->ui->label_Object_Value_1->setText(QString("%1").arg(m.hp));//, 9, 10, QChar(' ')
        this->ui->label_Object_Value_2->setText(QString("%1").arg(m.atk));
        this->ui->label_Object_Value_3->setText(QString("%1").arg(m.def));
        this->ui->label_Object_Value_4->setText(QString("%1").arg(m.coin));
        this->ui->label_Object_Description->setText(m.desc.isNull() ? "None." : m.desc);
        this->ui->label_Enchantment->setText("Skill:");
        this->ui->label_Enchantment->show();
        this->ui->label_Object_Enchantment->setText(m.skill.isNull() ? "None." : m.skill);
        this->ui->label_Object_Enchantment->show();
        if(id == 31 || id == 32) { id += 277; }
        else if(id == 39) { id = 310; }
    }else if(id < 200 && id > 100){
        this->ui->label_Enchantment->hide();
        this->ui->label_Object_Enchantment->hide();
        Items item(id);
        this->ui->label_Object_Name->setText(item.name);
        this->ui->label_Object_Name->move(500 - item.name.length()*3, 355);
        if(item.type > 0 && item.type <= 4){
            this->ui->label_Enchantment->show();
            this->ui->label_Object_Enchantment->show();
            int n = 0;
            QString str = "";
            if(item.buff.hp != 0) {//, 8, QChar(' ')
                str.setNum(item.buff.hp);n = 1;
                if(item.type == 3) str.insert(1, "%");
                else str.insert(0, '+');
                this->ui->label_Object_Value_1->setText(str);
                uiItem[19]->setPix(QPixmap("./resources/pic/value_icons.png").copy(0, 0, 20, 20));uiItem[19]->show();
            }
            if(item.buff.mp != 0) {
                str.setNum(item.buff.mp);str.insert(0, '+');
            }
            if(item.buff.atk != 0) {
                str.setNum(item.buff.atk);
                if(item.buff.atk > 0) str.insert(0, '+');
                if(n == 0) {
                    this->ui->label_Object_Value_1->setText(str);n = 1;
                }else {
                    this->ui->label_Object_Value_2->setText(str);n = 2;
                }
                uiItem[18+n]->setPix(QPixmap("./resources/pic/value_icons.png").copy(20, 0, 20, 20));uiItem[18+n]->show();
            }
            if(item.buff.def != 0) {
                str.setNum(item.buff.def);
                if(item.buff.def > 0) str.insert(0, '+');
                if(n == 0) {
                    this->ui->label_Object_Value_1->setText(str);n = 1;
                }else if(n == 1) {
                    this->ui->label_Object_Value_2->setText(str);n = 2;
                }else {
                    this->ui->label_Object_Value_3->setText(str);n = 3;
                }
                uiItem[18+n]->setPix(QPixmap("./resources/pic/value_icons.png").copy(40, 0, 20, 20));uiItem[18+n]->show();
            }
            this->ui->label_Object_Enchantment->setText(item.enchantment.isNull() ? "None." : item.enchantment);
        }
        this->ui->label_Object_Description->setText(item.desc.isNull() ? "None." : item.desc);
    }else if(id < 300 && id > 200) {
        this->ui->label_Enchantment->hide();
        this->ui->label_Object_Enchantment->hide();
        Object obj(id);
        this->ui->label_Object_Name->setText(obj.name);
        this->ui->label_Object_Name->move(500 - obj.name.length()*3, 355);
        this->ui->label_Object_Description->setText(obj.desc.isNull() ? "None." : obj.desc);
    }else if(id == 401) {
        this->ui->label_Object_Name->setText(player.name);
        this->ui->label_Object_Name->move(500 - player.name.length()*3, 355);
        uiItem[19]->show();uiItem[20]->show();uiItem[21]->show();
        this->ui->label_Object_Value_1->setText(QString("%1").arg(player.max_hp));//, 9, 10, QChar(' ')
        this->ui->label_Object_Value_2->setText(QString("%1").arg(player.base_atk));
        this->ui->label_Object_Value_3->setText(QString("%1").arg(player.base_def));
        this->ui->label_Object_Description->setText("Savior of the World. He came here on a Great Mission to save the Princess.");
        this->ui->label_Enchantment->setText("Skill:");
        this->ui->label_Enchantment->show();
        this->ui->label_Object_Enchantment->setText("The Protagonist: Move with his free will.");
        this->ui->label_Object_Enchantment->show();
    }
    this->showObject(uiItem[18], id);
}

void MainWindow::aiWalk(int x, int y)
{
    SquareGrid grid(13, 13);
    for(int i = 0; i < 13; ++i){
        for(int j = 0; j < 13; ++j){
            if(objectMap[i][j] != 0 && (i != x || j != y)){
                grid.add_wall(i, j);
            }
        }
    }
    Grid start{player.x, player.y};
    Grid goal{x, y};
    map<Grid, Grid> came_from;
    map<Grid, double> cost_so_far;
    if(a_star_search(grid, start, goal, came_from, cost_so_far)) {
        vector<Grid> path = reconstruct_path(start, goal, came_from);
        isWalking = true;
        short temp = floorNumber;
        while(!path.empty() && isWalking){
            Grid next = path.back();
            path.pop_back();
            if(next.x == player.x + 1) { token = 3; }
            else if(next.x == player.x - 1) { token = 2; }
            else if(next.y == player.y + 1) { token = 0; }
            else if(next.y == player.y - 1) { token = 1; }
            player.x = next.x;
            player.y = next.y;
            playerItem->move(MARGIN + player.x*32, MARGIN + H_MENU + player.y*32);
            playerItem->setCoordinates(player.x, player.y);
            if(pace == 1){
                playerItem->setPix(QPixmap(sourcePix[0]->copy((token*3+1)*32, 10*32, 32, 32)));
                pace = 0;
            }else if(pace == 0){
                playerItem->setPix(QPixmap(sourcePix[0]->copy((token*3+2)*32, 10*32, 32, 32)));
                pace = 1;
            }
            Delay_MSec(200);
            if(this->eventTrigger(1)) {
                isWalking = false;break;
            }
            if(temp == floorNumber){
                if(objectMap[player.x][player.y] == 206){
                    isPressed = false;
                    this->autoSave();
                    this->setFloor(--floorNumber);this->setPlayer(207);
                    break;
                }else if(objectMap[player.x][player.y] == 207){
                    isPressed = false;
                    this->autoSave();
                    this->setFloor(++floorNumber);this->setPlayer(206);
                    break;
                }
            }
        }
        playerItem->setPix(QPixmap(sourcePix[0]->copy(token*3*32, 10*32, 32, 32)));
        isWalking = false;
    }
}

void MainWindow::playerWalk()
{
    isPressed = true;
    short temp = floorNumber;
    while(isPressed){
        if(objectMap[player.x+x_token[token]][player.y+y_token[token]] == 0
                || objectMap[player.x+x_token[token]][player.y+y_token[token]] == 206 || objectMap[player.x+x_token[token]][player.y+y_token[token]] == 207){
            player.x += x_token[token];
            player.y += y_token[token];
            playerItem->move(MARGIN + player.x*32, MARGIN + H_MENU + player.y*32);
            playerItem->setCoordinates(player.x, player.y);
            if(pace == 1){
                playerItem->setPix(QPixmap(sourcePix[0]->copy((token*3+1)*32, 10*32, 32, 32)));
                pace = 0;
            }else if(pace == 0){
                playerItem->setPix(QPixmap(sourcePix[0]->copy((token*3+2)*32, 10*32, 32, 32)));
                pace = 1;
            }
            Delay_MSec(200);
            if(this->eventTrigger(1)) return;
            if(temp == floorNumber){
                if(objectMap[player.x][player.y] == 206){
                    isPressed = false;
                    this->autoSave();
                    this->setFloor(--floorNumber);this->setPlayer(207);
                    return;
                }else if(objectMap[player.x][player.y] == 207){
                    isPressed = false;
                    this->autoSave();
                    this->setFloor(++floorNumber);this->setPlayer(206);
                    return;
                }
            }
        }else{
            break;
        }
    }
    playerItem->setPix(QPixmap(sourcePix[0]->copy(token*3*32, 10*32, 32, 32)));
}

void MainWindow::autoSave()
{
    QFile f(QString("./resources/map/%1.autosave").arg(floorNumber));
    if(f.open(QFile::WriteOnly | QIODevice::Text)){
        QTextStream ts(&f);
        for(int i = 0; i < 13; i++){
            for(int j = 0; j < 13; j++){
                if(j < 12){
                    if(objectMap[j][i] == 137) objectMap[j][i] = 0;
                    ts << objectMap[j][i] << '\t';
                }else ts << objectMap[j][i] << '\n';
            }
        }
        ts << floorName << '\n';
        while(!eventList.isEmpty()) {
            ts << eventList.front().flag << '\t' << eventList.front().type << '\t' << eventList.front().x << '\t'
               << eventList.front().y << '\t' << eventList.front().id << '\n';
            eventList.pop_front();
        }
        f.close();
    }
}

void MainWindow::saveGame()
{
    int slot;
    for(slot = 1; slot < 11; ++slot) {
        if(saveActions->actions().at(slot-1)->isChecked()) break;
    }
    if(QFile::exists(QString("./resources/save/%1/0.floor").arg(slot, 2, 10, QChar('0')))){
        int ret = QMessageBox::warning(this, tr("Warning"),
                                       tr("Do you really want to overwrite this save slot?"),
                                       QMessageBox::Save | QMessageBox::Cancel,
                                       QMessageBox::Save);
        if(ret != QMessageBox::Save) return;
    }
    this->autoSave();
    QFile f(QString("./resources/save/%1/profile.ini").arg(slot, 2, 10, QChar('0')));
    if(f.open(QFile::WriteOnly | QIODevice::Text)){
        QTextStream ts(&f);
        ts << floorNumber << '\t' << player.x << '\t' << player.y << '\t' << player.hp << '\t'
           << player.max_hp << '\t' << player.base_atk << '\t' << player.base_def << '\n';
        for(int i = 0; i < 12; ++i) {
            ts << player.item[i].id << '\t' << player.item_amount[i] << '\n';
        }
        ts << player.item[12].id << '\t' << player.item[13].id << '\t' << player.item[14].id;
        f.close();
    }
    for(int i = 0; i < 50; i++) {
        QFile f(QString("./resources/save/%1/%2.floor").arg(slot, 2, 10, QChar('0')).arg(i));
        if(f.exists()) {
            f.remove();
        }
        QString fileName1 = QString("./resources/map/%1.autosave").arg(i);
        QString fileName2 = QString("./resources/save/%1/%2.floor").arg(slot, 2, 10, QChar('0')).arg(i);
        QString fileName3 = QString("./resources/save/%1/%2.floor").arg(saveSlot, 2, 10, QChar('0')).arg(i);
        if(QFile::exists(fileName1)) {
            QFile::copy(fileName1, fileName2);
        }else if(QFile::exists(fileName3) && saveSlot != 0) {
            QFile::copy(fileName3, fileName2);
        }
    }
    if(loadActions->checkedAction() != 0) loadActions->checkedAction()->setChecked(false);
    loadActions->actions().at(slot-1)->setEnabled(true);
    for(int i = 0; i < maxFloorNum; ++i) {
        QFile f(QString("./resources/map/%1.autosave").arg(i));
        if(f.exists()) f.remove();
    }
    saveSlot = slot;
    f.setFileName("./resources/save/save.ini");
    if(f.open(QFile::WriteOnly | QIODevice::Text)){
        QTextStream ts(&f);
        ts << saveSlot;
        f.close();
    }
}

void MainWindow::loadGame()
{
    for(int i = 0; i < maxFloorNum; ++i) {
        QFile f(QString("./resources/map/%1.autosave").arg(i));
        if(f.exists()) f.remove();
    }
    int slot;
    for(slot = 1; slot < 11; ++slot) {
        if(loadActions->actions().at(slot-1)->isChecked()) break;
    } if(slot == 11) { slot = 0; }
    this->saveSlot = slot;
    QFile f("./resources/save/save.ini");
    if(f.open(QFile::WriteOnly | QIODevice::Text)){
        QTextStream ts(&f);
        ts << saveSlot;
        f.close();
    }
    if(saveSlot == 0) {
        this->setFloor(0);this->setPlayer(6, 11);
    }else {
        QFile f(QString("./resources/save/%1/profile.ini").arg(saveSlot, 2, 10, QChar('0')));
        if(f.open(QFile::ReadOnly)){
            QTextStream ts(&f);
            ts >> floorNumber >> player.x >> player.y >> player.hp >> player.max_hp >> player.base_atk >> player.base_def;
            for(int i = 0; i < 12; ++i) {
                int id, num;ts >> id >> num;
                player.item[i] = Items(id);player.item_amount[i] = num;
            }
            int id[3];ts >> id[0] >> id[1] >> id[2];
            player.item[12] = Items(id[0]);player.item[13] = Items(id[1]);player.item[14] = Items(id[2]);
            f.close();
        }
        this->setFloor(floorNumber);this->setPlayer(player.x, player.y);
    }
    this->setUI();
    if(saveActions->checkedAction() != 0) saveActions->checkedAction()->setChecked(false);
}

void MainWindow::saveMap()
{
    int ret = QMessageBox::warning(this, tr("Warning"),
                                    tr("Do you really want to save your changes?"),
                                    QMessageBox::Save | QMessageBox::Cancel,
                                    QMessageBox::Save);
    if(ret != QMessageBox::Save) return;
    QFile f(QString("./resources/map/%1.floor").arg(floorNumber));
    if(f.open(QFile::WriteOnly | QIODevice::Text)){
        QTextStream ts(&f);
        for(int i = 0; i < 13; i++){
            for(int j = 0; j < 13; j++){
                if(j < 12){
                    if(objectMap[j][i] == 137) objectMap[j][i] = 0;
                    ts << objectMap[j][i] << '\t';
                }else ts << objectMap[j][i] << '\n';
            }
        }
        ts << floorName;
        f.close();
    }
}

void MainWindow::setMode(bool isEditMode)
{
    this->isEditMode = isEditMode;
    selectedItemId = 0;
    this->setRef();
    this->setUI();
}

void MainWindow::syncAnimations()
{
    for(int i = 0; i < 13; i++){
        for(int j = 0; j < 13; j++){
            if(object[i][j]->isAnimation()){
                object[i][j]->restartAnimation();
            }
        }
    }
}

void MainWindow::gotoFloor()
{
    bool ok;
    int floorNum = QInputDialog::getInt(this, tr("Go to Floor..."), tr("Floor Number:"), floorNumber, 0, maxFloorNum, 1, &ok);
    if(ok) {
        if(floorNum > floorNumber) {
            this->setFloor(floorNum);this->setPlayer(206);
        }
        else {
            this->setFloor(floorNum);this->setPlayer(207);
        }
    }
}

void MainWindow::setNum(NumberItem * item, QPoint pt, int num)
{
    item->setNumber(num);
    item->move(pt.x()+item->dx, pt.y()+24);
}

void MainWindow::notice(QString notice)
{
    this->ui->label_Notice->setText(notice);
}

void Delay_MSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < _Timer){
        QCoreApplication::processEvents(QEventLoop::AllEvents);
    }
}

void MainWindow::on_actionNew_Game_triggered()
{
    if(saveActions->checkedAction() != 0) this->saveActions->checkedAction()->setChecked(false);
    if(loadActions->checkedAction() != 0) this->loadActions->checkedAction()->setChecked(false);
    player.reset();token = 0;showObject(playerItem, 401);
    this->loadGame();
}

void MainWindow::objectMove(int objId, int sx, int sy, int ex, int ey)
{
    SquareGrid grid(13, 13);
    for(int i = 0; i < 13; ++i){
        for(int j = 0; j < 13; ++j){
            if(objectMap[i][j] != 0 && (i != player.x && j != player.y)){
                grid.add_wall(i, j);
            }
        }
    }
    Grid start{sx, sy};
    Grid goal{ex, ey};
    map<Grid, Grid> came_from;
    map<Grid, double> cost_so_far;
    if(a_star_search(grid, start, goal, came_from, cost_so_far)) {
        vector<Grid> path = reconstruct_path(start, goal, came_from);
        Grid curr{sx, sy};
        while(!path.empty()){
            Grid next = path.back();
            path.pop_back();
            showObject(object[curr.x][curr.y], 0);
            objectMap[curr.x][curr.y] = 0;
            showObject(object[next.x][next.y], objId);
            objectMap[next.x][next.y] = objId;
            Delay_MSec(200);
            curr = next;
        }
    }
}

bool MainWindow::eventTrigger(int eventType)//0.  1.Enter 2.Fight&Talk 3.Defeat
{
    for(int i = 0; i < eventList.count(); ++i) {
        if(eventList.at(i).type == eventType && eventList.at(i).flag == 0) {
            Event e = eventList.at(i);
            int nx = player.x+x_token[token], ny = player.y+y_token[token];
            if(eventType == 1 && player.x == e.x && player.y == e.y) {
            }else if(eventType == 2 && nx == e.x && ny == e.y) {
            }else if(eventType == 3 && nx == e.x && ny == e.y) {
            }else return false;
            hasControl = false;
            scene(e.id);
            if(eventList.at(i).id == e.id) {
                eventList[i].flag = 1;
            }
            hasControl = true;
            return true;
        }
    }
    return false;
}

void MainWindow::scene(int id)
{
    switch(id) {
    case 1://Dark Lord appears
        showAnimation(object[5][7], 16, 30, true);//TDL dialog
        dialog->addDialog(30, "I am Zeno, the Lord of the Dark. I welcome you to my Magic Tower. I am expecting man like you to come here, weather for wealth or power, "
                              "you can find them here. But first, let's test your strength and see if you are worth of my time. Come hither! My royal minions!");
        dialog->updateDialog();
        while(dialog->count != 0) Delay_MSec(100);
        showAnimation(object[5][8], 16, 29, true);
        showAnimation(object[4][9], 16, 29, true);
        showAnimation(object[5][10], 16, 29, true);
        showAnimation(object[6][9], 16, 29, true);
        effect[1]->move(object[5][9]->pos());effect[1]->show();
        effect[1]->setFlexablePix(QPixmap("./resources/pic/effect.png").copy(0, 0, 32, 32));
        Delay_MSec(200);
        showObject(effect[1], 0);effect[1]->hide();
        player.hp = player.max_hp = 400;
        player.item[12] = Items(0);player.item[13] = Items(0);player.item[14] = Items(0);
        objectMap[5][7] = 0;objectMap[5][8] = 0;objectMap[4][9] = 0;objectMap[5][10] = 0;objectMap[6][9] = 0;
        for(int i = 0; i < eventList.count(); ++i) {
            if(eventList.at(i).id == 1) {
                eventList[i].flag = 1;
            }
        }
        autoSave();setFloor(1);setPlayer(3, 8);setUI();
        break;

    case 2://blank
        dialog->addDialog(36, "Thank God! You finally wake up! When the Guards threw you in here I thought you were already dead.");
        dialog->addDialog(36, "Now that you are alright, I think it's time for us to escape. This brick wall seems fragile enough, let's dig our way out! Now Diiiiiig!!!");
        dialog->updateDialog();
        Delay_MSec(500);//dig sound
        showAnimation(object[2][7], 5);
        objectMap[2][7] = 0;
        Delay_MSec(300);
        dialog->addDialog(36, "Awesome! Now, I've had some unfinished stuff of my own so, I think it's time to part ways, maybe we will meet again!");
        dialog->addDialog(36, "One more thing. The monsters in this Tower is very tough so, make sure you grab some equipments before you engage some of them. "
                              "I think the 4th and the 9th Floor is where they keep their goods, you may find your sword and shield there. Good luck! See you later!");
        dialog->updateDialog();
        objectMove(36, 3, 7, 1, 10);
        showObject(object[1][10], 0);
        objectMap[1][10] = 0;
        break;

    case 3://Floor 2, Old Man
        dialog->addDialog(37, "Take this, may it helps you in your journey.");
        dialog->addDialog(37, "Use this on enemies and you can see how much HP you will lose to defeat them.");
        dialog->updateDialog();
        notice("You get the Orb of Strength!");
        player.pickup(120, 1);setUI();
        break;

    case 4://Floor 3, Old Man
        dialog->addDialog(37, "There are doors which cannot be openned with a key. These doors can only be opened by beatting the guarding monsters.");

    default:
        return;
    }
}
