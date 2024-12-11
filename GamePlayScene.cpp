#include "GamePlayScene.h"
#include"HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "LevelSelectionScene.h"
#include "GameManager.h"
#include<vector>
#include "ui/CocosGUI.h"
#include <memory>  // 引入智能指针
#include "cocos2d.h"
USING_NS_CC;

using namespace cocos2d::ui;
// 在 MAP_SCENE 类中，添加一个成员变量来保存提示图标




//世界坐标与数组的转换
struct array {
    int row;
    int col;
};

Vec2 array_to_vec2(int row, int col) { //返回Vec2类型，即世界坐标
    Vec2 vec;
    vec.x = 64 + 128 * col;
    vec.y = 1024 - 64 - 128 * row;
    return vec;
}

array vec2_to_array(Vec2 vec) { //返回array类型，即数组
    array arr;
    arr.row = 8 - static_cast<int>((vec.y / 128)) - 0.5;
    arr.col = static_cast<int>((vec.x / 128));
    return arr;
}




MAP_SCENE::MAP_SCENE()
    : background(nullptr)
{
    // 初始化地图状态数组
    memset(map, SPACE, sizeof(map)); // 默认所有位置为空白
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

void MAP_SCENE::initializeMap()
{
    // 这里可以根据需求初始化路径、障碍物、塔等

    //map1
    map[2][1] = PATH;   // (2, 3) 是路径
    map[3][1] = PATH;
    map[4][1] = PATH;
    map[5][1] = PATH;
    map[5][2] = PATH;
    map[5][3] = PATH;
    map[5][4] = PATH;
    map[4][4] = PATH;
    map[4][5] = PATH;
    map[4][6] = PATH;
    map[4][7] = PATH;
    map[5][7] = PATH;
    map[5][8] = PATH;
    map[5][9] = PATH;
    map[5][10] = PATH;
    map[4][10] = PATH;
    map[3][10] = PATH;
    map[2][10] = PATH;

    //map2
    /* map[3][1] = PATH;
    map[3][2] = PATH;
    map[3][3] = PATH;
    map[3][4] = PATH;
    map[3][5] = PATH;
    map[2][6] = PATH;
    map[2][7] = PATH;
    map[2][8] = PATH;
    map[2][9] = PATH;
    map[3][9] = PATH;
    map[4][9] = PATH;
    map[5][9] = PATH;
    map[5][8] = PATH;
    map[5][7] = PATH;
    map[5][6] = PATH;
    map[5][5] = PATH;
    map[5][4] = PATH;
    map[5][3] = PATH;
    map[5][2] = PATH;
    map[6][2] = PATH;
    map[7][2] = PATH;
    map[7][3] = PATH;
    map[7][4] = PATH;
    map[7][5] = PATH;
    map[7][6] = PATH;
    map[7][7] = PATH;
    map[7][8] = PATH;
    map[7][9] = PATH;
    map[7][10] = PATH;
    map[7][11] = PATH;


    map3
    map[4][2] = PATH;
    map[5][2] = PATH;
    map[6][2] = PATH;
    map[7][2] = PATH;
    map[4][3] = PATH;
    map[4][4] = PATH;
    map[4][5] = PATH;
    map[3][5] = PATH;
    map[3][6] = PATH;
    map[3][7] = PATH;
    map[2][7] = PATH;
    map[2][8] = PATH;
    map[2][9] = PATH;
    map[2][10] = PATH;
    map[3][10] = PATH;
    map[4][10] = PATH;
    map[5][10] = PATH;
    map[5][9] = PATH;
    map[5][8] = PATH;
    map[5][7] = PATH;
    map[6][7] = PATH;
    map[6][6] = PATH;
    map[6][5] = PATH;
    map[7][5] = PATH;
    map[7][4] = PATH;
    map[7][3] = PATH;
    map[7][2] = PATH;

    */

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
    }



    // 初始化地图
    initializeMap();

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
    if (map[row][col] == PATH) {
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
    else if (map[row][col] == SPACE) {
        // 如果可以放塔，可能需要显示一个不同的反馈，或者清除之前的反馈
        auto feedback = Sprite::create("GamePlayScene/Grid.png");
        if (feedback != nullptr) {
            feedback->setPosition(array_to_vec2(row, col));
            this->addChild(feedback, 2);
            addTowerPreview(row, col);
            warningSprites.push_back(feedback);
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
    if (map[row][col] == BARRIER)
    {
        cocos2d::log("无法放置塔，位置被障碍物占用！");
    }
    else if (map[row][col] == EXISTED_TOWER)
    {
        cocos2d::log("该位置已有塔！");
    }
    else if (map[row][col] == PATH)
    {
        cocos2d::log("该位置为路径，不能放置塔！");
    }
    else if (map[row][col] == SPACE)
    {
        //map[row][col] = EXISTED_TOWER;
        cocos2d::log("放置了防御塔！");
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
                isTowerSelected = true;  // 更新状态
                onTowerPreviewClicked(i, row, col);  // 处理点击逻辑
                });
        }
    }
}

