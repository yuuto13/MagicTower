#include "animateditem.h"

AnimatedItem & AnimatedItem::operator = (AnimatedItem & item)
{
    if(&item != this) {
        delete timer;
        timer = new QTimer(this);
        isLoop = item.isLoop;
        pixCount = item.pixCount;
        currentIndex = 0;
        dragable = item.dragable;
        if(item.timer->isActive()) {
            timer->setInterval(item.timer->interval());
            pixList = item.pixList;
            currentPix = pixList.at(0);
            setFixedSize(currentPix.width(), currentPix.height());
            update();
            timer->start();
        } else {
            setFlexablePix(item.currentPix);
        }
    }
    return *this;
}

AnimatedItem::AnimatedItem(QWidget * parent) : QWidget(parent)
{
    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(updateAnimation()));
}

AnimatedItem::~AnimatedItem()
{
    delete timer;
}

void AnimatedItem::setPix(const QPixmap &pix)
{
    timer->stop();
    if(!pixList.empty()){
        pixList.clear();
    }
    isLoop = true;
    currentPix = pix;
    this->setFixedSize(currentPix.width(), currentPix.height());
    this->update();
}

void AnimatedItem::setFlexablePix(const QPixmap &pix)
{
    timer->stop();
    if(!pixList.empty()){
        pixList.clear();
    }
    isLoop = true;
    currentPix = pix;
    this->setGeometry(this->pos().x(), this->pos().y(), currentPix.width(), currentPix.height());
    this->update();
}

void AnimatedItem::setAnimation(const QPixmap &pix, const short count, const int msec, const bool isLoop, const bool isReverse)
{
    this->isLoop = isLoop;
    pixCount = count;
    currentIndex = 0;
    this->timer->setInterval(msec);
    if(!pixList.empty()){
        pixList.clear();
    }
    if(!isReverse){
        for(short i = 0; i < count; i++){
                pixList.append(pix.copy(i * (pix.width() / count), 0, pix.width() / count, pix.height()));
        }
    }else{
        for(short i = count-1; i > -1; i--){
                pixList.append(pix.copy(i * (pix.width() / count), 0, pix.width() / count, pix.height()));
        }
    }
    currentPix = pixList.at(0);
    this->setFixedSize(currentPix.width(), currentPix.height());
    this->update();
    this->timer->start();
}

void AnimatedItem::restartAnimation()
{
    currentIndex = 0;
    currentPix = pixList.at(0);
    this->timer->start();
}

void AnimatedItem::updateAnimation()
{
    if(currentIndex < pixCount && currentIndex >= 0){
        currentPix = pixList.at(currentIndex);
        this->update();
        if(currentIndex == pixCount - 1){
            if(isLoop){
                currentIndex = 0;
            }else{
                this->timer->stop();
                currentIndex = 0;
                emit animationFinished();
            }
        }else{
           currentIndex++;
        }
    }
}

void AnimatedItem::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), currentPix);
}

void AnimatedItem::mousePressEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton){
        emit mousePressed(x, y);
    }else if(event->button() == Qt::RightButton){
        emit rightPressed(x, y);
    }
}

void AnimatedItem::enterEvent(QEvent *)
{
    emit mouseEntered(x, y);
}

void AnimatedItem::leaveEvent(QEvent *)
{
     emit mouseLeaved();
}

//*********************************** Number Item ****************************************

NumberItem::NumberItem(QWidget *parent) : QWidget(parent)
{

}

NumberItem::NumberItem(const NumberItem &item)
{
    if(&item != this) {
        this->setPix(item.pix);
        this->setNumber(item.num);
    }
}

NumberItem & NumberItem::operator = (const NumberItem & item)
{
    if(&item != this) {
        this->setPix(item.pix);
        this->setNumber(item.num);
    }
    return *this;
}

void NumberItem::setPix(const QPixmap &pix)
{
    this->pix = pix;
    w = pix.width()/10;
    h = pix.height();
}

void NumberItem::setNumber(short number)
{
    this->num = number;
    str_num.setNum(number);
    len = str_num.length();
    dx = 30 - len * w / 2;
    this->setGeometry(this->pos().x(), this->pos().y(), w*len, h);
    this->update();
}

void NumberItem::paintEvent(QPaintEvent *)
{
    if(num > 1) {
        QPainter painter(this);
        for(int i = 0; i < len; i++){
            painter.drawPixmap(i*w, 0, w, h,
                               pix.copy(str_num.mid(i, 1).toInt()*w, 0, w, h));
        }
    }
}

bool operator == (NumberItem a, NumberItem b){return a.number() == b.number();}
bool operator != (NumberItem a, NumberItem b){return a.number() != b.number();}
bool operator < (NumberItem a, NumberItem b){return a.number() < b.number();}

//*********************************** My Dialog ****************************************

MyDialog::MyDialog(QWidget *parent) : QWidget(parent)
{
    this->setGeometry(70, 130, 300, 200);
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(68, 68, 68));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    icon = new AnimatedItem(this);
    frame = new AnimatedItem(this);
    icon->move(40, 30);
    frame->move(0, 0);
    frame->setPix(QPixmap("./resources/pic/dialog_frame.png"));
    text = new QLabel(this);
    name = new QLabel(this);
    text->setGeometry(20, 62, 260, 118);
    name->setGeometry(85, 40, 100, 20);
    QFont font("Adobe Gothic Std B", 10, 75);
    text->setFont(font);name->setFont(font);
    QPalette pal2;pal2.setColor(QPalette::WindowText, QColor(255, 255, 255));
    text->setPalette(pal2);name->setPalette(pal2);
    text->setWordWrap(true);
    for(int i = 0; i < 4; i++){
        buttons[i] = new QPushButton("Button", this);
        buttons[i]->move(40+i*50, 200);

    }
    this->update();
}
MyDialog::~MyDialog()
{
    delete icon;
    delete frame;
    for(int i = 0; i < 4; i++){
        delete buttons[i];
    }
}

void MyDialog::updateDialog()
{
    if(count == 0) {
        this->releaseMouse();
        this->parentWidget()->setFocus();
        this->hide();
        return;
    }else {
        this->show();
        this->setFocus();this->grabMouse();
    }
    int id = idList.front();
    if(id <= 30 && id > 0){
        icon->setAnimation(QPixmap("./resources/pic/model.png").copy((id-1)*2%12*32, (id-1)/6*32, 64, 32), 2, 300);
    }else if(id > 32 && id < 39){
        icon->setAnimation(QPixmap("./resources/pic/model.png").copy((id-33)*2*32, 8*32, 64, 32), 2, 300);
    }
    name->setText(Monsters(id).name);
    text->setText(textList.front());
    idList.pop_front();
    textList.pop_front();
}

void MyDialog::addDialog(int id, QString text)
{
    idList.append(id);
    textList.append(text);
    ++count;
}

void MyDialog::mousePressEvent(QMouseEvent *)
{
    --count;
    this->updateDialog();
}

void MyDialog::keyPressEvent(QKeyEvent *)
{
    --count;
    this->updateDialog();
}

//*********************************** My Button ****************************************

MyButton::MyButton(QWidget *parent) : QWidget(parent)
{

}

