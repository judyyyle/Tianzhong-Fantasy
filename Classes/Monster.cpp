#include "HelloWorldScene.h"

#include "MonsterStart.h"
#include "Monster.h"

#include <string>


/*
extern int carrot_HP;//萝卜血量

extern int monster_number;//怪物总数
extern int monster_wave_number;//怪物攻击波束
extern int barrier_number;//障碍总数
extern std::vector<Monster*>monster;
extern std::vector<Monster*>barrier;

extern Monster* destination;//怪物位置

*/

//定义一个结构体，表示二维数组中的位置
static struct array {
    int row;
    int col;
};

//将二维网格坐标转换为世界坐标 (Vec2 类型)
static Vec2 array_to_vec2(int row, int col) { 
    Vec2 vec;
    vec.x = 64 + 128 * col;
    vec.y = 1024 - 64 - 128 * row;
    return vec;
}

// HelloWorld 类的 createScene 方法，用于创建场景
Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Monster 类的初始化方法，继承自 Sprite
bool Monster::init(){
	if (!Sprite::init()) { //调用父类的初始化方法
		return false;
	}
    CCLOG("Monster initialized successfully!");

    //调用 Cocos2d 的定时器，定期更新怪物的位置
	this->scheduleUpdate();

	return true;
}

//初始化怪物的类型和路径
void Monster::initType(int monster_type,int map_type) {
    path_count = 0;

    //根据地图类型 (map_type) 设置不同的路径
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

    //设置不同怪物类型的血量、最大血量和速度
    MonsterType types[MONSTER_TOTAL];
    types[NORMAL] = { 100,100,100 };
    types[FAST] = { 80,80,150 };
    types[HUGE] = { 120,120,80 };
    types[BOSS] = { 1000,1000,50 };

    //图片数组，即各怪物的图片
    std::string picture_1[MONSTER_TOTAL] = { "/Monster/Normal_1.png","/Monster/Huge_1.png","/Monster/Fast_1.png" ,"/Monster/Boss_1.png" };
    std::string picture_2[MONSTER_TOTAL] = { "/Monster/Normal_2.png","/Monster/Huge_2.png","/Monster/Fast_2.png","/Monster/Boss_1.png" };
    
    //动画帧数组，用于怪物的动画
    Vector<SpriteFrame*> animFrames;
    animFrames.reserve(2);
    animFrames.pushBack(SpriteFrame::create(picture_1[monster_type], Rect(0, 0, 128, 128)));
    animFrames.pushBack(SpriteFrame::create(picture_2[monster_type], Rect(0, 0, 128, 128)));

    //创建动画
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    Animate* animate = Animate::create(animation);
    
    //设置怪物的属性
    this->monster_type.hp = types[monster_type].hp;
    this->monster_type.max_hp = types[monster_type].max_hp;
    this->monster_type.speed = types[monster_type].speed;
    
    //设置怪物的初始图片
    this->setTexture(picture_1[monster_type]);

    //根据怪物类型设置锚点
    if (monster_type == FAST)
        this->setAnchorPoint(Vec2(0.5, 0.3));
    else
        this->setAnchorPoint(Vec2(0.4, 0.3));

    //设置怪物的位置
    this->setPosition(path[path_count].point);

    //开始播放动画
    this->runAction(RepeatForever::create(animate)->clone());

    //初始化血条
    hp_border= Sprite::create("/Monster/Hp/HpBorder.png");
    hp = Sprite::create("/Monster/Hp/Hp.png");
    
    //根据怪物类型调整血条的位置
    if (monster_type == FAST) {
        hp_border->setPosition(65, 130);
        hp->setPosition(65, 130);
    }
    else {
        hp_border->setPosition(55, 130);
        hp->setPosition(55, 130);
    }
    
    //将血条添加到怪物的子节点中
    this->addChild(hp_border,100);
    this->addChild(hp,100);
}

//每帧更新怪物的位置和血量
void Monster::update(float dt) {
    
    //计算当前怪物在路径上的位置（根据路径的 x 和 y 增量）
    float now_x = this->getPositionX() + path[path_count].x * dt * this->monster_type.speed;
    float now_y = this->getPositionY() + path[path_count].y * dt * this->monster_type.speed;
    
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
        if (now_y > path[path_count+1].point.y)
            this->setPosition(now_x, now_y);
        else  //如果怪物到达或超过了目标点，切换到下一个路径点
            path_count++;
        break;

    case LEFT:
        //如果怪物还没有到达下一个路径点（在 X 方向上向左移动），则更新位置
        if (now_x > path[path_count+1].point.x)
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

    case STOP:  //如果路径指示怪物停止移动，不做操作直接返回
        return;
    }
    
    //如果怪物的血量大于 0 且小于最大血量，更新血条的显示
    if (this->monster_type.hp > 0 && this->monster_type.hp < this->monster_type.max_hp) {
        //获取血条的当前尺寸
        Size hpSize = hp->getContentSize();

        //计算根据当前血量比例需要显示的血条宽度
        float width = hpSize.width;
        width *= this->monster_type.hp / this->monster_type.max_hp;

        // 更新血条的宽度（即当前血量）
        hp->setContentSize(Size(width, hpSize.height));
    }
}
