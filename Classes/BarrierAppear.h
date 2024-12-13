#ifndef BARRIER_MANAGER_H
#define BARRIER_MANAGER_H

#include<string>
#include<vector>

#include "cocos2d.h"

using namespace cocos2d;

/*
* #define BARRIER_1_1 0
#define BARRIER_1_2 1
#define BARRIER_2_1 2
#define BARRIER_2_2 3
#define BARRIER_4_1 4
#define BARRIER_4_2 5 
*/
extern int mapGrid[8][12];

#define PATH -1
#define SPACE 0
#define BARRIER 1
#define EXISTED_TOWER 2



//世界坐标与数组的转换
static struct array_BA {
    int row;
    int col;
};

static Vec2 array_to_vec2_BA(int row, int col) { //返回Vec2类型，即世界坐标
    Vec2 vec;
    vec.x = 64 + 128 * col;
    vec.y = 1024 - 64 - 128 * row;
    return vec;
}

static array_BA vec2_to_array_BA(Vec2 vec) { //返回array类型，即数组
    array_BA arr;
    arr.row = 8 - static_cast<int>((vec.y / 128)) - 0.5;
    arr.col = static_cast<int>((vec.x / 128));
    return arr;
}



class BarrierInfo : public Node {
public:
    CREATE_FUNC(BarrierInfo);
   
    Sprite* barrierSprite = nullptr;  // 障碍物
    Sprite* hpHolder = nullptr;      // 血条背景
    Sprite* hpSlider = nullptr;      // 血条滑块
    Sprite* arrow = nullptr;         // 血条滑块
    int hp = 0;                      // 当前血量
    int maxHp = 0;                   // 最大血量
    int Type = 1;
    int gridX = -1;  
    int gridY = -1; 
    // 初始化障碍物
    bool initWithParams(const std::string& spritePath, const Vec2& position, int initialHp,int type) {
        // 创建障碍物
        barrierSprite = Sprite::create(spritePath);
        if (!barrierSprite) return false;
        barrierSprite->setPosition(position);
        this->addChild(barrierSprite);

        // 初始化血量
        maxHp = initialHp * 8;
        hp = initialHp * 8;
        Type = type;
        gridX=position.x;
        gridY = position.y ;
     
        // 创建血条背景
        hpHolder = Sprite::create("/Barrier/HpHolder.png");
        hpHolder->setPosition(Vec2(position.x, position.y + barrierSprite->getContentSize().height / 2.25));
        hpHolder->setVisible(false);
        this->addChild(hpHolder);

        // 创建血条滑块
        hpSlider = Sprite::create("/Barrier/HpSlider.png");
        hpSlider->setAnchorPoint(Vec2(0, 0.5));
        hpSlider->setPosition(Vec2(hpHolder->getPositionX() - hpHolder->getContentSize().width / 2,
            hpHolder->getPositionY()));
        hpSlider->setVisible(false);
        this->addChild(hpSlider);

        // 初始化显示箭头
        arrow = Sprite::create("/Barrier/arrow.png");
        arrow->setPosition(Vec2(barrierSprite->getPositionX(), hpHolder->getPositionY() + 19));
        arrow->setVisible(false);
        this->addChild(arrow);

        return true;
    }

    // 显示血条
    void showHpBar() {
        hpHolder->setVisible(true);
        hpSlider->setVisible(true);
        arrow->setVisible(true);
    }

    // 隐藏血条
    void hideHpBar() {
        hpHolder->setVisible(false);
        hpSlider->setVisible(false);
        arrow->setVisible(false);
    }

    // 更新血量
    void takeDamage(int damage) {
        hp = std::max(hp - damage, 0);
        float hpRatio = static_cast<float>(hp) / maxHp;
        hpSlider->setScaleX(hpRatio);
    }

    // 检查是否死亡
    bool isDead() const {
        return hp <= 0;
    }

    // 移除障碍物及血条
    void removeFromScene() {
        if (barrierSprite) barrierSprite->removeFromParent();
        if (hpHolder) hpHolder->removeFromParent();
        if (hpSlider) hpSlider->removeFromParent();
        if (arrow) arrow->removeFromParent();
    }
};


class BarrierManager : public Node {
public:
    CREATE_FUNC(BarrierManager);

    Vector<BarrierInfo*> barriers;      // 存储所有障碍物
    BarrierInfo* selectedBarrier = nullptr;  // 当前选中的障碍物

