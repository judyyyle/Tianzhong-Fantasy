#pragma once
#include "cocos2d.h"
#include <vector>
#include <algorithm>
class Bullet : public cocos2d::Sprite {
public:
    static Bullet* create(const std::string& fileName, cocos2d::Vec2 startPos, cocos2d::Vec2 targetPos, float speed, int Type, int level);

    virtual void update(float dt) override;

    int GetType();  // ��ȡ�ӵ�����
    float GetTime(); // ��ȡ����ʱ��
    int GetDamage(); // ��ȡ�ӵ��˺�
protected:
    cocos2d::Vec2 startPosition;   // ���λ��
    cocos2d::Vec2 targetPosition;  // Ŀ��λ��
    float speed;                   // �ӵ������ٶ�
    int damage;                    // �ӵ��˺�
    float speedcuttime;            // �������͵��ٶȼ���ʱ��
    int BulletType;                // �ӵ�����
};


class Sunflowerfire :public Bullet {
public:
    static Sunflowerfire* create(const std::string& fileName ,  int level) {
        Sunflowerfire* ret = new Sunflowerfire();
        if (ret && ret->initWithFile(fileName)) {
            ret->autorelease();
            ret->damage = 20 * level;   // �ӵ��˺����ŵȼ�����
            return ret;
        }
        delete ret;
        return nullptr;  // �����ʼ��ʧ�ܣ����ؿ�ָ��
    }

};