#pragma once
#include"cocos2d.h"
#include "AppDelegate.h"
#include "HelloWorldScene.h"
struct position {
    int row;
    int col;
};
class Tower :public cocos2d::Sprite
{
protected:
    int Type;//种类
    int level;//当前等级
    int value;//当前价值
    float attack_power;//单次攻击力
    float attack_range;//攻击范围
    float attack_speed;
    cocos2d::Sprite* nearestEnemy; // 最近的敌人
    cocos2d::Vec2 fireTarget;
    position Tower_base;
    cocos2d::Sprite* weapon;  // 武器
public:
    // 初始化防御塔的属性
    virtual bool init() = 0; // 纯虚函数，子类必须实现
    virtual void attack() = 0; // 攻击行为
    Tower(const std::string& fileName, const cocos2d::ValueMap& data) {
        initWithFile(fileName);
        loadProperty(data);
    }
    // 通用方法
    void levelUp() {
        if (level < 3) {
            level++;
            attack_power *= 1.2;  // 增强攻击力
            attack_range += 64;         // 增加攻击范围
        }
    }
    cocos2d::Sprite* findNearestMonster() {
        auto& monsterPool = GameManager::getInstance()->getMonsterPool();//得到怪兽池
        float currMinDistant = attack_range;
        nearestEnemy = nullptr;

        for (const auto& monster : monsterPool) {
            float distance = getPosition().distance(monster->getPosition());
            if (distance < currMinDistant) {
                currMinDistant = distance;
                nearestEnemy = monster;
            }
        }
        return nearestEnemy;
    }

    int getValue() const { return value; }
    float getRange() const { return attack_range; }
};

class BottleTower :public Tower {
public:
    BottleTower(const cocos2d::ValueMap& data) : Tower("Bottle_3.png", data) {
        // 每 0.5 秒旋转并开火
        schedule(CC_SCHEDULE_SELECTOR(BottleTower::onRotateAndFire), 0.5f);
    }
    virtual bool init() override {
        level = 1;
        value = 100;
        attack_power = 50;//待定
        attack_speed = 8;//待定
        attack_range = 192;
        return true;
    }
    void onRotateAndFire(float dt) {
        auto nearestEnemy = findNearestMonster();
        if (nearestEnemy) {
            weapon->stopAllActions();
            fireTarget = nearestEnemy->getPosition();

            // 计算旋转角度
            auto rotateVector = fireTarget - getPosition();
            float rotateRadians = rotateVector.getAngle();
            float rotateDegrees = CC_RADIANS_TO_DEGREES(-rotateRadians);

            // 旋转动作
            float speed = 0.5f / M_PI;
            float rotateDuration = fabs(rotateRadians * speed);
            auto move = cocos2d::RotateTo::create(rotateDuration, rotateDegrees);
            auto callback = cocos2d::CallFunc::create([this]() { this->onFire(); });
            auto action = cocos2d::Sequence::create(move, callback, nullptr);
            weapon->runAction(action);
        }
    }

    // 子弹发射逻辑
    void onFire() override {
        CCLOG("Bottle::onFire() : 发射子弹！");
        // 添加子弹逻辑
    }
};

class BulletBase :public cocos2d::Sprite {
public:
    float speed;
    cocos2d::Vec2 target;

    CREATE_FUNC(BulletBase);
};




class ShitTower :public Tower {
public:
    virtual bool init() override {
        level = 1;
        value = 100;
        attack_power = 20;//待定
        attack_speed = 8;//待定
        attack_range = 192;
        return true;
    }
};

class SunTower :public Tower {
public:
    virtual bool init() override {
        level = 1;
        value = 180;
        attack_power = 100;//待定
        attack_speed = 8;//待定
        attack_range = 192;
        return true;
    }

};
