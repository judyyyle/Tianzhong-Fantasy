#pragma once
#include "cocos2d.h"
#include "HelloWorldScene.h"
USING_NS_CC;

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

class Monster_value {
public:
	int type;                      //����ϰ�������
	int HP;                        //����ֵ
	int full_HP;                   //��Ѫ
	int ATK;                       //����ֵ

	int Coin;                      //��õĽ��

	int Speed;                     //�ƶ��ٶ�
	int move_Count;                //�ƶ�����
	double poison_Time = 0;        //�ж�ʱ��
	int poison_Speed;              //�ж����ƶ��ٶ�

	cocos2d::Sprite* enemy_picture;//�ϰ���ͼ
};

class Monster : public cocos2d::Sprite {
private:
    Monster_value monster;
public:
    bool init() override {
        if (!Sprite::init()) {
            return false;
        }
        // ��ʼ�� monster
        // this->monster = Monster_value{...};
        return true;
    }

    void initType(int stage);

    CREATE_FUNC(Monster);

    static cocos2d::Sprite* createSprite();
};

