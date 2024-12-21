#pragma once
#include "cocos2d.h"
#include <vector>

class Bullet : public cocos2d::Sprite {
public:
    static Bullet* create(const std::string& fileName, cocos2d::Vec2 startPos, cocos2d::Vec2 targetPos, float speed, int Type, int level);

    virtual void update(float dt) override;

    int GetType();  // 获取子弹类型
    float GetTime(); // 获取减速时间
    int GetDamage(); // 获取子弹伤害
protected:
    cocos2d::Vec2 startPosition;   // 起点位置
    cocos2d::Vec2 targetPosition;  // 目标位置
    float speed;                   // 子弹飞行速度
    int damage;                    // 子弹伤害
    float speedcuttime;            // 攻击类型的速度减少时间
    int BulletType;                // 子弹类型
};


class Sunflowerfire :public Bullet {
public:
    static Sunflowerfire* create(const std::string& fileName ,  int level) {
        Sunflowerfire* ret = new Sunflowerfire();
        if (ret && ret->initWithFile(fileName)) {
            ret->autorelease();
            ret->damage = 10 * level;   // 子弹伤害随着等级增加
            return ret;
        }
        delete ret;
        return nullptr;  // 如果初始化失败，返回空指针
    }

};
