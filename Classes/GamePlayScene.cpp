#include "GamePlayScene.h"
#include"HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "LevelSelectionScene.h"
#include "ui/CocosGUI.h"
#include <memory>  // 引入智能指针
#include "cocos2d.h"
USING_NS_CC;

using namespace cocos2d::ui;


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
    arr.row = 12 - static_cast<int>((vec.y / 128));
    arr.col = static_cast<int>((vec.x / 128));
    return arr;
}

/**********************  全局变量  ***********************/
int coinNumber = 1234;//记录当前金币的数量
/*********************************************************/

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
        background ->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));


        background->setContentSize(Size(1536, 1024)); // 设置为1536x1024的大小


        // add the sprite as a child to this layer
        this->addChild(background, 0);//背景层级为0，确保它在最底层
    }

    //菜单栏背景
    auto menu_image = Sprite::create("/GamePlayScene/background.png");
    menu_image->setPosition(Vec2(origin.x + visibleSize.width / 2,
    origin.y + visibleSize.height - menu_image->getContentSize().height / 2));
    menu_image->setOpacity(230);
    this->addChild(menu_image);
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
    
    // 初始化地图
    initializeMap();


  

    // 创建触摸事件监听器
    auto listener1 = EventListenerTouchOneByOne::create();

    listener1->onTouchBegan = [=](Touch* touch, Event* event) {
        // 获取触摸位置
        Vec2 touchLocation = touch->getLocation();
        // 处理点击位置的行为
        handleMapAction(touchLocation.x, touchLocation.y);
        return true;
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
 

    return true;
}



void MAP_SCENE::initializeMap()
{
    // 这里可以根据需求初始化路径、障碍物、塔等
    // 举例：将一些障碍物设置为1
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
    */
}





void MAP_SCENE::handleMapAction(int row, int col) //暂时用文字表示 需要添加
{
    // 判断点击的地方是什么状态
    if (map[row][col] == BARRIER)
    {
        // 如果是障碍物，不能放塔
        cocos2d::log("无法放置塔，位置被障碍物占用！");
    }
    else if (map[row][col] == EXISTED_TOWER)
    {
        // 如果已经有塔，不能再放置
        cocos2d::log("该位置已有塔！");
    }
    else if (map[row][col] == PATH)
    {
        // 如果是路径，不能放塔
        cocos2d::log("该位置为路径，不能放置塔！");
    }
    else if (map[row][col] == SPACE)
    {
        // 如果是空白位置，可以放塔
        map[row][col] = EXISTED_TOWER;
        cocos2d::log("放置了防御塔！");
        // 这里可以添加创建塔的逻辑，例如创建一个塔的精灵
        // cocos2d::Sprite* tower = cocos2d::Sprite::create("tower.png");
        // tower->setPosition(array_to_vec2(row, col));
        // this->addChild(tower);
    }
}


void MAP_SCENE::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    // 获取触摸位置
    cocos2d::Vec2 touchLocation = touch->getLocation();

    // 将世界坐标转换为数组坐标
    array arr = vec2_to_array(touchLocation);

    // 处理点击位置的行为
    handleMapAction(arr.row, arr.col);

    //return true;
}









