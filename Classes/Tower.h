#pragma once
#include"cocos2d.h"
#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "Monster.h"
#include <string>
extern std::vector<Monster*> monsters;
USING_NS_CC;
#define BOTTLEBULLET 0
#define SHITBULLET 1
#define SUNFLOWER 2

extern int isPause;//是否暂停
extern int coinNumber;//记录当前金币的数量

class Bullet : public cocos2d::Sprite {
public:
    static Bullet* create(const std::string& fileName, cocos2d::Vec2 startPos, cocos2d::Vec2 targetPos, float speed,int Type,int level) {
        Bullet* ret = new Bullet();
        if (ret && ret->initWithFile(fileName)) {
            ret->autorelease();
            ret->startPosition = startPos;
            ret->targetPosition = targetPos;
            ret->speed = speed;
            ret->BulletType = Type-1;
            ret->damage = 10*level;
            ret->speedcuttime = 0.5f*level;
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
    int GetType() { return BulletType; }


private:
    cocos2d::Vec2 startPosition;   // 起点位置
    cocos2d::Vec2 targetPosition;  // 目标位置
    float speed;     // 子弹飞行速度
    int damage;
    float speedcuttime;
    int BulletType;
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
private:
    cocos2d::Sprite* currentTarget = nullptr;  // 当前目标敌人
    bool isLockedTarget = false;  // 是否锁定目标，防止频繁切换目标
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
        ImageBase = "bottle";
        value = 100;
        Type = 1;
        cost = 100;
        attack_power = 50;
        attack_speed = 2;
        attack_range = 196;
    }
    int getType() { return Type; }
    virtual bool init() override {

        this->scheduleUpdate();  // 定期调用 update 函数

        // 每 2 秒调用一次 task 函数
        schedule([this](float deltaTime) {
            this->shoot();
            }, 0.8f, "shoot");  // 注册定时任务
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

        findNearestEnemy(monsters);  // 寻找最近的敌人
        if (currentTarget) {
            rotateTowardsEnemy();  // 面向最近的敌人
        }
    }
    void findNearestEnemy(const std::vector<Monster*>& enemies) {
        // 如果目标已锁定，检查目标是否在攻击范围内
        if (isLockedTarget && currentTarget) {
            cocos2d::Vec2 towerPos = this->getPosition();
            float distanceToTarget = towerPos.distance(currentTarget->getPosition());

            // 如果目标超出了攻击范围，取消锁定目标
            if (distanceToTarget > attack_range) {
                currentTarget = nullptr;  // 取消当前目标
                isLockedTarget = false;   // 取消锁定状态
            }
            else {
                return;  // 如果目标在攻击范围内，不做改变
            }
        }

        // 如果没有锁定目标或当前目标不在攻击范围内，重新寻找最近的敌人
        float minDistance = attack_range;  // 限制攻击范围
        cocos2d::Sprite* closestEnemy = nullptr;
        cocos2d::Vec2 towerPos = this->getPosition();

        for (auto enemy : enemies) {
            cocos2d::Vec2 enemyPos = enemy->getPosition();
            float distance = towerPos.distance(enemyPos);
            if (distance < minDistance) {
                minDistance = distance;
                closestEnemy = enemy;
            }
        }

        if (closestEnemy) {
            currentTarget = closestEnemy;  // 更新锁定目标
            isLockedTarget = true;         // 锁定目标
        }
    }
    // 修改 rotateTowardsEnemy 函数
    void rotateTowardsEnemy() {
        if (currentTarget) {
            // 获取防御塔的位置
            cocos2d::Vec2 towerPos = this->getPosition();
            // 获取当前目标敌人的位置
            cocos2d::Vec2 enemyPos = currentTarget->getPosition();

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
    void shoot() {
        if (currentTarget) {
            // 获取敌人的位置
            cocos2d::Vec2 enemyPos = currentTarget->getPosition();
            // 获取塔的位置
            cocos2d::Vec2 towerPos = this->getPosition();
            // 创建子弹
            auto bullet = Bullet::create("/GamePlayScene/bottle_level_"+ std::to_string(level) + "_bullet.png", towerPos, enemyPos, 600.0f,Type,level);
            this->getParent()->addChild(bullet,3); // 将子弹添加到场景中
        }
    }

};





class ShitTower : public Tower {
private:
    cocos2d::Sprite* currentTarget = nullptr;  // 当前目标敌人
    bool isLockedTarget = false;  // 是否锁定目标，防止频繁切换目标
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
        this->scheduleUpdate();  // 定期调用 update 函数

        // 每 0.8 秒调用一次 shoot 函数
        schedule([this](float deltaTime) {
            this->shoot();
            }, 0.8f, "shoot");

        return true;
    }

    void upgrade() {
        if (level < 3) {  // 假设最多升级到3级
            level++;

            // 根据等级提升属性
            attack_power *= 1.2f;  // 攻击力提升20%
            attack_range *= 1.1f;  // 攻击范围提升10%
            attack_speed *= 0.9f;  // 攻击速度提升（减少）
            value += 80;  // 升级所需的价值（假设每级增加80）
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
        return value + 80;  // 升级费用
    }

    int getsellPrice() const override {
        return cost * 0.8;  // 卖出价格
    }

    void update(float dt) {
        findNearestEnemy(monsters);  // 寻找最近的敌人
    }

    void findNearestEnemy(const std::vector<Monster*>& enemies) {
        // 如果目标已锁定，检查目标是否在攻击范围内
        if (isLockedTarget && currentTarget) {
            cocos2d::Vec2 towerPos = this->getPosition();
            float distanceToTarget = towerPos.distance(currentTarget->getPosition());

            // 如果目标超出了攻击范围，取消锁定目标
            if (distanceToTarget > attack_range) {
                currentTarget = nullptr;  // 取消当前目标
                isLockedTarget = false;   // 取消锁定状态
            }
            else {
                return;  // 如果目标在攻击范围内，不做改变
            }
        }

        // 如果没有锁定目标或当前目标不在攻击范围内，重新寻找最近的敌人
        float minDistance = attack_range;  // 限制攻击范围
        cocos2d::Sprite* closestEnemy = nullptr;
        cocos2d::Vec2 towerPos = this->getPosition();

        for (auto enemy : enemies) {
            cocos2d::Vec2 enemyPos = enemy->getPosition();
            float distance = towerPos.distance(enemyPos);
            if (distance < minDistance) {
                minDistance = distance;
                closestEnemy = enemy;
            }
        }

        if (closestEnemy) {
            currentTarget = closestEnemy;  // 更新锁定目标
            isLockedTarget = true;         // 锁定目标
        }
    }

    void shoot() {
        if (currentTarget) {
            // 获取敌人的位置
            cocos2d::Vec2 enemyPos = currentTarget->getPosition();
            // 获取塔的位置
            cocos2d::Vec2 towerPos = this->getPosition();

            // 创建子弹
            auto bullet = Bullet::create("/GamePlayScene/shit_level_" + std::to_string(level) + "_bullet.png", towerPos, enemyPos, 600.0f, Type, level);

            // 计算子弹旋转角度
            cocos2d::Vec2 direction = enemyPos - towerPos;
            float angle = CC_RADIANS_TO_DEGREES(direction.getAngle());

            // 设置子弹的旋转角度
            bullet->setRotation(-angle);

            // 将子弹添加到场景中
            this->getParent()->addChild(bullet,3);
        }
    }
};







class SunflowerTower : public Tower {
private:
    bool isAttacking = false;  // 是否正在攻击
    float scaleFactor = 1.0f; // 子弹的缩放因子
    cocos2d::Sprite* currentTarget = nullptr;  // 当前目标敌人
    bool isLockedTarget = false;  // 是否锁定目标，防止频繁切换目标
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
        attack_range = 200;  // 假设攻击范围为100
    }

    int getType() { return Type; }

    virtual bool init() override {
        this->scheduleUpdate(); // 定期调用 update 函数
        // 每 2 秒调用一次 task 函数
        schedule([this](float deltaTime) {
            if (currentTarget)
            {
                startAttack(); // 开始攻击
            }
            else return;
       
        }, 1.2f, "attack");  // 注册定时任务
        return true;
    }

    void upgrade() {
        if (level < 3) {  // 假设最多升级到3级
            level++;

            // 根据等级提升属性
            attack_power *= 1.2f;  // 攻击力提升20%
            attack_range *= 1.2f;  // 攻击范围提升10%
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
        return value + 100;  // 举例：假设 ShitTower 的升级费用是当前价值 + 100
    }

    int getsellPrice() const override {
        return cost * 0.8;  // 举例：假设售价是成本的80%
    }

    void update(float dt) {
        findNearestEnemy(monsters); // 寻找最近的敌人

 
    }

    void startAttack() {
        isAttacking = true;
        auto bullet = cocos2d::Sprite::create("GamePlayScene/sunflower_level_1_bullet.png");
        bullet->setPosition(this->getPosition());
        bullet->setScale(1000.0f);
        this->getParent()->addChild(bullet,3);

        auto scaleUp = cocos2d::ScaleTo::create(0.5f, 2.0f * (attack_range/100-0.8)); // 根据等级放大
        auto fadeOut = cocos2d::FadeOut::create(0.5f);
        auto removeBullet = cocos2d::CallFunc::create([bullet]() {
            bullet->removeFromParent();
            });
        auto sequence = cocos2d::Sequence::create(scaleUp, fadeOut, removeBullet, nullptr);
        bullet->runAction(sequence);
    }

    void findNearestEnemy(const std::vector<Monster*>& enemies) {
        float minDistance = attack_range;  // 限制攻击范围
        cocos2d::Sprite* closestEnemy = nullptr;
        cocos2d::Vec2 towerPos = this->getPosition();

        for (auto enemy : enemies) {
            cocos2d::Vec2 enemyPos = enemy->getPosition();
            float distance = towerPos.distance(enemyPos);
            if (distance < minDistance) {
                minDistance = distance;
                closestEnemy = enemy;
            }
        }

        if (closestEnemy) {
            currentTarget = closestEnemy;  // 更新锁定目标
        }
        else {
            currentTarget = nullptr; // 如果没有目标，清空
        }
    }
};

