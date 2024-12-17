#pragma once
#include <vector>
#include "cocos2d.h"
#include "HelloWorldScene.h"

USING_NS_CC;
using namespace cocos2d;

extern int isPause; //是否暂停
extern int allWaves; //总的波数
extern int currentWave; //当前怪物的波数

//总共有 4 种怪物类型
#define MONSTER_TOTAL 4   

//四种怪物的编号
#define NORMAL 0            //正常
#define HUGE 1              //血厚
#define FAST 2              //高速
#define BOSS 3              //BOSS怪

//三种怪物的编号
#define ADVENTURE1 0    //map1
#define ADVENTURE2 1    //map2
#define BOSS1 2         //map3

//怪物移动的方向
#define STOP -1   //停止
#define UP 0      //上移
#define DOWN 1    //下移
#define LEFT 2    //左移
#define RIGHT 3   //右移

/*
#define BARRIER 0
#define MONSTER 1

#define NORMAL 0            //正常怪物
#define FAST 1              //高速怪物
#define HUGE 2              //血厚怪物
#define BARRIER_1 3         //一格障碍
#define BARRIER_2 4         //两格障碍
#define BARRIER_3 5         //四格障碍
//障碍
#define BARRIER_ONE   1     //一格的障碍
#define BARRIER_TWO   2     //两格的障碍
#define BARRIER_FOUR  4     //四格的障碍
#define BARRIER_HP_ONE     1600               //一格障碍血量
#define BARRIER_HP_TWO     3 * BARRIER_HP_ONE //两格障碍血量
#define BARRIER_HP_FOUR    3 * BARRIER_HP_TWO //四格障碍血量
#define BARRIER_COIN_ONE     50
#define BARRIER_COIN_TWO     2 * BARRIER_COIN_ONE //两格障碍金币
#define BARRIER_COIN_FOUR    2 * BARRIER_COIN_TWO //四格障碍金币
//怪物
#define MONSTER_NORMAL 0    //正常怪物
#define MONSTER_FAST   1    //高速怪物
#define MONSTER_HUGE   2    //血厚怪物
#define MONSTER_FAST_HP       20                     //高速怪物血量
#define MONSTER_NORMAL_HP     3 * MONSTER_FAST_HP    //正常怪物血量
#define MONSTER_HUGE_HP       3 * MONSTER_NORMAL_HP  //血厚怪物血量
#define MONSTER_HUGE_SPEED    100                        //血厚怪物速度
#define MONSTER_NORMAL_SPEED  1.5*MONSTER_HUGE_SPEED     //正常怪物速度
#define MONSTER_FAST_SPEED    2*MONSTER_NORMAL_SPEED     //高速怪物速度
#define MONSTER_COIN_NORMAL   18    //正常怪物和血厚怪物掉落金币
#define MONSTER_COIN_HUGE     188   //血厚怪物掉落金币
//路径
#define BARRIER_PICTURE  "/Enemy/barrier/0/"   //障碍路径
#define MONSTER_PICTURE  "/Enemy/monster/0/"   //怪物路径

*/

struct MonsterType {
    //	int type;                      //怪物、障碍的种类
    int hp;           //实时血量
    int max_hp;       //满血量                 
    float speed;        //移动速度
    bool is_slowing;    //是否减速
    float slowing_time; //减速时间
    //	int full_HP;                   //满血
    //	int ATK;                       //攻击值

    //	int coin;                      //获得的金币

    //	int move_Count;                //移动步数
    //	double poison_Time = 0;        //中毒时间
    //	int poison_Speed;              //中毒的移动速度
};

struct MapPath {
    Vec2 point;     // 路径上的坐标点
    int x;          // X 方向的增量
    int y;          // Y 方向的增量
    int direction;  // 移动方向，UP/DOWN/LEFT/RIGHT/STOP
};

//定义怪物类
class Monster : public Sprite {
private:   
    MapPath* path;             // 存储怪物路径的数组
    MonsterType type;  // 怪物的类型（血量、速度等）
    Sprite* hp_border, * hp;   // 怪物的血条背景和当前血量
    int path_count;            // 当前怪物所在路径的索引
    int path_total;            // 路径的总数
public:
    CREATE_FUNC(Monster);