void MAP_SCENE::onTowerPreviewClicked(int towerIndex, int row, int col)
{
    // 根据点击的塔类型选择相应图片
    std::string towerImage;
    switch (towerIndex)
    {
    case 0:
        towerImage = "GamePlayScene/bottle1.png";
        break;
    case 1:
        towerImage = "GamePlayScene/shit1.png";
        break;
    case 2:
        towerImage = "GamePlayScene/sunflower1.png";
        break;
    default:
        return;
    }

    // 放置防御塔
    auto tower = Sprite::create(towerImage);
    if (tower != nullptr)
    {
        tower->setPosition(array_to_vec2(row, col));  // 在grid位置放置
        this->addChild(tower, 1);
    }

    // 更新地图数据
    map[row][col] = EXISTED_TOWER;

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

void MAP_SCENE::onExitGame() {
    GameManager::getInstance()->endGame(); // 调用 endGame 中止游戏并保存状态
    Director::getInstance()->end();  // 结束游戏
}

void MAP_SCENE::onEnterGame() {
    GameManager::getInstance()->loadGameState(); // 进入时加载状态

    // 根据加载的状态初始化关卡
    this->initLevel(GameManager::getInstance()->level);
    this->initWaves(GameManager::getInstance()->allWaves);
}

void MAP_SCENE::initializeMapArray(int level) {
    switch (level) {
    case 0:
        //map1
        map[2][1] = PATH;   // (2, 3) 是路径
        map[3][1] = PATH;
        map[4][1] = PATH;
        map[5][1] = PATH;
        map[5][2] = PATH;
        map[5][3] = PATH;
        map[5][4] = PATH;
        map[4][4] = PATH;
        map[4][5] = PATH;
        map[4][6] = PATH;
        map[4][7] = PATH;
        map[5][7] = PATH;
        map[5][8] = PATH;
        map[5][9] = PATH;
        map[5][10] = PATH;
        map[4][10] = PATH;
        map[3][10] = PATH;
        map[2][10] = PATH;
        map[vec2_to_array(Vec2(320, 448)).row][vec2_to_array(Vec2(320, 448)).col] = BARRIER;
        map[vec2_to_array(Vec2(1216, 448)).row][vec2_to_array(Vec2(1216, 448)).col] = BARRIER;
        map[vec2_to_array(Vec2(576, 576)).row][vec2_to_array(Vec2(576, 576)).col] = BARRIER;
        map[vec2_to_array(Vec2(960, 576)).row][vec2_to_array(Vec2(960, 576)).col] = BARRIER;
        map[vec2_to_array(Vec2(768, 320)).row][vec2_to_array(Vec2(768, 320)).col] = BARRIER;
        map[vec2_to_array(Vec2(768, 768)).row][vec2_to_array(Vec2(768, 768)).col] = BARRIER;
        map[vec2_to_array(Vec2(512, 768)).row][vec2_to_array(Vec2(512, 768)).col] = BARRIER;
        map[vec2_to_array(Vec2(1024, 768)).row][vec2_to_array(Vec2(1024, 768)).col] = BARRIER;
        break;
    case 1:
        map[3][1] = PATH;
        map[3][2] = PATH;
        map[3][3] = PATH;
        map[3][4] = PATH;
        map[3][5] = PATH;
        map[2][6] = PATH;
        map[2][7] = PATH;
        map[2][8] = PATH;
        map[2][9] = PATH;
        map[3][9] = PATH;
        map[4][9] = PATH;
        map[5][9] = PATH;
        map[5][8] = PATH;
        map[5][7] = PATH;
        map[5][6] = PATH;
        map[5][5] = PATH;
        map[5][4] = PATH;
        map[5][3] = PATH;
        map[5][2] = PATH;
        map[6][2] = PATH;
        map[7][2] = PATH;
        map[7][3] = PATH;
        map[7][4] = PATH;
        map[7][5] = PATH;
        map[7][6] = PATH;
        map[7][7] = PATH;
        map[7][8] = PATH;
        map[7][9] = PATH;
        map[7][10] = PATH;
        map[7][11] = PATH;
        map[vec2_to_array(Vec2(836, 832)).row][vec2_to_array(Vec2(836, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(836, 192)).row][vec2_to_array(Vec2(836, 192)).col] = BARRIER;
        map[vec2_to_array(Vec2(964, 192)).row][vec2_to_array(Vec2(964, 192)).col] = BARRIER;
        map[vec2_to_array(Vec2(320, 448)).row][vec2_to_array(Vec2(320, 448)).col] = BARRIER;
        map[vec2_to_array(Vec2(448, 448)).row][vec2_to_array(Vec2(448, 448)).col] = BARRIER;
        map[vec2_to_array(Vec2(1216, 192)).row][vec2_to_array(Vec2(1216, 192)).col] = BARRIER;
        map[vec2_to_array(Vec2(1088, 832)).row][vec2_to_array(Vec2(1088, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(832, 576)).row][vec2_to_array(Vec2(832, 576)).col] = BARRIER;
        map[vec2_to_array(Vec2(512, 192)).row][vec2_to_array(Vec2( 512, 192)).col] = BARRIER;
        map[vec2_to_array(Vec2(640, 448)).row][vec2_to_array(Vec2(640, 448)).col] = BARRIER;
        map[vec2_to_array(Vec2(1024, 512)).row][vec2_to_array(Vec2(1024, 512)).col] = BARRIER;
        map[vec2_to_array(Vec2(1408, 512)).row][vec2_to_array(Vec2(1408, 512)).col] = BARRIER;
        map[vec2_to_array(Vec2(384, 768)).row][vec2_to_array(Vec2(384, 768)).col] = BARRIER;
        break;

    case 2:
        map[4][2] = PATH;
        map[5][2] = PATH;
        map[6][2] = PATH;
        map[7][2] = PATH;
        map[4][3] = PATH;
        map[4][4] = PATH;
        map[4][5] = PATH;
        map[3][5] = PATH;
        map[3][6] = PATH;
        map[3][7] = PATH;
        map[2][7] = PATH;
        map[2][8] = PATH;
        map[2][9] = PATH;
        map[2][10] = PATH;
        map[3][10] = PATH;
        map[4][10] = PATH;
        map[5][10] = PATH;
        map[5][9] = PATH;
        map[5][8] = PATH;
        map[5][7] = PATH;
        map[6][7] = PATH;
        map[6][6] = PATH;
        map[6][5] = PATH;
        map[7][5] = PATH;
        map[7][4] = PATH;
        map[7][3] = PATH;
        map[7][2] = PATH;
    
        map[vec2_to_array(Vec2(64, 832)).row][vec2_to_array(Vec2(64, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(192, 832)).row][vec2_to_array(Vec2(192, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(320, 832)).row][vec2_to_array(Vec2(320, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(448, 832)).row][vec2_to_array(Vec2(448, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(576, 832)).row][vec2_to_array(Vec2(1472, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(960, 832)).row][vec2_to_array(Vec2(960, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(1088, 832)).row][vec2_to_array(Vec2(1088, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(1472, 832)).row][vec2_to_array(Vec2(1472, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(1280, 832)).row][vec2_to_array(Vec2(1280, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(832, 448)).row][vec2_to_array(Vec2(832, 448)).col] = BARRIER;
        map[vec2_to_array(Vec2(64, 704)).row][vec2_to_array(Vec2(64, 704)).col] = BARRIER;
        map[vec2_to_array(Vec2(64, 576)).row][vec2_to_array(Vec2(64, 576)).col] = BARRIER;
        map[vec2_to_array(Vec2(1472, 704)).row][vec2_to_array(Vec2(1472, 704)).col] = BARRIER;
        map[vec2_to_array(Vec2(1472, 576)).row][vec2_to_array(Vec2(1472, 576)).col ] = BARRIER;
        map[vec2_to_array(Vec2(1472, 448)).row][vec2_to_array(Vec2(1472, 448)).col] = BARRIER;
        map[vec2_to_array(Vec2(1472, 320)).row][vec2_to_array(Vec2(1472, 320)).col] = BARRIER;
        map[vec2_to_array(Vec2(1472, 192)).row][vec2_to_array(Vec2(1472, 192)).col] = BARRIER;
        map[vec2_to_array(Vec2(1344, 192)).row][vec2_to_array(Vec2(1344, 192)).col] = BARRIER;
        map[vec2_to_array(Vec2(960, 64)).row][vec2_to_array(Vec2(960, 64)).col] = BARRIER;
        map[vec2_to_array(Vec2(832, 64)).row][vec2_to_array(Vec2(832, 64)).col] = BARRIER;
        map[vec2_to_array(Vec2(512, 704)).row][vec2_to_array(Vec2(512, 704)).col] = BARRIER;
        map[vec2_to_array(Vec2(1152, 64)).row][vec2_to_array(Vec2(1152, 64)).col] = BARRIER;
        map[vec2_to_array(Vec2(832, 64)).row][vec2_to_array(Vec2(832, 64)).col] = BARRIER;
        map[vec2_to_array(Vec2(768, 320)).row][vec2_to_array(Vec2(768, 320)).col] = BARRIER;
        map[vec2_to_array(Vec2(512, 576)).row][vec2_to_array(Vec2(512, 576)).col] = BARRIER;
        map[vec2_to_array(Vec2(128, 384)).row][vec2_to_array(Vec2(128, 384)).col] = BARRIER;
        map[vec2_to_array(Vec2(512, 256)).row][vec2_to_array(Vec2(512, 256)).col] = BARRIER;
        map[vec2_to_array(Vec2(1152, 512)).row][vec2_to_array(Vec2(1152, 512)).col] = BARRIER;
        map[vec2_to_array(Vec2(768, 768)).row][vec2_to_array(Vec2(768, 768)).col] = BARRIER;
        map[vec2_to_array(Vec2(256, 640)).row][vec2_to_array(Vec2(256, 640)).col] = BARRIER;
        break;

    }
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
void initWaves(int allWaves)  // 初始化波次的方法
{

}
