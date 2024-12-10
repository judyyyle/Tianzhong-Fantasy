#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "Monster.h"
#include "cocos2d.h"
#include<vector>
using std::vector;

enum GameState {
	ONGOING,  // ��Ϸ������
	WIN,      // ��Ϸʤ��
	LOSE      // ��Ϸʧ��
};


class GameManager
{
public:
	static  GameManager* getInstance();

	void saveGameState();              // ������Ϸ״̬
	void loadGameState();              // ������Ϸ״̬

	// ��ӹ��ﵽ�б�
	void addMonster(Monster* monster);

	// �Ƴ�����
	void removeMonster(Monster* monster);

	// ��ȡ��ǰ���������д����� 
	const vector<Monster*>& getActiveMonsters()const;


	// ��Ϸ״̬���
	void setGameState(GameState state);  // ������Ϸ״̬
	GameState getGameState() const;      // ��ȡ��ǰ��Ϸ״̬

	void endGame();
	void GameManager::checkForGameOver() {
		if (carrotHP <= 0) {
			setGameState(LOSE);  // ����Ϊʧ��״̬
		}
	}

	void GameManager::checkForVictory() {
		if (activeMonsters.empty() && currentWaves >= allWaves) {
			setGameState(WIN);  // ����Ϊʤ��״̬
		}
	}

	// ��Ϸ״̬����
	int allWaves;        // �ܲ���
	int currentWaves;    // ��ǰ����
	int carrotHP;        // �ܲ�Ѫ��
	int coinNumber;      // �������
	int level;           // ��ǰ�ؿ����
	int towerCount;      // ����������

private:
	GameManager();
	// �洢��ǰ���й���
	std::vector<Monster*> activeMonsters;
	GameState currentState; // ��Ϸ��ǰ״̬
	// ˽�й��캯��������ģʽ��
	static GameManager* instance;      // ��̬����ʵ��


};

#endif // __GAME_MANAGER_H__