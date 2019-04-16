#ifndef MYSCENE_H
#define MYSCENE_H

#include <QObject>

class myScene : public QObject
{
    Q_OBJECT
public:
    explicit myScene(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MYSCENE_H