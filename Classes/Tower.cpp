#include "Tower.h"
// 构造函数：初始化防御塔的图片

Tower::Tower(const std::string& fileName) {
    initWithFile(fileName);
    level = 1;    // 默认等级为1
    value = 100;  // 默认价值
    cost = 100;   // 默认花费
    attack_power = 10.0f;  // 默认攻击力
    attack_range = 150.0f; // 默认攻击范围
    attack_speed = 1.0f;   // 默认攻击速度
    nearestEnemy = nullptr;
}

// 升级防御塔：默认实现为空，具体逻辑由子类重写
void Tower::upgrade() {
    // 默认升级逻辑：增加等级和攻击力
    level++;
    attack_power += 5.0f;
    cost += 50; // 假设升级增加50成本
}

// 获取升级花费
int Tower::getUpgradeCost() const {
    return value + 50; // 假设升级费用是当前价值加50
}

// 获取出售价格
int Tower::getsellPrice() const {
    return value / 2; // 假设出售价格是当前价值的一半
}

// 获取当前等级
int Tower::getLevel() const {
    return level;
}

// 每帧更新方法：默认实现为空
void Tower::update(float dt) {
    if (isPause) {
        return;
    }
    // 子类可以重写此方法，实现具体逻辑
}
void Tower::findNearestEnemy() {

    if (clickedMonster && clickedMonster->isRemoved == false) {
        cocos2d::Vec2 towerPos = this->getPosition();
        float distanceToclickedMonster = towerPos.distance(clickedMonster->getPosition());

        if (distanceToclickedMonster <= attack_range) {
            currentTarget = clickedMonster;
            return;
        }
    }
    float minDistance = attack_range;  // 限制攻击范围
    Monster* closestEnemy = nullptr;
    cocos2d::Vec2 towerPos = this->getPosition();
    // 如果没有锁定目标或当前目标不在攻击范围内，重新寻找最近的敌人
    for (auto enemy : monsters) {
        cocos2d::Vec2 enemyPos = enemy->getPosition();
        float distance = towerPos.distance(enemyPos);
        if (distance < minDistance) {
            minDistance = distance;
            closestEnemy = enemy;
        }
    }
    //if(currentTarget->getParent()==NULL)
    //if (currentTarget->isRemoved)
    if (!isLockedTarget || currentTarget == NULL || currentTarget->isRemoved == true) {
        currentTarget = closestEnemy;  // 更新锁定目标
    }
    if (closestEnemy) {
        isLockedTarget = true;         // 锁定目标
    }
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
}


