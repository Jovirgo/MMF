//
//  GameBlock.cpp
//  NewFlipop
//
//  Created by OJO on 14/12/31.
//
//

#include "GameBlock.h"
#include "GameBlocksManager.h"
#include "GameBlocksCountManager.h"
#include "GameResources.h"

USING_NS_CC;

bool IsGameCardType(int type)
{
    return type > TYPE_NULL && type <= CARD_WILD;
}

bool IsGameToolType(int type)
{
    return type > CARD_WILD && type < TYPE_TOTAL;
}


//---------------------------------
//
// GameBlock
//
//---------------------------------


GameBlock* GameBlock::create(void)
{
    GameBlock* pRet = new GameBlock(TYPE_NULL);
    
    if (pRet && pRet->initWithFile(GAMEPLAY_CardRev))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return pRet;
    }
}

bool GameBlock::initWithFile(const char* szFileName)
{
    bool bRet = Sprite::initWithFile(szFileName);
    
    initGameBlock();
    
    return bRet;
}

bool GameBlock::initWithSpriteFrameName(const char* szFrameName)
{
    bool bRet = Sprite::initWithSpriteFrameName(szFrameName);
    
    initGameBlock();
    
    return bRet;
}

void GameBlock::initGameBlock(void)
{
    setVisible(false);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [this](Touch* touch, Event* event)
    {
		if (!_isTouchEnabled || _isPaired)
		{
			return false;
		}

        return this->onTouchBegan(touch, event);
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool GameBlock::onTouchBegan(Touch* touch, Event* event)
{
    return false;
}

void GameBlock::updateForFalling(float dt)
{
    float rows = dynamic_cast<GameBlocksManager*>(_parent)->getRows();
    
    float endPosY = -GAMEPLAY_BlockHeight * float(rows+1)/2;
    
    for (GameBlock* block = this
         ; block->getBelowBlock()
         ; block = block->getBelowBlock())
    {
        endPosY += GAMEPLAY_BlockHeight;
    }
    
    
    float newPosY = _position.y - 17.f;     // speed = 17.f
    
    if (newPosY < endPosY)
    {
        if (_isFalling)
        {
            _isFalling = false;
            
            setPositionY(endPosY);
            
            didFinishFalling();
        }
    }
    else
    {
        _isFalling = true;
        
        setPositionY(newPosY);
    }
    
    setVisible(_position.y <= GAMEPLAY_BlockHeight * float(rows)/2);
}

void GameBlock::didFinishFalling(void)
{
    runAction(CCSequence::create(ScaleTo::create(0.04f, 1.15f, 0.85f),
                                 ScaleTo::create(0.08f, 0.90f, 1.10f),
                                 ScaleTo::create(0.08f, 1.f),
                                 nullptr));
	
	EventCustom event( EVENT_StopFallingAndFilling );
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

FiniteTimeAction* GameBlock::removeSelfAction()
{
	return nullptr;
}

void GameBlock::removeFromBlocksManager(void)
{
    if (_belowBlock)
    {
        _belowBlock->setAboveBlock(_aboveBlock);
    }
    
    if (_aboveBlock)
    {
        _aboveBlock->setBelowBlock(_belowBlock);
    }
    
    this->removeFromParent();
}

void GameBlock::setFeverModeEnabled(bool enabled)
{
}

GameBlock::GameBlock(int type)
: _isFalling(false)
, _type(type)
, _aboveBlock(nullptr)
, _belowBlock(nullptr)
, _isTouchEnabled(false)
, _isPaired(false)
{
    GameBlocksCountManager::getInstance()->increaseTotalCountWithType(_type);
}

GameBlock::~GameBlock(void)
{
    GameBlocksCountManager::getInstance()->increaseClearCountWithType(_type);
    
    CC_SAFE_RELEASE(_aboveBlock);
    CC_SAFE_RELEASE(_belowBlock);
}


//---------------------------------
//
// GameCard
//
//---------------------------------
GameCard* GameCard::createWithType(int type)
{
    auto pRet = new GameCard(type);
    
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool GameCard::init(void)
{
    bool bRet = GameBlock::initWithFile( GAMEPLAY_CardRev );
    
    Sprite* revSideSprite = nullptr;
    
    switch (_type) {
        case CARD_1: revSideSprite = Sprite::create( GAMEPLAY_CardRev1 ); break;
        case CARD_2: revSideSprite = Sprite::create( GAMEPLAY_CardRev2 ); break;
        case CARD_3: revSideSprite = Sprite::create( GAMEPLAY_CardRev3 ); break;
        case CARD_4: revSideSprite = Sprite::create( GAMEPLAY_CardRev4 ); break;
        case CARD_5: revSideSprite = Sprite::create( GAMEPLAY_CardRev5 ); break;
        case CARD_6: revSideSprite = Sprite::create (GAMEPLAY_CardRev6 ); break;
        default:
            CCAssert(false, "invalid type");
            break;
    }
    
    auto revSideHint = ProgressTimer::create(revSideSprite);
    addChild(revSideHint);
    
    revSideHint->setPosition(Vec2(_contentSize.width /2,
                                  _contentSize.height/2));
    revSideHint->setName("RevSideHint");
    revSideHint->setType(ProgressTimer::Type::BAR);
    revSideHint->setMidpoint(Vec2::ZERO);
    revSideHint->setBarChangeRate(Vec2(0.f, 1.f));
    revSideHint->setPercentage(0.f);
    
    schedule( schedule_selector(GameCard::updateForRevSideHint));
    
    return bRet;
}

bool GameCard::onTouchBegan(Touch* touch, Event* event)
{
    Vec2 touchPoint = this->convertToNodeSpaceAR( touch->getLocation());
    
    Rect myRect = Rect(-_anchorPoint.x * GAMEPLAY_BlockWidth,
                       -_anchorPoint.y * GAMEPLAY_BlockHeight,
                       GAMEPLAY_BlockWidth,
                       GAMEPLAY_BlockHeight);
    
    if (myRect.containsPoint(touchPoint))
    {
        if (!getShowObvSideEnabled())
        {
            showObvSideColor();
        }
    }
    
    return false;
}

void GameCard::setFeverModeEnabled(bool enabled)
{
    if (enabled)
    {
        showRevSideHint();
    }
    else
    {
        hideRevSideHint();
    }
}

FiniteTimeAction* GameCard::removeSelfAction()
{
	float actionDuration = 0.4f;

	auto effectCallfunc = [=](){
		auto emitter = ParticleSystemQuad::create( GAMEPLAY_CardParticle );
		emitter->setPosition(_position);
		emitter->setAutoRemoveOnFinish(true);
		getParent()->addChild(emitter, 5);


		ccBlendFunc blendFunc = {GL_ONE, GL_ONE};

		CCSprite * lights = Sprite::create( GAMEPLAY_CardEffect );
		lights->setPosition(_position);
		lights->setBlendFunc(blendFunc);
		getParent()->addChild(lights, 5);

		auto actionOfEffect = Sequence::create( Spawn::create(ScaleTo::create(actionDuration/2, 1.8f),
															  FadeOut::create(actionDuration/2),
															  nullptr),
												RemoveSelf::create(),
												nullptr);

		lights->runAction(actionOfEffect);
	};

	return Sequence::create( CallFunc::create([this](){ setIsPaired(true); }),
							 CallFunc::create(effectCallfunc),
							 DelayTime::create(0.1f),
							 Hide::create(),
							 DelayTime::create(actionDuration-0.1f),
							 CallFunc::create(CC_CALLBACK_0(GameBlock::removeFromBlocksManager, this)),
							 nullptr);
}

FiniteTimeAction* GameCard::showObvSideAction()
{
	if (_isShowObvSideEnabled)
	{
		return DelayTime::create(0.1f);
	}
	else
	{
		return Sequence::create(OrbitCamera::create(0.05f, 1, 0,  0, 90, 0, 0),
								CallFunc::create(CC_CALLBACK_0(GameCard::ShowObvSideEnabledTrue, this)),
								OrbitCamera::create(0.05f, 1, 0, 90, 90, 0, 0),
								nullptr);
	}
}

FiniteTimeAction* GameCard::hideObvSideAction()
{
	if (_isShowObvSideEnabled)
	{
		return Sequence::create(OrbitCamera::create(0.05f, 1, 0, 180, -90, 0, 0),
								CallFunc::create(CC_CALLBACK_0(GameCard::ShowObvSideEnabledFalse, this)),
								OrbitCamera::create(0.05f, 1, 0,  90, -90, 0, 0),
								nullptr);
	}
	else
	{
		return DelayTime::create(0.1f);
	}
}

void GameCard::showObvSideColor()
{
    auto action = Sequence::create(showObvSideAction(),
                                   CallFunc::create(CC_CALLBACK_0(GameCard::didShowObvSideColorFinish, this)),
                                   nullptr);
    runAction(action);
}

void GameCard::hideObvSideColor()
{
    runAction(hideObvSideAction());
}

void GameCard::showObvSideColorByWildCard()
{
    auto action = Sequence::create(showObvSideAction(),
                                   removeSelfAction(),
                                   nullptr);
    runAction(action);
}

void GameCard::didShowObvSideColorFinish()
{
    dynamic_cast<GameBlocksManager*>(_parent)->openTheCard(this);
}

void GameCard::ShowObvSideEnabledFalse(void)
{
    setShowObvSideEnabled(false);
}

void GameCard::ShowObvSideEnabledTrue(void)
{
    setShowObvSideEnabled(true);
}

bool GameCard::getShowObvSideEnabled(void)
{
    return _isShowObvSideEnabled;
}

void GameCard::setShowObvSideEnabled(bool var)
{
    if (_isShowObvSideEnabled == var)
    {
        return;
    }
    else
    {
        _isShowObvSideEnabled = var;
    }
    
    if (_isShowObvSideEnabled)
    {
        switch (_type) {
            case CARD_1: setTexture( GAMEPLAY_CardObv1 ); break;
            case CARD_2: setTexture( GAMEPLAY_CardObv2 ); break;
            case CARD_3: setTexture( GAMEPLAY_CardObv3 ); break;
            case CARD_4: setTexture( GAMEPLAY_CardObv4 ); break;
            case CARD_5: setTexture( GAMEPLAY_CardObv5 ); break;
            case CARD_6: setTexture( GAMEPLAY_CardObv6 ); break;
            default: break;
        }
    }
    else
    {
        setTexture( GAMEPLAY_CardRev );
    }

	/*
	 * change the camera angle
	 * ...
	 */
    
    getChildByName("RevSideHint")->setVisible( !_isShowObvSideEnabled);
}
void GameCard::showRevSideHint(void)
{
    setShowRevHintEnabled(true);
}

void GameCard::hideRevSideHint(void)
{
    GameBlocksManager * manager = dynamic_cast<GameBlocksManager*>(_parent);
    
    if (!manager->getFeverModeEnabled())
    {
        setShowRevHintEnabled(false);
    }
}

void GameCard::showRevSideHintWithDuration(float duration)
{
	_showRevSideHintDuration = duration;
	_isShowRevSideHint = true;
        
    if (!_isShowObvSideEnabled && !_isShowRevHintEnabled)
    {
		showRevSideHint();
	}
}

void GameCard::updateForRevSideHint(float dt)
{
    const float delta = 10.f;
    
    auto revSideHint = dynamic_cast<ProgressTimer *>(getChildByName("RevSideHint"));
    
    if (_isShowRevHintEnabled)
    {
        float percentage = revSideHint->getPercentage() + delta;
        float percentMax = 100.f;
        
        if (percentage > percentMax)
        {
            revSideHint->setPercentage(percentMax);
        }
        else
        {
            revSideHint->setPercentage(percentage);
        }
    }
    else
    {
        float percentage = revSideHint->getPercentage() - delta;
        float percentMin = 0.f;
        
        if (percentage < percentMin)
        {
            revSideHint->setPercentage(percentMin);
        }
        else
        {
            revSideHint->setPercentage(percentage);
        }
    }
    
    if (_isShowRevSideHint)
    {
        if (_showRevSideHintDuration < 0.f)
        {
            _showRevSideHintDuration = 0.f;
            
            hideRevSideHint();
            
            _isShowRevSideHint = false;
        }
        else
        {
            _showRevSideHintDuration -= dt;
        }
    }
}




//---------------------------------
//
// WildCard
//
//---------------------------------
bool WildCard::init(void)
{
    bool bRet = GameBlock::initWithSpriteFrameName("WildCardObv_000");
    
    // obv side
    auto obvAni = AnimationCache::getInstance()->getAnimation( ANI_WildObv );
    
    runAction( RepeatForever::create( Animate::create(obvAni)));
    
    // rev side
    auto revSideHint = Sprite::create(GAMEPLAY_CardRev);
    revSideHint->setPosition(Vec2(_contentSize.width /2,
                                  _contentSize.height/2));
    revSideHint->setName("RevSideHint");
    revSideHint->setVisible(true);
    addChild(revSideHint);
    
    auto revAni = AnimationCache::getInstance()->getAnimation( ANI_WildRev );
    
    revSideHint->runAction( RepeatForever::create( Animate::create(revAni)));
    
    return bRet;
}

void WildCard::setShowObvSideEnabled(bool var)
{
    if (_isShowObvSideEnabled == var)
    {
        return;
    }
    else
    {
        _isShowObvSideEnabled = var;
    }
    
    getChildByName("RevSideHint")->setVisible( !_isShowObvSideEnabled);
}





//---------------------------------
//
// ToolCoin
//
//---------------------------------
bool ToolCoin::init(void)
{
    bool bRet = GameBlock::initWithSpriteFrameName("CoinAnimation_000");
    
    auto animation = AnimationCache::getInstance()->getAnimation( ANI_Coin );
    
    auto seq = Sequence::create(Animate::create(animation), DelayTime::create(1.f), NULL);
    runAction( RepeatForever::create(dynamic_cast<ActionInterval *>(seq)));
    
    return bRet;
}





//---------------------------------
//
// ToolTime
//
//---------------------------------
bool ToolTime::init(void)
{
    bool bRet = GameBlock::initWithFile( GAMETOOL_Time0 );
    
    auto hand = CCSprite::create( GAMETOOL_Time1 );
    hand->setName("TimeHand");
    hand->setPosition( Vec2(getContentSize().width /2,
                            getContentSize().height/2 + 5));
    addChild(hand);
    hand->runAction( RepeatForever::create(CCRotateBy::create(2.f, -360.f)));
    
    return bRet;
}





//---------------------------------
//
// ToolMult
//
//---------------------------------
bool ToolMult::init(void)
{
    bool bRet = GameBlock::initWithSpriteFrameName("MultiplyAnimation_000");
    
//    char szFileName[30];
//    sprintf(szFileName, "Block/x%d.png", GameInstance::sharedGameInstance()->nUnitMulti + 1);
//    
//    m_obText = CCSprite::create(fileName);
//    m_obText->setPosition(ccp(getContentSize().width /2,
//                              getContentSize().height/2));
//    addChild(m_obText);
    
    auto animation = AnimationCache::getInstance()->getAnimation( ANI_Mult );
    
    runAction( RepeatForever::create(Animate::create(animation)));

    return bRet;
}
























