#pragma once
#include"cocos2d.h"
#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include <string>
USING_NS_CC;
class Tower :public cocos2d::Sprite
{
protected:
    int Type;//����
    int level;//��ǰ�ȼ�
    int value;//��ǰ��ֵ
    int cost;//�ܻ���
    float attack_power;//���ι�����
    float attack_range;//������Χ
    float attack_speed;
    cocos2d::Sprite* nearestEnemy; // ����ĵ���
    cocos2d::Vec2 fireTarget;
    std::string ImageBase;
    cocos2d::Sprite* weapon;  // ����
public:
    // ��ʼ��������������
    Tower(const std::string& fileName) {
        initWithFile(fileName);
    }
    virtual void upgrade() {
        // Ĭ�������߼�������Ϊ�գ������߼����������ж���
    }
    virtual int getUpgradeCost() const {
        return value + 50; // �������������ǵ�ǰ��ֵ����һ���̶�ֵ
    }
    virtual int getsellPrice() const {
        return value + 50; // �������������ǵ�ǰ��ֵ����һ���̶�ֵ
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
        // ��ʼ������
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
        if (level < 3) {  // �������������3��
            level++;

            // ���ݵȼ���������
            attack_power *= 1.2f;  // ����������20%
            attack_range *= 1.1f;  // ������Χ����10%
            attack_speed *= 0.9f;  // �����ٶ����������٣�
            value += 80;  // ��������ļ�ֵ������ÿ������50��
            cost += value;
            // ������ʾ
            updateAppearance();
        }
    }

    // ���·���������ۣ�����ͼƬ�仯��
    void updateAppearance() {
        std::string imagePath = "GamePlayScene/bottle_level_" + std::to_string(level) + ".png";
        this->setTexture(imagePath);  // ���ݵȼ�����ͼƬ
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
        // ��ʼ������
        level = 1;
        ImageBase = "Shit";
        value = 100;  // �趨��ֵ
        Type = 2;  // �趨����
        cost = 100;
        attack_power = 70;  // ���蹥����Ϊ70
        attack_speed = 6;  // ���蹥���ٶ�Ϊ6
        attack_range = 150;  // ���蹥����ΧΪ150
    }

    int getType() { return Type; }

    virtual bool init() override {
        return true;
    }

    void upgrade() {
        if (level < 3) {  // �������������3��
            level++;

            // ���ݵȼ���������
            attack_power *= 1.2f;  // ����������20%
            attack_range *= 1.1f;  // ������Χ����10%
            attack_speed *= 0.9f;  // �����ٶ����������٣�
            value += 80;  // ��������ļ�ֵ������ÿ������60��
            cost += value;
            // ������ʾ
            updateAppearance();
        }
    }

    // ���·���������ۣ�����ͼƬ�仯��
    void updateAppearance() {
        std::string imagePath = "GamePlayScene/shit_level_" + std::to_string(level) + ".png";
        this->setTexture(imagePath);  // ���ݵȼ�����ͼƬ
    }
    int getUpgradeCost() const override {
        return value + 80;  // ���������� ShitTower �����������ǵ�ǰ��ֵ + 60
    }
    int getsellPrice() const override {
        return cost*0.8;  // ���������� ShitTower �����������ǵ�ǰ��ֵ + 60
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
        // ��ʼ������
        level = 1;
        ImageBase = "Sunflower";
        value = 200;  // �趨��ֵ
        cost = 200;
        Type = 3;  // �趨����
        attack_power = 20;  // ���蹥����Ϊ20
        attack_speed = 5;  // ���蹥���ٶ�Ϊ5
        attack_range = 100;  // ���蹥����ΧΪ100
    }

    int getType() { return Type; }

    virtual bool init() override {
        return true;
    }

    void upgrade() {
        if (level < 3) {  // �������������3��
            level++;

            // ���ݵȼ���������
            attack_power *= 1.2f;  // ����������20%
            attack_range *= 1.1f;  // ������Χ����10%
            attack_speed *= 0.9f;  // �����ٶ����������٣�
            value += 100;  
            cost += value;

            // ������ʾ
            updateAppearance();
        }
    }

    // ���·���������ۣ�����ͼƬ�仯��
    void updateAppearance() {
        std::string imagePath = "GamePlayScene/sunflower_level_" + std::to_string(level) + ".png";
        this->setTexture(imagePath);  // ���ݵȼ�����ͼƬ
    }
    int getUpgradeCost() const override {
        return value + 100;  // ���������� ShitTower �����������ǵ�ǰ��ֵ + 60
    }
    int getsellPrice() const override {
        return cost*0.8;  // ���������� ShitTower �����������ǵ�ǰ��ֵ + 60
    }

};