BottleTower* BottleTower::create(const std::string& fileName) {
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

BottleTower::BottleTower(const std::string& fileName) : Tower(fileName) {
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
int  BottleTower::getType() { return Type; }
bool  BottleTower::init() {

    this->scheduleUpdate();  // 定期调用 update 函数

    schedule([this](float deltaTime) {
        if (isPause) {
            return;
        }
        this->shoot();
        }, 0.8f, "shoot");  // 注册定时任务
    return true;
}
void  BottleTower::upgrade() {
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
void  BottleTower::updateAppearance() {
    std::string imagePath = "GamePlayScene/bottle_level_" + std::to_string(level) + ".png";
    this->setTexture(imagePath);  // 根据等级更新图片
}
int  BottleTower::getUpgradeCost() const {
    return value + 80;
}
int  BottleTower::getsellPrice() const {
    return cost * 0.8;
}
void  BottleTower::update(float dt) {
    if (isPause) {
        return;
    }
    findNearestEnemy();  // 寻找最近的敌人
    if (currentTarget|| barrierManager->selectedBarrier && barrierManager->selectedBarrier->isDead() == false) {
        rotateTowardsEnemy();  // 面向最近的敌人
    }
}

// 修改 rotateTowardsEnemy 函数
void  BottleTower::rotateTowardsEnemy() {
    if (barrierManager->selectedBarrier&& barrierManager->selectedBarrier->isDead() == false) {
        // 获取防御塔的位置
        cocos2d::Vec2 towerPos = this->getPosition();
        float distanceToBarrier = towerPos.distance(barrierManager->selectedBarrier->barrierSprite->getPosition());

        if (distanceToBarrier <= attack_range) {
            cocos2d::Vec2 towerPos = this->getPosition();
            // 获取当前目标敌人的位置
            cocos2d::Vec2 enemyPos = barrierManager->selectedBarrier->barrierSprite->getPosition();

            // 计算防御塔到敌人的方向向量
            cocos2d::Vec2 direction = enemyPos - towerPos;

            // 计算方向向量的角度（单位：弧度）
            float angle = direction.getAngle();  // 返回的是弧度，范围是[-pi, pi]

            // 将弧度转化为角度
            float angleInDegrees = CC_RADIANS_TO_DEGREES(angle);  // 转换为度数

            // 设置旋转角度，注意 cocos2d 的旋转是逆时针为正，所以角度保持原样即可
            this->setRotation(-angleInDegrees);  // 使防御塔旋转到目标方向
            return;
        }
        
    }
    if (currentTarget&&currentTarget->isRemoved ==false) {
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
        return;
    }
}

void  BottleTower::shoot() {
    // 加载图片
    auto frame1 = cocos2d::Sprite::create("/GamePlayScene/bottle_level_" + std::to_string(level) + ".png");
    auto frame2 = cocos2d::Sprite::create("/GamePlayScene/bottle_level_" + std::to_string(level) + "_fire.png");
    auto frame3 = cocos2d::Sprite::create("/GamePlayScene/bottle_level_" + std::to_string(level) + "_fire2.png");
    cocos2d::Size frame1Size = frame1->getContentSize();
    cocos2d::Size frame2Size = frame2->getContentSize();
    cocos2d::Size frame3Size = frame3->getContentSize();
    cocos2d::Vector<cocos2d::SpriteFrame*> bottleanimFrames;
    Animation* bottleanimation;
    Animate* bottleanimate;
    bottleanimFrames.reserve(2);
    bottleanimFrames.pushBack(cocos2d::SpriteFrame::create("/GamePlayScene/bottle_level_" + std::to_string(level) + "_fire.png", cocos2d::Rect(0, 0, frame2Size.width, frame2Size.height)));
    bottleanimFrames.pushBack(cocos2d::SpriteFrame::create("/GamePlayScene/bottle_level_" + std::to_string(level) + "_fire2.png", cocos2d::Rect(0, 0, frame3Size.width, frame3Size.height)));
    bottleanimFrames.pushBack(cocos2d::SpriteFrame::create("/GamePlayScene/bottle_level_" + std::to_string(level) + ".png", cocos2d::Rect(0, 0, frame1Size.width, frame1Size.height)));

    bottleanimation = Animation::createWithSpriteFrames(bottleanimFrames, 0.2f);
    bottleanimate = Animate::create(bottleanimation);
    auto repeatAction = cocos2d::RepeatForever::create(bottleanimate);
    if (barrierManager->selectedBarrier && barrierManager->selectedBarrier->isDead() == false) {
        this->runAction(repeatAction->clone());
        // 获取防御塔的位置
        cocos2d::Vec2 towerPos = this->getPosition();
        float distanceToBarrier = towerPos.distance(barrierManager->selectedBarrier->barrierSprite->getPosition());

        if (distanceToBarrier <= attack_range) {
            // 获取敌人的位置
            cocos2d::Vec2 enemyPos = barrierManager->selectedBarrier->barrierSprite->getPosition();
            // 获取塔的位置
            cocos2d::Vec2 towerPos = this->getPosition();
            // 创建子弹
            auto bullet = Bullet::create("/GamePlayScene/bottle_level_" + std::to_string(level) + "_bullet.png", towerPos, enemyPos, 600.0f, Type, level);
            this->getParent()->addChild(bullet, 3); // 将子弹添加到场景中
            bulletsTowardBarrier.push_back(bullet);
            return;
        }

    }
    if (currentTarget && currentTarget->isRemoved == false) {
        this->runAction(repeatAction->clone());
        // 获取敌人的位置
        cocos2d::Vec2 enemyPos = currentTarget->getPosition();
        // 获取塔的位置
        cocos2d::Vec2 towerPos = this->getPosition();
        // 创建子弹
        auto bullet = Bullet::create("/GamePlayScene/bottle_level_" + std::to_string(level) + "_bullet.png", towerPos, enemyPos, 600.0f, Type, level);
        this->getParent()->addChild(bullet, 3); // 将子弹添加到场景中
        bullets.push_back(bullet);
        return;
    }
    else
    {
        this->stopAllActions();
        this->setTexture("/GamePlayScene/bottle_level_" + std::to_string(level) + ".png");
    }
}




ShitTower* ShitTower::create(const std::string& fileName) {
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

ShitTower::ShitTower(const std::string& fileName) : Tower(fileName) {
    // 初始化属性
    level = 1;
    ImageBase = "shit";
    value = 100;  // 设定价值
    Type = 2;  // 设定类型
    cost = 100;
    attack_power = 70;  // 假设攻击力为70
    attack_speed = 6;  // 假设攻击速度为6
    attack_range = 150;  // 假设攻击范围为150

    //this->Sprite::create(fileName);
}

int ShitTower::getType() { return Type; }

bool ShitTower::init() {
    this->scheduleUpdate();  // 定期调用 update 函数

    // 每 0.8 秒调用一次 shoot 函数
    schedule([this](float deltaTime) {
        if (isPause) {
            return;
        }
        this->shoot();
        }, 0.8f, "shoot");

    return true;
}

void ShitTower::upgrade() {
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
void ShitTower::updateAppearance() {
    std::string imagePath = "GamePlayScene/shit_level_" + std::to_string(level) + ".png";
    this->setTexture(imagePath);  // 根据等级更新图片
}

int ShitTower::getUpgradeCost() const {
    return value + 80;  // 升级费用
}

int ShitTower::getsellPrice() const {
    return cost * 0.8;  // 卖出价格
}

void ShitTower::update(float dt) {
    if (isPause) {
        this->stopAllActions();
        this->setTexture("/GamePlayScene/shit_level_" + std::to_string(level) + ".png");
        return;
    }
    findNearestEnemy();  // 寻找最近的敌人
}



void ShitTower::shoot() {
    // 加载图片
    auto frame1 = cocos2d::Sprite::create("/GamePlayScene/shit_level_" + std::to_string(level) + ".png");
    auto frame2 = cocos2d::Sprite::create("/GamePlayScene/shit_level_" + std::to_string(level) + "_fire.png");

    cocos2d::Size frame1Size = frame1->getContentSize();
    cocos2d::Size frame2Size = frame2->getContentSize();

    cocos2d::Vector<cocos2d::SpriteFrame*> shitanimFrames;
    Animation* shitanimation;
    Animate* shitanimate;
    shitanimFrames.reserve(2);

    shitanimFrames.pushBack(cocos2d::SpriteFrame::create("/GamePlayScene/shit_level_" + std::to_string(level) + "_fire.png", cocos2d::Rect(0, 0, frame2Size.width, frame2Size.height)));
    shitanimFrames.pushBack(cocos2d::SpriteFrame::create("/GamePlayScene/shit_level_" + std::to_string(level) + ".png", cocos2d::Rect(0, 0, frame1Size.width, frame1Size.height)));
    shitanimation = Animation::createWithSpriteFrames(shitanimFrames, 0.4f);
    shitanimate = Animate::create(shitanimation);
    auto repeatAction = cocos2d::RepeatForever::create(shitanimate);
    if (barrierManager->selectedBarrier&& barrierManager->selectedBarrier->barrierSprite && barrierManager->selectedBarrier->isDead() == false) {
        // 获取防御塔的位置
        cocos2d::Vec2 towerPos = this->getPosition();
        float distanceToBarrier = towerPos.distance(barrierManager->selectedBarrier->barrierSprite->getPosition());

        if (distanceToBarrier <= attack_range) {
            this->runAction(repeatAction->clone());
            // 获取敌人的位置
            cocos2d::Vec2 enemyPos = barrierManager->selectedBarrier->barrierSprite->getPosition();
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
            this->getParent()->addChild(bullet, 3);
            bulletsTowardBarrier.push_back(bullet);
            return;
        }
            
        
    }
    if (currentTarget && currentTarget->isRemoved == false) {

        this->runAction(repeatAction->clone());
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
        this->getParent()->addChild(bullet, 3);
        bullets.push_back(bullet);
    }
    else
    {
        this->stopAllActions();
        this->setTexture("/GamePlayScene/shit_level_" + std::to_string(level) + ".png");
    }
}


SunflowerTower* SunflowerTower::create(const std::string& fileName) {
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

SunflowerTower::SunflowerTower(const std::string& fileName) : Tower(fileName) {
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

int SunflowerTower::getType() { return Type; }

bool SunflowerTower::init() {
    this->scheduleUpdate(); // 定期调用 update 函数
    // 每 1.2 秒调用一次 attack 函数
    schedule([this](float deltaTime) {
        if (isPause) {
            return;
        }
        if (barrierManager->selectedBarrier && barrierManager->selectedBarrier->barrierSprite&& barrierManager->selectedBarrier->isDead() == false)
        {
            cocos2d::Vec2 towerPos = this->getPosition();
            float distanceToBarrier = towerPos.distance(barrierManager->selectedBarrier->barrierSprite->getPosition());

            if (distanceToBarrier <= attack_range) {
                startAttack(); // 开始攻击
            }

        }
        if (currentTarget && currentTarget->isRemoved == false)
        {
            startAttack(); // 开始攻击
        }
        else return;

        }, 1.2f, "attack");  // 注册定时任务
    return true;
}

void SunflowerTower::upgrade() {
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
void SunflowerTower::updateAppearance() {
    std::string imagePath = "GamePlayScene/sunflower_level_" + std::to_string(level) + ".png";
    this->setTexture(imagePath);  // 根据等级更新图片
}

int SunflowerTower::getUpgradeCost() const {
    return value + 100;
}

int SunflowerTower::getsellPrice() const {
    return cost * 0.8;
}

void SunflowerTower::update(float dt) {
    if (isPause) {
        return;
    }
    findNearestEnemy(); // 寻找最近的敌人


}

void SunflowerTower::startAttack() {
    isAttacking = true;
    auto bullet = Sunflowerfire::create("GamePlayScene/sunflower_level_1_bullet.png", level);
    sunflowers.push_back(bullet);
    bullet->setPosition(this->getPosition());
    //bullet->setScale(1000.0f);
    this->getParent()->addChild(bullet, 3);
    auto scaleUp = cocos2d::ScaleTo::create(0.3f, 2.0f * (attack_range / 100 - 0.8)); // 根据等级放大
    auto fadeOut = cocos2d::FadeOut::create(0.5f);
    auto removeBullet = cocos2d::CallFunc::create([bullet]() {
        bullet->removeFromParent();
        auto it = std::find(sunflowers.begin(), sunflowers.end(), bullet);
        if (it != sunflowers.end()) {
            sunflowers.erase(it);
        }
        });
    auto sequence = cocos2d::Sequence::create(scaleUp, fadeOut, removeBullet, nullptr);
    bullet->runAction(sequence);
}

void SunflowerTower::findNearestEnemy() {
    float minDistance = attack_range;  // 限制攻击范围
    Monster* closestEnemy = nullptr;
    cocos2d::Vec2 towerPos = this->getPosition();


    if (clickedMonster && clickedMonster->isRemoved == false) {
        cocos2d::Vec2 towerPos = this->getPosition();
        float distanceToclickedMonster = towerPos.distance(clickedMonster->getPosition());

        if (distanceToclickedMonster <= attack_range) {
            currentTarget = clickedMonster;
            return;
        }
    }
    for (auto enemy : monsters) {
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
