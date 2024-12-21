#include "HelloWorldScene.h"
#include "Monster.h"
#include <string>

/*******************************/
extern int carrot_HP;//萝卜血量
extern Carrot* globalCarrot;//萝卜
/*******************************/
extern int coinNumber;
extern std::vector<Bullet*> bullets;
extern std::vector<Sunflowerfire*>sunflowers;

//定义一个结构体，表示二维数组中的位置
static struct arr_mon {
    int row;
    int col;
};

//将二维网格坐标转换为世界坐标 (Vec2 类型)
static Vec2 arr_to_vec2_mon(int row, int col) {
    Vec2 vec;
    vec.x = 64 + 128 * col;
    vec.y = 1024 - 64 - 128 * row;
    return vec;
}

// Monster 类的初始化方法，继承自 Sprite
bool Monster::init() {
    if (!Sprite::init()) { //调用父类的初始化方法
        return false;
    }
    CCLOG("Monster initialized successfully!");

    //调用 Cocos2d 的定时器，定期更新怪物的位置
    this->scheduleUpdate();

    return true;
}

//初始化怪物的类型和路径
void Monster::initType(int m, int map_type) {
    monster_type = m;
    path_count = 0;
    isRemoved = false;
    selected = false;
    //根据地图类型 (map_type) 设置不同的路径
    switch (map_type) {
    case ADVENTURE1:
        path = new MapPath[8];
        path_total = 8;
        path[0] = { arr_to_vec2_mon(2,1),0,-1,DOWN };
        path[1] = { arr_to_vec2_mon(5,1),1,0,RIGHT };
        path[2] = { arr_to_vec2_mon(5,4),0,1,UP };
        path[3] = { arr_to_vec2_mon(4,4),1,0,RIGHT };
        path[4] = { arr_to_vec2_mon(4,7),0,-1,DOWN };
        path[5] = { arr_to_vec2_mon(5,7),1,0,RIGHT };
        path[6] = { arr_to_vec2_mon(5,10),0,1,UP };
        path[7] = { arr_to_vec2_mon(2,10),-2,-2,STOP };
        break;
    case ADVENTURE2:
        path = new MapPath[8];
        path_total = 8;
        path[0] = { arr_to_vec2_mon(3,0),1,0,RIGHT };
        path[1] = { arr_to_vec2_mon(3,5),0,1,UP };
        path[2] = { arr_to_vec2_mon(2,5),1,0,RIGHT };
        path[3] = { arr_to_vec2_mon(2,9),0,-1,DOWN };
        path[4] = { arr_to_vec2_mon(5,9),-1,0,LEFT };
        path[5] = { arr_to_vec2_mon(5,1),0,-1,DOWN };
        path[6] = { arr_to_vec2_mon(7,1),1,0,RIGHT };
        path[7] = { arr_to_vec2_mon(7,9),-2,-2,STOP };
        break;
    case BOSS1:
        path = new MapPath[12];
        path_total = 12;
        path[0] = { arr_to_vec2_mon(3,5),1,0,RIGHT };
        path[1] = { arr_to_vec2_mon(3,7),0,1,UP };
        path[2] = { arr_to_vec2_mon(2,7),1,0,RIGHT };
        path[3] = { arr_to_vec2_mon(2,10),0,-1,DOWN };
        path[4] = { arr_to_vec2_mon(5,10),-1,0,LEFT };
        path[5] = { arr_to_vec2_mon(5,7),0,-1,DOWN };
        path[6] = { arr_to_vec2_mon(6,7),-1,0,LEFT };
        path[7] = { arr_to_vec2_mon(6,5),0,-1,DOWN };
        path[8] = { arr_to_vec2_mon(7,5),-1,0,LEFT };
        path[9] = { arr_to_vec2_mon(7,2),0,1,UP };
        path[10] = { arr_to_vec2_mon(4,2),1,0,RIGHT };
        path[11] = { arr_to_vec2_mon(4,5),0,1,UP };
    }

    //设置不同怪物类型的血量、最大血量和速度
    MonsterType types[MONSTER_TOTAL];
    types[NORMAL] = { currentWave * 30,currentWave * 30,100,false,0 };
    types[HUGE] = { currentWave * 40,currentWave * 40,80,false,0 };
    types[FAST] = { currentWave * 20,currentWave * 20,170,false,0 };
    types[BOSS] = { 1000,1000,50,false,0 };

    //图片数组，即各怪物的图片
    std::string picture_1[MONSTER_TOTAL] = { "/Monster/Normal_1.png","/Monster/Huge_1.png","/Monster/Fast_1.png" ,"/Monster/Boss_1.png" };
    std::string picture_2[MONSTER_TOTAL] = { "/Monster/Normal_2.png","/Monster/Huge_2.png","/Monster/Fast_2.png","/Monster/Boss_2.png" };

    //动画帧数组，用于怪物的动画
    Vector<SpriteFrame*> animFrames;
    animFrames.reserve(2);
    if (monster_type == BOSS) {
        animFrames.pushBack(SpriteFrame::create(picture_1[monster_type], Rect(0, 0, 150, 150)));
        animFrames.pushBack(SpriteFrame::create(picture_2[monster_type], Rect(0, 0, 150, 150)));
    }
    else {
        animFrames.pushBack(SpriteFrame::create(picture_1[monster_type], Rect(0, 0, 128, 128)));
        animFrames.pushBack(SpriteFrame::create(picture_2[monster_type], Rect(0, 0, 128, 128)));
    }
    
    //创建动画
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    Animate* animate = Animate::create(animation);

    //设置怪物的属性
    type.hp = types[monster_type].hp;
    type.max_hp = types[monster_type].max_hp;
    type.speed = types[monster_type].speed;
    type.is_slowing = types[monster_type].is_slowing;
    type.slowing_time = types[monster_type].slowing_time;

    //设置怪物的初始图片
    this->Sprite::create(picture_1[monster_type]);

    //根据怪物类型设置锚点
    if (monster_type == FAST)
        this->setAnchorPoint(Vec2(0.5, 0.3));
    else
        this->setAnchorPoint(Vec2(0.4, 0.3));

    //设置怪物的位置
    this->setPosition(path[path_count].point);

    //开始播放动画
    this->runAction(RepeatForever::create(animate)->clone());

    if (monster_type != BOSS) {
        //初始化血条
        hp_border = Sprite::create("/Monster/Hp/HpBorder.png");
        hp = Sprite::create("/Monster/Hp/Hp.png");
        hp_border->setAnchorPoint(Vec2(0, 0.5));
        hp->setAnchorPoint(Vec2(0, 0.5));

        //根据怪物类型调整血条的位置
        if (monster_type == FAST) {
            hp_border->setPosition(35, 130);
            hp->setPosition(35, 130);
        }
        else {
            hp_border->setPosition(25, 130);
            hp->setPosition(25, 130);
        }

        //将血条添加到怪物的子节点中
        this->addChild(hp_border);
        this->addChild(hp);
    }
    else {
        hp = NULL;
        hp_border = NULL;
    }
    closestBullet = NULL;
}

