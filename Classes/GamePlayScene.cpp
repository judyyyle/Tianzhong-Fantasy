#include "GamePlayScene.h"
#include"HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "LevelSelectionScene.h"
#include "GameManager.h"
#include<vector>
#include "ui/CocosGUI.h"
#include <memory>  // 引入智能指针s
#include "cocos2d.h"
#include "Tower.h"
#include "BarrierAppear.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

using namespace cocos2d::ui;
// 在 MAP_SCENE 类中，添加一个成员变量来保存提示图标
int mapGrid[8][12] = { 0 };
int level;
/**********************  全局变量  ***********************/
//关卡选项
bool isLevel2Unlocked = 0;//第二关是否解锁
int isPause;//是否暂停
int isEND = false;
//游戏内数据
int allWaves = 0;//总的波数
int currentWave = 1;//当前怪物的波数
int carrotHP = 10;//记录萝卜的血量
int coinNumber = 0;//记录当前金币的数量
Carrot* globalCarrot = nullptr;//萝卜
std::vector<Monster*> monsters;
BarrierManager* barrierManager = nullptr;  // 管理障碍物 
Monster* clickedMonster = NULL;//选中的优先攻击怪兽
Monster* previousSelectedMonster = NULL;
std::vector<Bullet*>bullets;
std::vector<Bullet*>bulletsTowardBarrier;
std::vector<Sunflowerfire*>sunflowers;

//世界坐标与数组的转换
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

static array vec2_to_array(Vec2 vec) { //返回array类型，即数组
    array arr;
    arr.row = 8 - static_cast<int>((vec.y / 128)) - 0.5;
    arr.col = static_cast<int>((vec.x / 128));
    return arr;
}

void clearAllVectors() {
    monsters.clear();
    bullets.clear();
    bulletsTowardBarrier.clear();
    sunflowers.clear();
}

/*********************  GameMenu  ************************/
Layer* GameMenu::createLayer() {
    auto layer = GameMenu::create();
    if (layer == nullptr) {
        CCLOG("Failed to create GameMenu layer");
        return nullptr;
    }
    return layer;
}
//倒计时特效
void GameMenu::start() {
    TouchManager::getInstance().disableTouch();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //倒计时页
    auto time_layer = Layer::create();
    this->addChild(time_layer);
    //倒计时背景盘
    auto readyGoPanel = Sprite::create("/GamePlayScene/ReadyGoPanel.png");
    readyGoPanel->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2));
    time_layer->addChild(readyGoPanel);
    //倒计时数字
    auto readyGoNumber = Sprite::create("/GamePlayScene/ReadyGo_1.png");
    readyGoNumber->setPosition(Vec2(origin.x + visibleSize.width * 0.504,
        origin.y + visibleSize.height / 2));
    time_layer->addChild(readyGoNumber);
    //倒计时转圈
    auto readyGoing = Sprite::create("/GamePlayScene/ReadyGoing.png");
    readyGoing->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2));
    time_layer->addChild(readyGoing);
    readyGoing->runAction(Sequence::create(Repeat::create(RotateBy::create(1, -360), 3), FadeOut::create(0.1), nullptr));
    readyGoPanel->runAction(Sequence::create(DelayTime::create(3), FadeOut::create(0.1), nullptr));
    //帧动画
    Vector<SpriteFrame*> frame;
    frame.pushBack(SpriteFrame::create("/GamePlayScene/ReadyGo_3.png", Rect(0, 0, 95, 114)));
    frame.pushBack(SpriteFrame::create("/GamePlayScene/ReadyGo_2.png", Rect(0, 0, 95, 114)));
    frame.pushBack(SpriteFrame::create("/GamePlayScene/ReadyGo_1.png", Rect(0, 0, 95, 114)));
    readyGoNumber->runAction(Sequence::create(Animate::create(Animation::createWithSpriteFrames(frame, 1)), FadeOut::create(0.1), nullptr));
    //倒计时的时候所有可用格点闪烁
    Sprite* grid[7][12];
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 12; j++) {
            grid[i][j] = static_cast<Sprite*>(this->getChildByTag(i * 100 + j));
        }
    }
    //出怪倒计时特效
    auto readyGo = Sprite::create("/GamePlayScene/ReadyGo1.png");
    readyGo->setPosition(196, 650);
    this->addChild(readyGo, 0);
    readyGo->setVisible(false); // 初始隐藏
    //帧动画
    Vector<SpriteFrame*> frame1;
    if (level == 0) {
        frame1.pushBack(SpriteFrame::create("/MonsterStart/ReadyGo1.png", Rect(0, 0, 200, 260)));
        frame1.pushBack(SpriteFrame::create("/MonsterStart/ReadyGo2.png", Rect(0, 0, 200, 260)));
        frame1.pushBack(SpriteFrame::create("/MonsterStart/ReadyGo3.png", Rect(0, 0, 200, 260)));
    }
    else if (level == 1) {
        frame1.pushBack(SpriteFrame::create("/MonsterStart/1.png", Rect(30, -90, 200, 260)));
        frame1.pushBack(SpriteFrame::create("/MonsterStart/2.png", Rect(30, -90, 200, 260)));
        frame1.pushBack(SpriteFrame::create("/MonsterStart/3.png", Rect(30, -90, 200, 260)));
    }
    else if (level == 2) {
        frame1.pushBack(SpriteFrame::create("/MonsterStart/1.png", Rect(-1100, -180, 1400, 450)));
        frame1.pushBack(SpriteFrame::create("/MonsterStart/2.png", Rect(-1100, -180, 1400, 450)));
        frame1.pushBack(SpriteFrame::create("/MonsterStart/3.png", Rect(-1100, -180, 1400, 450)));
    }
    //动画
    auto animation = Animate::create(Animation::createWithSpriteFrames(frame1, 1.0f)); // 每帧1秒
    auto sequence = Sequence::create(
        DelayTime::create(3.0f), // 延迟3秒
        CallFunc::create([readyGo]() {
            readyGo->setVisible(true); // 3秒后显示精灵
            }),
        animation,               // 播放动画
        FadeOut::create(0.1f),   // 动画完成后淡出
        nullptr
    );
    readyGo->runAction(sequence);
    TouchManager::getInstance().enableTouch();
}

