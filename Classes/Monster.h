#pragma once
#include <vector>
#include "cocos2d.h"
#include "HelloWorldScene.h"

USING_NS_CC;
using namespace cocos2d;

#define MONSTER_TOTAL 4

#define NORMAL 0            //正常
#define HUGE 1              //血厚
#define FAST 2              //高速
#define BOSS 3              //BOSS怪

#define ADVENTURE1 0    //map1
#define ADVENTURE2 1    //map2
#define BOSS1 2         //map3

#define STOP -1
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

/*
#define BARRIER 0
#define MONSTER 1

#define NORMAL 0            //正常怪物
#define FAST 1              //高速怪物
#define HUGE 2              //血厚怪物
#define BARRIER_1 3         //一格障碍
#define BARRIER_2 4         //两格障碍
#define BARRIER_3 5         //四格障碍
//障碍
#define BARRIER_ONE   1     //一格的障碍
#define BARRIER_TWO   2     //两格的障碍
#define BARRIER_FOUR  4     //四格的障碍
#define BARRIER_HP_ONE     1600               //一格障碍血量
#define BARRIER_HP_TWO     3 * BARRIER_HP_ONE //两格障碍血量
#define BARRIER_HP_FOUR    3 * BARRIER_HP_TWO //四格障碍血量
#define BARRIER_COIN_ONE     50
#define BARRIER_COIN_TWO     2 * BARRIER_COIN_ONE //两格障碍金币
#define BARRIER_COIN_FOUR    2 * BARRIER_COIN_TWO //四格障碍金币
//怪物
#define MONSTER_NORMAL 0    //正常怪物
#define MONSTER_FAST   1    //高速怪物
#define MONSTER_HUGE   2    //血厚怪物
#define MONSTER_FAST_HP       20                     //高速怪物血量
#define MONSTER_NORMAL_HP     3 * MONSTER_FAST_HP    //正常怪物血量
#define MONSTER_HUGE_HP       3 * MONSTER_NORMAL_HP  //血厚怪物血量
#define MONSTER_HUGE_SPEED    100                        //血厚怪物速度
#define MONSTER_NORMAL_SPEED  1.5*MONSTER_HUGE_SPEED     //正常怪物速度
#define MONSTER_FAST_SPEED    2*MONSTER_NORMAL_SPEED     //高速怪物速度
#define MONSTER_COIN_NORMAL   18    //正常怪物和血厚怪物掉落金币
#define MONSTER_COIN_HUGE     188   //血厚怪物掉落金币
//路径
#define BARRIER_PICTURE  "/Enemy/barrier/0/"   //障碍路径
#define MONSTER_PICTURE  "/Enemy/monster/0/"   //怪物路径

*/


struct MonsterType {
    //	int type;                      //怪物、障碍的种类
    int hp;           //实时血量
    int max_hp;       //满血量                 
    int speed;        //移动速度
    //	int full_HP;                   //满血
    //	int ATK;                       //攻击值

    //	int coin;                      //获得的金币

    //	int move_Count;                //移动步数
    //	double poison_Time = 0;        //中毒时间
    //	int poison_Speed;              //中毒的移动速度
};

struct MapPath {
    Vec2 point;
    int x;
    int y;
    int direction;
};

class Monster : public Sprite {
private:   
    MapPath* path;
    MonsterType monster_type;
    Sprite* hp_border, * hp;
    int path_count;
    int path_total;
public:
    CREATE_FUNC(Monster);

    //static Sprite* createSprite();

    virtual bool init();
    void initType(int monster_type,int map_type);
    void update(float dt);
};

extern std::vector<Monster*> monsters;

class MonsterCreate : public Sprite {    
public:
    CREATE_FUNC(MonsterCreate);

    void initMonster(int monster_type, int map_type) {
        auto monster = Monster::create();
        if (!monster) {
            CCLOG("Failed to create monster sprite!");
            return;
        }
        monster->initType(monster_type, map_type);

        monsters.push_back(monster);
        this->addChild(monster);
    }

    void MonsterWaves(int map_type) {
        switch (map_type) {
        case ADVENTURE1:
            for (int i = 0; i < 5; i++) {
                this->runAction(Sequence::create(DelayTime::create(i * 1.4f),
                    CallFunc::create([=]() {
                        initMonster(NORMAL, map_type);
                        }),
                    nullptr));
            }
            for (int i = 0; i < 5; i++) {
                this->runAction(Sequence::create(DelayTime::create((i + 5) * 1.0f),
                    CallFunc::create([=]() {
                        initMonster(FAST, ADVENTURE1);
                        }),
                    nullptr));

            }

            for (int i = 0; i < 5; i++) {
                this->runAction(Sequence::create(DelayTime::create((i + 10) * 1.6f),
                    CallFunc::create([=]() {
                        initMonster(HUGE, ADVENTURE1);
                        }),
                    nullptr));

            }
            break;
        case ADVENTURE2:
            break;
        case BOSS1:
            break;
        }       
    }
};
