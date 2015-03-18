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
    
    int getNewCardTypeInMinimum();
    
    int getNewCardTypeAtRandom();
    
private:
    int _newType;
    
    CC_PROPERTY(int, _cardTypes, CardTypes);
    
public:
    void addBlockTypeToPriorityQueue(int type)
    {
        CCASSERT(type > TYPE_NULL && type < TYPE_TOTAL, "");
        
        _priorityBlockTypeQueue.push_back(type);
    }
    
private:
//    void addFormationToPriorityQueue(const std::vector<int>& formation);
    
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
    int _totalBlocksCount[TYPE_TOTAL];
    int _clearBlocksCount[TYPE_TOTAL];
    
private:
    GameBlocksCountManager(void)
    : _newType(TYPE_NULL)
    , _cardTypes(5)
    {
        resetBlocksCount();
    }
};

#endif /* defined(__NewFlipop__GameBlocksCountManager__) */
