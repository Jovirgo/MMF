//
//  GameBlock.h
//  NewFlipop
//
//  Created by OJO on 15/4/22.
//
//
#ifndef __NewFlipop__GameBlock__
#define __NewFlipop__GameBlock__

#include "cocos2d.h"

enum GameBlockType
{
	GB_NULL = 0,

	// Reversible
	GB_Rvs1,
	GB_Rvs2,
	GB_Rvs3,
	GB_Rvs4,
	GB_Rvs5,

	GB_TypeCount
};

class GameBlock: public cocos2d::Sprite
{
public:	
	static GameBlock* create();	// Create block with GB_NULL type



public:	
	/**
	* Init
	*/
    virtual bool initWithFile(const char* szFileName);

	void initGameBlock();

    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
//    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
//    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);



public:
	/**
	* Above & Below
	*/
    CC_SYNTHESIZE_RETAIN(GameBlock*, _aboveBlock, AboveBlock);
    CC_SYNTHESIZE_RETAIN(GameBlock*, _belowBlock, BelowBlock);

    void removeFromBlockManager(void);


public:
	/**
	* Falling
	*/
	void updateForFalling(float dt);

	bool isFalling() { return _isFalling; }

protected:
	virtual void didFinishFalling();

	bool _isFalling;


public:
	/**
	* Type
	*/
	int getType() const
	{ 
		return _type; 
	}

private:
	int _type;



public:
	/**
	* Active
	*/
	bool  isActive() { return _isActive; }
	void setActive(bool var);
private:
	bool _isActive;


public:
	// Touch enabled
	CC_SYNTHESIZE(bool, _isTouchEnabled, TouchEnabled);

	// Row
	int getRowByPosition();	// >= 0
	int getRowByRelation();	// >= 0

	// Fever mode
    virtual void setFeverModeEnabled(bool enabled);

	// Action before removself
	virtual cocos2d::FiniteTimeAction* removeSelfAction();


protected:
	GameBlock(int type);
	virtual ~GameBlock();
};



class ReversibleBlock: public GameBlock
{
public:
    static ReversibleBlock* createWithType(int type);
    
    virtual bool init(void);



public:
	/**
	* Flip to Obv|Rev
	*/
	void flipToObvSideAndRemoveSelf();

    void flipToObvSide();
    void flipToRevSide();

	void setFlipToObvSideEnabled(bool var);

private:
	void callBlockManagerToMatch();
    
    cocos2d::FiniteTimeAction* flipToObvSideAction();
    cocos2d::FiniteTimeAction* flipToRevSideAction();

	bool _isFlipToObvSideEnabled;


	
public:
	/**
	* Hint at reverse side
	*/
	void showRevSideHintWithDuration(float duration);			// control by duration

protected:
    virtual void updateForRevSideHint(float dt);
	void updateForLaunchRevSideHint(float dt);
	void updateForFinishRevSideHint(float dt);

    float	_showRevSideHintDuration;

public:
	void showRevSideHint() { _showRevSideHintEnabled = true;  }	// control by user
	void hideRevSideHint() { _showRevSideHintEnabled = false; }	// control by user

protected:
	bool	_showRevSideHintEnabled;



public:
	/**
	* GameBlock
	*/
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    virtual void setFeverModeEnabled(bool enabled);

	virtual cocos2d::FiniteTimeAction* removeSelfAction();



private:
    ReversibleBlock(int type) 
		: GameBlock(type)
		, _isFlipToObvSideEnabled(false)
		, _showRevSideHintDuration(0.f)
		, _showRevSideHintEnabled(false)
    {
    }
};



#endif /* defined(__NewFlipop__GameBlock__) */