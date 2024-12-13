#pragma once
#include <vector>
#include "cocos2d.h"
#include "HelloWorldScene.h"

USING_NS_CC;
using namespace cocos2d;

#define MONSTER_TOTAL 4

#define NORMAL 0            //����
#define HUGE 1              //Ѫ��
#define FAST 2              //����
#define BOSS 3              //BOSS��

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

#define NORMAL 0            //��������
#define FAST 1              //���ٹ���
#define HUGE 2              //Ѫ�����
#define BARRIER_1 3         //һ���ϰ�
#define BARRIER_2 4         //�����ϰ�
#define BARRIER_3 5         //�ĸ��ϰ�
//�ϰ�
#define BARRIER_ONE   1     //һ����ϰ�
#define BARRIER_TWO   2     //������ϰ�
#define BARRIER_FOUR  4     //�ĸ���ϰ�
#define BARRIER_HP_ONE     1600               //һ���ϰ�Ѫ��
#define BARRIER_HP_TWO     3 * BARRIER_HP_ONE //�����ϰ�Ѫ��
#define BARRIER_HP_FOUR    3 * BARRIER_HP_TWO //�ĸ��ϰ�Ѫ��
#define BARRIER_COIN_ONE     50
#define BARRIER_COIN_TWO     2 * BARRIER_COIN_ONE //�����ϰ����
#define BARRIER_COIN_FOUR    2 * BARRIER_COIN_TWO //�ĸ��ϰ����
//����
#define MONSTER_NORMAL 0    //��������
#define MONSTER_FAST   1    //���ٹ���
#define MONSTER_HUGE   2    //Ѫ�����
#define MONSTER_FAST_HP       20                     //���ٹ���Ѫ��
#define MONSTER_NORMAL_HP     3 * MONSTER_FAST_HP    //��������Ѫ��
#define MONSTER_HUGE_HP       3 * MONSTER_NORMAL_HP  //Ѫ�����Ѫ��
#define MONSTER_HUGE_SPEED    100                        //Ѫ������ٶ�
#define MONSTER_NORMAL_SPEED  1.5*MONSTER_HUGE_SPEED     //���������ٶ�
#define MONSTER_FAST_SPEED    2*MONSTER_NORMAL_SPEED     //���ٹ����ٶ�
#define MONSTER_COIN_NORMAL   18    //���������Ѫ����������
#define MONSTER_COIN_HUGE     188   //Ѫ����������
//·��
#define BARRIER_PICTURE  "/Enemy/barrier/0/"   //�ϰ�·��
#define MONSTER_PICTURE  "/Enemy/monster/0/"   //����·��

*/


struct MonsterType {
    //	int type;                      //����ϰ�������
    int hp;           //ʵʱѪ��
    int max_hp;       //��Ѫ��                 
    int speed;        //�ƶ��ٶ�
    //	int full_HP;                   //��Ѫ
    //	int ATK;                       //����ֵ

    //	int coin;                      //��õĽ��

    //	int move_Count;                //�ƶ�����
    //	double poison_Time = 0;        //�ж�ʱ��
    //	int poison_Speed;              //�ж����ƶ��ٶ�
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