void Monster::findNearestBullet() {
    for (auto bullet : bullets) {
        Vec2 bulletPos = bullet->getPosition();
        if (this->getBoundingBox().containsPoint(bulletPos))
            //if(bullet->getBoundingBox().containsPoint(myPos))
            closestBullet = bullet;
    }
}

//每帧更新怪物的位置和血量
void Monster::update(float dt) {
    //暂停
    if (isPause) {
        return;
    }
    //BOSS的血条初始化
    if (monster_type == BOSS) {
        if (hp == NULL) {
            hp_border = Sprite::create("/Monster/Hp/boss_hp_border.png");
            hp = Sprite::create("/Monster/Hp/boss_hp.png");
            hp_border->setPosition(704, 980);
            cocos2d::Rect rect=hp_border->getBoundingBox();
            hp->setAnchorPoint(Vec2(0, 0.5));
            hp->setPosition(384,980);
            this->getParent()->addChild(hp_border, 3);
            this->getParent()->addChild(hp,3);
        }
    }
    //选择怪物
    if (selected == true) {
        if (arrow == NULL) {
            arrow= Sprite::create("/Barrier/arrow.png");
            arrow->setPosition(60,150);
            this->addChild(arrow);
        }
    }
    else {
        if (arrow) {
            arrow->removeFromParent();
            arrow = NULL;
        }         
    }
    //实现BOSS关卡的循环
    if (path_count >= path_total) {
        path_count %= path_total;
    }
    
    //计算当前怪物在路径上的位置（根据路径的 x 和 y 增量）
    float now_x = this->getPositionX() + path[path_count].x * dt * type.speed;
    float now_y = this->getPositionY() + path[path_count].y * dt * type.speed;

    //根据当前路径的方向，判断需要更新怪物位置的方向
    switch (path[path_count].direction) {
    case UP:
        //如果怪物还没有到达下一个路径点（在 Y 方向上向上移动），则更新位置
        if (now_y < path[path_count + 1].point.y)
            this->setPosition(now_x, now_y);
        else  //如果怪物到达或超过了目标点，切换到下一个路径点
            path_count++;
        break;

    case DOWN:
        //如果怪物还没有到达下一个路径点（在 Y 方向上向下移动），则更新位置
        if (now_y > path[path_count + 1].point.y)
            this->setPosition(now_x, now_y);
        else  //如果怪物到达或超过了目标点，切换到下一个路径点
            path_count++;
        break;

    case LEFT:
        //如果怪物还没有到达下一个路径点（在 X 方向上向左移动），则更新位置
        if (now_x > path[path_count + 1].point.x)
            this->setPosition(now_x, now_y);
        else  //如果怪物到达或超过了目标点，切换到下一个路径点
            path_count++;
        break;

    case RIGHT:
        //如果怪物还没有到达下一个路径点（在 X 方向上向右移动），则更新位置
        if (now_x < path[path_count + 1].point.x)
            this->setPosition(now_x, now_y);
        else  //如果怪物到达或超过了目标点，切换到下一个路径点
            path_count++;
        break;

    case STOP:  //如果路径指示怪物停止移动，不做操作
        break;
    }


    //减速状态
    if (type.is_slowing == true) {
        type.slowing_time += dt;

        //停止减速
        if (type.slowing_time >= type.total_slowing_time) {
            //获取减速特效精灵并删除
            if (slowAnimationSprite) {
                this->unschedule("syncEffectPosition");
                slowAnimationSprite->removeFromParent();
                slowAnimationSprite = NULL;
            }
            //还原初始化
            type.is_slowing = false;
            type.slowing_time = 0;
            type.speed /= 0.8;
        }
    }
    if (path[path_count].direction == STOP) {
        // 更新萝卜血量
        carrotHP--;  // 假设 carrotHP 是全局变量

        // 调用 Carrot 类的 updateDisplay 来更新萝卜血量和形象
        if (globalCarrot) {
            globalCarrot->updateDisplay();  // 更新萝卜显示
        }
    }
    //太阳花攻击
    bool sun_attacked = false;
    for (auto sunflower : sunflowers) {
        cocos2d::Rect fire_rect = sunflower->getBoundingBox();
        SunFire* target = NULL;
        for (auto& fire : fires) {
            if (fire.sun_fire == sunflower)
                target = &fire;
        }
        //受到攻击
        if (sunflower->getBoundingBox().containsPoint(this->getPosition())) {
            //新攻击，加入vector
            if (target == NULL) {
                SunFire new_fire(sunflower, true);
                fires.push_back(new_fire);
                type.hp -= sunflower->GetDamage();
                sun_attacked = true;
            }
            //未攻击，实行攻击
            else if (target->attacked == false) {
                type.hp -= sunflower->GetDamage();
                target->attacked = true;
                sun_attacked = true;
            }
        }
        //不在攻击范围内，将attacked改为false
        else {
            if (target != NULL)
                target->attacked = false;
        }
    }
    //更新血条
    if (sun_attacked) {
        //如果怪物的血量大于 0 且小于最大血量，更新血条的显示
        if (type.hp > 0 && type.hp < type.max_hp) {
            //获取血条的当前尺寸
            Size hpSize = hp->getContentSize();

            //计算根据当前血量比例需要显示的血条宽度
            float width;
            if (monster_type == BOSS)
                width = 640;
            else
                width = 54;
            width *= static_cast<float>(type.hp) / static_cast<float>(type.max_hp);

            // 更新血条的宽度（即当前血量）
            hp->setContentSize(Size(width, hpSize.height));
        }
    }

    findNearestBullet();
    //受到攻击
    if (closestBullet != NULL) {
        if (closestBullet->GetType() == SHITBULLET) {
            type.is_slowing = true;

            //如果是第一次减速（即首次进入减速状态），调用 createSlowAnimation 函数产生减速特效
            if (type.slowing_time == 0) {
                type.speed *= 0.8;
                createSlowAnimation(now_x, now_y);
            }
            type.total_slowing_time = closestBullet->GetTime();
            type.slowing_time += dt;
        }
        // 生命值减少
        type.hp -= closestBullet->GetDamage();

        //如果怪物的血量大于 0 且小于最大血量，更新血条的显示
        if (type.hp > 0 && type.hp < type.max_hp) {
            //获取血条的当前尺寸
            Size hpSize = hp->getContentSize();

            //计算根据当前血量比例需要显示的血条宽度
            float width;
            if (monster_type == BOSS)
                width = 640;
            else
                width = 54;
            width *= static_cast<float>(type.hp) / static_cast<float>(type.max_hp);

            // 更新血条的宽度（即当前血量）
            hp->setContentSize(Size(width, hpSize.height));
        }
        if (closestBullet) {
            closestBullet->removeFromParent();  // 从父节点移除子弹
            // 从 bullets 容器中移除自身
            auto it = std::find(bullets.begin(), bullets.end(), closestBullet);
            if (it != bullets.end()) {
                bullets.erase(it);
            }
            closestBullet = NULL;
        }
    }

    if (type.hp <= 0 || path[path_count].direction == STOP) {
        if (type.is_slowing == true) {
            //获取减速特效精灵并删除
            if (slowAnimationSprite) {
                this->unschedule("syncEffectPosition");
                slowAnimationSprite->removeFromParent();
                slowAnimationSprite = NULL;
            }
        }
        /**********************************************/
        // 创建消失特效动画（两秒持续时间）
        Vector<SpriteFrame*> frames;
        for (int i = 1; i <= 6; ++i) {
            std::string frameName = "/MonsterStart/monster_vanish_effect" + std::to_string(i) + ".png";
            frames.pushBack(SpriteFrame::create(frameName, Rect(0, 0, 128, 128)));
        }
        // 每帧显示时间 = 总时间 / 帧数
        float frameDuration = 0.5f / frames.size();
        auto vanishAnimation = Animation::createWithSpriteFrames(frames, frameDuration);
        auto vanishAnimate = Animate::create(vanishAnimation);
        // 创建消失特效精灵
        auto effect = Sprite::create("/MonsterStart/monster_vanish_effect1.png");
        effect->setContentSize(Size(60, 60));
        effect->setPosition(now_x, now_y); // 特效初始位置与怪物一致
        this->getParent()->addChild(effect, 2);
        // 消失怪物的回调
        auto disappearMonster = CallFunc::create([effect]() {
            effect->removeFromParent();         // 删除特效精灵

            });
        // 动作序列：播放动画 -> 隐藏怪物
        auto monsterSequence = Sequence::create(vanishAnimate, disappearMonster, nullptr);
        effect->runAction(monsterSequence);
        /**********************************************/

        monsters.erase(find_if(monsters.begin(), monsters.end(), [this](const Monster* monster) {return monster == this; }));
        this->removeFromParent();
        if (type.hp <= 0)
            coinNumber += 20;//金币增加
        this->isRemoved = true;
    }


}

