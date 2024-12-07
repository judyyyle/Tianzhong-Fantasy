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
    //添加一个“关闭”图标以退出进度。它是一个自动释放对象
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0) {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    //创建菜单，它是一个自动释放对象
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    //3.在下面添加您的代码.
    //添加“map1”启动画面”
    auto sprite = Sprite::create("map1.png");
    if (sprite == nullptr) {
        problemLoading("'map1.png'");
    }
    else {
        //将sprite置于屏幕中央
        sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        //将sprite作为子元素添加到此层
        this->addChild(sprite, 0);
    }

   

    return true;
}

//关闭cocos2d-x游戏场景，退出应用
void HelloWorld::menuCloseCallback(Ref* pSender){
    //关闭cocos2d-x游戏场景，退出应用
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
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
    //获取屏幕大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
}

//倒计时特效
void MonsterStart::start() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //倒计时页
    auto time_layer = Layer::create();
    this->addChild(time_layer);
    //倒计时背景盘
    auto readyGoPanel = Sprite::create("/GameScene/ReadyGoPanel.png");
    readyGoPanel->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2));
    time_layer->addChild(readyGoPanel);
    //倒计时数字
    auto readyGoNumber = Sprite::create("/MonsterStart/ReadyGo_1.png");
    readyGoNumber->setPosition(Vec2(origin.x + visibleSize.width * 0.504,
        origin.y + visibleSize.height / 2));
    time_layer->addChild(readyGoNumber);
    //倒计时转圈
    auto readyGoing = Sprite::create("/MonsterStart/ReadyGoing.png");
    readyGoing->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2));
    time_layer->addChild(readyGoing);
    readyGoing->runAction(Sequence::create(Repeat::create(RotateBy::create(1, -360), 3), FadeOut::create(0.1), nullptr));
    readyGoPanel->runAction(Sequence::create(DelayTime::create(3), FadeOut::create(0.1), nullptr));
    //帧动画
    Vector<SpriteFrame*> frame;
    frame.pushBack(SpriteFrame::create("/MonsterStart/ReadyGo_3.png", Rect(0, 0, 95, 114)));
    frame.pushBack(SpriteFrame::create("/MonsterStart/ReadyGo_2.png", Rect(0, 0, 95, 114)));
    frame.pushBack(SpriteFrame::create("/MonsterStart/ReadyGo_1.png", Rect(0, 0, 95, 114)));
    readyGoNumber->runAction(Sequence::create(Animate::create(Animation::createWithSpriteFrames(frame, 1)), FadeOut::create(0.1), nullptr));
    //倒计时的时候所有可用格点闪烁
    Sprite* grid[7][12];
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 12; j++) {
            grid[i][j] = static_cast<Sprite*>(this->getChildByTag(i * 100 + j));
        }
    }
    //出怪倒计时特效
    auto readyGo = Sprite::create("/MonsterStart/ReadyGo1.png");
    readyGo->setPosition(196, 650);
    this->addChild(readyGo, 0);
    readyGo->setVisible(false); // 初始隐藏
    //帧动画
    Vector<SpriteFrame*> frame1;
    frame1.pushBack(SpriteFrame::create("/MonsterStart/ReadyGo1.PNG", Rect(0, 0, 200, 260)));
    frame1.pushBack(SpriteFrame::create("/MonsterStart/ReadyGo2.PNG", Rect(0, 0, 200, 260)));
    frame1.pushBack(SpriteFrame::create("/MonsterStart/ReadyGo3.PNG", Rect(0, 0, 200, 260)));
    //动画
    auto animation = Animate::create(Animation::createWithSpriteFrames(frame1, 1.0f)); // 每帧1秒
    auto sequence = Sequence::create(
        DelayTime::create(3.0f), // 延迟3秒
        CallFunc::create([readyGo]() {
            readyGo->setVisible(true); // 3秒后显示精灵
            }),
        animation,               // 播放动画
        FadeOut::create(0.1f),   // 动画完成后淡出
        nullptr
    );
    readyGo->runAction(sequence);
}

//出怪特效
void MonsterStart::MonsterAppear(int Type) {
    auto effect = Sprite::create("/MonsterStart/monster_appear_effect.png");
    effect->setContentSize(Size(60, 60));
    effect->setPosition(192, 704);
    this->addChild(effect, 1);
    //怪物
    auto monster = Sprite::create("/Monster/normal1_1.png");
    monster->setContentSize(Size(128, 128));
    monster->setPosition(192, 704);
    monster->setVisible(false);
    this->addChild(monster, 1);
    // 出怪放大、淡出特效
    auto monsterScaleEffect = ScaleTo::create(0.5f, 3.5f); // 0.5秒放大到3.5倍
    auto monsterFadeEffect = FadeOut::create(0.5f);       // 同时淡出
    // 显示怪物
    auto showMonster = CallFunc::create([monster]() {
        monster->setVisible(true);
        });
    // 动作序列：放大特效 -> 淡出特效 -> 显示怪物
    auto monsterSequence = Sequence::create(monsterScaleEffect, monsterFadeEffect, showMonster, nullptr);
    effect->runAction(monsterSequence);
}

//萝卜形象、血量的变化
void MonsterStart::CarrotAppear() {
    //萝卜形象（根据血量数值变化）
    auto carrot = Sprite::create("/Carrot/HP_MAX.png");
    carrot->setContentSize(Size(96, 142));
    carrot->setPosition(1360, 768);
    carrot->setName("carrot");
    this->addChild(carrot, 0);
    //萝卜血量背景板
    auto carrot_hp = Sprite::create("/Carrot/Hp.png");
    carrot_hp->setContentSize(Size(96, 48));
    carrot_hp->setPosition(1452, 760);
    this->addChild(carrot_hp, 0);
    //萝卜血量数值
    auto carrot_hp_number = Label::createWithTTF(std::to_string(carrot_HP), "/fonts/TMON Monsori.ttf", 28);
    carrot_hp_number->setTextColor(Color4B::YELLOW);
    carrot_hp_number->setName("HpNumber");
    carrot_hp_number->setPosition(1467, 760);
    this->addChild(carrot_hp_number);
    carrot_hp_number->setString(std::to_string(carrot_HP));// 更新萝卜血量数值显示
}
