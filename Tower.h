#pragma once
#include"cocos2d.h"
#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "Monster.h"
#include <string>
extern std::vector<Monster*> monsters;
USING_NS_CC;
class Bullet : public cocos2d::Sprite {
public:
    static Bullet* create(const std::string& fileName, cocos2d::Vec2 startPos, cocos2d::Vec2 targetPos, float speed) {
        Bullet* ret = new Bullet();
        if (ret && ret->initWithFile(fileName)) {
            ret->autorelease();
            ret->startPosition = startPos;
            ret->targetPosition = targetPos;
            ret->speed = speed;
            ret->setPosition(startPos);
            ret->scheduleUpdate(); // 定期更新子弹位置
            return ret;
        }
        delete ret;
        return nullptr;
    }

    void update(float dt) override {
        // 子弹移动逻辑
        cocos2d::Vec2 direction = (targetPosition - startPosition).getNormalized();
        this->setPosition(this->getPosition() + direction * speed * dt);

        // 检查是否到达目标位置
        if (this->getPosition().distance(targetPosition) < 5.0f) {
            this->removeFromParent(); // 从父节点移除子弹
        }
    }



private:
    cocos2d::Vec2 startPosition;   // 起点位置
    cocos2d::Vec2 targetPosition;  // 目标位置
    float speed;                   // 子弹飞行速度
};


class Tower :public cocos2d::Sprite
{
protected:
    int Type;//种类
    int level;//当前等级
    int value;//当前价值
    int cost;//总花费
    float attack_power;//单次攻击力
    float attack_range;//攻击范围
    float attack_speed;
    cocos2d::Sprite* nearestEnemy; // 最近的敌人
    cocos2d::Vec2 fireTarget;
    std::string ImageBase;
    cocos2d::Sprite* weapon;  // 武器
public:
    // 初始化防御塔的属性
    Tower(const std::string& fileName) {
        initWithFile(fileName);
    }
    virtual void upgrade() {
        // 默认升级逻辑，可以为空，具体逻辑在派生类中定义
    }
    virtual int getUpgradeCost() const {
        return value + 50; // 假设升级费用是当前价值加上一个固定值
    }
    virtual int getsellPrice() const {
        return value + 50; // 假设升级费用是当前价值加上一个固定值
    }
    int getLevel() const { return level; }
    virtual void update(float dt) {
    }

};

class BottleTower : public Tower {
public:
    static BottleTower* create(const std::string& fileName) {
        BottleTower* ret = new BottleTower(fileName);
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        else {
            delete ret;
            return nullptr;
        }
    }

    BottleTower(const std::string& fileName) : Tower(fileName) {
        // 初始化属性
        level = 1;
        ImageBase = "Bottle";
        value = 100;
        Type = 1;
        cost = 100;
        attack_power = 50;
        attack_speed = 2;
        attack_range = 200;
    }
    int getType() { return Type; }
    virtual bool init() override {

        this->scheduleUpdate();  // 定期调用 update 函数
        return true;
    }
    void upgrade() {
        if (level < 3) {  // 假设最多升级到3级
            level++;

            // 根据等级提升属性
            attack_power *= 1.2f;  // 攻击力提升20%
            attack_range *= 1.1f;  // 攻击范围提升10%
            attack_speed *= 0.9f;  // 攻击速度提升（减少）
            value += 80;  // 升级所需的价值（假设每级增加50）
            cost += value;
            // 更新显示
            updateAppearance();
        }
    }

