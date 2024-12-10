#include "GameManager.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "LevelSelectionScene.h"
#include "cocos2d.h"  // �Ѿ������� ui/CocosGUI.h ���Բ���Ҫ�ٰ���

using namespace cocos2d;  // ��ʹ�� cocos2d �����ռ�


GameManager::GameManager() {
    // ��ʼ����Ա����
    allWaves = 0;
    currentWaves = 0;
    carrotHP = 10;  // Ĭ��Ѫ��
    coinNumber = 100;
    level = 1;
    towerCount = 0;
    currentState = ONGOING; // Ĭ��״̬Ϊ������
}

// ��̬��Ա������ʼ��

GameManager* GameManager::instance = nullptr;

// ��ȡ����ʵ��
GameManager* GameManager::getInstance() {
    if (!instance) {
        instance = new GameManager();
    }
    return instance;
}

// ������Ϸ״̬
void GameManager::setGameState(GameState state) {
    currentState = state;
    // ����Ը���״̬��һЩ����Ĳ����������¼��־��
    CCLOG("Game state changed: %d", currentState);
}

// ��ȡ��ǰ��Ϸ״̬
GameState GameManager::getGameState() const {
    return currentState;
}

//������Ϸ״̬
void GameManager::saveGameState() {
    UserDefault::getInstance()->setIntegerForKey("all_waves", allWaves);
    UserDefault::getInstance()->setIntegerForKey("current_waves", currentWaves);
    UserDefault::getInstance()->setIntegerForKey("carrot_hp", carrotHP);
    UserDefault::getInstance()->setIntegerForKey("coin_number", coinNumber);
    UserDefault::getInstance()->setIntegerForKey("current_level", level);
    UserDefault::getInstance()->setIntegerForKey("tower_count", towerCount);

    UserDefault::getInstance()->flush(); // ȷ������д�����
    CCLOG("Game state saved!");
}

//������Ϸ״̬
void GameManager::loadGameState() {
    allWaves = UserDefault::getInstance()->getIntegerForKey("all_waves", 0);
    currentWaves = UserDefault::getInstance()->getIntegerForKey("current_waves", 0);
    carrotHP = UserDefault::getInstance()->getIntegerForKey("carrot_hp", 10); // Ĭ���ܲ�Ѫ��Ϊ 10
    coinNumber = UserDefault::getInstance()->getIntegerForKey("coin_number", 100);
    level = UserDefault::getInstance()->getIntegerForKey("current_level", 1); // Ĭ�Ϲؿ�Ϊ 1
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

// ��ֹ��Ϸ�����浱ǰ��Ϸ״̬
void GameManager::endGame() {
    saveGameState();  // ���浱ǰ����Ϸ״̬
    setGameState(GameState::LOSE);  // ������Ϸ״̬Ϊʧ�ܣ����Ը�����Ҫ��Ϊ WIN ������״̬
    CCLOG("Game has ended. Current game state saved.");
}