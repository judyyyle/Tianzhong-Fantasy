#include "GamePlayScene.h"
#include"HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "LevelSelectionScene.h"
#include "GameManager.h"
#include<vector>
#include "ui/CocosGUI.h"
#include <memory>  // ��������ָ��
#include "cocos2d.h"
USING_NS_CC;

using namespace cocos2d::ui;
// �� MAP_SCENE ���У����һ����Ա������������ʾͼ��




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
    arr.row = 8 - static_cast<int>((vec.y / 128)) - 0.5;
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

void MAP_SCENE::initializeMap()
{
    // ������Ը��������ʼ��·�����ϰ������

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


    map3
    map[4][2] = PATH;
    map[5][2] = PATH;
    map[6][2] = PATH;
    map[7][2] = PATH;
    map[4][3] = PATH;
    map[4][4] = PATH;
    map[4][5] = PATH;
    map[3][5] = PATH;
    map[3][6] = PATH;
    map[3][7] = PATH;
    map[2][7] = PATH;
    map[2][8] = PATH;
    map[2][9] = PATH;
    map[2][10] = PATH;
    map[3][10] = PATH;
    map[4][10] = PATH;
    map[5][10] = PATH;
    map[5][9] = PATH;
    map[5][8] = PATH;
    map[5][7] = PATH;
    map[6][7] = PATH;
    map[6][6] = PATH;
    map[6][5] = PATH;
    map[7][5] = PATH;
    map[7][4] = PATH;
    map[7][3] = PATH;
    map[7][2] = PATH;

    */

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
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));


        background->setContentSize(Size(1536, 1024)); // ����Ϊ1536x1024�Ĵ�С


        // add the sprite as a child to this layer
        this->addChild(background, 0);//�����㼶Ϊ0��ȷ��������ײ�
    }



    // ��ʼ����ͼ
    initializeMap();

    // ���������¼�������
    auto listener1 = EventListenerTouchOneByOne::create();

    // ������ʼʱ�Ļص�����
    listener1->onTouchBegan = [=](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();
        array arr = vec2_to_array(touchLocation);
        handleMapAction(arr.row, arr.col);
        return true; // ����true��ʾ���ɸ��¼��������ط����ٴ���
        };

    // �����ƶ�ʱ�Ļص�����
   /*listener1->onTouchMoved = [=](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();
        array arr = vec2_to_array(touchLocation);

        // ʵʱ������������Ҫչʾ��ʾ�����߸ı�Ŀ��λ�õ�״̬
        // ����������ʾһ�������ܷ�����������ʾ
        showBuildFeedback(arr.row, arr.col);
        };*/

        // ��������ʱ�Ļص�����
    listener1->onTouchEnded = [=](Touch* touch, Event* event) {
        // ����������ȷ��������
        Vec2 touchLocation = touch->getLocation();
        array arr = vec2_to_array(touchLocation);
        handleMapAction(arr.row, arr.col);
        showBuildFeedback(arr.row, arr.col);
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);

    return true;
}

// ����ʵʱ�����ĺ���
void MAP_SCENE::showBuildFeedback(int row, int col)
{
    if (map[row][col] == PATH) {
        // ������ܷ���������ʾ��ʾ��������ʾ���ͼ�꣩
        auto feedback = Sprite::create("GamePlayScene/cantBuild.png");
        if (feedback != nullptr) {
            feedback->setPosition(array_to_vec2(row, col));
            this->addChild(feedback, 2); // ȷ������������֮��
            // ������ʱ 0.5 ���ɾ������Ķ���
            auto delay = DelayTime::create(0.5f);  // �ӳ� 0.5 ��
            auto remove = CallFunc::create([this, feedback]() {
                this->removeChild(feedback);  // ɾ������
                });

            // ִ�ж���
            feedback->runAction(Sequence::create(delay, remove, nullptr));
        }
    }
    else if (map[row][col] == SPACE) {
        // ������Է�����������Ҫ��ʾһ����ͬ�ķ������������֮ǰ�ķ���
        auto feedback = Sprite::create("GamePlayScene/Grid.png");
        if (feedback != nullptr) {
            feedback->setPosition(array_to_vec2(row, col));
            this->addChild(feedback, 2);
            addTowerPreview(row, col);
            warningSprites.push_back(feedback);
        }
    }
}

