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
    if (_priorityBlockTypeQueue.empty())
    {
        _newType = getNewCardTypeInMinimum();
    }
    else
    {
        _newType = _priorityBlockTypeQueue.front();
        
        _priorityBlockTypeQueue.pop_front();
    }
    
    return _newType;
}

int GameBlocksCountManager::getNewCardTypeInMinimum()
{
    int minType_1 = CARD_1;
    int minType_2 = CARD_2;
    
    for (int type = CARD_3; type <= _cardTypes; ++type)
    {
        if (getRemainingCountWithType(minType_2) > getRemainingCountWithType(type))
        {
            minType_2 = type;
        }
        
        if (getRemainingCountWithType(minType_1) > getRemainingCountWithType(minType_2))
        {
            int temp  = minType_1;
            minType_1 = minType_2;
            minType_2 = temp;
        }
    }
    
    return (_newType != minType_1) ? minType_1 : minType_2;
}

int GameBlocksCountManager::getNewCardTypeAtRandom()
{
    return CARD_1 + int(CCRANDOM_0_1()*_cardTypes) % _cardTypes;
}

void GameBlocksCountManager::setCardTypes(int var)
{
    CCASSERT(var > 2, "");
    
    _cardTypes = var;
}

int GameBlocksCountManager::getCardTypes()
{
    return _cardTypes;
}

//void GameBlocksCountManager::addFormationToPriorityQueue(const std::vector<int>& formation)
//{
//    for (int col = 0; col != BLOCKS_COL_MAX; ++col)
//    {
//        for (int row = BLOCKS_ROW_MAX-1; row >= 0; --row)
//        {
//            addBlockTypeToPriorityQueue( formation[col+row*BLOCKS_COL_MAX] );
//        }
//    }
//}

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
		EventCustom event("enabledFallingAndFilling");
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
	}
}

void GameBlocksCountManager::resetBlocksCount()
{
    for (int type = CARD_1; type != TYPE_TOTAL; ++type)
    {
        _totalBlocksCount[type] = 0;
        _clearBlocksCount[type] = 0;
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
