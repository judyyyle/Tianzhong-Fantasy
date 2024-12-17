#pragma once
#ifndef __TOWER_H__
#define __TOWER_H__
#include"cocos2d.h"
#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "Monster.h"
#include "Bullet.h"
#include <string>
extern std::vector<Monster*> monsters;
extern std::vector<Bullet*> bullets;
USING_NS_CC;
#define BOTTLEBULLET 0
#define SHITBULLET 1
#define SUNFLOWER 2

extern int isPause;//是否暂停
extern int coinNumber;//记录当前金币的数量


class Tower : public cocos2d::Sprite {
protected:
    int Type; // 种类
    int level; // 当前等级
    int value; // 当前价值
    int cost;  // 总花费
    float attack_power; // 单次攻击力
    float attack_range; // 攻击范围
    float attack_speed; // 攻击速度
    cocos2d::Sprite* nearestEnemy; // 最近的敌人
    cocos2d::Vec2 fireTarget; // 目标坐标
    std::string ImageBase; // 图片路径
    cocos2d::Sprite* weapon; // 武器

public:
    // 构造函数
    Tower(const std::string& fileName);

    // 升级塔的方法，虚函数，可在派生类中重写
    virtual void upgrade();

    // 获取升级花费
    virtual int getUpgradeCost() const;

    // 获取出售价格
    virtual int getsellPrice() const;

    // 获取等级
    int getLevel() const;

    // 每帧更新方法
    virtual void update(float dt);
};



class BottleTower : public Tower {
private:
    Monster* currentTarget = nullptr;  // 当前目标敌人
    bool isLockedTarget = false;  // 是否锁定目标，防止频繁切换目标

public:
    static BottleTower* create(const std::string& fileName);

    BottleTower(const std::string& fileName);

    virtual bool init() override;

    void upgrade();

    void updateAppearance();

    int getUpgradeCost() const override;

    int getsellPrice() const override;

    void update(float dt) override;

    void findNearestEnemy(const std::vector<Monster*>& enemies);

    void rotateTowardsEnemy();

    void shoot();

    int getType();
};



class ShitTower : public Tower {
private:
    Monster* currentTarget = nullptr;  // 当前目标敌人
    bool isLockedTarget = false;  // 是否锁定目标，防止频繁切换目标

public:
    static ShitTower* create(const std::string& fileName);

    ShitTower(const std::string& fileName);

    virtual bool init() override;

    void upgrade();

    void updateAppearance();

    int getUpgradeCost() const override;

    int getsellPrice() const override;

    void update(float dt) override;

    void findNearestEnemy(const std::vector<Monster*>& enemies);

    void shoot();

    int getType();
};



class SunflowerTower : public Tower {
private:
    bool isAttacking = false;  // 是否正在攻击
    float scaleFactor = 1.0f;  // 子弹的缩放因子
    cocos2d::Sprite* currentTarget = nullptr;  // 当前目标敌人
    bool isLockedTarget = false;  // 是否锁定目标，防止频繁切换目标

public:
    static SunflowerTower* create(const std::string& fileName);

    SunflowerTower(const std::string& fileName);

    virtual bool init() override;

    void upgrade();

    void updateAppearance();

    int getUpgradeCost() const override;

    int getsellPrice() const override;

    void update(float dt) override;

    void startAttack();

    void findNearestEnemy(const std::vector<Monster*>& enemies);

    int getType();
};

#endif // __TOWER_H__
