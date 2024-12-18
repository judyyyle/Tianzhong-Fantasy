#include "Bullet.h"
extern std::vector<Bullet*> bullets;

// 静态工厂方法，用于创建 Bullet 对象
Bullet* Bullet::create(const std::string& fileName, cocos2d::Vec2 startPos, cocos2d::Vec2 targetPos, float speed, int Type, int level) {
    Bullet* ret = new Bullet();
    if (ret && ret->initWithFile(fileName)) {
        ret->autorelease();
        ret->startPosition = startPos;
        ret->targetPosition = targetPos;
        ret->speed = speed;
        ret->BulletType = Type - 1;  // Type 值减 1 对应 BulletType
        ret->damage = 10 * level;   // 子弹伤害随着等级增加
        ret->speedcuttime = (Type == 2 ? 0.5f * (level+3) : 0);  // 如果是类型 2，速度减少
        ret->setPosition(startPos);  // 设置初始位置
        ret->scheduleUpdate();       // 定期更新子弹位置
        return ret;
    }
    delete ret;
    return nullptr;  // 如果初始化失败，返回空指针
}

// 子弹更新函数，控制子弹的飞行和销毁
void Bullet::update(float dt) {
    // 计算子弹飞行的方向并进行移动
    cocos2d::Vec2 direction = (targetPosition - startPosition).getNormalized();
    this->setPosition(this->getPosition() + direction * speed * dt);

   /* // 检查是否到达目标位置
    if (this->getPosition().distance(targetPosition) < 5.0f) {
        this->removeFromParent();  // 从父节点移除子弹
        // 从 bullets 容器中移除自身
        auto it = std::find(bullets.begin(), bullets.end(), this);
        if (it != bullets.end()) {
            bullets.erase(it);
        }
    }*/
}

// 获取子弹类型
int Bullet::GetType() {
    return BulletType;
}
// 获取减速时间
float Bullet::GetTime() {
    return speedcuttime;
}

// 获取子弹伤害
int Bullet::GetDamage() {
    return damage;
}