//初始化
bool GameMenu::init()
{
    if (!Layer::init()) {
        return false;
    }
    //获取屏幕大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //菜单栏背景
    auto menu_image = Sprite::create("/GamePlayScene/background.png");
    menu_image->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - menu_image->getContentSize().height / 2));
    menu_image->setOpacity(230);
    this->addChild(menu_image);
    //菜单
    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    //金币形象
    auto coin = Sprite::create();
    coin->setName("Coin");
    this->addChild(coin, 1);
    //金币数值
    auto coin_number = Label::createWithTTF(std::to_string(coinNumber), "/fonts/TMON Monsori.ttf", 40);
    coin_number->setTextColor(Color4B::WHITE);
    coin_number->setName("CoinNumber");
    coin_number->setPosition(282, 980);
    this->addChild(coin_number);

    // 定时更新金币数值
    this->schedule([=](float dt) {
        // 通过直接更新 coin_number 来显示最新的 coinNumber
        coin_number->setString(std::to_string(coinNumber));
        }, 0.1f, "update_coin_key");

    //波数显示
    auto waves_image = Sprite::create("/GamePlayScene/wave_number.png");
    waves_image->setPosition(Vec2(origin.x + visibleSize.width * 0.4,
        origin.y + visibleSize.height * 0.95 + 12));
    this->addChild(waves_image);
    auto waves_label = Label::createWithTTF(std::to_string(currentWave / 10 % 10) + "    " + std::to_string(currentWave % 10), "/fonts/TMON Monsori.ttf", 38);
    waves_label->setName("WavesLabel");
    waves_label->setColor(Color3B::YELLOW);
    waves_label->setPosition(Vec2(origin.x + visibleSize.width * 0.4,
        origin.y + visibleSize.height * 0.94 + 14));
    this->addChild(waves_label);
    auto waves_txt = Label::createWithTTF("/ " + std::to_string(allWaves) + " Waves", "/fonts/TMON Monsori.ttf", 40);
    waves_txt->setPosition(Vec2(origin.x + visibleSize.width * 0.525,
        origin.y + visibleSize.height * 0.94 + 16));
    this->addChild(waves_txt);
    if (level == 2) {
        waves_image->setVisible(false);
        waves_label->setVisible(false);
        waves_txt->setVisible(false);
        auto hp = Sprite::create("/Monster/Hp/boss_hp.png");
        hp->setPosition(704, 980);
        this->addChild(hp, 3);
    }

    // 定时更新波数显示
    this->schedule([waves_label](float dt) {
        // 更新波数显示
        waves_label->setString(std::to_string(currentWave / 10 % 10) + "    " + std::to_string(currentWave % 10));
        }, 0.1f, "update_wave_key");

    //暂停开关：初始isPause=0，如勾选则暂停
    auto paused = Sprite::create("/GamePlayScene/paused.png");
    paused->setScale(1.4);
    paused->setPosition(Vec2(origin.x + visibleSize.width * 0.48,
        origin.y + visibleSize.height * 0.95 + 12));
    paused->setVisible(false);
    this->addChild(paused, 1);
    auto pause_off_sprite = Sprite::create("/GamePlayScene/isNotPaused.png");
    auto pause_off = MenuItemSprite::create(pause_off_sprite, pause_off_sprite);
    auto pause_on_sprite = Sprite::create("/GamePlayScene/isPaused.png");
    auto pause_on = MenuItemSprite::create(pause_on_sprite, pause_on_sprite);
    auto pause_toggle = MenuItemToggle::createWithCallback([=](Ref* psender) {
        if (isPause == 0) {//若勾选，则表示暂停
            isPause = 1;
            paused->setVisible(true);
            waves_image->setVisible(false);
            waves_label->setVisible(false);
            waves_txt->setVisible(false);
        }
        else {
            isPause = 0;
            paused->setVisible(false);
            waves_image->setVisible(true);
            waves_label->setVisible(true);
            waves_txt->setVisible(true);
        }
        }, pause_off, pause_on, nullptr);
    pause_toggle->setPosition(Vec2(Vec2(origin.x + visibleSize.width * 0.7,
        origin.y + visibleSize.height * 0.955)));
    menu->addChild(pause_toggle);
    start();

    //选项
    auto options_btn = Button::create("/GamePlayScene/touming-hd.pvr_28.PNG", "/GamePlayScene/touming-hd.pvr_26.PNG");
    options_btn->setPosition(Vec2(origin.x + visibleSize.width * 0.8,
        origin.y + visibleSize.height * 0.95 + 5));
    options_btn->addTouchEventListener([this](Ref* psender, Button::TouchEventType type) {
        switch (type) {
        case Button::TouchEventType::BEGAN:
            break;
        case Button::TouchEventType::MOVED:
            break;
        case Button::TouchEventType::CANCELED:
            break;
        case Button::TouchEventType::ENDED:
            options();

            break;
        }
        });
    this->addChild(options_btn);
    return true;
}
//选项:设置界面
void GameMenu::options() {
    // 获取屏幕大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /************************  纯色层  *****************************/
    isPause = 1;  // 游戏暂停
    auto blackLayer = LayerColor::create(Color4B::BLACK);
    blackLayer->setPosition(Vec2::ZERO);
    blackLayer->setOpacity(90);  // 不透明度0.9
    this->addChild(blackLayer, 2);  // 将黑色层添加到第二层，确保在其他内容上方

    /************************  事件监听器  *****************************/
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);  // 确保触摸事件不传递给下层
    listener->onTouchBegan = [blackLayer](Touch* touch, Event* event) {
        // 确保返回true来阻止下层的触摸事件
        return true;
        };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, blackLayer);

    /******************  背景  ***************************/
    auto optionsBackground = Sprite::create("/GamePlayScene/options_bg.png");
    optionsBackground->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

    blackLayer->getParent()->getParent()->addChild(optionsBackground, 3);  // 添加到第三层

    /*******************  菜单  ***************************/
    auto optionsMenu = Menu::create();
    optionsMenu->setPosition(Vec2::ZERO);
    blackLayer->getParent()->getParent()->addChild(optionsMenu, 3);  // 确保菜单在最上层

    // 继续游戏按钮
    auto continueGame = MenuItemImage::create("/GamePlayScene/resume_normal.png", "/GamePlayScene/resume_selected.png");
    continueGame->setPosition(Vec2(visibleSize.width * 0.492, visibleSize.height * 0.603));
    // 继续游戏按钮回调
    continueGame->setCallback([this, blackLayer, optionsMenu, optionsBackground](Ref* psender) {
        // 移除黑色层
        this->removeChild(blackLayer);

        // 使用反向迭代器移除菜单中的所有子项
        auto children = optionsMenu->getChildren();
        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            optionsMenu->removeChild(*it, true);  // 第二个参数 true 表示清理资源
        }

        // 移除背景
        optionsBackground->removeFromParent();
        isPause = 0;
        });
    optionsMenu->addChild(continueGame, 3);

    auto restartGame = MenuItemImage::create("/GamePlayScene/restart_normal.png", "/GamePlayScene/restart_selected.png");
    restartGame->setPosition(Vec2(visibleSize.width * 0.492, visibleSize.height * 0.513));
    restartGame->setCallback([this, blackLayer](Ref* psender) {
        this->removeChildByName("PlayingLevel");

        //重新开始游戏时恢复初始状态
        //重置游戏状态
        clearAllVectors();
        // 重新加载游戏场景
        auto newMapScene = MAP_SCENE::create();
        newMapScene->initLevel(level);  // 初始化关卡
        Director::getInstance()->replaceScene(newMapScene);  // 切换到新的场景
        isPause = 0;
        });
    optionsMenu->addChild(restartGame, 3);

    // 选择关卡按钮
    auto returnSelect = MenuItemImage::create("/GamePlayScene/return_normal.png", "/GamePlayScene/return_selected.png");
    returnSelect->setPosition(Vec2(visibleSize.width * 0.492, visibleSize.height * 0.42));
    returnSelect->setCallback([this, blackLayer](Ref* psender) {
        isPause = 0;
        clearAllVectors();
        // 跳转到 HelloWorld 场景
        auto helloWorldScene = HelloWorld::createScene();  // 创建 HelloWorld 场景
        Director::getInstance()->replaceScene(helloWorldScene);  // 替换当前场景
        });
    optionsMenu->addChild(returnSelect, 3);
}




