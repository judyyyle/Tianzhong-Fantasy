#ifndef BARRIER_MANAGER_H
#define BARRIER_MANAGER_H

#include<string>
#include<vector>
#include "Bullet.h"
#include "GamePlayScene.h"
#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d;

extern int mapGrid[8][12];
extern int coinNumber;
extern std::vector<Bullet*> bulletsTowardBarrier;
extern std::vector<Sunflowerfire*>sunflowers;


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

struct barrierSun {
    Sunflowerfire* sun_fire;
    bool attacked;
    barrierSun(Sunflowerfire* f, bool a) :sun_fire(f), attacked(a) {}
    barrierSun() :sun_fire(NULL), attacked(true) {}

    // 定义 operator==
    bool operator==(const barrierSun& other) const {
        // 判断 sun_fire 指针是否指向同一对象，以及 attacked 是否相等
        return (sun_fire == other.sun_fire) && (attacked == other.attacked);
    }
};


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

    Bullet* closestBullet = nullptr; // 最近的子弹
    std::vector<barrierSun>fires;

    // 初始化障碍物
    bool initWithParams(const std::string& spritePath, const Vec2& position, int initialHp,int type) {
        // 创建障碍物
        barrierSprite = Sprite::create(spritePath);
        if (!barrierSprite) return false;
        barrierSprite->setPosition(position);
        this->addChild(barrierSprite);

        // 初始化血量
        maxHp = initialHp * 100;
        hp = initialHp * 100;
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

        this->scheduleUpdate();  // 定期调用 update 函数

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
        if (hpHolder) {
            hpHolder->setVisible(false);
        }
        if (hpSlider) {
            hpSlider->setVisible(false);
        }
        if (arrow) {
            arrow->setVisible(false);
        }
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
        if (barrierSprite) {
            // 更新 mapGrid 状态
            if (Type == 0|| Type == 1) {
                mapGrid[vec2_to_array_BA(Vec2(gridX, gridY)).row][vec2_to_array_BA(Vec2(gridX, gridY)).col] = SPACE;
            }
            else if (Type == 2|| Type == 3) {
                mapGrid[vec2_to_array_BA(Vec2(gridX - 64, gridY)).row][vec2_to_array_BA(Vec2(gridX - 64, gridY)).col] = SPACE;
                mapGrid[vec2_to_array_BA(Vec2(gridX + 64, gridY)).row][vec2_to_array_BA(Vec2(gridX + 64, gridY)).col] = SPACE;
            }
            else if (Type == 4||Type == 5) {
                mapGrid[vec2_to_array_BA(Vec2(gridX - 64, gridY - 64)).row][vec2_to_array_BA(Vec2(gridX - 64, gridY - 64)).col] = SPACE;
                mapGrid[vec2_to_array_BA(Vec2(gridX + 64, gridY - 64)).row][vec2_to_array_BA(Vec2(gridX + 64, gridY - 64)).col] = SPACE;
                mapGrid[vec2_to_array_BA(Vec2(gridX - 64, gridY + 64)).row][vec2_to_array_BA(Vec2(gridX - 64, gridY + 64)).col] = SPACE;
                mapGrid[vec2_to_array_BA(Vec2(gridX + 64, gridY + 64)).row][vec2_to_array_BA(Vec2(gridX + 64, gridY + 64)).col] = SPACE;
            }

            if (Type == 0 || Type == 1) {
                coinNumber += 50;
            }
            else if (Type == 2 || Type == 3) {
                coinNumber += 100;
            }
            else if (Type == 4 || Type == 5) {
                coinNumber += 200;
            }
            barrierSprite->removeFromParent();
            barrierSprite = nullptr;
        }
        if (hpHolder) {
            hpHolder->removeFromParent();
            hpHolder = nullptr;
        }
        if (hpSlider) {
            hpSlider->removeFromParent();
            hpSlider = nullptr;
        }
        if (arrow) {
            arrow->removeFromParent();
            arrow = nullptr;
        }
    }

    // 查找最近的子弹
    void findNearestBullet() {
        closestBullet = nullptr; // 初始化最近的子弹为空

        // 检查 barrierSprite 是否有效
        if (!barrierSprite) {
            CCLOG("Error: barrierSprite is nullptr");
            return;
        }

        for (auto bullet : bulletsTowardBarrier) {
            // 检查 bullet 是否有效
            if (!bullet) {
                CCLOG("Warning: bullet is nullptr");
                continue;
            }

            Vec2 bulletPos = bullet->getPosition();

            // 检查 barrierSprite 的边界是否包含子弹位置
            if (barrierSprite->getBoundingBox().containsPoint(bulletPos)) {
                closestBullet = bullet;
                break; // 找到第一个满足条件的子弹后立即退出
            }
        }
    }

    // 每帧调用的更新方法
    void update(float dt) {
        bool sun_attacked = false;
        for (auto sunflower : sunflowers) {
            cocos2d::Rect fire_rect = sunflower->getBoundingBox();
            barrierSun* target = nullptr;

            for (auto& fire : fires) {
                if (fire.sun_fire == sunflower)
                    target = &fire;
            }

            if (barrierSprite&&sunflower->getBoundingBox().containsPoint(barrierSprite->getPosition())) {
                if (target == nullptr) {
                    barrierSun new_fire(sunflower, true);
                    fires.push_back(new_fire);
                    takeDamage(sunflower->GetDamage());
                    showHpBar();
                    sun_attacked = true;
                    break;
                }
                else if (target->attacked == false) {
                    takeDamage(sunflower->GetDamage());
                    showHpBar();
                    target->attacked = true;
                    sun_attacked = true;
                    break;
                }
            }
            else {
                if (target != NULL)
                    target->attacked = false;
            }
        }
        // 如果血量为 0，移除障碍物
        if (isDead()) {
            removeFromScene();
        }
        

        // 更新障碍物逻辑
        findNearestBullet();  // 查找最近的子弹
        if (closestBullet != nullptr) {
            // 如果找到最近的子弹，处理伤害
            takeDamage(closestBullet->GetDamage());
            showHpBar();  // 显示血条
            // 如果血量为 0，移除障碍物
            if (isDead()) {
                removeFromScene();
            }
            if (closestBullet) {
                closestBullet->removeFromParent();  // 从父节点移除子弹
                // 从 bullets 容器中移除自身
                auto it = std::find(bulletsTowardBarrier.begin(), bulletsTowardBarrier.end(), closestBullet);
                if (it != bulletsTowardBarrier.end()) {
                    bulletsTowardBarrier.erase(it);
                }
                closestBullet = NULL;
            }
        }
        
    }
};