    // 初始化障碍物
    void BarrierAppear(int type, float positionX, float positionY, int initialHp) {
        // 障碍物的图片路径
        std::string picture[] = {
            "Barrier/one1.png", "Barrier/one2.png",
            "Barrier/two1.png", "Barrier/two2.png",
            "Barrier/four1.png", "Barrier/four2.png"
        };

        // 创建 BarrierInfo 实例
        auto barrierInfo = BarrierInfo::create();
        if (barrierInfo && barrierInfo->initWithParams(picture[type], Vec2(positionX, positionY), initialHp,type)) {
            this->addChild(barrierInfo);
            barriers.pushBack(barrierInfo);
        }
    }

    // 鼠标点击事件监听器
    void createMouseEventListener() {
        auto listener = EventListenerMouse::create();

        listener->onMouseDown = [this](Event* event) {
            auto mouseEvent = dynamic_cast<EventMouse*>(event);
            Vec2 clickPosition = mouseEvent->getLocationInView();
            clickPosition = this->convertToNodeSpace(clickPosition);

            // 判断是否点击障碍物
            BarrierInfo* clickedBarrier = getClickedBarrier(clickPosition);
            if (clickedBarrier) {
                // 如果点击了障碍物，显示其血条并使其成为选中障碍物
                deselectBarrier();  // 取消之前的选中状态
                selectedBarrier = clickedBarrier;
                selectedBarrier->showHpBar();


                selectedBarrier->takeDamage(1);  // 示例：点击后减少 1 点血量

                // 如果障碍物血量为 0，移除障碍物
                if (selectedBarrier->isDead()) {
                    removeBarrier(selectedBarrier);
                    selectedBarrier = nullptr;
                }
            }
            else {
                // 点击空白区域隐藏血条
                deselectBarrier();
            }
            };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }

    // 根据点击位置获取障碍物
    BarrierInfo* getClickedBarrier(const Vec2& position) {
        for (auto& info : barriers) {
            if (info->barrierSprite && info->barrierSprite->getBoundingBox().containsPoint(position)) {
                return info;
            }
        }
        return nullptr;
    }

    // 移除障碍物
    void removeBarrier(BarrierInfo* barrierInfo) {
        barriers.eraseObject(barrierInfo);
        barrierInfo->removeFromScene();
        if (barrierInfo->Type == 0 || barrierInfo->Type == 1) {
           mapGrid[vec2_to_array_BA(Vec2(barrierInfo->gridX, barrierInfo->gridY)).row][vec2_to_array_BA(Vec2(barrierInfo->gridX, barrierInfo->gridY)).col] = SPACE;
        }
        else if (barrierInfo->Type == 2 || barrierInfo->Type == 3) {
          
            mapGrid[vec2_to_array_BA(Vec2(barrierInfo->gridX-64, barrierInfo->gridY)).row][vec2_to_array_BA(Vec2(barrierInfo->gridX-64, barrierInfo->gridY)).col] = SPACE;
            mapGrid[vec2_to_array_BA(Vec2(barrierInfo->gridX+64, barrierInfo->gridY)).row][vec2_to_array_BA(Vec2(barrierInfo->gridX+64, barrierInfo->gridY)).col] = SPACE;
        
        }
        else if (barrierInfo->Type == 4 || barrierInfo->Type == 5) {
            mapGrid[vec2_to_array_BA(Vec2(barrierInfo->gridX-64, barrierInfo->gridY-64)).row][vec2_to_array_BA(Vec2(barrierInfo->gridX-64, barrierInfo->gridY-64)).col] = SPACE;
            mapGrid[vec2_to_array_BA(Vec2(barrierInfo->gridX + 64, barrierInfo->gridY - 64)).row][vec2_to_array_BA(Vec2(barrierInfo->gridX + 64, barrierInfo->gridY - 64)).col] = SPACE;
            mapGrid[vec2_to_array_BA(Vec2(barrierInfo->gridX - 64, barrierInfo->gridY + 64)).row][vec2_to_array_BA(Vec2(barrierInfo->gridX - 64, barrierInfo->gridY + 64)).col] = SPACE;
            mapGrid[vec2_to_array_BA(Vec2(barrierInfo->gridX + 64, barrierInfo->gridY + 64)).row][vec2_to_array_BA(Vec2(barrierInfo->gridX + 64, barrierInfo->gridY + 64)).col] = SPACE;
        }

        

    }

    // 取消选中障碍物
    void deselectBarrier() {
        if (selectedBarrier) {
            selectedBarrier->hideHpBar();
            selectedBarrier = nullptr;
        }
    }
};




#endif // BARRIER_MANAGER_H