void GameMenu::showLosePopup() {//失败结算画面
    CCLOG("showLosePopup called");
    isPause = true; // 停止游戏逻辑
    clearAllVectors();
    /*******************************  数据更新  *****************************/
   // UserDefault::getInstance()->setIntegerForKey("money_statistics", UserDefault::getInstance()->getIntegerForKey("money_statistics") + coinNumber);
    //UserDefault::getInstance()->setIntegerForKey("monster_statistics", UserDefault::getInstance()->getIntegerForKey("monster_statistics") + activeMonsters.size());
    // 添加其他统计更新逻辑...

    /********************************  显示  ******************************/
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /************************  纯色层  *****************************/
    auto black_layer = LayerColor::create(Color4B::BLACK);
    black_layer->setPosition(Vec2::ZERO);
    black_layer->setOpacity(90);

    auto runningScene = Director::getInstance()->getRunningScene();
    if (runningScene) {
        runningScene->addChild(black_layer, 2);
        CCLOG("Black layer added to running scene.");
    }
    else {
        CCLOG("No running scene. Unable to add black layer.");
    }

    /************************  事件监听器  *****************************/
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true); // 阻止触摸穿透
    listener->onTouchBegan = [black_layer](Touch* touch, Event* event) {
        return true;
        };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, black_layer);

    /******************  背景  ***************************/
    auto lose_bg = Sprite::create("/GamePlayScene/lose.png");
    lose_bg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    lose_bg->setScale(1.7);
    black_layer->addChild(lose_bg, 2);

    // 当前波数显示
    auto waves_label = Label::createWithTTF(std::to_string(currentWave / 10 % 10) + "   " + std::to_string(currentWave % 10), "/fonts/Marker Felt.ttf", 32);
    waves_label->setColor(Color3B::YELLOW);
    waves_label->setPosition(Vec2(origin.x + visibleSize.width * 0.475 + 19, origin.y + visibleSize.height * 0.52));
    black_layer->addChild(waves_label, 2);

    // 波数显示
    auto waves_txt = Label::createWithTTF(std::to_string(allWaves), "/fonts/Marker Felt.ttf", 32);
    waves_txt->setPosition(Vec2(origin.x + visibleSize.width * 0.58 - 50, origin.y + visibleSize.height * 0.52));
    black_layer->addChild(waves_txt, 2);

    // 关卡显示
    auto level_txt = Label::createWithTTF("0" + std::to_string(level), "/fonts/Marker Felt.ttf", 32);
    level_txt->setPosition(Vec2(origin.x + visibleSize.width * 0.4 + 50, origin.y + visibleSize.height * 0.43 + 20));
    black_layer->addChild(level_txt, 2);

    /*******************  菜单  **************************/
    auto options_menu = Menu::create();
    options_menu->setPosition(Vec2::ZERO);
    black_layer->addChild(options_menu, 2);

    // 重新开始按钮
    auto again_btn = MenuItemImage::create("/GamePlayScene/again_normal.png", "/GamePlayScene/again_selected.png");
    again_btn->setPosition(Vec2(visibleSize.width * 0.6, visibleSize.height * 0.3));
    again_btn->setCallback([this](Ref* psender) {
        // button_sound_effect();
        auto newMapScene = MAP_SCENE::create();
        newMapScene->initLevel(level);  // 初始化关卡
        Director::getInstance()->replaceScene(newMapScene);  // 切换到新的场景
        isPause = 0;
        currentWave;
        });
    options_menu->addChild(again_btn, 2);

    // 返回关卡选择按钮
    auto return_btn = MenuItemImage::create("/GamePlayScene/return_normal.png", "/GamePlayScene/return_selected.png");
    return_btn->setPosition(Vec2(visibleSize.width * 0.35, visibleSize.height * 0.3));
    return_btn->setCallback([](Ref* psender) { 
        isPause = 0;
        currentWave = 1;
        // button_sound_effect();
        auto helloWorldScene = HelloWorld::createScene();  // 创建 HelloWorld 场景
        Director::getInstance()->replaceScene(helloWorldScene);  // 替换当前场景

        });
    options_menu->addChild(return_btn, 2);
   
}

void GameMenu::showWinPopup() { //胜利结算画面
    isPause = true; // 停止游戏逻辑
    clearAllVectors();
    if (level == 0) {
        bool isLevel2Unlocked = true;  //玩家解锁了第二关
            // 使用 UserDefault 保存进度
            cocos2d::UserDefault::getInstance()->setBoolForKey("level2Unlocked", isLevel2Unlocked);
            cocos2d::UserDefault::getInstance()->flush();  // 确保立即写入
    }
    /*******************************  数据更新  *****************************/
   // UserDefault::getInstance()->setIntegerForKey("money_statistics", UserDefault::getInstance()->getIntegerForKey("money_statistics") + coinNumber);
   // UserDefault::getInstance()->setIntegerForKey("adventure_statistics", level + 1);
    // 添加其他统计更新逻辑...

    /********************************  显示  ******************************/
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /************************  纯色层  *****************************/
    auto black_layer = LayerColor::create(Color4B::BLACK);
    black_layer->setPosition(Vec2::ZERO);
    black_layer->setOpacity(90);
    Director::getInstance()->getRunningScene()->addChild(black_layer, 1);

    /************************  事件监听器  *****************************/
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [black_layer](Touch* touch, Event* event) {
        return true;
        };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, black_layer);

    /******************  背景  ***************************/
    auto win_bg = Sprite::create("/GamePlayScene/win.png");
    win_bg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    win_bg->setScale(1.7);
    black_layer->addChild(win_bg);


    // 当前波数显示
    auto waves_label = Label::createWithTTF(std::to_string(currentWave / 10 % 10) + "   " + std::to_string(currentWave % 10), "/fonts/Marker Felt.ttf", 32);
    waves_label->setColor(Color3B::YELLOW);
    waves_label->setPosition(Vec2(origin.x + visibleSize.width * 0.475 + 19, origin.y + visibleSize.height * 0.52 - 10));
    black_layer->addChild(waves_label, 2);

    // 波数显示
    auto waves_txt = Label::createWithTTF(std::to_string(allWaves), "/fonts/Marker Felt.ttf", 32);
    waves_txt->setPosition(Vec2(origin.x + visibleSize.width * 0.58 - 50, origin.y + visibleSize.height * 0.52 - 10));
    black_layer->addChild(waves_txt, 2);

    // 关卡显示
    auto level_txt = Label::createWithTTF("0" + std::to_string(level + 1), "/fonts/Marker Felt.ttf", 32);
    level_txt->setPosition(Vec2(origin.x + visibleSize.width * 0.4 + 50, origin.y + visibleSize.height * 0.43 + 20));
    black_layer->addChild(level_txt, 2);

    /*******************  菜单  **************************/
    auto options_menu = Menu::create();
    options_menu->setPosition(Vec2::ZERO);
    black_layer->addChild(options_menu, 2);

    // 返回关卡选择按钮
    auto return_btn = MenuItemImage::create("/GamePlayScene/return_normal.png", "/GamePlayScene/return_selected.png");
    return_btn->setPosition(Vec2(visibleSize.width * 0.48, visibleSize.height * 0.35));
    return_btn->setCallback([](Ref* psender) {
        CCLOG("Button clicked!");  // 调试输出
        currentWave = 1;
        isPause = 0;
        clearAllVectors();
        auto newhelloWorldScene = HelloWorld::createScene();  // 创建 HelloWorld 场景
        Director::getInstance()->replaceScene(newhelloWorldScene);  // 替换当前场景
        });
    options_menu->addChild(return_btn, 2);
 
}

/*********************************************************/



//游戏界面类
/*********************************************************/
MAP_SCENE::MAP_SCENE()
    : background(nullptr)
{
    // 初始化地图状态数组
    memset(mapGrid, SPACE, sizeof(mapGrid)); // 默认所有位置为空白
}

MAP_SCENE::~MAP_SCENE()
{
    // 资源清理（如果有的话）
}

MAP_SCENE* MAP_SCENE::create()
{
    MAP_SCENE* ret = new MAP_SCENE();
    if (ret && ret->init())
    {

        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        return nullptr;
    }
}



// 初始化方法
bool MAP_SCENE::init() {
    // 调用父类的 init
    if (!Scene::init()) {
        return false;
    }
    // 初始化场景的默认内容
    CCLOG("MAP_SCENE default init completed.");
    // 创建触摸事件监听器
  
    // 调用 scheduleUpdate，使得 update 函数每帧被调用
    this->scheduleUpdate();

    return true;
}
void MAP_SCENE::update(float deltaTime) {
    // 更新游戏状态，例如萝卜血量、怪物位置等

        // 判断游戏结束条件
    if (carrotHP <= 0) {
        carrotHP = 10;

        GameMenu::getInstance()->showLosePopup();  // 使用单例调用
        
        return;
    }
    else if ((currentWave == allWaves + 1) && carrotHP > 0)
    {
        currentWave--;
        GameMenu::getInstance()->showWinPopup();  // 使用单例调用
        if (level == 0)isLevel2Unlocked = 1;
       
        return;
    }
}

