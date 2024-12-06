#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#include "MonsterStart.h"
#include "Monster.h"
#include "Tower.h"

#include<string>
#include <cmath>
#include <vector>
#include<algorithm>

USING_NS_CC;

extern int carrot_HP;//�ܲ�Ѫ��

extern int monster_number;//��������
extern int monster_wave_number;//���﹥������
extern int barrier_number;//�ϰ�����
extern std::vector<Monster*>monster;
extern std::vector<Monster*>barrier;

extern Monster* destination;//����λ��

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

Sprite* Monster::createSprite() {
	return Monster::create();
}

//��ͬ����ϰ�����Ϣ��ʼ��
void Monster::initType(int stage) {
	monster.type = stage;
	std::string picture1[] = { "normal01-2.PNG","fast01-2.PNG","huge01-2.PNG","One1.PNG","One2.PNG","Two1.PNG","Two2.PNG","Four1.PNG","Four2.PNG" };
	std::string picture2[] = { "normal01-1.PNG","fast01-1.PNG","huge01-1.PNG" };


	Vector<SpriteFrame*> monster;

	switch (stage) {
	case 0: //��������
		monster.pushBack(SpriteFrame::create(picture1[0], Rect(0, 0, 68, 87)));
		monster.pushBack(SpriteFrame::create(picture2[0], Rect(0, 0, 75, 74)));
		// ��thisָ��ָ����ֵ��Ա����
		this->monster.full_HP = MONSTER_NORMAL_HP;
		this->monster.HP = MONSTER_NORMAL_HP;
		this->monster.ATK = 1;
		this->monster.Speed = MONSTER_NORMAL_SPEED;
		this->monster.Coin = MONSTER_COIN_NORMAL;

		break;
	}
}






