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
USING_NS_CC;

using namespace cocos2d::ui;
// 在 MAP_SCENE 类中，添加一个成员变量来保存提示图标
int mapGrid[8][12];
int Level = 1;
/**********************  全局变量  ***********************/
//关卡选项

int isPause;//是否暂停
//游戏内数据
int allWaves = 20;//总的波数
int currentWaves = 14;//当前怪物的波数
int carrotHP = 10;//记录萝卜的血量
int coinNumber = 1234;//记录当前金币的数量

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




/*********************  GameMenu  ************************/
Layer* GameMenu::createLayer() {
    auto layer = GameMenu::create();
    if (layer == nullptr) {
        CCLOG("Failed to create GameMenu layer");
        return nullptr;
    }
    return layer;
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
    coin_number->setString(std::to_string(coinNumber));// 更新金币数值显示
    //波数显示
    auto waves_image = Sprite::create("/GamePlayScene/wave_number.png");
    waves_image->setPosition(Vec2(origin.x + visibleSize.width * 0.4,
        origin.y + visibleSize.height * 0.95 + 12));
    this->addChild(waves_image);
    auto waves_label = Label::createWithTTF(std::to_string(currentWaves / 10 % 10) + "    " + std::to_string(currentWaves % 10), "/fonts/TMON Monsori.ttf", 38);
    waves_label->setName("WavesLabel");
    waves_label->setColor(Color3B::YELLOW);
    waves_label->setPosition(Vec2(origin.x + visibleSize.width * 0.4,
        origin.y + visibleSize.height * 0.94 + 14));
    this->addChild(waves_label);
    auto waves_txt = Label::createWithTTF("/ " + std::to_string(allWaves) + " Waves", "/fonts/TMON Monsori.ttf", 40);
    waves_txt->setPosition(Vec2(origin.x + visibleSize.width * 0.525,
        origin.y + visibleSize.height * 0.94 + 16));
    this->addChild(waves_txt);
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
    //选项
    auto options_btn = Button::create("/GamePlayScene/touming-hd.pvr_28.PNG", "/GameScene/touming-hd.pvr_26.PNG");
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
    //获取屏幕大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    /************************  纯色层  *****************************/
    isPause = 1;//游戏暂停
    auto blackLayer = LayerColor::create(Color4B::BLACK);
    blackLayer->setPosition(Vec2::ZERO);
    blackLayer->setOpacity(90);//不透明度0.9
    this->addChild(blackLayer, 1);
    /************************  事件监听器  *****************************/
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);  // 不拦截其他触摸事件

    listener->onTouchBegan = [blackLayer](Touch* touch, Event* event) {
        return true;
        };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, blackLayer);
    /******************  背景  ***************************/
    auto optionsBackground = Sprite::create("/GamePlayScene/options_bg.png");
    optionsBackground->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2));
    blackLayer->addChild(optionsBackground);
    /*******************  菜单  **************************/
    auto optionsMenu = Menu::create();
    optionsMenu->setPosition(Vec2::ZERO);
    blackLayer->addChild(optionsMenu);
    //继续游戏
    auto continueGame = MenuItemImage::create("/GamePlayScene/resume_normal.png", "/GamePlayScene/resume_selected.png");
    continueGame->setPosition(Vec2(visibleSize.width * 0.492, visibleSize.height * 0.603));
    continueGame->setCallback([this, blackLayer](Ref* psender) {//按钮回调事件，返回上一级
        this->removeChild(blackLayer);
        isPause = 0;
        });
    optionsMenu->addChild(continueGame);
    //重新开始
    auto restartGame = MenuItemImage::create("/GamePlayScene/restart_normal.png", "/GamePlayScene/restart_selected.png");
    restartGame->setPosition(Vec2(visibleSize.width * 0.492, visibleSize.height * 0.513));
    restartGame->setCallback([this, blackLayer](Ref* psender) {//按钮回调事件，返回上一级
        this->removeChildByName("PlayingLevel");
        
        isPause = 0;
        });
    optionsMenu->addChild(restartGame);
    //选择关卡
    auto returnSelect = MenuItemImage::create("/GameScene/return_normal.png", "/GameScene/return_selected.png");
    returnSelect->setPosition(Vec2(visibleSize.width * 0.492, visibleSize.height * 0.42));
    returnSelect->setCallback([this, blackLayer](Ref* psender) {//按钮回调事件，返回上一级
        });
    optionsMenu->addChild(returnSelect);
}
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



