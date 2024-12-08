#pragma once
#ifndef __Game_Play_Scene_H__
#define  __Game_Play_Scene_H__

#include "cocos2d.h"
#include<vector>

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
    void initializeMap() ;
    void onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);


protected:
    cocos2d::Sprite* background;  // ����
    int map[8][12];                   // ��ͼ״̬����

    // �����ͼ�ϲ�ͬλ�õ���Ϊ
    void handleMapAction(int row, int col);
};


#endif // __MAP_H__