class BarrierManager : public Node {
public:
    CREATE_FUNC(BarrierManager);

    Vector<BarrierInfo*> barriers;      // 存储所有障碍物
    BarrierInfo* selectedBarrier;  // 当前选中的障碍物
    

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

    // 取消选中障碍物
    void deselectBarrier() {
        if (selectedBarrier) {
            selectedBarrier->hideHpBar();
            selectedBarrier = nullptr;
        }
    }

    // 更新障碍物状态
    void update(float dt) {
        for (auto barrier : barriers) {
            barrier->update(dt);  // 调用每个障碍物的 update 方法

            // 如果障碍物死亡，移除它
            if (barrier->isDead()) {
                removeBarrier(barrier);
            }
        }
    }

    // 移除障碍物
    void removeBarrier(BarrierInfo* barrierInfo) {
        if (barrierInfo) {
            // 确保清理选中障碍物的指针
            if (selectedBarrier == barrierInfo) {
                selectedBarrier = nullptr;
            }

            // 从 barriers 容器中移除
            barriers.eraseObject(barrierInfo);

            // 调用障碍物的移除逻辑
            barrierInfo->removeFromScene();

            // 更新 mapGrid 状态
            if (barrierInfo->Type == 1) {
                mapGrid[vec2_to_array_BA(Vec2(barrierInfo->gridX, barrierInfo->gridY)).row][vec2_to_array_BA(Vec2(barrierInfo->gridX, barrierInfo->gridY)).col] = SPACE;
            }
            else if (barrierInfo->Type == 2) {
                mapGrid[vec2_to_array_BA(Vec2(barrierInfo->gridX - 64, barrierInfo->gridY)).row][vec2_to_array_BA(Vec2(barrierInfo->gridX - 64, barrierInfo->gridY)).col] = SPACE;
                mapGrid[vec2_to_array_BA(Vec2(barrierInfo->gridX + 64, barrierInfo->gridY)).row][vec2_to_array_BA(Vec2(barrierInfo->gridX + 64, barrierInfo->gridY)).col] = SPACE;
            }
            else if (barrierInfo->Type == 4) {
                mapGrid[vec2_to_array_BA(Vec2(barrierInfo->gridX - 64, barrierInfo->gridY - 64)).row][vec2_to_array_BA(Vec2(barrierInfo->gridX - 64, barrierInfo->gridY - 64)).col] = SPACE;
                mapGrid[vec2_to_array_BA(Vec2(barrierInfo->gridX + 64, barrierInfo->gridY - 64)).row][vec2_to_array_BA(Vec2(barrierInfo->gridX + 64, barrierInfo->gridY - 64)).col] = SPACE;
                mapGrid[vec2_to_array_BA(Vec2(barrierInfo->gridX - 64, barrierInfo->gridY + 64)).row][vec2_to_array_BA(Vec2(barrierInfo->gridX - 64, barrierInfo->gridY + 64)).col] = SPACE;
                mapGrid[vec2_to_array_BA(Vec2(barrierInfo->gridX + 64, barrierInfo->gridY + 64)).row][vec2_to_array_BA(Vec2(barrierInfo->gridX + 64, barrierInfo->gridY + 64)).col] = SPACE;
            }
        }
    }
};

#endif // BARRIER_MANAGER_H
