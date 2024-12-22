#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "LevelSelectionScene.h" 

//宏定义
#define PATH -1
#define SPACE 0
#define BARRIER 1
#define EXISTED_TOWER 2

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

  private:
        cocos2d::MenuItemImage* startItem;  // 开始按钮
        cocos2d::MenuItemImage* exitItem;   // 退出按钮
        cocos2d::MenuItemImage* adventureItem; // 去冒险模式按钮
        cocos2d::MenuItemImage* bossItem;   // 去 Boss 模式按钮
};

#endif // __HELLOWORLD_SCENE_H__
