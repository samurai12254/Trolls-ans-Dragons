
#include <bits/stdc++.h>
#include <QApplication>
#ifndef BACKEND_H
#define BACKEND_H


class GameUnit {
public:
    GameUnit() {}
    GameUnit(int health, int damage) : health_(health), damage_(damage) {}
    bool attack(GameUnit* A) {
        A->health_ -= damage_;
        return A->is_live();
    }
    bool is_live() {
        return health_ > 0;
    }
    int get_hp(){
        return health_;
    }
    int get_damage(){
        return damage_;
    }
protected:
    int health_ = 0;
    int damage_ = 0;
};

class Enemies : public GameUnit {
public:
    Enemies(int health, int damage) : GameUnit(health, damage) {}
    virtual std::string set_answer() = 0;
    virtual std::string question() = 0;
    virtual int get_max_health() = 0;
    virtual bool check_answer(int user_answer) = 0;
    virtual ~Enemies() = 0;
};

class EnemiesDragon : public Enemies {
public:
    EnemiesDragon (int health,int damage) : Enemies(health,damage){
        max_health_ = health;
    }
    std::string set_answer() override;
    std::string question() override;
    QString get_type(){
        return type_;
    }
    int get_max_health() override{
        return max_health_;
    }
    bool check_answer(int user_answer) override;
    virtual ~EnemiesDragon() = 0;
protected:
    int color_;
    int answer_;
    int max_health_;
    std::string quest_;
    QString type_;
};

class GreenDragon : public EnemiesDragon {
public:
    GreenDragon(int health,int damage) : EnemiesDragon(health,damage){
        type_ = "GreenDragon";
    }
    std::string set_answer() override;
};

class RedDragon : public EnemiesDragon {
public:
    RedDragon(int health,int damage):EnemiesDragon(health,damage){
        type_ = "RedDragon";
    }
    std::string set_answer() override;
};

class BlueDragon : public EnemiesDragon {
public:
    BlueDragon(int health,int damage):EnemiesDragon(health,damage){
        type_ = "BlueDragon";
    }
    std::string set_answer() override;
};

class Hero : public GameUnit {
public:
    static const int DELTA_HP = 10;
    static const int DELTA_DAMAGE = 3;
    static const int MAX_LEVEL = 10;

    Hero() {}
    Hero(std::string name, int health, int damage)
        : GameUnit(health, damage), name_(name), max_health_(health) {}

    void gain_exp(int amount) {
        if (level_ >= MAX_LEVEL) {
            return;
        }

        exp_ += amount;

        if (exp_ >= 100) {
            level_up();
        }
    }

    void level_up() {
        if (level_ < MAX_LEVEL) {
            level_++;
            exp_ = 0;
            max_health_ += DELTA_HP;
            damage_ += DELTA_DAMAGE;
            health_ = max_health_;
        }
    }

    std::string get_name() const { return name_; }
    int get_exp() const { return exp_; }
    int get_level() const { return level_; }
    int get_max_health() const { return max_health_; }

private:
    std::string name_;
    int exp_ = 0;
    int level_ = 1;
    int max_health_ = 0;
};



class Troll : public Enemies {
public:
    Troll(int health, int damage) : Enemies(health, damage) {
        max_health_ = health;
    }
    std::string question() override { return quest_; }
    QString get_type(){
        return type_;
    }
    int get_max_health() override{
        return max_health_;
    }
    bool check_answer(int user_answer) override { return user_answer == answer_; }
    virtual ~Troll() = 0;
protected:
    int answer_;
    std::string quest_;
    QString type_;
    int max_health_;
};

class MountainTroll : public Troll {
public:

    MountainTroll(int health, int damage) : Troll(health, damage) {
        type_ = "MountainTroll";
    }
    std::string set_answer() override;
};

class IceTroll : public Troll {
public:
    IceTroll(int health, int damage) : Troll(health, damage) {
        type_ = "IceTroll";
    }
    std::string set_answer() override;
};

class DesertTroll : public Troll {
public:
    DesertTroll(int health, int damage) : Troll(health, damage) {
        type_ = "DesertTroll";
    }
    std::string set_answer() override;
    bool check_answer(int user_answer) override;
private:
    std::vector<int> correct_factors_;
};



#endif // BACKEND_H