    //初始化怪物
    virtual bool init();

    //初始化怪物类型和路径
    void initType(int monster_type,int map_type);

    //每帧更新怪物的位置和血量
    virtual void update(float dt);

    //受到攻击
    void receiveDamage(float x,float y);
};

//存储所有怪物的 vector
extern std::vector<Monster*> monsters;

// MonsterCreate 类，用于创建怪物并管理怪物生成的波次
class MonsterCreate : public Sprite {
private:
    float waveTimer = 0.0f;  // 计时器，用来控制每波怪物的生成间隔
    int map_type;            //当前地图类型
public:
    CREATE_FUNC(MonsterCreate);

    virtual bool init() {
        if (!Sprite::init()) { //调用父类的初始化方法
            return false;
        }
        CCLOG("Monsters created successfully!");

        //调用 Cocos2d 的定时器，定期更新怪物的位置
        this->scheduleUpdate();

        return true;
    }

    //获取map_type数据
    void MonsterWaves(int MAPTYPE) {
        map_type = MAPTYPE;
    }

    //初始化怪物并将其添加到场景
    void initMonster(int monster_type) {
        /**********************************************/
        //出怪特效
        auto effect = Sprite::create("/MonsterStart/monster_appear_effect.png");
        effect->setContentSize(Size(60, 60));
        effect->setPosition(192, 704);
        this->addChild(effect, 3);
        // 出怪放大、淡出特效
        auto monsterScaleEffect = ScaleTo::create(0.2f, 3.0f); // 0.5秒放大到3.5倍
        auto monsterFadeEffect = FadeOut::create(0.5f);       // 同时淡出
        // 动作序列：放大特效 -> 淡出特效 -> 显示怪物
        auto monsterSequence = Sequence::create(monsterScaleEffect, monsterFadeEffect, nullptr);
        effect->runAction(monsterSequence);
        /**********************************************/

        //怪物出现
        auto monster = Monster::create();
        if (!monster) {
            CCLOG("Failed to create monster sprite!");
            return;
        }
        //初始化怪物类型和路径
        monster->initType(monster_type, map_type);

        //将怪物添加到 monsters 向量中，管理所有怪物
        monsters.push_back(monster);

        //将怪物添加为当前场景的子节点
        this->getParent()->addChild(monster, 2);
    }

    /*
    //控制怪物的波次生成
    void MonsterWaves(int MAPTYPE) {
        map_type = MAPTYPE;

        //针对不同地图类型，生成不同类型的怪物
        switch (map_type) {
        case ADVENTURE1:
            //生成 5 波正常怪物，每波间隔 1.4 秒
            for (int i = 0; i < 5; i++) {
                this->runAction(Sequence::create(DelayTime::create(i * 1.4f),
                    CallFunc::create([=]() {
                        initMonster(NORMAL);
                        }),
                    nullptr));
            }
            
            //生成 5 波高速怪物，每波间隔 1.0 秒
            for (int i = 0; i < 5; i++) {
                this->runAction(Sequence::create(DelayTime::create((i + 5) * 1.0f),
                    CallFunc::create([=]() {
                        initMonster(FAST);
                        }),
                    nullptr));

            }

            //生成 5 波血厚怪物，每波间隔 1.6 秒
            for (int i = 0; i < 5; i++) {
                this->runAction(Sequence::create(DelayTime::create((i + 10) * 1.6f),
                    CallFunc::create([=]() {
                        initMonster(HUGE);
                        }),
                    nullptr));

            }
            

            break;
        case ADVENTURE2:
            break;
        case BOSS1:
            break;
        }
    }
    */
    

    virtual void update(float dt) {
        //暂停
        if (isPause) {
            return;
        }

        //更新计时器
        waveTimer += dt;

        handleMonsterWaves(dt);
    }

