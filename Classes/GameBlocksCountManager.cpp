//
//  GameBlocksCountManager.cpp
//  NewFlipop
//
//  Created by OJO on 15/2/8.
//
//

#include "GameBlocksCountManager.h"
#include "GameBlockManager.h"

USING_NS_CC;

GameBlocksCountManager* GameBlocksCountManager::_instance = nullptr;

GameBlocksCountManager* GameBlocksCountManager::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new GameBlocksCountManager();
    }
    
    return _instance;
}

int GameBlocksCountManager::getNewBlockType()
{
	int type = GB_NULL;

    if (_priorityBlockTypeQueue.empty())
    {
        // type = TYPE_NULL;
    }
    else
    {
        type = _priorityBlockTypeQueue.front();
        
        _priorityBlockTypeQueue.pop_front();
    }
    
    return type;
}

int GameBlocksCountManager::getNewCardTypeAtRandom()
{
	CCASSERT(_typeLaunchedCount > 1, "");

	// index of launched type 
    int n = int(CCRANDOM_0_1()*_typeLaunchedCount) % _typeLaunchedCount;

	for (int type = GB_Rvs1; type != GB_TypeCount; ++type)
	{
		if (_isTypeLaunched[type])
		{
			if ((n--) == 0)
			{
				return type;
			}
		}
	}

	return GB_NULL;
}

void GameBlocksCountManager::setBlockTypeWithRowsAndColumns(int rows, int cols)
{
	std::vector<int> blockTypes;

	int totalCount = rows * cols;

	int typeCount = 0;
	int type = GB_NULL;

	while (true)
	{
		for (int i = 0; i != totalCount/2; ++i)
		{
			int newType = getNewCardTypeAtRandom();

			if (newType != type)
			{
				++typeCount;
				type = newType;
			}

			blockTypes.push_back(type);
			blockTypes.push_back(type);
		}

		if (typeCount < 2)	// at least 2 types
		{
			typeCount = 0;
			blockTypes.clear();
		}
		else
		{
			break;
		}
	}

	std::random_shuffle(blockTypes.begin(), blockTypes.end());

	for (auto iter = blockTypes.begin() ;iter != blockTypes.end() ;++iter)
	{
		addBlockTypeToPriorityQueue(*iter);
	}
}

void GameBlocksCountManager::increaseTotalCountWithType(int type)
{
    CCASSERT(type >= GB_NULL && type < GB_TypeCount, "");
    
    ++_totalBlocksCount[type];
}

void GameBlocksCountManager::increaseClearCountWithType(int type)
{
    CCASSERT(type >= GB_NULL && type < GB_TypeCount, "");
    
    ++_clearBlocksCount[type];

	// Will be moved
	if (getTotalCountWithRemaining() == 0)
	{
		EventCustom event( EVENT_BeginNextRound );
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
	}
}

void GameBlocksCountManager::resetBlocksCount()
{
	_typeLaunchedCount = 0;

    for (int type = GB_Rvs1; type != GB_TypeCount; ++type)
    {
        _totalBlocksCount[type] = 0;
        _clearBlocksCount[type] = 0;

		_isTypeLaunched[type] = false;
    }
}

int GameBlocksCountManager::getTotalCountWithRemaining()
{
	int totalRemaining = 0;

	for (int type = GB_Rvs1; type != GB_TypeCount; ++type)
	{
		totalRemaining += getRemainingCountWithType(type);
	}

	return totalRemaining;
}

void GameBlocksCountManager::setTypeLaunched(int type, bool var)
{
	if (_isTypeLaunched[type] == var)
	{
		return;
	}
	else
	{
		_isTypeLaunched[type] = var;
	}

	if (_isTypeLaunched[type])
	{
		++_typeLaunchedCount;
	}
	else
	{
		--_typeLaunchedCount;
	}
}
