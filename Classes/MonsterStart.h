#pragma once
#include "cocos2d.h"
#include"HelloWorldScene.h"
#include<vector>
USING_NS_CC;

//坐标
struct pos {
	int i;
	int j;
};
struct vec2 {
	float x;
	float y;
};

//怪物的出现和消失
class MonsterStart : public cocos2d::Layer {
private:
    int level;               // 当前关卡
    int current_waves;       // 当前波次
    int max_waves;           // 最大波次
    cocos2d::Vector<cocos2d::Sprite*> monsters; // 存储当前场景中的怪物
    float spawnInterval;     // 怪物出现的间隔时间
    float disappearTime;     // 怪物持续存在的时间

public:
    virtual bool init();
    void start();
    void MonsterAppear(int Type);

    static cocos2d::Layer* createLayer();

    CREATE_FUNC(MonsterStart);
};
