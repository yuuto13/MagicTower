#include "objectclass.h"

Object::Object(int id)
{
    if(id > 200 && id < 300) {
        this->id = id;
        QFile f(QString("./resources/object/%1.object").arg(id));
        if(f.open(QFile::ReadOnly)){
            QTextStream ts(&f);
            name = ts.readLine();
            desc = ts.readLine();
            f.close();
        }
    }
}

Items::Items(int id)
{
    if(id > 100 && id < 200) {
        this->id = id;
        buff.hp = buff.mp = buff.atk = buff.def = 0;
        QFile f(QString("./resources/item/%1.item").arg(id));
        if(f.open(QFile::ReadOnly)){
            QTextStream ts(&f);
            name = ts.readLine();
            ts >> type;
            if(type >= 1 && type <= 4){
                ts >> buff.hp >> buff.mp >> buff.atk >> buff.def;
            }
            ts.readLine();
            desc = ts.readLine();
            enchantment = ts.readLine();
            f.close();
        }
    }else{
        this->id = 0;
    }
}

Monsters::Monsters(int id)
{
    if(id > 0 && id < 100) {
        this->id = id;
        QFile f(QString("./resources/monster/%1.mst").arg(id));
        if(f.open(QFile::ReadOnly)){
            QTextStream ts(&f);
            name = ts.readLine();
            ts >> hp >> atk >> def >> coin;
            max_hp = hp;
            ts.readLine();
            desc = ts.readLine();
            skill = ts.readAll();
            f.close();
        }
    }else this->id = 0;
}

Player::Player()
{
    name = "yuuto";
    max_hp = 1000;
    max_mp = 100;
    hp = 1000;
    base_atk = 10;
    base_def = 10;

    item[0] = Items(0);    item[1] = Items(0);    item[2] = Items(0);    item[3] = Items(0);    item[4] = Items(0);    item[5] = Items(0);
    item_amount[0] = 0;    item_amount[1] = 0;    item_amount[2] = 0;    item_amount[3] = 0;    item_amount[4] = 0;    item_amount[5] = 0;

    item[6] = Items(0);    item[7] = Items(0);    item[8] = Items(0);    item[9] = Items(0);    item[10] = Items(0);    item[11] = Items(0);
    item_amount[6] = 0;    item_amount[7] = 0;    item_amount[8] = 0;    item_amount[9] = 0;    item_amount[10] = 0;    item_amount[11] = 0;

    item[12] = Items(101);//weapon
    item[13] = Items(106);//shield
    item[14] = Items(0);//accessory
}

void Player::reset()
{
    max_hp = 1000;
    max_mp = 100;
    hp = 1000;
    base_atk = 10;
    base_def = 10;

    item[0] = Items(0);    item[1] = Items(0);    item[2] = Items(0);    item[3] = Items(0);    item[4] = Items(0);    item[5] = Items(0);
    item_amount[0] = 0;    item_amount[1] = 0;    item_amount[2] = 0;    item_amount[3] = 0;    item_amount[4] = 0;    item_amount[5] = 0;

    item[6] = Items(0);    item[7] = Items(0);    item[8] = Items(0);    item[9] = Items(0);    item[10] = Items(0);    item[11] = Items(0);
    item_amount[6] = 0;    item_amount[7] = 0;    item_amount[8] = 0;    item_amount[9] = 0;    item_amount[10] = 0;    item_amount[11] = 0;

    item[12] = Items(101);//weapon
    item[13] = Items(106);//shield
    item[14] = Items(0);//accessory
}

int Player::findId(int id)
{
    int i = 0;
    for(i = 0; i < 12; ++i) {
        if(item[i].id == id) {
            return i;
        }
    }
    return -1;
}

int Player::findSpace(int id, int amount)
{
    int i = 0;
    for(i = 0; i < 12; ++i) {
        if(item[i].id == id) {
            if(item[i].type == 0 || item[i].type == 4) {
                if(item_amount[i] != 999){
                    if(item_amount[i] + amount > 999) {
                        item_amount[i] = 999;
                        amount = item_amount[i] + amount - 999;
                    }else {
                        item_amount[i] += amount;
                        return 0;
                    }
                }
            }
        }else if(item[i].id == 0) {
            item[i] = Items(id);
            item_amount[i] = amount;
            return 0;
        }
    }
    if(i == 12) {
        return amount;
    }else return 0;
}

int Player::modHp(bool isMod) {
    if(isMod) setHp(hp * item[14].buff.hp / 100);
    else setHp(-hp * item[14].buff.hp / (item[14].buff.hp + 100));
    return hp;
}

int Player::atk()
{
    return base_atk + item[12].buff.atk + item[14].buff.atk;
}

int Player::def()
{
    return base_def + item[13].buff.def + item[14].buff.def;
}

void Player::setHp(int hp_mod)
{
    hp += hp_mod;
    if(hp > max_hp) {
        max_hp = hp;
    }
}

int Player::setItem(int slot, int id, int amount)
{
    if(item[slot].id != id) {
        item[slot] = Items(id);
    }
    if(id == 0) {
        int temp = item_amount[slot];
        item_amount[slot] = 0;
        return temp;
    }
    if(slot > 11) return 0;
    if(item_amount[slot] + amount > 999) {
        return item_amount[slot] + amount - 999;
    }else {
        item_amount[slot] += amount;
        return 0;
    }
}

int Player::pickup(int itemId, int amount)
{
    if(findId(itemId) == -1) {
        if(findId(0) == -1) {
            return amount;
        } else slot = findId(0);
    } else {
        slot = findId(itemId);
        int total = item_amount[slot] + amount;
        if(total >= 999) {
            item_amount[slot] = 999;
            if(findId(0) == -1) {
                return total - 999;
            }else slot = findId(0);
        }else item_amount[slot] = total;return 0;
    }
    item[slot] = Items(itemId);
    item_amount[slot] = amount;
    return 0;
}

QString Player::damage(int id)
{
    Monsters m = Monsters(id);
    if(atk() < m.def) return QString("The Defence of " + m.name + " is too high!");
    else if(def() > m.atk) return QString("You are so tough, you will not even get a single scratch from " + m.name);
    else if(atk() > m.hp + m.def) return QString("You are so strong, you can one shot " + m.name);
    else {
        int n = 0;
        int pHit = atk() - m.def;
        int mHit = m.atk - def();
        while(m.hp > 0) {
            m.hp -= pHit;
            if(m.hp > 0) n += mHit;
        }return QString("You will lose %1 HP from the battle with" + m.name).arg(n);
    }
}
