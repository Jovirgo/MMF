//
//  GameBlocksCountManager.h
//  NewFlipop
//
//  Created by OJO on 15/2/8.
//
//

#ifndef __NewFlipop__GameBlocksCountManager__
#define __NewFlipop__GameBlocksCountManager__

#include "GameBlock.h"
#include <vector>

class GameBlocksCountManager
{
public:
    static GameBlocksCountManager* getInstance();
    
private:
    static GameBlocksCountManager* _instance;
    
public:
    int getNewBlockType();
    
    int getNewCardTypeAtRandom();
    
public:
    void addBlockTypeToPriorityQueue(int type)
    {
        CCASSERT(type > GB_NULL && type < GB_TypeCount, "");
        
        _priorityBlockTypeQueue.push_back(type);
    }

	void setBlockTypeWithRowsAndColumns(int rows, int cols);
    
private:
    std::list<int> _priorityBlockTypeQueue;
    
public:
    void increaseTotalCountWithType(int type);
    void increaseClearCountWithType(int type);
    
    void resetBlocksCount();
    
    int  getTotalCountWithType(int type) { return _totalBlocksCount[type]; }
    int  getClearCountWithType(int type) { return _clearBlocksCount[type]; }
    
    int  getRemainingCountWithType(int type)
    {
        return _totalBlocksCount[type] - _clearBlocksCount[type];
    }

	int getTotalCountWithRemaining();

private:
    int	_totalBlocksCount[GB_TypeCount];
    int _clearBlocksCount[GB_TypeCount];
	
public:
	void setTypeLaunched(int type, bool var);

private:
	bool _isTypeLaunched[GB_TypeCount];

	int  _typeLaunchedCount;
    
private:
    GameBlocksCountManager(void)
		: _typeLaunchedCount(0)
    {
        resetBlocksCount();
    }
};

#endif /* defined(__NewFlipop__GameBlocksCountManager__) */