    // 更新防御塔的外观（例如图片变化）
    void updateAppearance() {
        std::string imagePath = "GamePlayScene/bottle_level_" + std::to_string(level) + ".png";
        this->setTexture(imagePath);  // 根据等级更新图片
    }
    int getUpgradeCost() const override {
        return value + 80;
    }
    int getsellPrice() const override {
        return cost * 0.8;
    }
    void update(float dt) {
        static float shootCooldown = 0; // 用于控制射击间隔
        shootCooldown -= dt; // 减少冷却时间

        findNearestEnemy(monsters); // 寻找最近的敌人

        if (nearestEnemy) {
            rotateTowardsEnemy(); // 面向最近的敌人
            if (shootCooldown <= 0) { // 冷却时间已到
                shoot(); // 发射子弹
                shootCooldown = attack_speed; // 重置冷却时间
            }
        }
    }
    void rotateTowardsEnemy() {
        if (nearestEnemy) {
            // 获取防御塔的位置
            cocos2d::Vec2 towerPos = this->getPosition();
            // 获取最近敌人的位置
            cocos2d::Vec2 enemyPos = nearestEnemy->getPosition();

            // 计算防御塔到敌人的方向向量
            cocos2d::Vec2 direction = enemyPos - towerPos;

            // 计算方向向量的角度（单位：弧度）
            float angle = direction.getAngle();  // 返回的是弧度，范围是[-pi, pi]

            // 将弧度转化为角度
            float angleInDegrees = CC_RADIANS_TO_DEGREES(angle);  // 转换为度数

            // 设置旋转角度，注意 cocos2d 的旋转是逆时针为正，所以角度保持原样即可
            this->setRotation(-angleInDegrees);  // 使防御塔旋转到目标方向
        }
    }
    void findNearestEnemy(const std::vector<Monster*>& enemies) {
        float minDistance = attack_range; // 设置最大范围为攻击范围
        cocos2d::Sprite* closestEnemy = nullptr;

        cocos2d::Vec2 towerPos = this->getPosition();

        for (auto enemy : enemies) {
            // 获取敌人位置
            cocos2d::Vec2 enemyPos = enemy->getPosition();

            // 计算防御塔与敌人的距离
            float distance = towerPos.distance(enemyPos);

            // 判断是否在攻击范围内且距离更近
            if (distance < minDistance) {
                minDistance = distance;
                closestEnemy = enemy;
            }
        }

        // 如果没有找到敌人，明确将 nearestEnemy 设置为 nullptr
        if (closestEnemy == nullptr) {
            nearestEnemy = nullptr;
        }
        else {
            nearestEnemy = closestEnemy;
        }
    }
    void shoot() {
        if (nearestEnemy) {
            // 获取敌人的位置
            cocos2d::Vec2 enemyPos = nearestEnemy->getPosition();
            // 获取塔的位置
            cocos2d::Vec2 towerPos = this->getPosition();
            // 创建子弹
            auto bullet = Bullet::create("/GamePlayScene/bottle_level_1_bullet.png", towerPos, enemyPos, 600.0f); // 速度假设为 300
            this->getParent()->addChild(bullet); // 将子弹添加到场景中
        }
    }

};





class ShitTower : public Tower {
public:
    static ShitTower* create(const std::string& fileName) {
        ShitTower* ret = new ShitTower(fileName);
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        else {
            delete ret;
            return nullptr;
        }
    }

    ShitTower(const std::string& fileName) : Tower(fileName) {
        // 初始化属性
        level = 1;
        ImageBase = "Shit";
        value = 100;  // 设定价值
        Type = 2;  // 设定类型
        cost = 100;
        attack_power = 70;  // 假设攻击力为70
        attack_speed = 6;  // 假设攻击速度为6
        attack_range = 150;  // 假设攻击范围为150
    }

    int getType() { return Type; }

    virtual bool init() override {
        return true;
    }

    void upgrade() {
        if (level < 3) {  // 假设最多升级到3级
            level++;

            // 根据等级提升属性
            attack_power *= 1.2f;  // 攻击力提升20%
            attack_range *= 1.1f;  // 攻击范围提升10%
            attack_speed *= 0.9f;  // 攻击速度提升（减少）
            value += 80;  // 升级所需的价值（假设每级增加60）
            cost += value;
            // 更新显示
            updateAppearance();
        }
    }

    // 更新防御塔的外观（例如图片变化）
    void updateAppearance() {
        std::string imagePath = "GamePlayScene/shit_level_" + std::to_string(level) + ".png";
        this->setTexture(imagePath);  // 根据等级更新图片
    }
    int getUpgradeCost() const override {
        return value + 80;  // 举例：假设 ShitTower 的升级费用是当前价值 + 60
    }
    int getsellPrice() const override {
        return cost * 0.8;  // 举例：假设 ShitTower 的升级费用是当前价值 + 60
    }

};


class SunflowerTower : public Tower {
public:
    static SunflowerTower* create(const std::string& fileName) {
        SunflowerTower* ret = new SunflowerTower(fileName);
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        else {
            delete ret;
            return nullptr;
        }
    }

    SunflowerTower(const std::string& fileName) : Tower(fileName) {
        // 初始化属性
        level = 1;
        ImageBase = "Sunflower";
        value = 200;  // 设定价值
        cost = 200;
        Type = 3;  // 设定类型
        attack_power = 20;  // 假设攻击力为20
        attack_speed = 5;  // 假设攻击速度为5
        attack_range = 100;  // 假设攻击范围为100
    }

    int getType() { return Type; }

    virtual bool init() override {
        return true;
    }

    void upgrade() {
        if (level < 3) {  // 假设最多升级到3级
            level++;

            // 根据等级提升属性
            attack_power *= 1.2f;  // 攻击力提升20%
            attack_range *= 1.1f;  // 攻击范围提升10%
            attack_speed *= 0.9f;  // 攻击速度提升（减少）
            value += 100;
            cost += value;

            // 更新显示
            updateAppearance();
        }
    }

    // 更新防御塔的外观（例如图片变化）
    void updateAppearance() {
        std::string imagePath = "GamePlayScene/sunflower_level_" + std::to_string(level) + ".png";
        this->setTexture(imagePath);  // 根据等级更新图片
    }
    int getUpgradeCost() const override {
        return value + 100;  // 举例：假设 ShitTower 的升级费用是当前价值 + 60
    }
    int getsellPrice() const override {
        return cost * 0.8;  // 举例：假设 ShitTower 的升级费用是当前价值 + 60
    }

};