bool MAP_SCENE::init()
{
    if (!Scene::init())
    {
        return false;
    }

    // 基本初始化，比如添加背景

    // 获取屏幕大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();



    background = cocos2d::Sprite::create("GamePlayScene/map1.png");//先写死第一张地图 ，如何选择第几张地图待修改
   if (background == nullptr)
    {
        // problemLoading("'menu.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));


        background->setContentSize(Size(1536, 1024)); // 设置为1536x1024的大小


        // add the sprite as a child to this layer
        this->addChild(background, 0);//背景层级为0，确保它在最底层

        //  菜单层  
        /*
        auto menu_layer = GameMenu::createLayer();
        menu_layer->setName("GameMenu");
        this->addChild(menu_layer, 2);
        */
    }



  
    

    initializeMapArray(0);
    // 创建触摸事件监听器
    auto listener1 = EventListenerTouchOneByOne::create();

    // 触摸开始时的回调函数
    listener1->onTouchBegan = [=](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();
        array arr = vec2_to_array(touchLocation);
        handleMapAction(arr.row, arr.col);
        return true; // 返回true表示吞噬该事件，其他地方不再处理
        };

    // 触摸移动时的回调函数
   /*listener1->onTouchMoved = [=](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();
        array arr = vec2_to_array(touchLocation);

        // 实时反馈，可能需要展示提示，或者改变目标位置的状态
        // 比如给玩家显示一个“不能放置塔”的提示
        showBuildFeedback(arr.row, arr.col);
        };*/

        // 触摸结束时的回调函数
    listener1->onTouchEnded = [=](Touch* touch, Event* event) {
        // 触摸结束，确保放置塔
        Vec2 touchLocation = touch->getLocation();
        array arr = vec2_to_array(touchLocation);
        handleMapAction(arr.row, arr.col);
        showBuildFeedback(arr.row, arr.col);
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);

    return true;
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
        Vec2 clickPosition = array_to_vec2(row, col);

        // 假设 BarrierManager 已经被初始化并作为类成员变量
        BarrierInfo* clickedBarrier = barrierManager->getClickedBarrier(clickPosition);

        if (clickedBarrier) {
            // 显示箭头
            clickedBarrier->arrow->setVisible(true);

            // 标记为优先攻击目标
            barrierManager->deselectBarrier();  // 取消之前的选中障碍物
            barrierManager->selectedBarrier = clickedBarrier;

        }
    }

}

void MAP_SCENE::handleMapAction(int row, int col)
{
    // 如果未选择防御塔，禁止其他交互
    if (!isTowerSelected)
    {
        cocos2d::log("请先选择一个防御塔！");
        return;
    }

    // 清除之前的提示
    clearWarningSprites();

    // 根据点击的位置处理逻辑
    if (mapGrid[row][col] == BARRIER)
    {
        cocos2d::log("无法放置塔，位置被障碍物占用！");
    }
    else if (mapGrid[row][col] == EXISTED_TOWER)
    {
        cocos2d::log("该位置已有塔！");
    }
    else if (mapGrid[row][col] == PATH)
    {
        cocos2d::log("该位置为路径，不能放置塔！");
    }
    else if (mapGrid[row][col] == SPACE)
    {
        //map[row][col] = EXISTED_TOWER;
        cocos2d::log("放置了防御塔！");
    }

}

