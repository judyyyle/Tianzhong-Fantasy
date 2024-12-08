#ifndef __LEVEL_SELECTION_SCENE_H__
#define __LEVEL_SELECTION_SCENE_H__
#include"GamePlayScene.h"
#include "cocos2d.h"

class LevelSelectionScene : public cocos2d::Scene
{
public:
    LevelSelectionScene();
    ~LevelSelectionScene();

    static LevelSelectionScene* create();

    bool init() override;
};

#endif // __LEVEL_SELECTION_SCENE_H__