// 处理实时反馈的函数
void MAP_SCENE::showBuildFeedback(int row, int col)
{
    if (mapGrid[row][col] == PATH) {
        // 如果不能放塔，则显示提示（例如提示框或图标）
        auto feedback = Sprite::create("GamePlayScene/cantBuild.png");
        if (feedback != nullptr) {
            feedback->setPosition(array_to_vec2(row, col));
            this->addChild(feedback, 2); // 确保在其他内容之上
            // 创建延时 0.5 秒后删除精灵的动作
            auto delay = DelayTime::create(0.5f);  // 延迟 0.5 秒
            auto remove = CallFunc::create([this, feedback]() {
                this->removeChild(feedback);  // 删除精灵
                });

            // 执行动作
            feedback->runAction(Sequence::create(delay, remove, nullptr));
        }
    }
    else if (mapGrid[row][col] == SPACE) {
        // 如果可以放塔，可能需要显示一个不同的反馈，或者清除之前的反馈
        auto feedback = Sprite::create("GamePlayScene/Grid.png");
        if (feedback != nullptr) {
            feedback->setPosition(array_to_vec2(row, col));
            this->addChild(feedback, 2);
            addTowerPreview(row, col);
            warningSprites.push_back(feedback);
        }
    }
    else if (mapGrid[row][col] == EXISTED_TOWER) {
        handleTowerClick(row, col);
        updateordeleteTowerPreview(row, col);
    }

    else if (mapGrid[row][col] == BARRIER) {
        if (previousSelectedMonster != nullptr) {
            previousSelectedMonster->selected = false;
            previousSelectedMonster = nullptr;
        }

        Vec2 clickPosition = array_to_vec2(row, col);

        // 假设 BarrierManager 已经被初始化并作为类成员变量
        BarrierInfo* clickedBarrier = barrierManager->getClickedBarrier(clickPosition);

        if (clickedBarrier) {
            // 显示箭头
            clickedBarrier->arrow->setVisible(true);
            // 标记为优先攻击目标
             // 取消之前的选中障碍物
            if (clickedBarrier != barrierManager->selectedBarrier) {
                barrierManager->deselectBarrier();
                barrierManager->selectedBarrier = clickedBarrier;
            }
            else {
                barrierManager->deselectBarrier();
            }

        }
    }

}

void MAP_SCENE::handleMapAction(int row, int col)
{
    if (!isTowerSelected)
    {
        cocos2d::log("Please select a tower first");
        return;
    }
    clearWarningSprites();
    
    if (mapGrid[row][col] == BARRIER)
    {
        cocos2d::log("Cannot place tower, position is occupied by a barrier");
    }
    else if (mapGrid[row][col] == EXISTED_TOWER)
    {
        cocos2d::log("A tower already exists at this position");
    }
    else if (mapGrid[row][col] == PATH)
    {
        cocos2d::log("This position is a path, cannot place tower here");
    }
    else if (mapGrid[row][col] == SPACE)
    {
        cocos2d::log("Tower placed successfully");
    }
}


void MAP_SCENE::handleTowerClick(int row, int col)
{
    // 获取点击位置的塔对象
    Tower* tower = getTowerAt(row, col);
    if (tower == nullptr)
    {
        cocos2d::log("no tower");
        return;
    }

    // 使用 dynamic_cast 来判断塔的具体类型
    if (auto bottleTower = dynamic_cast<BottleTower*>(tower))
    {
        // 如果是 BottleTower 类型的塔
        cocos2d::log("clicked BottleTower");
        // 处理 BottleTower 的相关逻辑
    }
    else if (auto shitTower = dynamic_cast<ShitTower*>(tower))
    {
        // 如果是 ShitTower 类型的塔
        cocos2d::log("clicked ShitTower");
        // 处理 ShitTower 的相关逻辑
    }
    else if (auto sunflowerTower = dynamic_cast<SunflowerTower*>(tower))
    {
        // 如果是 SunflowerTower 类型的塔
        cocos2d::log("clicked SunflowerTower");
        // 处理 SunflowerTower 的相关逻辑
    }
    else
    {
        cocos2d::log("该塔不是已知类型");
    }
}



void MAP_SCENE::addTowerPreview(int row, int col)
{
    std::vector<std::string> towerImages = {
    (coinNumber < 100) ? "GamePlayScene/CantClickbottle.png" : "GamePlayScene/CanClickbottle.png",
    (coinNumber < 120) ? "GamePlayScene/CantClickshit.png" : "GamePlayScene/CanClickshit.png",
    (coinNumber < 200) ? "GamePlayScene/CantClickSun.png" : "GamePlayScene/CanClickSun.png"
    };

    // 每个按钮之间的水平间距
    float offsetX = 128;
    float offsetY = 128;  // 垂直方向的间距
    float startX = array_to_vec2(row, col).x;
    float startY = array_to_vec2(row, col).y;

    // 偏移量：确保按钮不会与grid重叠
    float offsetXForPreview = startX - 64;  // 默认偏移量
    float offsetYForPreview = startY + 64;  // 垂直位置稍微偏移

    // 特判：如果是点击最左边第一列，调整按钮位置
    if (col == 0)
    {
        offsetXForPreview += 64;  // 增加偏移量，确保按钮不会被遮挡
    }
    // 计算按钮位置，防止超出屏幕范围
    int screenWidth = 12;  // 地图的列数
    int screenHeight = 8;  // 地图的行数

    for (int i = 0; i < towerImages.size(); ++i)
    {
        auto button = ui::Button::create(towerImages[i]);
        if (button != nullptr)
        {
            // 使用数组坐标来排列按钮
            float offsetXForButton = offsetXForPreview + (i * offsetX);

            // 如果超出屏幕右边界，则调整到下一行
            if (offsetXForButton + button->getContentSize().width > screenWidth * offsetX)
            {
                offsetXForButton = offsetXForPreview;  // 调整按钮为从最左边开始
                offsetYForPreview -= offsetY;  // 换行放置
            }

            // 确保按钮位置不会超出地图范围
            if (offsetXForPreview < 0) offsetXForPreview = 0;
            if (offsetYForPreview < 0) offsetYForPreview = 0;

            // 判断图片是否属于 "CantClick" 类，如果是禁用按钮
            if (towerImages[i].find("CantClick") != std::string::npos)
            {
                button->setEnabled(false);  // 禁用按钮，不能点击
            }

            // 设置按钮位置
            button->setPosition(Vec2(offsetXForButton, offsetYForPreview));
            this->addChild(button, 1);
            warningSprites.push_back(button);

            // 按钮点击事件
            button->addClickEventListener([this, i, row, col](Ref* sender) {
                onTowerPreviewClicked(i, row, col);  // 处理点击逻辑
                });
        }
    }
}



void MAP_SCENE::onTowerPreviewClicked(int towerIndex, int row, int col)
{
    // 根据点击的塔类型选择相应图片并创建对应的塔类
    Tower* tower = nullptr;

    switch (towerIndex)
    {
    case 0:
        // 创建 BottleTower 实例
        tower = BottleTower::create("GamePlayScene/bottle_level_1.png");
        coinNumber -= 100;
        break;
    case 1:
        // 创建 ShitTower 
        tower = ShitTower::create("GamePlayScene/shit_level_1.png");
        coinNumber -= 120;
        break;
    case 2:
        // 创建 SunflowerTower 
        tower = SunflowerTower::create("GamePlayScene/sunflower_level_1.png");
        coinNumber -= 200;
        break;
    default:
        return; // 如果没有匹配的塔类型，直接返回
    }

    // 检查塔是否创建成功
    if (tower != nullptr)
    {
        tower->setPosition(array_to_vec2(row, col));  // 在grid位置放置塔
        int tag = row * 100 + col;  // 为塔分配唯一的tag
        tower->setTag(tag);  // 给塔设置tag
        this->addChild(tower, 1);  // 将塔添加到场景

    }
    towerArray[row][col] = tower;

    // 更新地图数据
    mapGrid[row][col] = EXISTED_TOWER;  // 设置该位置已放置塔

    // 更新状态
    isTowerSelected = true;  // 设置防御塔已选择

    // 清除所有提示
    clearWarningSprites();
}


void MAP_SCENE::clearWarningSprites()
{
    for (auto sprite : warningSprites)
    {
        this->removeChild(sprite);  // 移除提示图或按钮
    }
    warningSprites.clear();  // 清空列表
}

ui::Button* currentUpgradeButton = nullptr;  // 全局变量，用于记录当前的升级按钮
ui::Button* currentDeleteButton = nullptr;  // 全局变量，用于记录当前的删除按钮

