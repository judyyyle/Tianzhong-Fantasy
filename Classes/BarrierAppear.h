// BarrierManager.h
#ifndef BARRIER_MANAGER_H
#define BARRIER_MANAGER_H

#include<string>
#include<vector>
#include "cocos2d.h"
using namespace cocos2d;

class BarrierInfo : public Node {
public:
    CREATE_FUNC(BarrierInfo);

    Sprite* barrierSprite = nullptr;  // �ϰ���
    Sprite* hpHolder = nullptr;      // Ѫ������
    Sprite* hpSlider = nullptr;      // Ѫ������
    Sprite* arrow = nullptr;         // Ѫ������
    int hp = 0;                      // ��ǰѪ��
    int maxHp = 0;                   // ���Ѫ��

    // ��ʼ���ϰ���
    bool initWithParams(const std::string& spritePath, const Vec2& position, int initialHp) {
        // �����ϰ���
        barrierSprite = Sprite::create(spritePath);
        if (!barrierSprite) return false;
        barrierSprite->setPosition(position);
        this->addChild(barrierSprite);

        // ��ʼ��Ѫ��
        maxHp = initialHp*8;
        hp = initialHp*8;

        // ����Ѫ������
        hpHolder = Sprite::create("/Barrier/HpHolder.png");
        hpHolder->setPosition(Vec2(position.x, position.y + barrierSprite->getContentSize().height / 2.25));
        hpHolder->setVisible(false);  
        this->addChild(hpHolder);

        // ����Ѫ������
        hpSlider = Sprite::create("/Barrier/HpSlider.png");
        hpSlider->setAnchorPoint(Vec2(0, 0.5));
        hpSlider->setPosition(Vec2(hpHolder->getPositionX() - hpHolder->getContentSize().width / 2,
            hpHolder->getPositionY()));
        hpSlider->setVisible(false);  
        this->addChild(hpSlider);

        // ��ʼ����ʾ��ͷ
        arrow = Sprite::create("/Barrier/arrow.png");
        arrow->setPosition(Vec2(barrierSprite->getPositionX(), hpHolder->getPositionY() + 19));
        arrow->setVisible(false);
        this->addChild(arrow);

        return true;
    }

    // ��ʾѪ��
    void showHpBar() {
        hpHolder->setVisible(true);
        hpSlider->setVisible(true);
        arrow->setVisible(true);
    }

    // ����Ѫ��
    void hideHpBar() {
        hpHolder->setVisible(false);
        hpSlider->setVisible(false);
        arrow->setVisible(false);
    }

    // ����Ѫ��
    void takeDamage(int damage) {
        hp = std::max(hp - damage, 0);
        float hpRatio = static_cast<float>(hp) / maxHp;
        hpSlider->setScaleX(hpRatio);
    }

    // ����Ƿ�����
    bool isDead() const {
        return hp <= 0;
    }

    // �Ƴ��ϰ��ＰѪ��
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

    Vector<BarrierInfo*> barriers;      // �洢�����ϰ���
    BarrierInfo* selectedBarrier = nullptr;  // ��ǰѡ�е��ϰ���

    // ��ʼ���ϰ���
    void BarrierAppear(int type, float positionX, float positionY, int initialHp) {
        // �ϰ����ͼƬ·��
        std::string picture[] = {
            "/Barrier/one1.png", "/Barrier/one2.png",
            "/Barrier/two1.png", "/Barrier/two2.png",
            "/Barrier/four1.png", "/Barrier/four2.png"
        };

        // ���� BarrierInfo ʵ��
        auto barrierInfo = BarrierInfo::create();
        if (barrierInfo && barrierInfo->initWithParams(picture[type], Vec2(positionX, positionY), initialHp)) {
            this->addChild(barrierInfo);
            barriers.pushBack(barrierInfo);
        }
    }

    // ������¼�������
    void createMouseEventListener() {
        auto listener = EventListenerMouse::create();

        listener->onMouseDown = [this](Event* event) {
            auto mouseEvent = dynamic_cast<EventMouse*>(event);
            Vec2 clickPosition = mouseEvent->getLocationInView();
            clickPosition = this->convertToNodeSpace(clickPosition);

            // �ж��Ƿ����ϰ���
            BarrierInfo* clickedBarrier = getClickedBarrier(clickPosition);
            if (clickedBarrier) {
                // ���������ϰ����ʾ��Ѫ����ʹ���Ϊѡ���ϰ���
                deselectBarrier();  // ȡ��֮ǰ��ѡ��״̬
                selectedBarrier = clickedBarrier;
                selectedBarrier->showHpBar();


                selectedBarrier->takeDamage(1);  // ʾ������������ 1 ��Ѫ��

                // ����ϰ���Ѫ��Ϊ 0���Ƴ��ϰ���
                if (selectedBarrier->isDead()) {
                    removeBarrier(selectedBarrier);
                    selectedBarrier = nullptr;
                }
            }
            else {
                // ����հ���������Ѫ��
                deselectBarrier();
            }
            };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }

    // ���ݵ��λ�û�ȡ�ϰ���
    BarrierInfo* getClickedBarrier(const Vec2& position) {
        for (auto& info : barriers) {
            if (info->barrierSprite && info->barrierSprite->getBoundingBox().containsPoint(position)) {
                return info;
            }
        }
        return nullptr;
    }

    // �Ƴ��ϰ���
    void removeBarrier(BarrierInfo* barrierInfo) {
        barriers.eraseObject(barrierInfo);
        barrierInfo->removeFromScene();
    }

    // ȡ��ѡ���ϰ���
    void deselectBarrier() {
        if (selectedBarrier) {
            selectedBarrier->hideHpBar();
            selectedBarrier = nullptr;
        }
    }
};




#endif // BARRIER_MANAGER_H
