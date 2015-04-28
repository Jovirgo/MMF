//
//  GameBlockManager.h
//  NewFlipop
//
//  Created by OJO on 15/1/2.
//
//

#ifndef __NewFlipop__GameBlockManager__
#define __NewFlipop__GameBlockManager__


// event id
#define EVENT_PairCorrect	"FeverDecreaseEvent"
#define EVENT_PairIncorrect	"FeverIncreaseEvent"

#define EVENT_PauseTimer	"PauseTimerEvent"
#define EVENT_ResumeTimer	"ResumeTimerEvent"

#define EVENT_PutScore		"PutScoreEvent"
#define EVENT_GetScore		"GetScoreEvent"
#define UDATA_PutScore		"%d,%d,%.1f,%.1f"

// event id
#define EVENT_BeginNextRound		"enabledFallingAndFilling"
#define EVENT_StopFallingAndFilling	"disabledFallingAndFilling"



#include "GameBlock.h"
#include <Set>
#include "../Classes/GameConstant.h"

class GameBlockManager: public cocos2d::Node
{
public:
	/**
	* Init
	*/
	virtual bool init(int rows, int cols);

	virtual void update(float dt);



public:
	/**
	* Matching
	*/
	void matchingReversibleBlock(ReversibleBlock* rBlock);

protected:
	// Pair mode : Default
	void matchingInPairMode(ReversibleBlock* rBlock);
	void matchPairCorret();
	void matchPairIncorret();

    CC_SYNTHESIZE(ReversibleBlock*, _1stPairBlock, 1stPairBlock);
    CC_SYNTHESIZE(ReversibleBlock*, _2ndPairBlock, 2ndPairBlock);

public:
	// Specified mode
	void matchingInSpecifiedMode(ReversibleBlock* rBlock);
	void matchSpecifiedTypeCorret(ReversibleBlock* rBlock);
	void matchSpecifiedTypeIncorret(ReversibleBlock* rBlock);

	void addSpecifiedType(int type);
	void removeSpecifiedType(int type);
	bool isSpecifiedType(int type);
	
	void setSpecifiedMode(bool var)	{ _isSpecifiedMode = var;  }
	bool  isSpecifiedMode(void)		{ return _isSpecifiedMode; }

protected:
	bool _isSpecifiedMode;

	std::set<int> _specifiedTypeSet;



public:
	/**
	* Falling
	*/
	void updateForFalling(float dt);

	void setFallingEnabled(bool var);
	bool  isFallingEnabled();

protected:
	bool _isFallingEnabled;


	
public:
	/**
	* Filling
	*/
	void updateForFilling(float dt);

	void setFillingEnabled(bool var);
	bool  isFillingEnabled();

protected:
	bool _isFillingEnabled;



public:
	/**
	* All the blocks
	*/
	// add
	void addBlockToTailOfCol(GameBlock* block, int col);

	// remove
	void removeBlock(GameBlock* block);

	// find
	GameBlock* getBlockWithPositionRow(int row, int col);	// >= 0
	GameBlock* getBlockWithRelationRow(int row, int col);	// >= 0

	// replace
	void replaceBlock(GameBlock* oldBlock, GameBlock* newBlock);

protected:
	GameBlock* _headBlockOfCol[ GAMEPLAY_BlocksColMax ];
    GameBlock* _tailBlockOfCol[ GAMEPLAY_BlocksColMax ];



public:
	/**
	* Row & Col
	*/
    int getRows() { return _rows; }
    int getCols() { return _cols; }

protected:
    int _rows;
    int _cols;



public:
	// Touch enabled
	CC_PROPERTY(bool,	_isTouchEnabled,	TouchEnabled);

	// Side
	CC_SYNTHESIZE(bool,	_isObvSideDefault,	ObvSideDefault);

	// Reverse side hint
	CC_SYNTHESIZE(bool, _isRevSideHint,		RevSideHint);

protected:
	GameBlockManager()
		: _1stPairBlock(nullptr)
		, _2ndPairBlock(nullptr)
		, _isSpecifiedMode(false)
		, _isFallingEnabled(false)
		, _isFillingEnabled(false)
		, _rows(GAMEPLAY_BlocksRowMax)
		, _cols(GAMEPLAY_BlocksColMax)
		, _isTouchEnabled(false)
		, _isObvSideDefault(false)
		, _isRevSideHint(false)
	{
		for (int idx = 0; idx != GAMEPLAY_BlocksColMax; ++idx)
		{
			_headBlockOfCol[idx] = nullptr;
			_tailBlockOfCol[idx] = nullptr;
		}
	}

	virtual ~GameBlockManager();
};

#endif /* defined(__NewFlipop__GameBlockManager__) */