void MAP_SCENE::updateordeleteTowerPreview(int row, int col)
{
    // 如果有其他按钮，先移除它们
    if (currentUpgradeButton != nullptr)
    {
        currentUpgradeButton->removeFromParent();
        currentUpgradeButton = nullptr;
    }
    if (currentDeleteButton != nullptr)
    {
        currentDeleteButton->removeFromParent();
        currentDeleteButton = nullptr;
    }

    // 获取当前格子上的塔
    Tower* tower = getTowerAt(row, col);
    if (tower == nullptr)
    {
        cocos2d::log("该位置没有塔，无法显示按钮！");
        return;
    }

    // 获取塔的升级费用
    const int upgradeCost = tower->getUpgradeCost();
    const int sellPrice = tower->getsellPrice();

    // 创建升级按钮
    auto upgradeButton = ui::Button::create("GamePlayScene/CanUpLevel.png");

    if (coinNumber < tower->getUpgradeCost())
    {
        upgradeButton->loadTextures("GamePlayScene/CantUpLevel.PNG", "GamePlayScene/CantUpLevel.PNG", "");  // 显示“已达最大级别”提示
        upgradeButton->setEnabled(false);  // 禁用按钮，无法点击
    }
   upgradeButton->setPosition(array_to_vec2(row, col) + Vec2(0, 80));  // 按钮位置
    upgradeButton->setTitleText(std::to_string(upgradeCost));           // 设置显示升级费用
    upgradeButton->setTitleColor(Color3B::BLACK);
    upgradeButton->setTitleFontSize(20);
    auto up_label = upgradeButton->getTitleLabel();
    up_label->setPosition(Vec2(upgradeButton->getContentSize().width / 2 + 8,
        upgradeButton->getContentSize().height / 2) + Vec2(0, -25));

    if (tower->getLevel() == 3)
    {
        upgradeButton->loadTextures("GamePlayScene/cant_update.PNG", "GamePlayScene/cant_update.PNG", "");  // 显示“已达最大级别”提示
        upgradeButton->setTitleText("");  // 清空按钮的文字
        upgradeButton->setEnabled(false);  // 禁用按钮，无法点击
    }

    this->addChild(upgradeButton, 2); 
    

    // 保存当前升级按钮
    currentUpgradeButton = upgradeButton;
    // 按钮点击事件：执行升级并移除按钮
    upgradeButton->addClickEventListener([this, tower, row, col, upgradeCost](Ref* sender) mutable {
        // 获取当前背景音乐的音量，保留背景音乐的音量不变
        float bgVolume = CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
        // 设置背景音乐音量为原来的值，以免被修改
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(bgVolume);
        // 播放sell_tower.mp3音效，增加音量（不影响背景音乐）
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(6.0f);  // 设置音效的音量，增加音量
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/uplevel_tower", false);

        upgradeTower(row, col);

        coinNumber -= upgradeCost;

        // 升级后移除按钮
        auto button = dynamic_cast<ui::Button*>(sender);
        // 升级后移除删除按钮
        if (currentDeleteButton != nullptr)
        {
            currentDeleteButton->removeFromParent();
            currentDeleteButton = nullptr;  // 清空全局变量
            cocos2d::log("升级后，删除按钮已移除！");
        }
        if (button != nullptr)
        {
            button->removeFromParent();
            currentUpgradeButton = nullptr;  // 清空全局变量
        }
        });

    // 创建删除按钮
    auto deleteButton = ui::Button::create("GamePlayScene/SellTower.png");  // 替换为删除按钮的图片
    deleteButton->setPosition(array_to_vec2(row, col) + Vec2(0, -80));  // 按钮位置，塔下方
    deleteButton->setTitleText(std::to_string(sellPrice));           // 设置显示升级费用
    deleteButton->setTitleColor(Color3B::BLACK);
    deleteButton->setTitleFontSize(20);
    auto sell_label = deleteButton->getTitleLabel();
    sell_label->setPosition(Vec2(deleteButton->getContentSize().width / 2 + 8,
        deleteButton->getContentSize().height / 2) + Vec2(0, -25));
    this->addChild(deleteButton, 2);

    // 保存当前删除按钮
    currentDeleteButton = deleteButton;

    // 删除按钮点击事件：删除塔
    deleteButton->addClickEventListener([this, tower, row, col, sellPrice](Ref* sender) mutable {

        // 获取当前背景音乐的音量，保留背景音乐的音量不变
        float bgVolume = CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
        // 设置背景音乐音量为原来的值，以免被修改
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(bgVolume);
        // 播放sell_tower.mp3音效，增加音量（不影响背景音乐）
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(5.0f);  // 设置音效的音量，增加音量
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/sell_tower.mp3", false);

        deleteTower(row, col);
        coinNumber += sellPrice;
        
 // 移除删除按钮
        auto button = dynamic_cast<ui::Button*>(sender);
        if (button != nullptr)
        {
            button->removeFromParent();
            currentDeleteButton = nullptr;  // 清空全局变量
        }

        // 同时移除升级按钮
        if (currentUpgradeButton != nullptr)
        {
            currentUpgradeButton->removeFromParent();
            currentUpgradeButton = nullptr;
        }

        cocos2d::log("塔在位置 (%d, %d) 已被删除！", row, col);
        });

    // 在场景中添加点击事件监听器
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [this](Touch* touch, Event* event) {
        // 转换触摸点到节点空间
        Vec2 location = this->convertToNodeSpace(touch->getLocation());

        // 如果点击不在按钮范围内，移除所有按钮
        if (currentUpgradeButton != nullptr)
        {
            // 检查升级按钮是否禁用且点击不在按钮范围内
            if (!currentUpgradeButton->isEnabled() ||
                !currentUpgradeButton->getBoundingBox().containsPoint(location))
            {
                currentUpgradeButton->removeFromParent();
                currentUpgradeButton = nullptr;
                cocos2d::log("点击在按钮外或按钮禁用，移除升级按钮！");
            }
        }

        if (currentDeleteButton != nullptr)
        {
            // 检查删除按钮是否禁用且点击不在按钮范围内
            if (!currentDeleteButton->isEnabled() ||
                !currentDeleteButton->getBoundingBox().containsPoint(location))
            {
                currentDeleteButton->removeFromParent();
                currentDeleteButton = nullptr;
                cocos2d::log("点击在按钮外或按钮禁用，移除删除按钮！");
            }
        }

        return false;
        // 返回 false，不阻止事件继续传递
        };

    // 将事件监听器注册到场景
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}



void MAP_SCENE::deleteTower(int row, int col)
{
    // 获取指定位置的塔对象
    Tower* tower = getTowerAt(row, col);

if (tower == nullptr)
{
    cocos2d::log("该位置没有塔，无法删除！");
    return;
}

// 从父节点中移除塔的可视化节点（如果有显示的 Sprite 或其他内容）
if (tower != nullptr)
{
    tower->removeFromParent();  // 删除塔的精灵（Sprite）
}

// 从管理数组或数据结构中移除塔对象引用
towerArray[row][col] = nullptr;  // 清除塔的引用
mapGrid[row][col] = SPACE;


cocos2d::log("塔在位置 (%d, %d) 已成功删除！", row, col);
}
void MAP_SCENE::upgradeTower(int row, int col)
{
    // 获取当前的位置的塔实例
    Tower* tower = getTowerAt(row, col);  // 获取该位置的塔

    if (tower != nullptr)
    {
        // 执行升级逻辑，这里假设每个塔类都有`upgrade`函数
        tower->upgrade();  // 假设塔类中有升级方法

        // 你可以根据需要更新塔的属性，例如增加攻击力，攻击速度，生命值等
        cocos2d::log("塔在位置 (%d, %d) 升级成功！", row, col);
    }
    else
    {
        cocos2d::log("该位置没有塔，无法升级！");
    }
}

