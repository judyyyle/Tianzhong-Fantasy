#pragma once
#ifndef __Game_Play_Scene_H__
#define  __Game_Play_Scene_H__

#include "cocos2d.h"
#include<vector>
#include"Tower.h"
#include "BarrierAppear.h"
//宏定义
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

class MAP_SCENE : public cocos2d::Scene
{
public:
    MAP_SCENE();        // 构造函数
    virtual ~MAP_SCENE(); // 析构函数

    // 创建场景的静态方法
    static MAP_SCENE* create();

    // 初始化方法，子类可以重写
    bool init() override;

    // 用于初始化地图（子类重写此方法）
    void initializeMap();
    void initializeMapArray(int level);
    void onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void showBuildFeedback(int row, int col);
    void clearWarningSprites();//清除提示图标

    void addTowerPreview(int row, int col);
    void deleteTower(int row, int col);
    void upgradeTower(int row, int col);
    Tower* getTowerAt(int row, int col);
    void onTowerPreviewClicked(int towerIndex, int row, int col);
    void updateordeleteTowerPreview(int row, int col);
    void handleTowerClick(int row, int col);
    void initLevel(int level); // 初始化关卡的方法
    void setBackground(const std::string& backgroundImage);
    //void initWaves(int allWaves); // 初始化波次的方法
    void onExitGame();
   // void onEnterGame();
   
    void createMouseEventListener();
protected:
    cocos2d::Sprite* background;  // 背景
       // 地图状态数组
    bool isTowerSelected = false; // 是否已选择防御塔
    std::vector<Node*> warningSprites; // 存储提示图片和按钮
    // 处理地图上不同位置的行为
    void handleMapAction(int row, int col);
    Tower* towerArray[8][12] = { nullptr };
    BarrierManager* barrierManager = nullptr;  // 管理障碍物
};
//游戏菜单类
class GameMenu :public cocos2d::Layer
{
public:
    //构造函数
    static cocos2d::Layer* createLayer();
    virtual bool init();
    CREATE_FUNC(GameMenu);
    //选项
    void options();
    void start();
};

#endif // __MAP_H__
