#ifndef OBJECTCLASS_H
#define OBJECTCLASS_H

#include <QString>
#include <QFile>
#include <QTextStream>

struct Buff {
    int hp = 0;
    int mp = 0;
    int atk = 0;
    int def = 0;
};

class Object
{
public:
    Object() {}
    Object(int id);

    int id = 0;
    QString name = "";
    QString desc = "";
};

class Items : public Object
{
public:
    Items() {}
    Items(int id);

    int type = -1;
    Buff buff;
    QString enchantment = "";
};

class Monsters : public Object
{
public:
    Monsters() {}
    Monsters(int id);

    int max_hp = 0;
    int hp = 0;
    int atk = 0;
    int def = 0;
    int coin = 0;
    QString skill = "";
};

class Player
{
public:
    Player();
    int findId(int id); //return the slot number, return -1 if id id not found
    int findSpace(int id, int amount);
    int modHp(bool);
    int atk();
    int def();
    void setHp(int hp_mod);
    int setItem(int slot, int id, int amount = 1);
    int pickup(int itemId, int amount = 1);
    void reset();
    QString damage(int);

//private:
    QString name;
    short x, y;
    short floor;

    // status
    int max_hp = 0;
    int max_mp = 0;
    int hp = 0;
    int mod_hp = 0;
    int base_atk = 0;
    int base_def = 0;

    // items
    Items item[15];
    short item_amount[12];
    int slot;
};

#endif // OBJECTCLASS_H
