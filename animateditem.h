#ifndef ANIMATEDITEM_H
#define ANIMATEDITEM_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>
#include <QLabel>
#include "objectclass.h"

class AnimatedItem : public QWidget
{
    Q_OBJECT
public:
    explicit AnimatedItem(QWidget * parent = nullptr);
    ~AnimatedItem();

    AnimatedItem & operator = (AnimatedItem & item);

    void setPix(const QPixmap & pix);
    void setFlexablePix(const QPixmap & pix);
    void setAnimation(const QPixmap & pix, const short count, const int msec, const bool isLoop = true, const bool isReverse = false);
    void restartAnimation();

    void startAnimation() { timer->start(); }
    void stopAnimation() { timer->stop(); }
    void setCoordinates(int x, int y) { this->x = x, this->y = y; }
    short getX() { return x; }
    short getY() { return y; }
    int getWidth() { return currentPix.width(); }
    int getHeight() { return currentPix.height(); }
    bool isNull() { return currentPix.isNull(); }
    bool isAnimation() { return !pixList.empty(); }
    void setDragable(bool dragable) { this->dragable = dragable; }
    bool isDragable() { return dragable; }

signals:
    void animationFinished();
    void mousePressed(int, int);
    void rightPressed(int, int);
    void mouseEntered(int, int);
    void mouseLeaved();

public slots:
    void updateAnimation();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent * event);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    bool isLoop;
    short pixCount;
    short currentIndex;
    QTimer * timer;
    QPixmap currentPix;
    QList<QPixmap> pixList;

    short x = -1, y = -1;
    bool dragable = false;
};

class NumberItem : public QWidget
{
    Q_OBJECT
public:
    explicit NumberItem(QWidget * parent = nullptr);
    NumberItem(const NumberItem & item);
    NumberItem & operator = (const NumberItem & item);

    void setPix(const QPixmap & pix);
    void setNumber(short number);
    int number() { return num; }
    int length() { return len; }
    int width() { return w; }
    int height() { return h; }

    short dx;

protected:
    void paintEvent(QPaintEvent *);

private:
    short num;
    QString str_num;
    short len;
    short w, h;
    QPixmap pix;
};
bool operator == (NumberItem a, NumberItem b);
bool operator != (NumberItem a, NumberItem b);
bool operator < (NumberItem a, NumberItem b);

class MyDialog : public QWidget
{
    Q_OBJECT
public:
    explicit MyDialog(QWidget * parent = nullptr);
    ~MyDialog();
    void updateDialog();
    void addDialog(int, QString);
    void addDialog(int storeIndex);

    int count = 0;

protected:
    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    AnimatedItem * icon;
    AnimatedItem * frame;
    QLabel * text;
    QLabel * name;
    QPushButton * buttons[4];

    QList<int> idList;
    QList<QString> textList;
};

class MyButton : public QWidget
{
    Q_OBJECT
public:
    explicit MyButton(QWidget * parent = nullptr);

protected:
//    void mousePressEvent(QMouseEvent * event);
//    void mouseReleaseEvent(QMouseEvent * event);

private:


};

#endif // ANIMATEDITEM_H
