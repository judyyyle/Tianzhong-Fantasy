#include "GamePlayScene.h"
#include"HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "LevelSelectionScene.h"
#include "ui/CocosGUI.h"
#include <memory>  // ��������ָ��
#include "cocos2d.h"
USING_NS_CC;

using namespace cocos2d::ui;


//���������������ת��
struct array {
    int row;
    int col;
};

Vec2 array_to_vec2(int row, int col) { //����Vec2���ͣ�����������
    Vec2 vec;
    vec.x = 64 + 128 * col;
    vec.y = 1024 - 64 - 128 * row;
    return vec;
}

array vec2_to_array(Vec2 vec) { //����array���ͣ�������
    array arr;
    arr.row = 12 - static_cast<int>((vec.y / 128));
    arr.col = static_cast<int>((vec.x / 128));
    return arr;
}




MAP_SCENE::MAP_SCENE()
    : background(nullptr)
{
    // ��ʼ����ͼ״̬����
    memset(map, SPACE, sizeof(map)); // Ĭ������λ��Ϊ�հ�
}

MAP_SCENE::~MAP_SCENE()
{
    // ��Դ��������еĻ���
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

    // ������ʼ����������ӱ���
 
    // ��ȡ��Ļ��С
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

  

    background = cocos2d::Sprite::create("GamePlayScene/map1.png");//��д����һ�ŵ�ͼ �����ѡ��ڼ��ŵ�ͼ���޸�
    if (background == nullptr)
    {
        // problemLoading("'menu.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        background ->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));


        background->setContentSize(Size(1536, 1024)); // ����Ϊ1536x1024�Ĵ�С


        // add the sprite as a child to this layer
        this->addChild(background, 0);//�����㼶Ϊ0��ȷ��������ײ�
    }

    // ��ʼ����ͼ
    initializeMap();


  

    // ���������¼�������
    auto listener1 = EventListenerTouchOneByOne::create();

    listener1->onTouchBegan = [=](Touch* touch, Event* event) {
        // ��ȡ����λ��
        Vec2 touchLocation = touch->getLocation();
        // ������λ�õ���Ϊ
        handleMapAction(touchLocation.x, touchLocation.y);
        return true;
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
 

    return true;
}



void MAP_SCENE::initializeMap()
{
    // ������Ը��������ʼ��·�����ϰ������
    // ��������һЩ�ϰ�������Ϊ1
    //map1
    map[2][1] = PATH;   // (2, 3) ��·��
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





void MAP_SCENE::handleMapAction(int row, int col) //��ʱ�����ֱ�ʾ ��Ҫ���
{
    // �жϵ���ĵط���ʲô״̬
    if (map[row][col] == BARRIER)
    {
        // ������ϰ�����ܷ���
        cocos2d::log("�޷���������λ�ñ��ϰ���ռ�ã�");
    }
    else if (map[row][col] == EXISTED_TOWER)
    {
        // ����Ѿ������������ٷ���
        cocos2d::log("��λ����������");
    }
    else if (map[row][col] == PATH)
    {
        // �����·�������ܷ���
        cocos2d::log("��λ��Ϊ·�������ܷ�������");
    }
    else if (map[row][col] == SPACE)
    {
        // ����ǿհ�λ�ã����Է���
        map[row][col] = EXISTED_TOWER;
        cocos2d::log("�����˷�������");
        // ���������Ӵ��������߼������紴��һ�����ľ���
        // cocos2d::Sprite* tower = cocos2d::Sprite::create("tower.png");
        // tower->setPosition(array_to_vec2(row, col));
        // this->addChild(tower);
    }
}


void MAP_SCENE::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    // ��ȡ����λ��
    cocos2d::Vec2 touchLocation = touch->getLocation();

    // ����������ת��Ϊ��������
    array arr = vec2_to_array(touchLocation);

    // ������λ�õ���Ϊ
    handleMapAction(arr.row, arr.col);

    //return true;
}









