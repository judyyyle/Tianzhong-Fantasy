#include "GamePlayScene.h"
#include"HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "LevelSelectionScene.h"
#include<vector>
#include "ui/CocosGUI.h"
#include <memory>  // ��������ָ��
#include "cocos2d.h"
#include "Tower.h"
USING_NS_CC;

using namespace cocos2d::ui;
// �� MAP_SCENE ���У����һ����Ա������������ʾͼ��



#define BARRIER_1_1 0
#define BARRIER_1_2 1
#define BARRIER_2_1 2
#define BARRIER_2_2 3
#define BARRIER_4_1 4
#define BARRIER_4_2 5



/**********************  ȫ�ֱ���  ***********************/
//�ؿ�ѡ��
int level = 1;//�ؿ�ѡ��


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
   
    if (level == 1) {
        auto barrierManager = BarrierManager::create();
        array barrierPosition;
        // ��Ӳ�ͬλ�ú����͵��ϰ���
        barrierManager->BarrierAppear(BARRIER_1_1, 320, 448, 1);
        barrierPosition = vec2_to_array({ 320, 448 });
        map[barrierPosition.row][barrierPosition.col] = BARRIER;
        barrierManager->BarrierAppear(BARRIER_1_1, 1216, 448, 1);
        barrierPosition = vec2_to_array({ 1216, 448 });
        map[barrierPosition.row][barrierPosition.col] = BARRIER;
        barrierManager->BarrierAppear(BARRIER_1_2, 576, 576, 1);
        barrierPosition = vec2_to_array({ 576, 576 });
        map[barrierPosition.row][barrierPosition.col] = BARRIER;
        barrierManager->BarrierAppear(BARRIER_1_2, 960, 576, 1);
        barrierPosition = vec2_to_array({ 960, 576 });
        map[barrierPosition.row][barrierPosition.col] = BARRIER;
        barrierManager->BarrierAppear(BARRIER_2_1, 768, 320, 2);
        barrierPosition = vec2_to_array({ 768, 320 });
        map[barrierPosition.row][barrierPosition.col] = BARRIER;
        barrierManager->BarrierAppear(BARRIER_4_1, 768, 768, 4);
        barrierPosition = vec2_to_array({ 768, 768 });
        map[barrierPosition.row][barrierPosition.col] = BARRIER;
        barrierManager->BarrierAppear(BARRIER_4_2, 512, 768, 4);
        barrierPosition = vec2_to_array({ 512, 768 });
        map[barrierPosition.row][barrierPosition.col] = BARRIER;
        barrierManager->BarrierAppear(BARRIER_4_2, 1024, 768, 4);
        barrierPosition = vec2_to_array({ 1024, 768 });
        map[barrierPosition.row][barrierPosition.col] = BARRIER;

        this->addChild(barrierManager);
        barrierManager->createMouseEventListener();
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
    else if (map[row][col] == EXISTED_TOWER) {
        handleTowerClick(row, col);
        updateordeleteTowerPreview(row, col);
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
    // ���ݵ����������ѡ����ӦͼƬ��������Ӧ������
    Tower* tower = nullptr;

    switch (towerIndex)
    {
        case 0:
            // ���� BottleTower ʵ��
            tower = BottleTower::create("GamePlayScene/bottle_level_1.png");
            break;
        case 1:
            // ���� ShitTower ʵ������������������ࣩ
            tower = ShitTower::create("GamePlayScene/shit_level_1.png");
            break;
        case 2:
            // ���� SunflowerTower ʵ������������������ࣩ
            tower = SunflowerTower::create("GamePlayScene/sun_level_1.png");
            break;
        default:
            return; // ���û��ƥ��������ͣ�ֱ�ӷ���
    }

    // ������Ƿ񴴽��ɹ�
    if (tower != nullptr)
    {
        tower->setPosition(array_to_vec2(row, col));  // ��gridλ�÷�����
        int tag = row * 100 + col;  // Ϊ������Ψһ��tag
        tower->setTag(tag);  // ��������tag
        this->addChild(tower, 1);  // ������ӵ�����
    }
    towerArray[row][col] = tower;

    // ���µ�ͼ����
    map[row][col] = EXISTED_TOWER;  // ���ø�λ���ѷ�����

    // ����״̬
    isTowerSelected = true;  // ���÷�������ѡ��

    // ���������ʾ
    clearWarningSprites();
}
void MAP_SCENE::handleTowerClick(int row, int col)
{
    // ��ȡ���λ�õ�������
    Tower* tower = getTowerAt(row, col);
    if (tower == nullptr)
    {
        cocos2d::log("û�����ڴ�λ��");
        return;
    }

    // ʹ�� dynamic_cast ���ж����ľ�������
    if (auto bottleTower = dynamic_cast<BottleTower*>(tower))
    {
        // ����� BottleTower ���͵���
        cocos2d::log("����� BottleTower!");
        // ���� BottleTower ������߼�
    }
    else if (auto shitTower = dynamic_cast<ShitTower*>(tower))
    {
        // ����� ShitTower ���͵���
        cocos2d::log("����� ShitTower!");
        // ���� ShitTower ������߼�
    }
    else if (auto sunflowerTower = dynamic_cast<SunflowerTower*>(tower))
    {
        // ����� SunflowerTower ���͵���
        cocos2d::log("����� SunflowerTower!");
        // ���� SunflowerTower ������߼�
    }
    else
    {
        cocos2d::log("����������֪����!");
    }
}

ui::Button* currentUpgradeButton = nullptr;  // ȫ�ֱ��������ڼ�¼��ǰ��������ť
ui::Button* currentDeleteButton = nullptr;  // ȫ�ֱ��������ڼ�¼��ǰ��ɾ����ť

void MAP_SCENE::updateordeleteTowerPreview(int row, int col)
{
    // �����������ť�����Ƴ�����
    if (currentUpgradeButton != nullptr)
    {
        currentUpgradeButton->removeFromParent();
        currentUpgradeButton = nullptr;
    }
    if (currentDeleteButton != nullptr)
    {
        currentDeleteButton->removeFromParent();
        currentDeleteButton = nullptr;
    }

    // ��ȡ��ǰ�����ϵ���
    Tower* tower = getTowerAt(row, col);
    if (tower == nullptr)
    {
        cocos2d::log("��λ��û�������޷���ʾ��ť��");
        return;
    }

    // ��ȡ������������
    int upgradeCost = tower->getUpgradeCost();
    int sellPrice = tower->getsellPrice();

    // ����������ť
    auto upgradeButton = ui::Button::create("GamePlayScene/CanUpLevel.png");
    upgradeButton->setPosition(array_to_vec2(row, col) + Vec2(0, 80));  // ��ťλ��
    upgradeButton->setTitleText(std::to_string(upgradeCost));           // ������ʾ��������
    upgradeButton->setTitleColor(Color3B::BLACK);
    upgradeButton->setTitleFontSize(20);
    auto up_label = upgradeButton->getTitleLabel();
    up_label->setPosition(Vec2(upgradeButton->getContentSize().width / 2 + 8,
        upgradeButton->getContentSize().height / 2) + Vec2(0, -25));
    if (tower->getLevel() == 3)
    {
        upgradeButton->loadTextures("GamePlayScene/cant_update.PNG", "GamePlayScene/cant_update.PNG","");  // ��ʾ���Ѵ���󼶱���ʾ
        upgradeButton->setTitleText("");  // ��հ�ť������

    }
    this->addChild(upgradeButton, 2);

    // ���浱ǰ������ť
    currentUpgradeButton = upgradeButton;

    // ��ť����¼���ִ���������Ƴ���ť
    upgradeButton->addClickEventListener([this, tower, row, col](Ref* sender) {
        upgradeTower(row, col);

        // �������Ƴ���ť
        auto button = dynamic_cast<ui::Button*>(sender);
        // �������Ƴ�ɾ����ť
        if (currentDeleteButton != nullptr)
        {
            currentDeleteButton->removeFromParent();
            currentDeleteButton = nullptr;  // ���ȫ�ֱ���
            cocos2d::log("������ɾ����ť���Ƴ���");
        }
        if (button != nullptr)
        {
            button->removeFromParent();
            currentUpgradeButton = nullptr;  // ���ȫ�ֱ���
        }
        });

    // ����ɾ����ť
    auto deleteButton = ui::Button::create("GamePlayScene/SellTower.png");  // �滻Ϊɾ����ť��ͼƬ
    deleteButton->setPosition(array_to_vec2(row, col) + Vec2(0, -80));  // ��ťλ�ã����·�
    deleteButton->setTitleText(std::to_string(sellPrice));           // ������ʾ��������
    deleteButton->setTitleColor(Color3B::BLACK);
    deleteButton->setTitleFontSize(20);
    auto sell_label = deleteButton->getTitleLabel();
    sell_label->setPosition(Vec2(deleteButton->getContentSize().width / 2 + 8,
        deleteButton->getContentSize().height / 2) + Vec2(0, -25));
    this->addChild(deleteButton, 2);

    // ���浱ǰɾ����ť
    currentDeleteButton = deleteButton;

    // ɾ����ť����¼���ɾ����
    deleteButton->addClickEventListener([this, tower, row, col](Ref* sender) {
        deleteTower(row, col);

        // �Ƴ�ɾ����ť
        auto button = dynamic_cast<ui::Button*>(sender);
        if (button != nullptr)
        {
            button->removeFromParent();
            currentDeleteButton = nullptr;  // ���ȫ�ֱ���
        }

        // ͬʱ�Ƴ�������ť
        if (currentUpgradeButton != nullptr)
        {
            currentUpgradeButton->removeFromParent();
            currentUpgradeButton = nullptr;
        }

        cocos2d::log("����λ�� (%d, %d) �ѱ�ɾ����", row, col);
        });

    // �ڳ�������ӵ���¼�������
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [this](Touch* touch, Event* event) {
        // ת�������㵽�ڵ�ռ�
        Vec2 location = this->convertToNodeSpace(touch->getLocation());

        // ���������ڰ�ť��Χ�ڣ��Ƴ����а�ť
        if (currentUpgradeButton != nullptr &&
            !currentUpgradeButton->getBoundingBox().containsPoint(location))
        {
            currentUpgradeButton->removeFromParent();
            currentUpgradeButton = nullptr;
            cocos2d::log("����ڰ�ť�⣬�Ƴ�������ť��");
        }

        if (currentDeleteButton != nullptr &&
            !currentDeleteButton->getBoundingBox().containsPoint(location))
        {
            currentDeleteButton->removeFromParent();
            currentDeleteButton = nullptr;
            cocos2d::log("����ڰ�ť�⣬�Ƴ�ɾ����ť��");
        }

        return false;  // ���� false������ֹ�¼���������
        };

    // ���¼�������ע�ᵽ����
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}


void MAP_SCENE::deleteTower(int row, int col)
{
    // ��ȡָ��λ�õ�������
    Tower* tower = getTowerAt(row, col);

    if (tower == nullptr)
    {
        cocos2d::log("��λ��û�������޷�ɾ����");
        return;
    }

    // �Ӹ��ڵ����Ƴ����Ŀ��ӻ��ڵ㣨�������ʾ�� Sprite ���������ݣ�
    if (tower != nullptr)
    {
        tower->removeFromParent();  // ɾ�����ľ��飨Sprite��
    }

    // �ӹ�����������ݽṹ���Ƴ�����������
    towerArray[row][col] = nullptr;  // �����������
    map[row][col] = SPACE;


    cocos2d::log("����λ�� (%d, %d) �ѳɹ�ɾ����", row, col);
}
void MAP_SCENE::upgradeTower(int row, int col)
{
    // ��ȡ��ǰ��λ�õ���ʵ��
    Tower* tower = getTowerAt(row, col);  // ��ȡ��λ�õ���

    if (tower != nullptr)
    {
        // ִ�������߼����������ÿ�����඼��`upgrade`����
        tower->upgrade();  // ��������������������

        // ����Ը�����Ҫ�����������ԣ��������ӹ������������ٶȣ�����ֵ��
        cocos2d::log("����λ�� (%d, %d) �����ɹ���", row, col);
    }
    else
    {
        cocos2d::log("��λ��û�������޷�������");
    }
}

Tower* MAP_SCENE::getTowerAt(int row, int col) {
    // ����Ƿ�Խ��
    if (row < 0 || row >= 8 || col < 0 || col >= 12) {
        cocos2d::log("��Ч��λ��: (%d, %d)", row, col);
        return nullptr;
    }

    // ֱ�ӷ��������е�������
    return towerArray[row][col];
}

void MAP_SCENE::clearWarningSprites()
{
    for (auto sprite : warningSprites)
    {
        this->removeChild(sprite);  // �Ƴ���ʾͼ��ť
    }
    warningSprites.clear();  // ����б�
}

