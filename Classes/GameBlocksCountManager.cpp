//
//  GameBlocksCountManager.cpp
//  NewFlipop
//
//  Created by OJO on 15/2/8.
//
//

#include "GameBlocksCountManager.h"
#include "GameBlocksManager.h"

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
	int type = TYPE_NULL;

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
	CCASSERT(_typeLaunchedCount != 0, "");

    int random = int(CCRANDOM_0_1()*_typeLaunchedCount) % _typeLaunchedCount;

	for (int type = CARD_1; type != TYPE_TOTAL; ++type)
	{
		if (_isTypeLaunched[type])
		{
			if (random-- == 0)
			{
				return type;
			}
		}
	}

	return TYPE_NULL;
}

void GameBlocksCountManager::increaseTotalCountWithType(int type)
{
    CCASSERT(type >= TYPE_NULL && type < TYPE_TOTAL, "");
    
    ++_totalBlocksCount[type];
}

void GameBlocksCountManager::increaseClearCountWithType(int type)
{
    CCASSERT(type >= TYPE_NULL && type < TYPE_TOTAL, "");
    
    ++_clearBlocksCount[type];

	if (getTotalCountWithRemaining() == 0)
	{
		EventCustom event( EVENT_BeginNextRound );
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
	}
}

void GameBlocksCountManager::resetBlocksCount()
{
	_typeLaunchedCount = 0;

    for (int type = CARD_1; type != TYPE_TOTAL; ++type)
    {
        _totalBlocksCount[type] = 0;
        _clearBlocksCount[type] = 0;

		_isTypeLaunched[type] = false;
    }
}

int GameBlocksCountManager::getTotalCountWithRemaining()
{
	int totalRemaining = 0;

	for (int type = CARD_1; type != TYPE_TOTAL; ++type)
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
