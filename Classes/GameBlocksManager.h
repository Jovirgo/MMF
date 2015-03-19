//
//  GameBlocksManager.h
//  NewFlipop
//
//  Created by OJO on 15/1/2.
//
//

#ifndef __NewFlipop__GameBlocksManager__
#define __NewFlipop__GameBlocksManager__

#include "cocos2d.h"
#include "GameBlock.h"
#include "GameConstant.h"


class GameBlock;
class GameCard;

class GameBlocksManager: public cocos2d::Node
{
public:
    static GameBlocksManager* create(int rows, int cols, int colors);
    
    virtual bool init(int rows, int cols, int cardTypes);
    
    virtual void update(float dt);
    
    virtual void updateForFilling(float dt);
    virtual void updateForFalling(float dt);

protected:
	bool _isFillingAndFalling;

	void setFillingAndFallingEnabled(bool var);
    
public:
    int getRow() { return _rows; }
    int getCol() { return _cols; }
    
protected:
    int _rows;
    int _cols;
    
    GameBlock* _headBlockOfCol[BLOCKS_COL_MAX];
    GameBlock* _tailBlockOfCol[BLOCKS_COL_MAX];
    
    GameBlock* createBlock();
    
    void pushBlockToCol(int col, GameBlock* block);
    
public:
    void openTheCard(GameCard* card);

	// tool 1:
	void showRevSideHintWithDuration(float duration);

	// tool 2:
	void autoMatchCards(int matchTimes);
    
protected:
    bool isMatch(GameCard const* lCard,
                 GameCard const* rCard);
    
    void pairingSucceed(void);
    
    void matchCardWithCard(GameCard* lCard, GameCard* rCard);
    void matchWildWithWild(GameCard* lWild, GameCard* rWild);
    void matchWildWithCard(GameCard*  wild, GameCard*  card);
    
    void pairingFailed(void);
    
    CC_SYNTHESIZE_RETAIN(GameCard*, _1stCard, 1stCard);
    CC_SYNTHESIZE_RETAIN(GameCard*, _2ndCard, 2ndCard);
    
    CC_PROPERTY(bool,	_isFeverModeEnabled,	FeverModeEnabled);
	CC_PROPERTY(bool,	_isTouchEnabled,		TouchEnabled);

protected:
    GameBlocksManager(void);
    
    virtual ~GameBlocksManager(void);
};

#endif /* defined(__NewFlipop__GameBlocksManager__) */