Tower* MAP_SCENE::getTowerAt(int row, int col) {
    // 检查是否越界
    if (row < 0 || row >= 8 || col < 0 || col >= 12) {
        cocos2d::log("无效的位置: (%d, %d)", row, col);
        return nullptr;
    }

    // 直接返回数组中的塔对象
    return towerArray[row][col];
}       

void  MAP_SCENE::initLevel(int level)// 初始化关卡的方法
{
    std::string backgroundImage;
    switch (level) {
    case 0:
        backgroundImage = "/GamePlayScene/map1.png";
        setBackground(backgroundImage);

        break;
    case 1:
        backgroundImage = "/GamePlayScene/map2.png";
        setBackground(backgroundImage);
        break;
    case 2:
        backgroundImage = "/GamePlayScene/map3.png";
        setBackground(backgroundImage);
        break;
    default:
        backgroundImage = "default_background.png";
        setBackground(backgroundImage);
        break;
    }
    initializeMapArray(level);
    initResources();

    // 设置游戏层的名字
    this->setName("PlayingLevel");  // 设置当前层的名字
    // 添加游戏菜单到场景
    auto menuLayer = GameMenu::createLayer();
    if (menuLayer) {
        menuLayer->setName("GameMenu");
        this->addChild(menuLayer, 1); // 添加到较高的层级，以确保在地图上方显示
    }

    //萝卜
    globalCarrot = Carrot::create();
    this->addChild(globalCarrot);  // 将 Carrot 实例添加到场景中

    MonsterCreate* monster_create = NULL;
    // 明确指定 DelayTime 类型
    cocos2d::DelayTime* delay = cocos2d::DelayTime::create(6);  // 显式指定为 cocos2d::DelayTime 类型

    // 创建一个延时6秒的动作执行怪物创建
    auto createMonsters = CallFunc::create([this, &monster_create, level]() {
        // 创建 MonsterCreate 实例
        monster_create = MonsterCreate::create();
        monster_create->MonsterWaves(level);  // 使用捕获的 `level`
        this->addChild(monster_create, 100);

        // 创建触摸事件监听器
        auto listener1 = EventListenerTouchOneByOne::create();

        // 触摸开始时的回调函数
        listener1->onTouchBegan = [=](Touch* touch, Event* event) {
            Vec2 touchLocation = touch->getLocation();
            array arr = vec2_to_array(touchLocation);
            handleMapAction(arr.row, arr.col);
            return true; // 返回true表示吞噬该事件，其他地方不再处理
            };

        // 触摸结束时的回调函数
        listener1->onTouchEnded = [=](Touch* touch, Event* event) {
            // 触摸结束，确保放置塔
            Vec2 touchLocation = touch->getLocation();
            array arr = vec2_to_array(touchLocation);
            // 检查是否点击到了怪物
            Monster* clickedMonster = checkMonsterClicked(touchLocation);
            // 如果点击到怪物，则不执行后续操作
            if (clickedMonster != nullptr) {

                // 如果当前有选中的怪物，取消选中状态
                if (previousSelectedMonster != nullptr) {
                    previousSelectedMonster->selected = false;  // 取消之前选中的怪物
                }
                // 选中当前点击到的怪物
                clickedMonster->selected = true;
                // 保存当前点击的怪物为之前选中的怪物
                previousSelectedMonster = clickedMonster;
                barrierManager->deselectBarrier();  // 取消之前的选中障碍物  
                CCLOG("Clicked on monster: %p", clickedMonster);
                return;  // 返回，不再执行后续的塔放置操作
            }
            else {
                // 如果没有点击到怪物，执行地图操作和放置塔
                handleMapAction(arr.row, arr.col);
                showBuildFeedback(arr.row, arr.col);
            }
            };

        // 添加触摸事件监听器
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
        });

    // 创建一个 Sequence 动作，先延时6秒，再执行怪物生成
    auto sequence = cocos2d::Sequence::create(delay, createMonsters, nullptr);

    // 确保 sequence 已经正确创建
    this->runAction(sequence);

}




Monster* MAP_SCENE::checkMonsterClicked(Vec2 touchLocation) {
    for (auto monster : monsters) {
        // 检查怪物是否有效，并且触摸位置是否在怪物的边界内
        if (monster != nullptr && monster->getBoundingBox().containsPoint(touchLocation)) {
            return monster;  // 返回被点击的怪物指针
        }
    }
    return nullptr;  // 如果没有点击到怪物，返回nullptr
}


void MAP_SCENE::setBackground(const std::string& backgroundImage) {

    // 获取屏幕大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    background = cocos2d::Sprite::create(backgroundImage);
    if (background == nullptr)
    {
        // problemLoading("'menu.png'");
    }
    else
    {
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        background->setContentSize(Size(1536, 1024)); // 设置为1536x1024的大小
        this->addChild(background, 0);//背景层级为0，确保它在最底层
    }

}


void MAP_SCENE::onExitGame() {
    GameManager::getInstance()->endGame(); // 调用 endGame 中止游戏并保存状态
    Director::getInstance()->end();  // 结束游戏
}

/*
void MAP_SCENE::onEnterGame() {
    GameManager::getInstance()->loadGameState(); // 进入时加载状态

    // 根据加载的状态初始化关卡
    this->initLevel(GameManager::getInstance()->level);
    this->initWaves(GameManager::getInstance()->allWaves);
}*/

void  MAP_SCENE::initResources() {
    switch (level) {
    case 0:
        coinNumber = 450;
        allWaves = 5;
        carrotHP = 10;
        currentWave = 1;
        break;
    case 1:
        coinNumber = 550;
        allWaves = 5;
        carrotHP = 10;
        currentWave = 1;
        break;
    case 2:
        coinNumber = 1000;
        allWaves = 5;
        currentWave = 1;
        break;
    }
}

