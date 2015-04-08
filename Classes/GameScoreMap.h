#ifndef __NewFlipop_GameScoreMap__
#define __NewFlipop_GameScoreMap__

#include <map>

class GameScoreMap
{
public:
	static GameScoreMap* getInstance();
private:
	static GameScoreMap* _instance;

public:
	void setScoreWithGameLevel(int level, int score);
	int  getScoreWithGameLevel(int level);

	void loadScoreMap();
	void saveScoreMap();

	void saveScoreWithLevel(int level, int score);

private:
	std::map<int, int>	_levelScoreMap;

	GameScoreMap();
};

#endif