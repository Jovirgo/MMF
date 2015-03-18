//
//  GameBlock.h
//  NewFlipop
//
//  Created by 欧洲 on 14/12/31.
//
//

#ifndef __NewFlipop__GameBlock__
#define __NewFlipop__GameBlock__

#include "cocos2d.h"
#include "GameConstant.h"

//---------------------------------
//
// GameBlock
//
//---------------------------------

// Block type
enum
{
    TYPE_NULL = 0,
    CARD_1,
    CARD_2,
    CARD_3,
    CARD_4,
    CARD_5,
    CARD_6,
//    CARD_7,
    CARD_WILD,
    TOOL_COIN,
    TOOL_TIME,
    TOOL_MULT,
    TYPE_TOTAL,
};

bool IsGameCardType(int type);
bool IsGameToolType(int type);


class GameBlock: public cocos2d::Sprite
{
public:
    static GameBlock* create(void);
    
    virtual bool initWithFile(const char* szFileName);
    virtual bool initWithSpriteFrameName(const char* szFrameName);
    
    void  initGameBlock(void);
    
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
//    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
//    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
public:
    void updateForFalling(float dt);
    
    bool isFalling(void) { return _isFalling; }
    
protected:
    virtual void didFinishFalling(void);
    
    bool _isFalling;
    
public:
    bool isTool(void) { return IsGameToolType(_type); }
    bool isCard(void) { return IsGameCardType(_type); }
    
    int getType(void) const
    {
        return _type;
    }
    
protected:
    int _type;
    
public:
    void removeFromBlocksManager(void);
    
    CC_SYNTHESIZE_RETAIN(GameBlock*, _aboveBlock, AboveBlock);
    CC_SYNTHESIZE_RETAIN(GameBlock*, _belowBlock, BelowBlock);
    
public:
    virtual void setFeverModeEnabled(bool enabled);
    
protected:
    GameBlock(int type);
    
    virtual ~GameBlock(void);
};






//---------------------------------
//
// GameCard
//
//---------------------------------
class GameCard: public GameBlock
{
public:
    static GameCard* createWithType(int type);
    
    virtual bool init(void);
    
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
//    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
//    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    virtual void setFeverModeEnabled(bool enabled);
    
protected:
//    virtual void didFinishFalling(void);
    
    bool _isHitBelowBlock;
    
public:
    void showObvSideColor();
    void hideObvSideColor();
    
    void showObvSideColorByWildCard();
    
protected:
    void didShowObvSideColorFinish(void);
    
    cocos2d::FiniteTimeAction* showObvSideAction();
    cocos2d::FiniteTimeAction* hideObvSideAction();
        
    void ShowObvSideEnabledTrue(void);
    void ShowObvSideEnabledFalse(void);
    
    CC_PROPERTY(bool, _isShowObvSideEnabled, ShowObvSideEnabled);
    
public:
    void showRevSideHint(void);
    void hideRevSideHint(void);
    
protected:
    CC_SYNTHESIZE(bool, _isShowRevHintEnabled, ShowRevHintEnabled);
    
protected:
    virtual void updateForRevSideHint(float dt);
    
    float _revHintEnabledTimer;   // use when hit below block
    
protected:
    GameCard(int type) : GameBlock(type)
    , _isHitBelowBlock(false)
    , _isShowObvSideEnabled(false)
    , _isShowRevHintEnabled(false)
    , _revHintEnabledTimer(0.f)
    {
        CCASSERT(isCard(), "");
    }
};




//---------------------------------
//
// WildCard
//
//---------------------------------
class WildCard: public GameCard
{
public:
    CREATE_FUNC(WildCard);
    
    virtual bool init(void);
    
    virtual void setShowObvSideEnabled(bool var);
    
    virtual void updateForRevSideHint(float dt) {}
    
protected:
    WildCard(void) : GameCard(CARD_WILD){}
};




//---------------------------------
//
// GameTool
//
//---------------------------------
class GameTool: public GameBlock
{
protected:
    GameTool(int type) : GameBlock(type) { CCASSERT(isTool(), ""); }
};




//---------------------------------
//
// ToolCoin
//
//---------------------------------
class ToolCoin: public GameTool
{
public:
    CREATE_FUNC(ToolCoin);
    
    virtual bool init(void);
    
protected:
    ToolCoin(void)
    : GameTool(TOOL_COIN)
    {}
};





//---------------------------------
//
// ToolMult
//
//---------------------------------
class ToolMult: public GameTool
{
public:
    CREATE_FUNC(ToolMult);
    
    virtual bool init(void);
    
protected:
    ToolMult(void)
    : GameTool(TOOL_MULT)
    {}
};





//---------------------------------
//
// ToolTime
//
//---------------------------------
class ToolTime: public GameTool
{
public:
    CREATE_FUNC(ToolTime);
    
    virtual bool init(void);
    
protected:
    ToolTime(void)
    : GameTool(TOOL_TIME)
    {}
};

#endif /* defined(__NewFlipop__GameBlock__) */
