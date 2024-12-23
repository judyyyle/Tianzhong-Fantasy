#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "LevelSelectionScene.h" 
#include "ui/CocosGUI.h"
#include "cocos2d.h"
USING_NS_CC;
#include "audio/include/AudioEngine.h"
using namespace cocos2d::ui;
using namespace cocos2d::experimental;
Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


    // 停止所有音频
    AudioEngine::stopAll();
    // 播放背景音乐（循环播放）
    AudioEngine::play2d("sound/CarrotFantasy.mp3", true);  // 第一个参数是文件路径，第二个参数是是否循环播放
    /////////////////////////////
   
    auto sprite = Sprite::create("MainMenuScene/menu.png");
    if (sprite == nullptr)
    {
        problemLoading("'menu.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);//背景层级为0，确保它在最底层
    }

   // GameManager::getInstance(); // 确保 GameManager 的单例被创建
  
    auto AdeventureModebutton = ui::Button::create("MainMenuScene/AdeventureMode.png", "MainMenuScene/AdventureModeAfterpress.png");
    AdeventureModebutton->setPosition(Vec2(384, visibleSize.height / 2 + origin.y - 300));
    AdeventureModebutton->setScale(2);
    AdeventureModebutton->addClickEventListener([=](Ref* sender) {
        CCLOG("Button clicked!");
        // 点击按钮后切换到 LevelSelectionScene
        Director::getInstance()->replaceScene(LevelSelectionScene::create());
        });
    this->addChild(AdeventureModebutton, 1);


    auto BossModebutton = ui::Button::create("MainMenuScene/BossMode.png", "MainMenuScene/BossModeAfterpress.png");
    BossModebutton->setPosition(Vec2(1080, visibleSize.height / 2 + origin.y - 300));
    BossModebutton->setScale(2);
    BossModebutton->addClickEventListener([=](Ref* sender) {
        CCLOG("Button clicked!");
        MAP_SCENE* newMapScene = MAP_SCENE::create();
        level = 2;
        // 初始化指定等级
        newMapScene->initLevel(level);
        // 切换到新的场景
        Director::getInstance()->replaceScene(newMapScene);
        });
    this->addChild(BossModebutton, 1);
    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
