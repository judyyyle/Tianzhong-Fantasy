#include "Monster.h"
#include "HelloWorldScene.h"
#include <cmath>
#include <vector>
#include<algorithm>

#include "Tower.h"

extern int carrot_HP;//萝卜血量

extern int monster_number;//怪物总数
extern int barrier_number;//障碍总数
extern std::vector<Monster*>monster;
extern std::vector<Monster*>barrier;

extern Monster* destination;//怪物位置