    //控制怪物波次生成的函数
    void handleMonsterWaves(float dt) {
        float wave_interval[MONSTER_TOTAL] = { 1.4f,1.0f,1.6f };  // 每波生成的时间间隔（1.4秒）

        // 判断当前时间是否到了生成怪物的时刻
        if (currentWave < 5) {  // 例如最多生成5波怪物
            // 第一波直接生成
            if (currentWave == 1) {
                // 生成怪物
                initMonster(NORMAL);  // 你可以根据需要改变生成怪物的类型

                // 增加波次计数
                currentWave++;

                // 重置计时器
                waveTimer = 0.0f;
            }
            // 从第二波开始，按照固定间隔生成
            else if (waveTimer >= wave_interval[NORMAL]) {
                // 生成怪物
                initMonster(NORMAL);  // 你可以根据需要改变生成怪物的类型

                // 增加波次计数
                currentWave++;

                // 重置计时器
                waveTimer = 0.0f;  // 重置计时器
            }
        }
    }
};
//用于管理萝卜的血量和形象更新
class Carrot : public Node {
public:
    // 创建一个实例
    CREATE_FUNC(Carrot);

    // 萝卜形象图片数组
    std::string picture[10] = {
        "/Carrot/HP_MAX.png",
        "/Carrot/HP_1.png", "/Carrot/HP_2.png", "/Carrot/HP_3.png",
        "/Carrot/HP_4.png", "/Carrot/HP_5-6.png", "/Carrot/HP_5-6.png",
        "/Carrot/HP_7-8.png", "/Carrot/HP_9.png", "/Carrot/HP_MAX.png"
    };

    // 初始化萝卜的显示
    bool init() override {
        if (!Node::init()) {
            return false;
        }
        if (level == 0){
            // 初始化萝卜血量背景板
            auto carrot_hp = Sprite::create("/Carrot/Hp.png");
            carrot_hp->setContentSize(Size(96, 48));
            carrot_hp->setPosition(1452, 760);
            this->addChild(carrot_hp, 0);
            // 初始化萝卜形象
            auto carrot = Sprite::create("/Carrot/HP_MAX.png");
            carrot->setContentSize(Size(96, 142));
            carrot->setPosition(1360, 768);
            carrot->setName("carrot");
            this->addChild(carrot, 0);

            // 初始化萝卜血量数值
            auto carrot_hp_number = Label::createWithTTF(std::to_string(carrotHP), "/fonts/TMON Monsori.ttf", 28);
            carrot_hp_number->setTextColor(Color4B::YELLOW);
            carrot_hp_number->setName("HpNumber");
            carrot_hp_number->setPosition(1467, 760);
            this->addChild(carrot_hp_number);
        }
        else if (level == 1) {
            // 初始化萝卜血量背景板
            auto carrot_hp = Sprite::create("/Carrot/Hp.png");
            carrot_hp->setContentSize(Size(96, 48));
            carrot_hp->setPosition(1324, 760);
            this->addChild(carrot_hp, 0);
            // 初始化萝卜形象
            auto carrot = Sprite::create("/Carrot/HP_MAX.png");
            carrot->setContentSize(Size(96, 142));
            carrot->setPosition(1232, 768);
            carrot->setName("carrot");
            this->addChild(carrot, 0);

            // 初始化萝卜血量数值
            auto carrot_hp_number = Label::createWithTTF(std::to_string(carrotHP), "/fonts/TMON Monsori.ttf", 28);
            carrot_hp_number->setTextColor(Color4B::YELLOW);
            carrot_hp_number->setName("HpNumber");
            carrot_hp_number->setPosition(1339, 760);
            this->addChild(carrot_hp_number);
        }

        return true;
    }

    // 更新萝卜形象和血量显示
    void updateDisplay() {
        // 确保从场景中获取到的子节点是指针类型
        auto carrot = dynamic_cast<Sprite*>(this->getChildByName("carrot"));
        auto carrot_hp_label = dynamic_cast<Label*>(this->getChildByName("HpNumber"));

        // 检查是否成功获取萝卜和血量标签
        if (!carrot || !carrot_hp_label) {
            CCLOG("Failed to find carrot or HpNumber label!");
            return;
        }

        // 更新萝卜形象
        if (carrotHP >= 0 && carrotHP < 10) {
            carrot->setTexture(picture[carrotHP]);
        }

        // 更新血量显示
        carrot_hp_label->setString(std::to_string(carrotHP));
    }
};
