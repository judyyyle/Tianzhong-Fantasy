#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "Monster.h"
#include "cocos2d.h"
#include<vector>
using std::vector;

enum GameState {
	ONGOING,  // 游戏进行中
	WIN,      // 游戏胜利
	LOSE      // 游戏失败
};


class GameManager
{
public:
	static  GameManager* getInstance();

	void saveGameState();              // 保存游戏状态
	void loadGameState();              // 加载游戏状态

	// 添加怪物到列表
	void addMonster(Monster* monster);

	// 移除怪物
	void removeMonster(Monster* monster);

	// 获取当前场景中所有存活怪物 
	const vector<Monster*>& getActiveMonsters()const;


	// 游戏状态相关
	void setGameState(GameState state);  // 设置游戏状态
	GameState getGameState() const;      // 获取当前游戏状态

	void endGame();
	void GameManager::checkForGameOver() {
		if (carrotHP <= 0) {
			setGameState(LOSE);  // 设置为失败状态
		}
	}

	void GameManager::checkForVictory() {
		if (activeMonsters.empty() && currentWaves >= allWaves) {
			setGameState(WIN);  // 设置为胜利状态
		}
	}

	// 游戏状态变量
	int allWaves;        // 总波数
	int currentWaves;    // 当前波数
	int carrotHP;        // 萝卜血量
	int coinNumber;      // 金币数量
	int level;           // 当前关卡编号
	int towerCount;      // 防御塔数量

private:
	GameManager();
	// 存储当前所有怪物
	std::vector<Monster*> activeMonsters;
	GameState currentState; // 游戏当前状态
	// 私有构造函数（单例模式）
	static GameManager* instance;      // 静态单例实例


};

#endif // __GAME_MANAGER_H__