void Monster::createSlowAnimation(float x, float y) {
    //动画帧数组，用于怪物减速的动画
    Vector<SpriteFrame*> animFrames;
    animFrames.reserve(2);
    animFrames.pushBack(SpriteFrame::create("/Monster/Attack/Slowdown_1.png", Rect(0, 0, 128, 30)));
    animFrames.pushBack(SpriteFrame::create("/Monster/Attack/Slowdown_2.png", Rect(0, 0, 128, 34)));

    //创建动画
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    Animate* animate = Animate::create(animation);

    // 创建减速动画精灵
    slowAnimationSprite = Sprite::create("/Monster/Attack/Slowdown_1.png");
    slowAnimationSprite->setAnchorPoint(Vec2(0.5, 1));
    slowAnimationSprite->setPosition(this->getPosition()); // 特效初始位置与怪物一致
    slowAnimationSprite->runAction(RepeatForever::create(animate)->clone());
    slowAnimationSprite->setName("slow");
    this->getParent()->addChild(slowAnimationSprite, 2);
    /**********************************************/
    // 开始同步特效位置
    if (slowAnimationSprite != nullptr)
        this->schedule([this, slowAnimationSprite](float) {
        slowAnimationSprite->setPosition(this->getPosition()); // 每帧同步特效位置
            }, "syncEffectPosition");
    /**********************************************/
}