void MAP_SCENE::handleMapAction(int row, int col)
{
    // ���δѡ�����������ֹ��������
    if (!isTowerSelected)
    {
        cocos2d::log("����ѡ��һ����������");
        return;
    }

    // ���֮ǰ����ʾ
    clearWarningSprites();

    // ���ݵ����λ�ô����߼�
    if (map[row][col] == BARRIER)
    {
        cocos2d::log("�޷���������λ�ñ��ϰ���ռ�ã�");
    }
    else if (map[row][col] == EXISTED_TOWER)
    {
        cocos2d::log("��λ����������");
    }
    else if (map[row][col] == PATH)
    {
        cocos2d::log("��λ��Ϊ·�������ܷ�������");
    }
    else if (map[row][col] == SPACE)
    {
        //map[row][col] = EXISTED_TOWER;
        cocos2d::log("�����˷�������");
    }
}



void MAP_SCENE::addTowerPreview(int row, int col)
{
    std::vector<std::string> towerImages = {
        "GamePlayScene/CanClick1.png",
        "GamePlayScene/CanClick1_shit.png",
        "GamePlayScene/SunflowerButton.png"
    };

    // ÿ����ť֮���ˮƽ���
    float offsetX = 128;
    float offsetY = 128;  // ��ֱ����ļ��
    float startX = array_to_vec2(row, col).x;
    float startY = array_to_vec2(row, col).y;

    // ƫ������ȷ����ť������grid�ص�
    float offsetXForPreview = startX - 128;  // ����ť��΢����ƫ��
    float offsetYForPreview = startY + 64;  // ����ť��΢����ƫ��

    // ���㰴ťλ�ã���ֹ������Ļ��Χ
    int screenWidth = 12;  // ��ͼ������
    int screenHeight = 8;  // ��ͼ������

    for (int i = 0; i < towerImages.size(); ++i)
    {
        auto button = ui::Button::create(towerImages[i]);
        if (button != nullptr)
        {
            // ʹ���������������а�ť
            float offsetXForButton = offsetXForPreview + (i * offsetX);

            // ���������Ļ�ұ߽磬���������һ��
            if (offsetXForButton + button->getContentSize().width > screenWidth * offsetX)
            {
                offsetXForButton = offsetXForPreview;  // ������ťΪ������߿�ʼ
                offsetYForPreview -= offsetY;  // ���з���
            }

            // ȷ����ťλ�ò��ᳬ����ͼ��Χ
            if (offsetXForPreview < 0) offsetXForPreview = 0;
            if (offsetYForPreview < 0) offsetYForPreview = 0;

            // ���ð�ťλ��
            button->setPosition(Vec2(offsetXForButton, offsetYForPreview));
            this->addChild(button, 1);
            warningSprites.push_back(button);

            // ��ť����¼�
            button->addClickEventListener([this, i, row, col](Ref* sender) {
                isTowerSelected = true;  // ����״̬
                onTowerPreviewClicked(i, row, col);  // �������߼�
                });
        }
    }
}

void MAP_SCENE::onTowerPreviewClicked(int towerIndex, int row, int col)
{
    // ���ݵ����������ѡ����ӦͼƬ
    std::string towerImage;
    switch (towerIndex)
    {
    case 0:
        towerImage = "GamePlayScene/bottle1.png";
        break;
    case 1:
        towerImage = "GamePlayScene/shit1.png";
        break;
    case 2:
        towerImage = "GamePlayScene/sunflower1.png";
        break;
    default:
        return;
    }

    // ���÷�����
    auto tower = Sprite::create(towerImage);
    if (tower != nullptr)
    {
        tower->setPosition(array_to_vec2(row, col));  // ��gridλ�÷���
        this->addChild(tower, 1);
    }

    // ���µ�ͼ����
    map[row][col] = EXISTED_TOWER;

    // ����״̬
    isTowerSelected = true;  // ���÷�������ѡ��

    // ���������ʾ
    clearWarningSprites();
}



void MAP_SCENE::clearWarningSprites()
{
    for (auto sprite : warningSprites)
    {
        this->removeChild(sprite);  // �Ƴ���ʾͼ��ť
    }
    warningSprites.clear();  // ����б�
}

void MAP_SCENE::onExitGame() {
    GameManager::getInstance()->endGame(); // ���� endGame ��ֹ��Ϸ������״̬
    Director::getInstance()->end();  // ������Ϸ
}