void MAP_SCENE::handleTowerClick(int row, int col)
{
    // 获取点击位置的塔对象
    Tower* tower = getTowerAt(row, col);
    if (tower == nullptr)
    {
        cocos2d::log("没有塔在此位置");
        return;
    }

    // 使用 dynamic_cast 来判断塔的具体类型
    if (auto bottleTower = dynamic_cast<BottleTower*>(tower))
    {
        // 如果是 BottleTower 类型的塔
        cocos2d::log("点击了 BottleTower!");
        // 处理 BottleTower 的相关逻辑
    }
    else if (auto shitTower = dynamic_cast<ShitTower*>(tower))
    {
        // 如果是 ShitTower 类型的塔
        cocos2d::log("点击了 ShitTower!");
        // 处理 ShitTower 的相关逻辑
    }
    else if (auto sunflowerTower = dynamic_cast<SunflowerTower*>(tower))
    {
        // 如果是 SunflowerTower 类型的塔
        cocos2d::log("点击了 SunflowerTower!");
        // 处理 SunflowerTower 的相关逻辑
    }
    else
    {
        cocos2d::log("该塔不是已知类型!");
    }
}




void MAP_SCENE::addTowerPreview(int row, int col)
{
    std::vector<std::string> towerImages = {
        "GamePlayScene/CanClick1.png",
        "GamePlayScene/CanClick1_shit.png",
        "GamePlayScene/SunflowerButton.png"
    };

    // 每个按钮之间的水平间距
    float offsetX = 128;
    float offsetY = 128;  // 垂直方向的间距
    float startX = array_to_vec2(row, col).x;
    float startY = array_to_vec2(row, col).y;

    // 偏移量：确保按钮不会与grid重叠
    float offsetXForPreview = startX - 128;  // 将按钮稍微向右偏移
    float offsetYForPreview = startY + 64;  // 将按钮稍微向下偏移

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

            // 设置按钮位置
            button->setPosition(Vec2(offsetXForButton, offsetYForPreview));
            this->addChild(button, 1);
            warningSprites.push_back(button);

            // 按钮点击事件
            button->addClickEventListener([this, i, row, col](Ref* sender) {
                //isTowerSelected = true;  // 更新状态
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
        break;
    case 1:
        // 创建 ShitTower 实例（假设你有这个塔类）
        tower = ShitTower::create("GamePlayScene/shit_level_1.png");
        break;
    case 2:
        // 创建 SunflowerTower 实例（假设你有这个塔类）
        tower = SunflowerTower::create("GamePlayScene/sunflower_level_1.png");
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
    int upgradeCost = tower->getUpgradeCost();
    int sellPrice = tower->getsellPrice();

    // 创建升级按钮
    auto upgradeButton = ui::Button::create("GamePlayScene/CanUpLevel.png");
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

    }
    this->addChild(upgradeButton, 2);

    // 保存当前升级按钮
    currentUpgradeButton = upgradeButton;

    // 按钮点击事件：执行升级并移除按钮
    upgradeButton->addClickEventListener([this, tower, row, col](Ref* sender) {
        upgradeTower(row, col);

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
    deleteButton->addClickEventListener([this, tower, row, col](Ref* sender) {
        deleteTower(row, col);

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
        if (currentUpgradeButton != nullptr &&
            !currentUpgradeButton->getBoundingBox().containsPoint(location))
        {
            currentUpgradeButton->removeFromParent();
            currentUpgradeButton = nullptr;
            cocos2d::log("点击在按钮外，移除升级按钮！");
        }

        if (currentDeleteButton != nullptr &&
            !currentDeleteButton->getBoundingBox().containsPoint(location))
        {
            currentDeleteButton->removeFromParent();
            currentDeleteButton = nullptr;
            cocos2d::log("点击在按钮外，移除删除按钮！");
        }

        return false;  // 返回 false，不阻止事件继续传递
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
        backgroundImage = "map1.png";
        setBackground(backgroundImage);
       

        break;
    case 1:
        backgroundImage = "map2.png";
        setBackground(backgroundImage);
        

        break;
    case 2:
        backgroundImage = "map3.png";
        setBackground(backgroundImage);

        break;
    default:
        backgroundImage = "default_background.png";
        setBackground(backgroundImage);
        break;
    }
    // initTowers(level);初始化防御塔位置
    initializeMapArray(level);//地图数组(路径) 
    //initResources(); 初始化游戏资源

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
        // position the sprite on the center of the screen
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        background->setContentSize(Size(1536, 1024)); // 设置为1536x1024的大小
        // add the sprite as a child to this layer
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

void MAP_SCENE::initializeMapArray(int level) {
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
        //barrierManager->createMouseEventListener();
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

        mapGrid[vec2_to_array(Vec2(768-64, 320)).row][vec2_to_array(Vec2(768-64, 320)).col] = BARRIER;//2
        mapGrid[vec2_to_array(Vec2(768 + 64, 320)).row][vec2_to_array(Vec2(768 + 64, 320)).col] = BARRIER;//2

        mapGrid[vec2_to_array(Vec2(768-64, 768-64)).row][vec2_to_array(Vec2(768-64, 768-64)).col] = BARRIER;//4
        mapGrid[vec2_to_array(Vec2(768 - 64, 768 + 64)).row][vec2_to_array(Vec2(768 - 64, 768 + 64)).col] = BARRIER;//4
        mapGrid[vec2_to_array(Vec2(768 + 64, 768 - 64)).row][vec2_to_array(Vec2(768 + 64, 768 - 64)).col] = BARRIER;//4
        mapGrid[vec2_to_array(Vec2(768 + 64, 768 + 64)).row][vec2_to_array(Vec2(768 + 64, 768 + 64)).col] = BARRIER;//4

        mapGrid[vec2_to_array(Vec2(512-64, 768-64)).row][vec2_to_array(Vec2(512-64, 768-64)).col] = BARRIER;//4
        mapGrid[vec2_to_array(Vec2(512+64, 768-64)).row][vec2_to_array(Vec2(512+64, 768-64)).col] = BARRIER;//4
        mapGrid[vec2_to_array(Vec2(512 - 64, 768 + 64)).row][vec2_to_array(Vec2(512 - 64, 768 + 64)).col] = BARRIER;//4
        mapGrid[vec2_to_array(Vec2(512 + 64, 768 + 64)).row][vec2_to_array(Vec2(512 + 64, 768 + 64)).col] = BARRIER;//4



        mapGrid[vec2_to_array(Vec2(1024-64, 768-64)).row][vec2_to_array(Vec2(1024-64, 768-64)).col] = BARRIER;//4
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
        //barrierManager->createMouseEventListener();

        mapGrid[3][1] = PATH;
        mapGrid[3][2] = PATH;
        mapGrid[3][3] = PATH;
        mapGrid[3][4] = PATH;
        mapGrid[3][5] = PATH;
        mapGrid[2][6] = PATH;
        mapGrid[2][7] = PATH;
        mapGrid[2][8] = PATH;
        mapGrid[2][9] = PATH;
        mapGrid[3][9] = PATH;
        mapGrid[4][9] = PATH;
        mapGrid[5][9] = PATH;
        mapGrid[5][8] = PATH;
        mapGrid[5][7] = PATH;
        mapGrid[5][6] = PATH;
        mapGrid[5][5] = PATH;
        mapGrid[5][4] = PATH;
        mapGrid[5][3] = PATH;
        mapGrid[5][2] = PATH;
        mapGrid[6][2] = PATH;
        mapGrid[7][2] = PATH;
        mapGrid[7][3] = PATH;
        mapGrid[7][4] = PATH;
        mapGrid[7][5] = PATH;
        mapGrid[7][6] = PATH;
        mapGrid[7][7] = PATH;
        mapGrid[7][8] = PATH;
        mapGrid[7][9] = PATH;
        mapGrid[7][10] = PATH;
        mapGrid[7][11] = PATH;
        mapGrid[vec2_to_array(Vec2(836, 832)).row][vec2_to_array(Vec2(836, 832)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(836, 192)).row][vec2_to_array(Vec2(836, 192)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(964, 192)).row][vec2_to_array(Vec2(964, 192)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(320, 448)).row][vec2_to_array(Vec2(320, 448)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(448, 448)).row][vec2_to_array(Vec2(448, 448)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1216, 192)).row][vec2_to_array(Vec2(1216, 192)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1088, 832)).row][vec2_to_array(Vec2(1088, 832)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(832, 576)).row][vec2_to_array(Vec2(832, 576)).col] = BARRIER;
        //2
        mapGrid[vec2_to_array(Vec2(512+64, 192)).row][vec2_to_array(Vec2(512+64, 192)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(512-64, 192)).row][vec2_to_array(Vec2(512-64, 192)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(640-64, 448)).row][vec2_to_array(Vec2(640-64, 448)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(640 + 64, 448)).row][vec2_to_array(Vec2(640 + 64, 448)).col] = BARRIER;
        //4
        mapGrid[vec2_to_array(Vec2(1024-64, 512-64)).row][vec2_to_array(Vec2(1024-64, 512-64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1024-64, 512+64)).row][vec2_to_array(Vec2(1024-64, 512+64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1024 + 64, 512 - 64)).row][vec2_to_array(Vec2(1024+64, 512-64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1024 + 64, 512 + 64)).row][vec2_to_array(Vec2(1024+64, 512 + 64)).col] = BARRIER;

        mapGrid[vec2_to_array(Vec2(1408-64, 512-64)).row][vec2_to_array(Vec2(1408-64, 512-64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1408-64, 512+64)).row][vec2_to_array(Vec2(1408-64, 512+64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1408 + 64, 512 - 64)).row][vec2_to_array(Vec2(1408 + 64, 512 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1408 + 64, 512 + 64)).row][vec2_to_array(Vec2(1408 + 64, 512 + 64)).col] = BARRIER;

        mapGrid[vec2_to_array(Vec2(384-64, 768-64)).row][vec2_to_array(Vec2(384-64, 768-64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(384 - 64, 768 + 64)).row][vec2_to_array(Vec2(384 - 64, 768 + 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(384 + 64, 768 - 64)).row][vec2_to_array(Vec2(384 + 64, 768 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(384 + 64, 768 + 64)).row][vec2_to_array(Vec2(384 + 64, 768 + 64)).col] = BARRIER;




        break;

    case 2:
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
       //barrierManager->createMouseEventListener();
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
        mapGrid[7][2] = PATH;

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
        mapGrid[vec2_to_array(Vec2(512, 704)).row][vec2_to_array(Vec2(512, 704)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1152, 64)).row][vec2_to_array(Vec2(1152, 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(832, 64)).row][vec2_to_array(Vec2(832, 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(768, 320)).row][vec2_to_array(Vec2(768, 320)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(512, 576)).row][vec2_to_array(Vec2(512, 576)).col] = BARRIER;
        //2
        mapGrid[vec2_to_array(Vec2(128-64, 384)).row][vec2_to_array(Vec2(128-64, 384)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(128+64, 384)).row][vec2_to_array(Vec2(128+64, 384)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(512-64, 256)).row][vec2_to_array(Vec2(512-64, 256)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(512+ 64, 256)).row][vec2_to_array(Vec2(512+64, 256)).col] = BARRIER;
        //4
        mapGrid[vec2_to_array(Vec2(1152-64, 512-64)).row][vec2_to_array(Vec2(1152-64, 512-64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1152 - 64, 512 +64)).row][vec2_to_array(Vec2(1152 - 64, 512+64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1152+ 64, 512 - 64)).row][vec2_to_array(Vec2(1152+64, 512 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(1152 + 64, 512 + 64)).row][vec2_to_array(Vec2(1152+64, 512+64)).col] = BARRIER;

        mapGrid[vec2_to_array(Vec2(768-64, 768-64)).row][vec2_to_array(Vec2(768-64, 768-64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(768+64, 768 - 64)).row][vec2_to_array(Vec2(768 + 64, 768 - 64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(768 - 64, 768 + 64)).row][vec2_to_array(Vec2(768 - 64, 768+64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(768 + 64, 768 + 64)).row][vec2_to_array(Vec2(768 + 64, 768 + 64)).col] = BARRIER;


        mapGrid[vec2_to_array(Vec2(256-64, 640-64)).row][vec2_to_array(Vec2(256-64, 640-64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(256-64, 640+64)).row][vec2_to_array(Vec2(256-64, 640+64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(256+64, 640-64)).row][vec2_to_array(Vec2(256+64, 640-64)).col] = BARRIER;
        mapGrid[vec2_to_array(Vec2(256 + 64, 640+64)).row][vec2_to_array(Vec2(256 + 64, 640+64)).col] = BARRIER;

        break;

    }
}
