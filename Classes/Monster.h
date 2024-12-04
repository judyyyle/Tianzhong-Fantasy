#pragma once
#include "cocos2d.h"
#include "HelloWorldScene.h"

class Monster_value {
public:
	int type;//怪物的种类
	int HP;//生命值
	int full_HP;//满血
	int ATK;//攻击值

	int Experience;//获得的金币

	int Speed;//移动速度
	int move_Count;//移动步数
	double poison_Time = 0;//中毒时间
	int poison_Speed;//中毒的移动速度

	cocos2d::Sprite* enemy_picture; //障碍贴图
};

class Monster : public cocos2d::Sprite{
private:
	Monster_value monster;
public:	
};
