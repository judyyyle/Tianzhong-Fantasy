#pragma once
#include "cocos2d.h"
#include "HelloWorldScene.h"

#define BARRIER 0
#define MONSTER 1

#define NORMAL 0            //正常怪物
#define FAST 1              //高速怪物
#define HUGE 2              //血厚怪物
#define BARRIER_1 3         //一格贴图
#define BARRIER_2 4         //两格贴图
#define BARRIER_3 5         //四格贴图
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

class Monster_value {
public:
	int type;                      //怪物的种类
	int HP;                        //生命值
	int full_HP;                   //满血
	int ATK;                       //攻击值

	int Coin;                      //获得的金币

	int Speed;                     //移动速度
	int move_Count;                //移动步数
	double poison_Time = 0;        //中毒时间
	int poison_Speed;              //中毒的移动速度

	cocos2d::Sprite* enemy_picture;//障碍贴图
};

class Monster : public cocos2d::Sprite{
private:
	Monster_value monster;
public:	
	bool init();
	void initType(int stage);

};
