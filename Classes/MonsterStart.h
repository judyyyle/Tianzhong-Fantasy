#pragma once
#include "cocos2d.h"
#include"HelloWorldScene.h"
#include<vector>
USING_NS_CC;

//世界坐标与数组的转换
struct array {
    int row;
    int col;
};

Vec2 array_to_vec2(int row, int col) { //返回Vec2类型，即世界坐标
    Vec2 vec;
    vec.x = 64 + 128 * col;
    vec.y = 1024 - 64 - 128 * row;
    return vec;
}

array vec2_to_array(Vec2 vec) { //返回array类型，即数组
    array arr;
    arr.row = 12 - static_cast<int>((vec.y / 128));
    arr.col = static_cast<int>((vec.x / 128));
    return arr;
}

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
    void MonsterAppear(int Type)；
    void CarrotAppear();

    static cocos2d::Layer* createLayer();

    CREATE_FUNC(MonsterStart);
};
