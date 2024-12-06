#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#include "MonsterStart.h"
#include "Monster.h"
#include "Tower.h"

#include<string>
#include <cmath>
#include <vector>
#include<algorithm>

USING_NS_CC;

extern std::vector<Monster*>monster;
extern std::vector<Monster*>barrier;


//当文件不存在时，打印有用的错误消息而不是segfaulting。
static void problemLoading(const char* filename)
{
    printf("加载时出错： %s\n", filename);
    printf("根据您的编译方式，您可能需要在HelloWorldScene.cpp中的文件名前面添加“Resources/”\n");
}

//在“init”时，需要初始化实例
bool HelloWorld::init()
{
    //////////////////////////////
    //1.超级初始化优先
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    //2.添加一个带有“X”图像的菜单项，单击该菜单项可退出程序
    //    你可以修改它。

    //添加一个“关闭”图标以退出进度。它是一个自动释放对象
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
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    //创建菜单，它是一个自动释放对象
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    //三.在下面添加您的代码.

    //添加标签显示“Hello World”
    //创建并初始化一个标签

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        //将标签放置在屏幕中心
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height));

        //将标签作为子层添加到该层
        this->addChild(label, 1);
    }

    //添加“map1”启动画面”
    auto sprite = Sprite::create("map1.png");
    if (sprite == nullptr)
    {
        problemLoading("'map1.png'");
    }
    else
    {
        //将sprite置于屏幕中央
        sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        //将sprite作为子元素添加到此层
        this->addChild(sprite, 0);
    }

    //出怪特效
    auto effect = Sprite::create("/MonsterStart/monster_appear_effect.png");
    effect->setContentSize(Size(60, 60));
    effect->setPosition(192, 704);
    this->addChild(effect, 0);
    effect->runAction(Sequence::create(ScaleTo::create(0.4, 3), CallFunc::create([effect]() {effect->removeFromParent(); }), nullptr));

    auto carrot = Sprite::create("/Carrot/HP_MAX.png");
    carrot->setContentSize(Size(96, 142));
    carrot->setPosition(1360, 768);
    this->addChild(carrot, 0);

    auto hp = Sprite::create("/Carrot/Hp.png");
    hp->setContentSize(Size(96, 48));
    hp->setPosition(1452, 760);
    this->addChild(hp, 0);

    auto hp_number = Sprite::create("/Carrot/num_1.png");
    hp_number->setContentSize(Size(20, 36));
    hp_number->setPosition(1467, 760);
    this->addChild(hp_number, 0);

    return true;
}

//关闭cocos2d-x游戏场景，退出应用
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //关闭cocos2d-x游戏场景，退出应用
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /* 要导航回原生iOS屏幕（如果存在）而不退出应用程序，
    请不要使用Director：：getInstance（）->end（）和exit（0），
    而是触发在RootViewController.mm中创建的自定义事件，如下所示 */

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

//初始化
cocos2d::Layer* MonsterStart::createLayer() {
	return MonsterStart::create();
}
bool MonsterStart::init() {
	if (!Layer::init()) {
		return false;
	}
	return true;
}

void MonsterStart::start() {
    current_waves = 0;
    this->schedule([this](float dt) {
        if (current_waves >= max_waves) {
            this->unschedule("WaveScheduler");
            return;
        }
        current_waves++;
        this->MonsterAppear(1); // 简单示例，生成一个类型 1 的怪物
        }, 2.0f, "WaveScheduler");
}

void MonsterStart::MonsterAppear(int Type) {
    auto monster = cocos2d::Sprite::create("monster.png");
    if (!monster) return;
    monster->setPosition(cocos2d::Vec2(rand() % 480, rand() % 320));
    this->addChild(monster);
}
