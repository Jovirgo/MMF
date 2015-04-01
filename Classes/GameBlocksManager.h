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

// event id
#define EVENT_BeginNextRound		"enabledFallingAndFilling"
#define EVENT_StopFallingAndFilling	"disabledFallingAndFilling"

#define ANI_Coin	"CoinAnimation"
#define ANI_WildObv	"WildObvAnimation"
#define ANI_WildRev	"WildRevAnimation"
#define ANI_Mult	"MultAnimation"



class GameBlock;
class GameCard;

class GameBlocksManager: public cocos2d::Node
{
public:
    static GameBlocksManager* create(int rows, int cols, const int color[5]);
    
    virtual bool init(int rows, int cols, const int color[5]);
    
    virtual void update(float dt);
    
    virtual void updateForFilling(float dt);
    virtual void updateForFalling(float dt);

protected:
	bool _isFillingAndFalling;

	void setFillingAndFallingEnabled(bool var);
	void pauseFillingAndFalling();
	void resumeFillingAndFalling();
    
public:
    int getRows() { return _rows; }
    int getCols() { return _cols; }
    
protected:
    int _rows;
    int _cols;
    
	GameBlock* _headBlockOfCol[ GAMEPLAY_BlocksColMax ];
    GameBlock* _tailBlockOfCol[ GAMEPLAY_BlocksColMax ];
    
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
    
	// succeed
    void pairingSucceed(void);
    
    void matchCardWithCard(GameCard* lCard, GameCard* rCard);
    void matchWildWithWild(GameCard* lWild, GameCard* rWild);
    void matchWildWithCard(GameCard*  wild, GameCard*  card);

	void dispatchPutScoreEventWithCard(int score, GameCard* card);
    
	// failed
    void pairingFailed(void);
    
    CC_SYNTHESIZE(GameCard*, _1stCard, 1stCard);
    CC_SYNTHESIZE(GameCard*, _2ndCard, 2ndCard);
    
    CC_PROPERTY(bool,	_isFeverModeEnabled,	FeverModeEnabled);
	CC_PROPERTY(bool,	_isTouchEnabled,		TouchEnabled);

protected:
    GameBlocksManager(void);
    
    virtual ~GameBlocksManager(void);
};

#endif /* defined(__NewFlipop__GameBlocksManager__) */
