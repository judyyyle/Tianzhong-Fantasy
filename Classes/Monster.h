#pragma once
#include <vector>
#include <algorithm>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Tower.h"
#include "Bullet.h"

USING_NS_CC;
using namespace cocos2d;

extern int isPause; //是否暂停
extern int allWaves; //总的波数
extern int currentWave; //当前怪物的波数
extern int carrotHP;
extern int level;

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

struct MonsterType {
    int hp;           //实时血量
    int max_hp;       //满血量                 
    float speed;        //移动速度
    bool is_slowing;    //是否减速
    float slowing_time; //减速时间
    float total_slowing_time; //总减速时间
};

struct MapPath {
    Vec2 point;     // 路径上的坐标点
    int x;          // X 方向的增量
    int y;          // Y 方向的增量
    int direction;  // 移动方向，UP/DOWN/LEFT/RIGHT/STOP
};

//太阳花的攻击结构体
struct SunFire {
    Sunflowerfire* sun_fire; //太阳花攻击的类指针
    bool attacked; //是否已攻击
    SunFire(Sunflowerfire* f, bool a) :sun_fire(f), attacked(a) {}
    SunFire() :sun_fire(NULL), attacked(true) {}

    // 定义 operator==
    bool operator==(const SunFire& other) const {
        // 判断 sun_fire 指针是否指向同一对象，以及 attacked 是否相等
        return (sun_fire == other.sun_fire) && (attacked == other.attacked);
    }
};

//定义怪物类
class Monster : public Sprite {
private:
    Sprite* slowAnimationSprite = NULL; //减速特效的指针
    int monster_type;          // 怪物种类
    MapPath* path;             // 存储怪物路径的数组
    MonsterType type;  // 怪物的类型（血量、速度等）
    Sprite* hp_border, * hp;   // 怪物的血条背景和当前血量
    int path_count;            // 当前怪物所在路径的索引
    int path_total;            // 路径的总数
    Sprite* arrow = NULL;      // 选中图案的指针

    Bullet* closestBullet;     // 最近的子弹
    std::vector<SunFire>fires; // 太阳花的火焰攻击
public:
    bool isRemoved;   //是否消失
    bool selected;    //是否被选中
    CREATE_FUNC(Monster);

    //初始化怪物
    virtual bool init();

    //初始化怪物类型和路径
    void initType(int monster_type, int map_type);

    //寻找最近的子弹
    void findNearestBullet();

    //每帧更新怪物的位置和血量
    virtual void update(float dt);

    //受到攻击
    void receiveDamage();

    //减速特效
    void createSlowAnimation(float x, float y);
};

//存储所有怪物的 vector
extern std::vector<Monster*> monsters;

