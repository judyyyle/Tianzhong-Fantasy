#include "Bullet.h"
extern std::vector<Bullet*> bullets;

// ��̬�������������ڴ��� Bullet ����
Bullet* Bullet::create(const std::string& fileName, cocos2d::Vec2 startPos, cocos2d::Vec2 targetPos, float speed, int Type, int level) {
    Bullet* ret = new Bullet();
    if (ret && ret->initWithFile(fileName)) {
        ret->autorelease();
        ret->startPosition = startPos;
        ret->targetPosition = targetPos;
        ret->speed = speed;
        ret->BulletType = Type - 1;  // Type ֵ�� 1 ��Ӧ BulletType
        ret->damage = 10 * level;   // �ӵ��˺����ŵȼ�����
        ret->speedcuttime = (Type == 2 ? 0.5f * (level+3) : 0);  // ��������� 2���ٶȼ���
        ret->setPosition(startPos);  // ���ó�ʼλ��
        ret->scheduleUpdate();       // ���ڸ����ӵ�λ��
        return ret;
    }
    delete ret;
    return nullptr;  // �����ʼ��ʧ�ܣ����ؿ�ָ��
}

// �ӵ����º����������ӵ��ķ��к�����
void Bullet::update(float dt) {
    // �����ӵ����еķ��򲢽����ƶ�
    cocos2d::Vec2 direction = (targetPosition - startPosition).getNormalized();
    this->setPosition(this->getPosition() + direction * speed * dt);

   /* // ����Ƿ񵽴�Ŀ��λ��
    if (this->getPosition().distance(targetPosition) < 5.0f) {
        this->removeFromParent();  // �Ӹ��ڵ��Ƴ��ӵ�
        // �� bullets �������Ƴ�����
        auto it = std::find(bullets.begin(), bullets.end(), this);
        if (it != bullets.end()) {
            bullets.erase(it);
        }
    }*/
}

// ��ȡ�ӵ�����
int Bullet::GetType() {
    return BulletType;
}
// ��ȡ����ʱ��
float Bullet::GetTime() {
    return speedcuttime;
}

// ��ȡ�ӵ��˺�
int Bullet::GetDamage() {
    return damage;
}