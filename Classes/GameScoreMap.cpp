#include "GameScoreMap.h"
#include "LoadAndSaveDataForCocos2d.h"
#include "cocos2d.h"

GameScoreMap* GameScoreMap::_instance = nullptr;

GameScoreMap* GameScoreMap::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new GameScoreMap();
	}

	return _instance;
}

void GameScoreMap::setScoreWithGameLevel(int level, int score)
{
	if (_levelScoreMap.count(level) == 0 || _levelScoreMap[level] < score)
	{
		_levelScoreMap[level] = score;

		saveScoreWithLevel(level, score);
		saveDataFlush();
	}
}

int GameScoreMap::getScoreWithGameLevel(int level)
{
	if (_levelScoreMap.count(level) == 0)
	{
		return 0;
	}
	else
	{
		return _levelScoreMap[level];
	}
}

void GameScoreMap::loadScoreMap()
{
	char szKey[10];

	for (int level = 1; true; ++level)
	{
		snprintf(szKey, 10, "Level%d", level);

		int score = loadDataByKey_int(szKey, true);
		if (score == 0)
		{
			break;
		}
		else
		{
			_levelScoreMap[level] = score;
		}
	}
}

void GameScoreMap::saveScoreMap()
{
	for (auto iter = _levelScoreMap.begin()
		; iter != _levelScoreMap.end()
		; ++iter)
	{
		saveScoreWithLevel(iter->first, iter->second);
	}
	
	saveDataFlush();
}

void GameScoreMap::saveScoreWithLevel(int level, int score)
{
	char szKey[10];
	snprintf(szKey, 10, "Level%d", level);

	saveDataByKey_int(szKey, score, true);
}

GameScoreMap::GameScoreMap()
{
	const char* haveSavedScoreMap = "HaveSavedScoreMap";

	if (loadDataByKey_bool( haveSavedScoreMap ))
	{
		loadScoreMap();
	}
	else
	{
		saveDataByKey_bool( haveSavedScoreMap, true);
	}
}