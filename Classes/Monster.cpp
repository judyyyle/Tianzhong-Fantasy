#include "HelloWorldScene.h"

#include "MonsterStart.h"
#include "Monster.h"

#include <string>



extern int carrot_HP;//萝卜血量

extern int monster_number;//怪物总数
extern int monster_wave_number;//怪物攻击波束
extern int barrier_number;//障碍总数
extern std::vector<Monster*>monster;
extern std::vector<Monster*>barrier;

extern Monster* destination;//怪物位置


static struct array {
    int row;
    int col;
};

static Vec2 array_to_vec2(int row, int col) { //返回Vec2类型，即世界坐标
    Vec2 vec;
    vec.x = 64 + 128 * col;
    vec.y = 1024 - 64 - 128 * row;
    return vec;
}

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool Monster::init()
{
	if (!Sprite::init()) {
		return false;
	}
    CCLOG("Monster initialized successfully!");
	this->scheduleUpdate();

	return true;
}

void Monster::initType(int monster_type,int map_type) {
    path_count = 0;
    switch (map_type) {
    case ADVENTURE1:
        path = new MapPath[8];
        path_total = 8;
        path[0] = { array_to_vec2(2,1),0,-1,DOWN }; 
        path[1] = { array_to_vec2(5,1),1,0,RIGHT };
        path[2] = { array_to_vec2(5,4),0,1,UP };
        path[3] = { array_to_vec2(4,4),1,0,RIGHT };
        path[4] = { array_to_vec2(4,7),0,-1,DOWN };
        path[5] = { array_to_vec2(5,7),1,0,RIGHT };
        path[6] = { array_to_vec2(5,10),0,1,UP };
        path[7] = { array_to_vec2(2,10),-2,-2,STOP };
        break;
    case ADVENTURE2:
        path = new MapPath[8];
        path_total = 8;
        path[0] = { array_to_vec2(3,0),1,0,RIGHT };
        path[1] = { array_to_vec2(3,5),0,1,UP };
        path[2] = { array_to_vec2(2,5),1,0,RIGHT };
        path[3] = { array_to_vec2(2,9),0,-1,DOWN };
        path[4] = { array_to_vec2(5,9),-1,0,LEFT };
        path[5] = { array_to_vec2(5,1),0,-1,DOWN };
        path[6] = { array_to_vec2(7,1),1,0,RIGHT };
        path[7] = { array_to_vec2(7,9),-2,-2,STOP };
        break;
    case BOSS1:
        path = new MapPath[12];
        path_total = 12;
        path[0] = { array_to_vec2(3,5),1,0,RIGHT };
        path[1] = { array_to_vec2(3,7),0,1,UP };
        path[2] = { array_to_vec2(2,7),1,0,RIGHT };
        path[3] = { array_to_vec2(2,10),0,-1,DOWN };
        path[4] = { array_to_vec2(5,10),-1,0,LEFT };
        path[5] = { array_to_vec2(5,7),0,-1,DOWN };
        path[6] = { array_to_vec2(6,7),-1,0,LEFT };
        path[7] = { array_to_vec2(6,5),0,-1,DOWN };
        path[8] = { array_to_vec2(7,5),-1,0,LEFT };
        path[9] = { array_to_vec2(7,2),0,1,UP };
        path[10] = { array_to_vec2(4,2),1,0,RIGHT };
        path[11] = { array_to_vec2(4,5),0,1,UP };
    }

    MonsterType types[MONSTER_TOTAL];
    types[NORMAL] = { 100,100,100 };
    types[FAST] = { 80,80,150 };
    types[HUGE] = { 120,120,80 };
    types[BOSS] = { 1000,1000,50 };

    std::string picture_1[MONSTER_TOTAL] = { "/Monster/Normal_1.png","/Monster/Huge_1.png","/Monster/Fast_1.png" ,"/Monster/Boss_1.png" };
    std::string picture_2[MONSTER_TOTAL] = { "/Monster/Normal_2.png","/Monster/Huge_2.png","/Monster/Fast_2.png","/Monster/Boss_1.png" };
    Vector<SpriteFrame*> animFrames;
    animFrames.reserve(2);
    animFrames.pushBack(SpriteFrame::create(picture_1[monster_type], Rect(0, 0, 128, 128)));
    animFrames.pushBack(SpriteFrame::create(picture_2[monster_type], Rect(0, 0, 128, 128)));

    Animation* animation;
    animation = Animation::createWithSpriteFrames(animFrames, 0.1f);

    Animate* animate;
    animate = Animate::create(animation);

    this->monster_type.hp = types[monster_type].hp;
    this->monster_type.max_hp = types[monster_type].max_hp;
    this->monster_type.speed = types[monster_type].speed;
    
    this->setTexture(picture_1[monster_type]);

    if (monster_type == FAST)
        this->setAnchorPoint(Vec2(0.5, 0.3));
    else
        this->setAnchorPoint(Vec2(0.4, 0.3));

    this->setPosition(path[path_count].point);
    this->runAction(RepeatForever::create(animate)->clone());

    hp_border= Sprite::create("/Monster/Hp/HpBorder.png");
    hp = Sprite::create("/Monster/Hp/Hp.png");

    if (monster_type == FAST) {
        hp_border->setPosition(65, 130);
        hp->setPosition(65, 130);
    }
    else {
        hp_border->setPosition(55, 130);
        hp->setPosition(55, 130);
    }
    
    this->addChild(hp_border,100);
    this->addChild(hp,100);
    /*
    auto hp_border = Sprite::create("/Monster/Hp/HpBorder.png");
    auto hp = Sprite::create("/Monster/Hp/Hp.png");
    hp->setAnchorPoint(Vec2(-0.5, -13));
    hp_border->setAnchorPoint(Vec2(-0.5, -13));
    this->addChild(hp_border);
    this->addChild(hp);
    */
    CCLOG("Initializing monster with type: %d, map_type: %d", monster_type, map_type);
    // 其他初始化代码
    
}

void Monster::update(float dt) {
    
    // 在 update 函数中更新精灵位置
    float now_x = this->getPositionX() + path[path_count].x * dt * this->monster_type.speed;
    float now_y = this->getPositionY() + path[path_count].y * dt * this->monster_type.speed;
    
    switch (path[path_count].direction) {
    case UP:
        if (now_y < path[path_count+1].point.y)
            this->setPosition(now_x, now_y);
        else
            path_count++;
        break;
    case DOWN:
        if (now_y > path[path_count+1].point.y)
            this->setPosition(now_x, now_y);
        else
            path_count++;
        break;
    case LEFT:
        if (now_x > path[path_count+1].point.x)
            this->setPosition(now_x, now_y);
        else
            path_count++;
        break;
    case RIGHT:
        if (now_x < path[path_count + 1].point.x)
            this->setPosition(now_x, now_y);
            
        else
            path_count++;
        break;
    case STOP:
        return;
    }
    
    if (this->monster_type.hp > 0 && this->monster_type.hp < this->monster_type.max_hp) {
        Size hpSize = hp->getContentSize();
        float width = hpSize.width;
        width *= this->monster_type.hp / this->monster_type.max_hp;
        hp->setContentSize(Size(width, hpSize.height));
    }
}