void MAP_SCENE::initializeMapArray(int level) {

    for (int i = 0; i < 12; i++) {
        mapGrid[0][i] = MENU;
    }
    barrierManager = BarrierManager::create();
    switch (level) {
    case 0:

        // 添加不同位置和类型的障碍物
        barrierManager->BarrierAppear(BARRIER_1_1, 320, 448, 1);
        barrierManager->BarrierAppear(BARRIER_1_1, 1216, 448, 1);
        barrierManager->BarrierAppear(BARRIER_1_2, 576, 576, 1);
        barrierManager->BarrierAppear(BARRIER_1_2, 960, 576, 1);
        barrierManager->BarrierAppear(BARRIER_2_1, 768, 320, 2);
        barrierManager->BarrierAppear(BARRIER_4_1, 768, 768, 4);
        barrierManager->BarrierAppear(BARRIER_4_2, 512, 768, 4);
        barrierManager->BarrierAppear(BARRIER_4_2, 1024, 768, 4);

        this->addChild(barrierManager);
         //map1
        mapGrid[2][1] = PATH;   // (2, 3) 是路径
        mapGrid[3][1] = PATH;
        mapGrid[4][1] = PATH;
        mapGrid[5][1] = PATH;
        mapGrid[5][2] = PATH;
        mapGrid[5][3] = PATH;
        mapGrid[5][4] = PATH;
        mapGrid[4][4] = PATH;
        mapGrid[4][5] = PATH;
        mapGrid[4][6] = PATH;
        mapGrid[4][7] = PATH;
        mapGrid[5][7] = PATH;
        mapGrid[5][8] = PATH;
        mapGrid[5][9] = PATH;
        mapGrid[5][10] = PATH;
        mapGrid[4][10] = PATH;
        mapGrid[3][10] = PATH;
        mapGrid[2][10] = PATH;
        mapGrid[vec2_to_array(Vec2(320, 448)).row][vec2_to_array(Vec2(320, 448)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1216, 448)).row][vec2_to_array(Vec2(1216, 448)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(576, 576)).row][vec2_to_array(Vec2(576, 576)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(960, 576)).row][vec2_to_array(Vec2(960, 576)).col] = BARRIER;

        mapGrid[vec2_to_array(Vec2(768 - 64, 320)).row][vec2_to_array(Vec2(768 - 64, 320)).col] = BARRIER;//2
        mapGrid[vec2_to_array(Vec2(768 + 64, 320)).row][vec2_to_array(Vec2(768 + 64, 320)).col] = BARRIER;//2

        mapGrid[vec2_to_array(Vec2(768 - 64, 768 - 64)).row][vec2_to_array(Vec2(768 - 64, 768 - 64)).col] = BARRIER;//4
        mapGrid[vec2_to_array(Vec2(768 - 64, 768 + 64)).row][vec2_to_array(Vec2(768 - 64, 768 + 64)).col] = BARRIER;//4
        mapGrid[vec2_to_array(Vec2(768 + 64, 768 - 64)).row][vec2_to_array(Vec2(768 + 64, 768 - 64)).col] = BARRIER;//4
        mapGrid[vec2_to_array(Vec2(768 + 64, 768 + 64)).row][vec2_to_array(Vec2(768 + 64, 768 + 64)).col] = BARRIER;//4

        mapGrid[vec2_to_array(Vec2(512 - 64, 768 - 64)).row][vec2_to_array(Vec2(512 - 64, 768 - 64)).col] = BARRIER;//4
        mapGrid[vec2_to_array(Vec2(512 + 64, 768 - 64)).row][vec2_to_array(Vec2(512 + 64, 768 - 64)).col] = BARRIER;//4
        mapGrid[vec2_to_array(Vec2(512 - 64, 768 + 64)).row][vec2_to_array(Vec2(512 - 64, 768 + 64)).col] = BARRIER;//4
        mapGrid[vec2_to_array(Vec2(512 + 64, 768 + 64)).row][vec2_to_array(Vec2(512 + 64, 768 + 64)).col] = BARRIER;//4



        mapGrid[vec2_to_array(Vec2(1024 - 64, 768 - 64)).row][vec2_to_array(Vec2(1024 - 64, 768 - 64)).col] = BARRIER;//4
        mapGrid[vec2_to_array(Vec2(1024 - 64, 768 + 64)).row][vec2_to_array(Vec2(1024 - 64, 768 + 64)).col] = BARRIER;//4
        mapGrid[vec2_to_array(Vec2(1024 + 64, 768 - 64)).row][vec2_to_array(Vec2(1024 + 64, 768 - 64)).col] = BARRIER;//4
        mapGrid[vec2_to_array(Vec2(1024 + 64, 768 + 64)).row][vec2_to_array(Vec2(1024 + 64, 768 + 64)).col] = BARRIER;//4

        break;
    case 1:
        // 添加不同位置和类型的障碍物
        barrierManager->BarrierAppear(BARRIER_1_1, 836, 832, 1);
        barrierManager->BarrierAppear(BARRIER_1_1, 836, 192, 1);
        barrierManager->BarrierAppear(BARRIER_1_1, 964, 192, 1);
        barrierManager->BarrierAppear(BARRIER_1_2, 320, 448, 1);
        barrierManager->BarrierAppear(BARRIER_1_2, 448, 448, 1);
        barrierManager->BarrierAppear(BARRIER_1_2, 1216, 192, 1);
        barrierManager->BarrierAppear(BARRIER_1_2, 1088, 832, 1);
        barrierManager->BarrierAppear(BARRIER_1_2, 832, 576, 1);
        barrierManager->BarrierAppear(BARRIER_2_1, 512, 192, 2);
        barrierManager->BarrierAppear(BARRIER_2_2, 640, 448, 2);
        barrierManager->BarrierAppear(BARRIER_4_1, 1024, 512, 4);
        barrierManager->BarrierAppear(BARRIER_4_2, 1408, 512, 4);
        barrierManager->BarrierAppear(BARRIER_4_2, 384, 768, 4);

        this->addChild(barrierManager);
        mapGrid[3][0] = PATH;
        mapGrid[3][1] = PATH;
        mapGrid[3][2] = PATH;
        mapGrid[3][3] = PATH;
        mapGrid[3][4] = PATH;
        mapGrid[2][5] = PATH;
        mapGrid[2][6] = PATH;
        mapGrid[2][7] = PATH;
        mapGrid[2][8] = PATH;
        mapGrid[3][8] = PATH;
        mapGrid[4][8] = PATH;
        mapGrid[5][8] = PATH;
        mapGrid[5][7] = PATH;
        mapGrid[5][6] = PATH;
        mapGrid[5][5] = PATH;
        mapGrid[5][4] = PATH;
        mapGrid[5][3] = PATH;
        mapGrid[5][2] = PATH;
        mapGrid[5][1] = PATH;
        mapGrid[6][1] = PATH;
        mapGrid[7][1] = PATH;
        mapGrid[7][2] = PATH;
        mapGrid[7][3] = PATH;
        mapGrid[7][4] = PATH;
        mapGrid[7][5] = PATH;
        mapGrid[7][6] = PATH;
        mapGrid[7][7] = PATH;
        mapGrid[7][8] = PATH;
        mapGrid[7][9] = PATH;
        mapGrid[7][10] = PATH;
        mapGrid[vec2_to_array(Vec2(836, 832)).row][vec2_to_array(Vec2(836, 832)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(836, 192)).row][vec2_to_array(Vec2(836, 192)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(964, 192)).row][vec2_to_array(Vec2(964, 192)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(320, 448)).row][vec2_to_array(Vec2(320, 448)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(448, 448)).row][vec2_to_array(Vec2(448, 448)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1216, 192)).row][vec2_to_array(Vec2(1216, 192)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1088, 832)).row][vec2_to_array(Vec2(1088, 832)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(832, 576)).row][vec2_to_array(Vec2(832, 576)).col] = BARRIER;
        //2
        mapGrid[vec2_to_array(Vec2(512 + 64, 192)).row][vec2_to_array(Vec2(512 + 64, 192)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(512 - 64, 192)).row][vec2_to_array(Vec2(512 - 64, 192)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(640 - 64, 448)).row][vec2_to_array(Vec2(640 - 64, 448)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(640 + 64, 448)).row][vec2_to_array(Vec2(640 + 64, 448)).col] = BARRIER;
        //4
        mapGrid[vec2_to_array(Vec2(1024 - 64, 512 - 64)).row][vec2_to_array(Vec2(1024 - 64, 512 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1024 - 64, 512 + 64)).row][vec2_to_array(Vec2(1024 - 64, 512 + 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1024 + 64, 512 - 64)).row][vec2_to_array(Vec2(1024 + 64, 512 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1024 + 64, 512 + 64)).row][vec2_to_array(Vec2(1024 + 64, 512 + 64)).col] = BARRIER;

        mapGrid[vec2_to_array(Vec2(1408 - 64, 512 - 64)).row][vec2_to_array(Vec2(1408 - 64, 512 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1408 - 64, 512 + 64)).row][vec2_to_array(Vec2(1408 - 64, 512 + 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1408 + 64, 512 - 64)).row][vec2_to_array(Vec2(1408 + 64, 512 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1408 + 64, 512 + 64)).row][vec2_to_array(Vec2(1408 + 64, 512 + 64)).col] = BARRIER;

        mapGrid[vec2_to_array(Vec2(384 - 64, 768 - 64)).row][vec2_to_array(Vec2(384 - 64, 768 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(384 - 64, 768 + 64)).row][vec2_to_array(Vec2(384 - 64, 768 + 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(384 + 64, 768 - 64)).row][vec2_to_array(Vec2(384 + 64, 768 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(384 + 64, 768 + 64)).row][vec2_to_array(Vec2(384 + 64, 768 + 64)).col] = BARRIER;




        break;

    case 2:

        // 添加不同位置和类型的障碍物
        barrierManager->BarrierAppear(BARRIER_1_1, 64, 832, 1);
        barrierManager->BarrierAppear(BARRIER_1_1, 192, 832, 1);
        barrierManager->BarrierAppear(BARRIER_1_2, 320, 832, 1);
        barrierManager->BarrierAppear(BARRIER_1_1, 448, 832, 1);
        barrierManager->BarrierAppear(BARRIER_1_2, 576, 832, 1);
        barrierManager->BarrierAppear(BARRIER_1_2, 960, 832, 1);
        barrierManager->BarrierAppear(BARRIER_1_2, 1088, 832, 1);
        barrierManager->BarrierAppear(BARRIER_1_1, 1472, 832, 1);
        barrierManager->BarrierAppear(BARRIER_2_1, 1280, 832, 2);
        barrierManager->BarrierAppear(BARRIER_1_2, 832, 448, 1);
        barrierManager->BarrierAppear(BARRIER_1_2, 64, 704, 1);
        barrierManager->BarrierAppear(BARRIER_1_1, 64, 576, 1);
        barrierManager->BarrierAppear(BARRIER_1_2, 1472, 704, 1);
        barrierManager->BarrierAppear(BARRIER_1_1, 1472, 576, 1);
        barrierManager->BarrierAppear(BARRIER_1_2, 1472, 448, 1);
        barrierManager->BarrierAppear(BARRIER_1_1, 1472, 320, 1);
        barrierManager->BarrierAppear(BARRIER_1_2, 1472, 192, 1);
        barrierManager->BarrierAppear(BARRIER_1_1, 1344, 192, 1);
        barrierManager->BarrierAppear(BARRIER_1_2, 960, 64, 1);
        barrierManager->BarrierAppear(BARRIER_1_1, 832, 64, 1);

        barrierManager->BarrierAppear(BARRIER_2_1, 512, 704, 2);
        barrierManager->BarrierAppear(BARRIER_2_1, 1152, 64, 2);
        barrierManager->BarrierAppear(BARRIER_2_1, 768, 320, 2);
        barrierManager->BarrierAppear(BARRIER_2_2, 512, 576, 2);
        barrierManager->BarrierAppear(BARRIER_4_1, 128, 384, 4);
        barrierManager->BarrierAppear(BARRIER_4_1, 512, 256, 4);
        barrierManager->BarrierAppear(BARRIER_4_1, 1152, 512, 4);
        barrierManager->BarrierAppear(BARRIER_4_2, 768, 768, 4);
        barrierManager->BarrierAppear(BARRIER_4_2, 256, 640, 4);



        this->addChild(barrierManager);
        mapGrid[4][2] = PATH;
        mapGrid[5][2] = PATH;
        mapGrid[6][2] = PATH;
        mapGrid[7][2] = PATH;
        mapGrid[4][3] = PATH;
        mapGrid[4][4] = PATH;
        mapGrid[4][5] = PATH;
        mapGrid[3][5] = PATH;
        mapGrid[3][6] = PATH;
        mapGrid[3][7] = PATH;
        mapGrid[2][7] = PATH;
        mapGrid[2][8] = PATH;
        mapGrid[2][9] = PATH;
        mapGrid[2][10] = PATH;
        mapGrid[3][10] = PATH;
        mapGrid[4][10] = PATH;
        mapGrid[5][10] = PATH;
        mapGrid[5][9] = PATH;
        mapGrid[5][8] = PATH;
        mapGrid[5][7] = PATH;
        mapGrid[6][7] = PATH;
        mapGrid[6][6] = PATH;
        mapGrid[6][5] = PATH;
        mapGrid[7][5] = PATH;
        mapGrid[7][4] = PATH;
        mapGrid[7][3] = PATH;


        mapGrid[vec2_to_array(Vec2(64, 832)).row][vec2_to_array(Vec2(64, 832)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(192, 832)).row][vec2_to_array(Vec2(192, 832)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(320, 832)).row][vec2_to_array(Vec2(320, 832)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(448, 832)).row][vec2_to_array(Vec2(448, 832)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(576, 832)).row][vec2_to_array(Vec2(1472, 832)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(960, 832)).row][vec2_to_array(Vec2(960, 832)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1088, 832)).row][vec2_to_array(Vec2(1088, 832)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1472, 832)).row][vec2_to_array(Vec2(1472, 832)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1280, 832)).row][vec2_to_array(Vec2(1280, 832)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(832, 448)).row][vec2_to_array(Vec2(832, 448)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(64, 704)).row][vec2_to_array(Vec2(64, 704)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(64, 576)).row][vec2_to_array(Vec2(64, 576)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1472, 704)).row][vec2_to_array(Vec2(1472, 704)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1472, 576)).row][vec2_to_array(Vec2(1472, 576)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1472, 448)).row][vec2_to_array(Vec2(1472, 448)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1472, 320)).row][vec2_to_array(Vec2(1472, 320)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1472, 192)).row][vec2_to_array(Vec2(1472, 192)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1344, 192)).row][vec2_to_array(Vec2(1344, 192)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(960, 64)).row][vec2_to_array(Vec2(960, 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(832, 64)).row][vec2_to_array(Vec2(832, 64)).col] = BARRIER;

        //2
        mapGrid[vec2_to_array(Vec2(512 - 64, 704)).row][vec2_to_array(Vec2(512 - 64, 704)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(512 + 64, 704)).row][vec2_to_array(Vec2(512 + 64, 704)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1152 - 64, 64)).row][vec2_to_array(Vec2(1152 - 64, 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1152 + 64, 64)).row][vec2_to_array(Vec2(1152 + 64, 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(832 - 64, 64)).row][vec2_to_array(Vec2(832 - 64, 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(832 + 64, 64)).row][vec2_to_array(Vec2(832 + 64, 64)).col] = BARRIER;

        mapGrid[vec2_to_array(Vec2(768 - 64, 320)).row][vec2_to_array(Vec2(768 - 64, 320)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(768 + 64, 320)).row][vec2_to_array(Vec2(768 + 64, 320)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(512 - 64, 576)).row][vec2_to_array(Vec2(512 - 64, 576)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(512 + 64, 576)).row][vec2_to_array(Vec2(512 + 64, 576)).col] = BARRIER;
        //4
        mapGrid[vec2_to_array(Vec2(128 - 64, 384 - 64)).row][vec2_to_array(Vec2(128 - 64, 384 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(128 + 64, 384 - 64)).row][vec2_to_array(Vec2(128 + 64, 384 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(128 - 64, 384 + 64)).row][vec2_to_array(Vec2(128 - 64, 384 + 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(128 + 64, 384 + 64)).row][vec2_to_array(Vec2(128 + 64, 384 + 64)).col] = BARRIER;

        mapGrid[vec2_to_array(Vec2(512 - 64, 256 - 64)).row][vec2_to_array(Vec2(512 - 64, 256 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(512 + 64, 256 - 64)).row][vec2_to_array(Vec2(512 + 64, 256 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(512 + 64, 256 + 64)).row][vec2_to_array(Vec2(512 + 64, 256 + 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(512 - 64, 256 + 64)).row][vec2_to_array(Vec2(512 - 64, 256 + 64)).col] = BARRIER;

        mapGrid[vec2_to_array(Vec2(1152 - 64, 512 - 64)).row][vec2_to_array(Vec2(1152 - 64, 512 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1152 - 64, 512 + 64)).row][vec2_to_array(Vec2(1152 - 64, 512 + 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1152 + 64, 512 - 64)).row][vec2_to_array(Vec2(1152 + 64, 512 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1152 + 64, 512 + 64)).row][vec2_to_array(Vec2(1152 + 64, 512 + 64)).col] = BARRIER;

        mapGrid[vec2_to_array(Vec2(768 - 64, 768 - 64)).row][vec2_to_array(Vec2(768 - 64, 768 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(768 + 64, 768 - 64)).row][vec2_to_array(Vec2(768 + 64, 768 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(768 - 64, 768 + 64)).row][vec2_to_array(Vec2(768 - 64, 768 + 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(768 + 64, 768 + 64)).row][vec2_to_array(Vec2(768 + 64, 768 + 64)).col] = BARRIER;


        mapGrid[vec2_to_array(Vec2(256 - 64, 640 - 64)).row][vec2_to_array(Vec2(256 - 64, 640 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(256 - 64, 640 + 64)).row][vec2_to_array(Vec2(256 - 64, 640 + 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(256 + 64, 640 - 64)).row][vec2_to_array(Vec2(256 + 64, 640 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(256 + 64, 640 + 64)).row][vec2_to_array(Vec2(256 + 64, 640 + 64)).col] = BARRIER;

        break;

    }
}