// MonsterCreate 类，用于创建怪物并管理怪物生成的波次
class MonsterCreate : public Sprite {
private:
    float elapsedTime = 0;  //间隔时间
    int boss_time = 90;  //倒计时
    cocos2d::Label* countdownLabel = NULL; // 倒计时显示的标签
    int current = 1;  //当前怪物个数
    int waves[6] = { 0,10,10,15,15,20 }; //每波怪物的总数
    float waveTimer = 1.6f;  // 计时器，用来控制每波怪物的生成间隔
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
        if (map_type == BOSS1)
            currentWave = 4;
    }

    //初始化怪物并将其添加到场景
    void initMonster(int monster_type) {
        /**********************************************/
        //出怪特效
        auto effect = Sprite::create("/MonsterStart/monster_appear_effect.png");
        effect->setContentSize(Size(60, 60));
        if (level == 0) {
            effect->setPosition(Vec2(192, 704));
        }
        else if (level == 1) {
            effect->setPosition(Vec2(90, 594));
        }
        else if (level == 2) {
            effect->setPosition(Vec2(704, 576));
        }
        this->getParent()->addChild(effect, 3);
        // 出怪放大、淡出特效
        auto monsterScaleEffect = ScaleTo::create(0.2f, 3.0f); // 0.5秒放大到3.5倍
        auto monsterFadeEffect = FadeOut::create(0.5f);       // 同时淡出
        // 动作序列：放大特效 -> 淡出特效 -> 显示怪物
        auto monsterSequence = Sequence::create(monsterScaleEffect, monsterFadeEffect, nullptr);
        effect->runAction(monsterSequence);
        /**********************************************/
        //BOSS关卡倒计时
        if (map_type == BOSS1) {
            //创建底图
            auto countdownBackground = cocos2d::Sprite::create("/Monster/boss_time.png");
            countdownBackground->setAnchorPoint(Vec2(0.5, 0));
            countdownBackground->setPosition(1350, 0);
            this->addChild(countdownBackground);
            //创建倒计时标签
            if (countdownLabel)
                countdownLabel->removeFromParent();
            countdownLabel = cocos2d::Label::createWithTTF(std::to_string(boss_time), "/fonts/arial.ttf", 48);
            countdownLabel->setPosition(1310, 45);
            countdownLabel->setColor(cocos2d::Color3B::WHITE);
            this->addChild(countdownLabel);
        }
        
        //怪物出现
        auto monster = Monster::create();
        if (!monster) {
            CCLOG("Failed to create monster sprite!");
            return;
        }
        //初始化怪物类型和路径
        monster->initType(monster_type, map_type);

        //将怪物添加为当前场景的子节点
        this->getParent()->addChild(monster, 2);

        //将怪物添加到 monsters 向量中，管理所有怪物
        if (monster)
            monsters.push_back(monster);
    }

    virtual void update(float dt) {
        //暂停
        if (isPause) {
            return;
        }
        // BOSS模式
        if (map_type == BOSS1) {
            elapsedTime += dt;  // 每帧累积时间

            // 如果倒计时大于 0，则继续倒计时
            if (boss_time > 0)
            {
                if (boss_time != 90 && monsters.empty()) {
                    currentWave = allWaves + 1;  // 游戏胜利
                    this->unschedule("update_key");  // 停止定时器
                }
                if (elapsedTime >= 1.0f) // 每秒更新一次倒计时
                {
                    boss_time--;  // 倒计时减 1
                    countdownLabel->setString(std::to_string(boss_time)); // 更新文本显示
                    elapsedTime = 0.0f; // 重置累积时间
                }
            }
            else
            {
                carrotHP = 0; // 游戏失败
                this->unschedule("update_key");  // 停止定时器
            }
        }
        //更新计时器
        waveTimer += dt;
        //判断是否开始新的一轮
        if (map_type != BOSS1 && current > waves[currentWave] && monsters.empty()) {
            currentWave++;
            current = 1;
            waveTimer = -2.1f;
        }
        handleMonsterWaves(dt);
    }

    //控制怪物波次生成的函数
    void handleMonsterWaves(float dt) {
        static int wave = 1;
        float wave_interval[MONSTER_TOTAL] = { 1.4f,1.0f,1.6f };  // 每波生成的时间间隔（1.4秒）

        if (current <= 5) {
            // 第一个直接生成
            if (current == 1 && (currentWave == 1 || map_type == BOSS1)) {
                initMonster(NORMAL);
                current++;
                waveTimer = 0.0f;
            }
            // 按照固定间隔生成
            else if (waveTimer >= wave_interval[NORMAL]) {
                initMonster(NORMAL);
                current++;
                waveTimer = 0.0f; //重置计时器
            }
        }

        switch (currentWave) {
        case 3:
        case 4:
        case 5:
            // 生成5个FAST怪物
            if (current > 10 && current <= waves[currentWave]) {
                // 按照固定间隔生成
                if (waveTimer >= wave_interval[FAST]) {
                    initMonster(FAST);
                    current++;
                    waveTimer = 0.0f; //重置计时器
                }
            }

        case 2:
            // 生成5个HUGE怪物
            if (current > 5 && current <= waves[currentWave]) {
                // 按照固定间隔生成
                if (waveTimer >= wave_interval[HUGE]) {
                    initMonster(HUGE);
                    current++;
                    waveTimer = 0.0f; //重置计时器
                }
            }
            break;

        case 1:
            // 生成5个NORMAL怪物
            if (current > 5 && current <= waves[currentWave]) {
                // 按照固定间隔生成
                if (waveTimer >= wave_interval[NORMAL]) {
                    initMonster(NORMAL);
                    current++;
                    waveTimer = 0.0f; //重置计时器
                }
            }
            break;
        }        

        if (map_type == BOSS1) {
            if (current == 16 && waveTimer >= 1.0f) {
                current++;
                initMonster(BOSS);
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
        if (level == 0) {
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
            carrot_hp->setPosition(1452, 120);
            this->addChild(carrot_hp, 0);
            // 初始化萝卜形象
            auto carrot = Sprite::create("/Carrot/HP_MAX.png");
            carrot->setContentSize(Size(96, 142));
            carrot->setPosition(1360, 128);
            carrot->setName("carrot");
            this->addChild(carrot, 0);

            // 初始化萝卜血量数值
            auto carrot_hp_number = Label::createWithTTF(std::to_string(carrotHP), "/fonts/TMON Monsori.ttf", 28);
            carrot_hp_number->setTextColor(Color4B::YELLOW);
            carrot_hp_number->setName("HpNumber");
            carrot_hp_number->setPosition(1467, 120);
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
