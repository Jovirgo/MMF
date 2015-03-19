//
//  GameData.h
//  NewFlipop
//
//  Created by OJO on 15/1/6.
//
//

#ifndef __NewFlipop__GameData__
#define __NewFlipop__GameData__

#include "cocos2d.h"
#include "GameConstant.h"

class GameData
{
public:
    static GameData * getInstance(void);
    
private:
    static GameData * _instance;
    
public:
    void increaseScore(int delta)
    {
        _score += delta;
    }
    
    void increaseCombo()
    {
		if (_comboMax < (++_combo))
		{
			_comboMax = _combo;
		}

		CCLOG("_combo = %d", _combo);
    }

	void increaseMatch()
	{
		++_match;
	}
    
    int getScoreDelta()
    {
        return BASE_SCORE * _combo * _multiply;
    }

	void resetGameData();
    
    CC_SYNTHESIZE(int,  _score,     Score);
    CC_SYNTHESIZE(int,  _combo,     Combo);
    CC_SYNTHESIZE(int,  _comboMax,  ComboMax);
    CC_SYNTHESIZE(int,  _multiply,  Multiply);
	CC_SYNTHESIZE(int,	_match,		Match);
    
public:
    void increaseCoins(int delta)
    {
        _coins += delta;
    }
    
    CC_SYNTHESIZE(int,	_coins,		 Coins);
    
private:
    GameData(void);
};

#endif /* defined(__NewFlipop__GameData__) */
