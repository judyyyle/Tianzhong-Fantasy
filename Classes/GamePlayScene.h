#pragma once

#ifndef __Game_Play_Scene_H__
#define __Game_Play_Scene_H__

#include "cocos2d.h"
#include <vector>
#include "Tower.h"
#include "BarrierAppear.h"
#include "Monster.h"

// 宏定义
#define MENU -2
#define PATH -1
#define SPACE 0
#define BARRIER 1
#define EXISTED_TOWER 2

#define BARRIER_1_1 0
#define BARRIER_1_2 1
#define BARRIER_2_1 2
#define BARRIER_2_2 3
#define BARRIER_4_1 4
#define BARRIER_4_2 5

extern int mapGrid[8][12];
extern bool isLevel2Unlocked;
class Monster; 
// 前向声明
class Tower;

class MAP_SCENE : public cocos2d::Scene
{
public:
    // 构造函数

  
    MAP_SCENE::MAP_SCENE();
    // 析构函数
    virtual ~MAP_SCENE();
    // 创建场景的静态方法
    static MAP_SCENE* create();
    // 初始化方法，子类可以重写
    bool init() override;
    // 游戏中各个功能的成员函数
    Monster* checkMonsterClicked(Vec2 touchLocation);
    void initializeMapArray(int level);
    void onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void showBuildFeedback(int row, int col);
    void clearWarningSprites();  // 清除提示图标
    void addTowerPreview(int row, int col);
    void deleteTower(int row, int col);
    void upgradeTower(int row, int col);
    Tower* getTowerAt(int row, int col);
    void onTowerPreviewClicked(int towerIndex, int row, int col);
    void updateordeleteTowerPreview(int row, int col);
    void handleTowerClick(int row, int col);
    void initLevel(int level);  // 初始化关卡的方法
    void initResources();
    void setBackground(const std::string& backgroundImage);
    void onExitGame();
    void createMouseEventListener();
    void update(float deltaTime);
  
protected:
    // 类的成员变量
    cocos2d::Sprite* background;  // 背景
    bool isTowerSelected = false;  // 是否已选择防御塔
    std::vector<Node*> warningSprites;  // 存储提示图片和按钮
    Tower* towerArray[8][12] = { nullptr };  // 防御塔数组
    void handleMapAction(int row, int col);// 处理地图上不同位置的行为
  
};

class GameMenu : public cocos2d::Layer
{
public:
    // 构造函数
    static cocos2d::Layer* createLayer();
    // 获取单例实例
    static GameMenu* getInstance() {
        static GameMenu instance;  // 使用静态局部变量来保证单例
        return &instance;
    }
    // 初始化
    virtual bool init();

    // 创建GameMenu对象的宏
    CREATE_FUNC(GameMenu);

    // 游戏操作函数
    void start();
    void options();
    void showLosePopup();
    void showWinPopup();
  

};



class TouchManager {
public:
    // 获取单例实例
    static TouchManager& getInstance() {
        static TouchManager instance;  // 静态局部变量，保证只会创建一次
        return instance;
    }

    // 设置触摸监听器
    void setListener(EventListenerTouchOneByOne* listener) {
        this->listener = listener;
    }

    // 启用触摸监听
    void enableTouch() {
        if (listener) {
            listener->setEnabled(true);
            CCLOG("Touch events enabled");
        }
    }

    // 禁用触摸监听
    void disableTouch() {
        if (listener) {
            listener->setEnabled(false);
            CCLOG("Touch events disabled");
        }
    }

private:
    // 私有构造函数，确保只会创建一个实例
    TouchManager() : listener(nullptr) {}
    // 防止复制构造函数和赋值操作符
    TouchManager(const TouchManager&) = delete;
    TouchManager& operator=(const TouchManager&) = delete;
    EventListenerTouchOneByOne* listener;  // 触摸事件监听器
};



#endif // __Game_Play_Scene_H__
