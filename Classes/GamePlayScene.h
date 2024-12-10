#pragma once
#ifndef __Game_Play_Scene_H__
#define  __Game_Play_Scene_H__

#include "cocos2d.h"
#include<vector>

//宏定义
#define PATH -1
#define SPACE 0
#define BARRIER 1
#define EXISTED_TOWER 2



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
    void initializeMap() ;
    void onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void showBuildFeedback(int row, int col);
    void clearWarningSprites();//清除提示图标
    void addTowerPreview(int row, int col);
    
    void onTowerPreviewClicked( int towerIndex, int row, int col);
protected:
    cocos2d::Sprite* background;  // 背景
    int map[8][12];                   // 地图状态数组
    bool isTowerSelected = false; // 是否已选择防御塔
    std::vector<Node*> warningSprites; // 存储提示图片和按钮
    // 处理地图上不同位置的行为
    void handleMapAction(int row, int col);
};


#endif // __MAP_H__