void MAP_SCENE::onEnterGame() {
    GameManager::getInstance()->loadGameState(); // ����ʱ����״̬

    // ���ݼ��ص�״̬��ʼ���ؿ�
    this->initLevel(GameManager::getInstance()->level);
    this->initWaves(GameManager::getInstance()->allWaves);
}

void MAP_SCENE::initializeMapArray(int level) {
    switch (level) {
    case 0:
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
        map[vec2_to_array(Vec2(320, 448)).row][vec2_to_array(Vec2(320, 448)).col] = BARRIER;
        map[vec2_to_array(Vec2(1216, 448)).row][vec2_to_array(Vec2(1216, 448)).col] = BARRIER;
        map[vec2_to_array(Vec2(576, 576)).row][vec2_to_array(Vec2(576, 576)).col] = BARRIER;
        map[vec2_to_array(Vec2(960, 576)).row][vec2_to_array(Vec2(960, 576)).col] = BARRIER;
        map[vec2_to_array(Vec2(768, 320)).row][vec2_to_array(Vec2(768, 320)).col] = BARRIER;
        map[vec2_to_array(Vec2(768, 768)).row][vec2_to_array(Vec2(768, 768)).col] = BARRIER;
        map[vec2_to_array(Vec2(512, 768)).row][vec2_to_array(Vec2(512, 768)).col] = BARRIER;
        map[vec2_to_array(Vec2(1024, 768)).row][vec2_to_array(Vec2(1024, 768)).col] = BARRIER;
        break;
    case 1:
        map[3][1] = PATH;
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
        map[vec2_to_array(Vec2(836, 832)).row][vec2_to_array(Vec2(836, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(836, 192)).row][vec2_to_array(Vec2(836, 192)).col] = BARRIER;
        map[vec2_to_array(Vec2(964, 192)).row][vec2_to_array(Vec2(964, 192)).col] = BARRIER;
        map[vec2_to_array(Vec2(320, 448)).row][vec2_to_array(Vec2(320, 448)).col] = BARRIER;
        map[vec2_to_array(Vec2(448, 448)).row][vec2_to_array(Vec2(448, 448)).col] = BARRIER;
        map[vec2_to_array(Vec2(1216, 192)).row][vec2_to_array(Vec2(1216, 192)).col] = BARRIER;
        map[vec2_to_array(Vec2(1088, 832)).row][vec2_to_array(Vec2(1088, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(832, 576)).row][vec2_to_array(Vec2(832, 576)).col] = BARRIER;
        map[vec2_to_array(Vec2(512, 192)).row][vec2_to_array(Vec2( 512, 192)).col] = BARRIER;
        map[vec2_to_array(Vec2(640, 448)).row][vec2_to_array(Vec2(640, 448)).col] = BARRIER;
        map[vec2_to_array(Vec2(1024, 512)).row][vec2_to_array(Vec2(1024, 512)).col] = BARRIER;
        map[vec2_to_array(Vec2(1408, 512)).row][vec2_to_array(Vec2(1408, 512)).col] = BARRIER;
        map[vec2_to_array(Vec2(384, 768)).row][vec2_to_array(Vec2(384, 768)).col] = BARRIER;
        break;

    case 2:
        map[4][2] = PATH;
        map[5][2] = PATH;
        map[6][2] = PATH;
        map[7][2] = PATH;
        map[4][3] = PATH;
        map[4][4] = PATH;
        map[4][5] = PATH;
        map[3][5] = PATH;
        map[3][6] = PATH;
        map[3][7] = PATH;
        map[2][7] = PATH;
        map[2][8] = PATH;
        map[2][9] = PATH;
        map[2][10] = PATH;
        map[3][10] = PATH;
        map[4][10] = PATH;
        map[5][10] = PATH;
        map[5][9] = PATH;
        map[5][8] = PATH;
        map[5][7] = PATH;
        map[6][7] = PATH;
        map[6][6] = PATH;
        map[6][5] = PATH;
        map[7][5] = PATH;
        map[7][4] = PATH;
        map[7][3] = PATH;
        map[7][2] = PATH;
    
        map[vec2_to_array(Vec2(64, 832)).row][vec2_to_array(Vec2(64, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(192, 832)).row][vec2_to_array(Vec2(192, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(320, 832)).row][vec2_to_array(Vec2(320, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(448, 832)).row][vec2_to_array(Vec2(448, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(576, 832)).row][vec2_to_array(Vec2(1472, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(960, 832)).row][vec2_to_array(Vec2(960, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(1088, 832)).row][vec2_to_array(Vec2(1088, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(1472, 832)).row][vec2_to_array(Vec2(1472, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(1280, 832)).row][vec2_to_array(Vec2(1280, 832)).col] = BARRIER;
        map[vec2_to_array(Vec2(832, 448)).row][vec2_to_array(Vec2(832, 448)).col] = BARRIER;
        map[vec2_to_array(Vec2(64, 704)).row][vec2_to_array(Vec2(64, 704)).col] = BARRIER;
        map[vec2_to_array(Vec2(64, 576)).row][vec2_to_array(Vec2(64, 576)).col] = BARRIER;
        map[vec2_to_array(Vec2(1472, 704)).row][vec2_to_array(Vec2(1472, 704)).col] = BARRIER;
        map[vec2_to_array(Vec2(1472, 576)).row][vec2_to_array(Vec2(1472, 576)).col ] = BARRIER;
        map[vec2_to_array(Vec2(1472, 448)).row][vec2_to_array(Vec2(1472, 448)).col] = BARRIER;
        map[vec2_to_array(Vec2(1472, 320)).row][vec2_to_array(Vec2(1472, 320)).col] = BARRIER;
        map[vec2_to_array(Vec2(1472, 192)).row][vec2_to_array(Vec2(1472, 192)).col] = BARRIER;
        map[vec2_to_array(Vec2(1344, 192)).row][vec2_to_array(Vec2(1344, 192)).col] = BARRIER;
        map[vec2_to_array(Vec2(960, 64)).row][vec2_to_array(Vec2(960, 64)).col] = BARRIER;
        map[vec2_to_array(Vec2(832, 64)).row][vec2_to_array(Vec2(832, 64)).col] = BARRIER;
        map[vec2_to_array(Vec2(512, 704)).row][vec2_to_array(Vec2(512, 704)).col] = BARRIER;
        map[vec2_to_array(Vec2(1152, 64)).row][vec2_to_array(Vec2(1152, 64)).col] = BARRIER;
        map[vec2_to_array(Vec2(832, 64)).row][vec2_to_array(Vec2(832, 64)).col] = BARRIER;
        map[vec2_to_array(Vec2(768, 320)).row][vec2_to_array(Vec2(768, 320)).col] = BARRIER;
        map[vec2_to_array(Vec2(512, 576)).row][vec2_to_array(Vec2(512, 576)).col] = BARRIER;
        map[vec2_to_array(Vec2(128, 384)).row][vec2_to_array(Vec2(128, 384)).col] = BARRIER;
        map[vec2_to_array(Vec2(512, 256)).row][vec2_to_array(Vec2(512, 256)).col] = BARRIER;
        map[vec2_to_array(Vec2(1152, 512)).row][vec2_to_array(Vec2(1152, 512)).col] = BARRIER;
        map[vec2_to_array(Vec2(768, 768)).row][vec2_to_array(Vec2(768, 768)).col] = BARRIER;
        map[vec2_to_array(Vec2(256, 640)).row][vec2_to_array(Vec2(256, 640)).col] = BARRIER;
        break;

    }
}


void  MAP_SCENE::initLevel(int level)// ��ʼ���ؿ��ķ���
{
    std::string backgroundImage;
    switch (level) {
    case 0:
        backgroundImage = "map1.png";
        setBackground(backgroundImage);
        break;
    case 1:
        backgroundImage = "map2.png";
        setBackground(backgroundImage);     
        break;
    case 2:
        backgroundImage = "map3.png";
        setBackground(backgroundImage);   
        break;
    default:
        backgroundImage = "default_background.png";
        setBackground(backgroundImage);
        break;
    }
    // initTowers(level);��ʼ��������λ��
    initializeMapArray(level);//��ͼ����(·��) 
    //initResources(); ��ʼ����Ϸ��Դ

}
void MAP_SCENE::setBackground(const std::string& backgroundImage) {

    // ��ȡ��Ļ��С
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    background = cocos2d::Sprite::create(backgroundImage);
    if (background == nullptr)
    {
        // problemLoading("'menu.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        background->setContentSize(Size(1536, 1024)); // ����Ϊ1536x1024�Ĵ�С
        // add the sprite as a child to this layer
        this->addChild(background, 0);//�����㼶Ϊ0��ȷ��������ײ�
    }
}
void initWaves(int allWaves)  // ��ʼ�����εķ���
{

}
