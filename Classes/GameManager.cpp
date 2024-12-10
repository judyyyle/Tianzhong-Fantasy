#include "GameManager.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "LevelSelectionScene.h"
#include "cocos2d.h"  // 已经包含了 ui/CocosGUI.h 所以不需要再包含

using namespace cocos2d;  // 仅使用 cocos2d 命名空间


GameManager::GameManager() {
    // 初始化成员变量
    allWaves = 0;
    currentWaves = 0;
    carrotHP = 10;  // 默认血量
    coinNumber = 100;
    level = 1;
    towerCount = 0;
    currentState = ONGOING; // 默认状态为进行中
}

// 静态成员变量初始化

GameManager* GameManager::instance = nullptr;

// 获取单例实例
GameManager* GameManager::getInstance() {
    if (!instance) {
        instance = new GameManager();
    }
    return instance;
}

// 设置游戏状态
void GameManager::setGameState(GameState state) {
    currentState = state;
    // 你可以根据状态做一些额外的操作，比如记录日志等
    CCLOG("Game state changed: %d", currentState);
}

// 获取当前游戏状态
GameState GameManager::getGameState() const {
    return currentState;
}

//保存游戏状态
void GameManager::saveGameState() {
    UserDefault::getInstance()->setIntegerForKey("all_waves", allWaves);
    UserDefault::getInstance()->setIntegerForKey("current_waves", currentWaves);
    UserDefault::getInstance()->setIntegerForKey("carrot_hp", carrotHP);
    UserDefault::getInstance()->setIntegerForKey("coin_number", coinNumber);
    UserDefault::getInstance()->setIntegerForKey("current_level", level);
    UserDefault::getInstance()->setIntegerForKey("tower_count", towerCount);

    UserDefault::getInstance()->flush(); // 确保立即写入磁盘
    CCLOG("Game state saved!");
}

//加载游戏状态
void GameManager::loadGameState() {
    allWaves = UserDefault::getInstance()->getIntegerForKey("all_waves", 0);
    currentWaves = UserDefault::getInstance()->getIntegerForKey("current_waves", 0);
    carrotHP = UserDefault::getInstance()->getIntegerForKey("carrot_hp", 10); // 默认萝卜血量为 10
    coinNumber = UserDefault::getInstance()->getIntegerForKey("coin_number", 100);
    level = UserDefault::getInstance()->getIntegerForKey("current_level", 1); // 默认关卡为 1
    towerCount = UserDefault::getInstance()->getIntegerForKey("tower_count", 0);

    CCLOG("Game state loaded: allWaves=%d, currentWaves=%d, carrotHP=%d, coinNumber=%d, level=%d, towerCount=%d",
        allWaves, currentWaves, carrotHP, coinNumber, level, towerCount);
}

void GameManager::addMonster(Monster* monster) {
    activeMonsters.push_back(monster);
}
void GameManager::removeMonster(Monster* monster) {
    activeMonsters.erase(std::remove(activeMonsters.begin(), activeMonsters.end(), monster), activeMonsters.end());
}

const std::vector<Monster*>& GameManager::getActiveMonsters() const {
    return activeMonsters;
}

// 中止游戏并保存当前游戏状态
void GameManager::endGame() {
    saveGameState();  // 保存当前的游戏状态
    setGameState(GameState::LOSE);  // 设置游戏状态为失败，可以根据需要改为 WIN 或其他状态
    CCLOG("Game has ended. Current game state saved.");
}