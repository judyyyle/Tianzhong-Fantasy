#pragma once
#include"cocos2d.h"
#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include <string>
USING_NS_CC;
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
        attack_speed = 8;
        attack_range = 192;
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
        return cost*0.8;  
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
        return cost*0.8;  // 举例：假设 ShitTower 的升级费用是当前价值 + 60
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
        return cost*0.8;  // 举例：假设 ShitTower 的升级费用是当前价值 + 60
    }

};
