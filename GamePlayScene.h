#pragma once
#ifndef __Game_Play_Scene_H__
#define  __Game_Play_Scene_H__

#include "cocos2d.h"
#include "Tower.h"
#include<vector>
#include"BarrierAppear.h"
//�궨��
#define PATH -1
#define SPACE 0
#define BARRIER 1
#define EXISTED_TOWER 2


class MAP_SCENE : public cocos2d::Scene
{
public:
    MAP_SCENE();        // ���캯��
    virtual ~MAP_SCENE(); // ��������

    // ���������ľ�̬����
    static MAP_SCENE* create();

    // ��ʼ�����������������д
    bool init() override;

    // ���ڳ�ʼ����ͼ��������д�˷�����
    void initializeMap();
    void onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void showBuildFeedback(int row, int col);
    void clearWarningSprites();//�����ʾͼ��
    void addTowerPreview(int row, int col);

    void handleTowerClick(int row, int col);
    void onTowerPreviewClicked(int towerIndex, int row, int col);
    void upgradeTower(int row, int col);
    void updateordeleteTowerPreview(int row, int col);
    void deleteTower(int row, int col);
    Tower* getTowerAt(int row, int col);
protected:
    cocos2d::Sprite* background;  // ����
    int map[8][12];   // ��ͼ״̬����
    Tower* towerArray[8][12] = {nullptr}; 
    bool isTowerSelected = false; // �Ƿ���ѡ�������
    std::vector<Node*> warningSprites; // �洢��ʾͼƬ�Ͱ�ť
    // �����ͼ�ϲ�ͬλ�õ���Ϊ
    void handleMapAction(int row, int col);
};


#endif // __MAP_H__