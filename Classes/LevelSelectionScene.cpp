#include"HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "LevelSelectionScene.h"
#include "ui/CocosGUI.h"
#include "GamePlayScene.h"

USING_NS_CC;
extern bool isLevel2Unlocked;
extern int level;
using namespace cocos2d::ui;
LevelSelectionScene::LevelSelectionScene()
{
}

LevelSelectionScene::~LevelSelectionScene()
{
}

LevelSelectionScene* LevelSelectionScene::create()
{
    LevelSelectionScene* ret = new LevelSelectionScene();
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

bool LevelSelectionScene::init()
{
    if (!Scene::init())
    {
        return false;
    }
    // 加载游戏进度
   isLevel2Unlocked = cocos2d::UserDefault::getInstance()->getBoolForKey("level2Unlocked", false);
    // 获取屏幕大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 添加简单的标签，表示这是关卡选择场景
    auto label = Label::createWithTTF("Level Selection", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width / 2 + origin.x, 900 + origin.y));
    this->addChild(label, 1);


    auto sprite = Sprite::create("LevelSelectionScene/bg.png");
    if (sprite == nullptr)
    {
        // problemLoading("'menu.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        sprite->setContentSize(Size(1536, 1024)); // 设置为1536x1024的大小
        // add the sprite as a child to this layer
        this->addChild(sprite, 0);//背景层级为0，确保它在最底层
    }

    auto Homebutton = ui::Button::create("LevelSelectionScene/home.png", "LevelSelectionScene/homeafterPress.png");
    Homebutton->setPosition(Vec2(1500, 1000));
    Homebutton->addClickEventListener([=](Ref* sender) {
        CCLOG("Button clicked!");
        // 点击按钮后切换到 LevelSelectionScene
        Director::getInstance()->replaceScene(HelloWorld::create());
        });
    this->addChild(Homebutton, 1);

    auto spritemap1 = Sprite::create("LevelSelectionScene/map1_unlocked.png");
    if (spritemap1 == nullptr)
    {
        // problemLoading("'menu.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        spritemap1->setPosition(Vec2(384 + origin.x, 500 + origin.y));
        spritemap1->setContentSize(Size(576, 384)); // 设置为1536x1024的大小
        // add the sprite as a child to this layer
        this->addChild(spritemap1, 1);//背景层级为0，确保它在最底层
    }

    auto startbutton1 = ui::Button::create("LevelSelectionScene/start.png", "LevelSelectionScene/start_after.png");
    startbutton1->setPosition(Vec2(384, 150));
    startbutton1->addClickEventListener([=](Ref* sender) {
        CCLOG("Button clicked!");
        // 点击按钮后切换到 LevelSelectionScene
        // 创建新的 MAP_SCENE
        MAP_SCENE* newMapScene = MAP_SCENE::create();
        level = 0;
        // 初始化指定等级
        newMapScene->initLevel(level);

        // 切换到新的场景
        Director::getInstance()->replaceScene(newMapScene);

        });
    this->addChild(startbutton1, 1);

   
    if (isLevel2Unlocked == 0) {


        auto spritemap2 = Sprite::create("LevelSelectionScene/map2_locked.png");
        if (spritemap2 == nullptr)
        {
            // problemLoading("'menu.png'");
        }
        else
        {
            // position the sprite on the center of the screen
            spritemap2->setPosition(Vec2(1100 + origin.x, 500 + origin.y));


            spritemap2->setContentSize(Size(576, 384)); // 设置为1536x1024的大小


            // add the sprite as a child to this layer
            this->addChild(spritemap2, 1);//背景层级为0，确保它在最底层
        }

        auto spritelock = Sprite::create("LevelSelectionScene/lock.png");
        if (spritelock == nullptr)
        {
            // problemLoading("'menu.png'");
        }
        else
        {
            // position the sprite on the center of the screen
            spritelock->setPosition(Vec2(1100 + origin.x, 150 + origin.y));
            // add the sprite as a child to this layer
            this->addChild(spritelock, 1);//背景层级为0，确保它在最底层
        }
    }
    else {


        auto spritemap2 = Sprite::create("LevelSelectionScene/map2_unlocked.png");
        if (spritemap2 == nullptr)
        {
            // problemLoading("'menu.png'");
        }
        else
        {
            // position the sprite on the center of the screen
            spritemap2->setPosition(Vec2(1100 + origin.x, 500 + origin.y));


            spritemap2->setContentSize(Size(576, 384)); // 设置为1536x1024的大小


            // add the sprite as a child to this layer
            this->addChild(spritemap2, 1);//背景层级为0，确保它在最底层
        }
        auto startbutton2 = ui::Button::create("LevelSelectionScene/start.png", "LevelSelectionScene/start_after.png");
        startbutton2->setPosition(Vec2(1100 + origin.x, 150 + origin.y));
        startbutton2->addClickEventListener([=](Ref* sender) {
            CCLOG("Button clicked!");
            // 点击按钮后切换到 LevelSelectionScene
            // 创建新的 MAP_SCENE
            MAP_SCENE* newMapScene = MAP_SCENE::create();
            level = 1;
            // 初始化指定等级
            newMapScene->initLevel(level);

            // 切换到新的场景
            Director::getInstance()->replaceScene(newMapScene);

            });
        this->addChild(startbutton2, 1); 
    }
    return true;
}
