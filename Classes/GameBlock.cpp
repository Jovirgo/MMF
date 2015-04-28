#include "GameBlock.h"
#include "GameResources.h"
#include "GameConstant.h"
#include "GameBlockManager.h"
#include "GameBlocksCountManager.h"

USING_NS_CC;

//---------------------------------
//
// GameBlock
//
//---------------------------------
GameBlock* GameBlock::create()
{
	GameBlock* pRet = new GameBlock(GB_NULL);

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

void GameBlock::initGameBlock(void)
{
    setVisible(false);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [this](Touch* touch, Event* event)
    {
		if (_isTouchEnabled && _isActive)
		{
			return this->onTouchBegan(touch, event);
		}
		
		return false;
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool GameBlock::onTouchBegan(Touch* touch, Event* event)
{
    return false;
}

void GameBlock::removeFromBlockManager(void)
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

void GameBlock::updateForFalling(float dt)
{
	float delta = GAMEPLAY_BlockFallingSpeed * dt;

    float endPosY = GAMEPLAY_BlockHeight * getRowByRelation();
    float newPosY = _position.y - delta;

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
	
    int rows = dynamic_cast<GameBlockManager*>(_parent)->getRows();
    setVisible(_position.y <= GAMEPLAY_BlockHeight * (rows + 0.5f));
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

void GameBlock::setActive(bool var)
{
	_isActive = var;
}

int GameBlock::getRowByPosition()
{
	return int(_position.y / GAMEPLAY_BlockHeight);
}

int GameBlock::getRowByRelation()
{
	int row = -1;
	for (auto block = this; block->getBelowBlock(); block = block->getBelowBlock())
	{
		++row;
	}
	return row;
}

void GameBlock::setFeverModeEnabled(bool enabled)
{
}

cocos2d::FiniteTimeAction* GameBlock::removeSelfAction()
{
	return nullptr;
}

GameBlock::GameBlock(int type) 
	: _type(type)
	, _aboveBlock(nullptr)
	, _belowBlock(nullptr)
	, _isFalling(false)
	, _isActive(false)
	, _isTouchEnabled(false)
{
	setActive(true);
	GameBlocksCountManager::getInstance()->increaseTotalCountWithType(_type);
}

GameBlock::~GameBlock(void)
{
//	setActive(false);
	GameBlocksCountManager::getInstance()->increaseClearCountWithType(_type);

    CC_SAFE_RELEASE(_aboveBlock);
    CC_SAFE_RELEASE(_belowBlock);
}








//---------------------------------
//
// ReversibleBlock
//
//---------------------------------
ReversibleBlock* ReversibleBlock::createWithType(int type)
{
    auto pRet = new ReversibleBlock(type);
    
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

bool ReversibleBlock::init(void)
{
    bool bRet = GameBlock::initWithFile( GAMEPLAY_CardRev );
    
    Sprite* revSideSprite = nullptr;
    
    switch (getType()) {
        case GB_Rvs1: revSideSprite = Sprite::create( GAMEPLAY_CardRev1 ); break;
        case GB_Rvs2: revSideSprite = Sprite::create( GAMEPLAY_CardRev2 ); break;
        case GB_Rvs3: revSideSprite = Sprite::create( GAMEPLAY_CardRev3 ); break;
        case GB_Rvs4: revSideSprite = Sprite::create( GAMEPLAY_CardRev4 ); break;
        case GB_Rvs5: revSideSprite = Sprite::create( GAMEPLAY_CardRev5 ); break;
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
    
    schedule( schedule_selector(ReversibleBlock::updateForRevSideHint));
    
    return bRet;
}

void ReversibleBlock::flipToObvSideAndRemoveSelf()
{
    auto action = Sequence::create(flipToObvSideAction(), removeSelfAction(), nullptr);
    runAction(action);
}

void ReversibleBlock::flipToObvSide()
{
    auto action = Sequence::create(flipToObvSideAction(),
                                   CallFunc::create(CC_CALLBACK_0(ReversibleBlock::callBlockManagerToMatch, this)),
                                   nullptr);
    runAction(action);
}

void ReversibleBlock::flipToRevSide()
{
    runAction( flipToRevSideAction() );
}


FiniteTimeAction* ReversibleBlock::flipToObvSideAction()
{
	if (_isFlipToObvSideEnabled)
	{
		return DelayTime::create(GAMEPLAY_BlockFlipDt);
	}
	else
	{
		return Sequence::create(OrbitCamera::create(GAMEPLAY_BlockFlipDt/2, 1, 0,  0, 90, 0, 0),
								CallFunc::create([this](){ setFlipToObvSideEnabled(true); }),
								OrbitCamera::create(GAMEPLAY_BlockFlipDt/2, 1, 0, 90, 90, 0, 0),
								nullptr);
	}
}

FiniteTimeAction* ReversibleBlock::flipToRevSideAction()
{
	if (_isFlipToObvSideEnabled)
	{
		return Sequence::create(OrbitCamera::create(GAMEPLAY_BlockFlipDt/2, 1, 0, 180, -90, 0, 0),
								CallFunc::create([this](){ setFlipToObvSideEnabled(false); }),
								OrbitCamera::create(GAMEPLAY_BlockFlipDt/2, 1, 0,  90, -90, 0, 0),
								nullptr);
	}
	else
	{
		return DelayTime::create(GAMEPLAY_BlockFlipDt);
	}
}
void ReversibleBlock::setFlipToObvSideEnabled(bool var)
{
    if (_isFlipToObvSideEnabled == var)
    {
        return;
    }
    else
    {
        _isFlipToObvSideEnabled = var;
    }
    
    if (_isFlipToObvSideEnabled)
    {
        switch (getType()) {
            case GB_Rvs1: setTexture( GAMEPLAY_CardObv1 ); break;
            case GB_Rvs2: setTexture( GAMEPLAY_CardObv2 ); break;
            case GB_Rvs3: setTexture( GAMEPLAY_CardObv3 ); break;
            case GB_Rvs4: setTexture( GAMEPLAY_CardObv4 ); break;
            case GB_Rvs5: setTexture( GAMEPLAY_CardObv5 ); break;
			default:
				CCAssert(false, "invalid type");
				break;
        }
    }
    else
    {
        setTexture( GAMEPLAY_CardRev );
    }

	// change the camera angle: ...
    
    getChildByName("RevSideHint")->setVisible( !_isFlipToObvSideEnabled);
}

void ReversibleBlock::callBlockManagerToMatch()
{
	dynamic_cast<GameBlockManager*>(_parent)->matchingReversibleBlock(this);
}

void ReversibleBlock::showRevSideHintWithDuration(float duration)
{
	if (_showRevSideHintDuration < duration)
	{
		_showRevSideHintDuration = duration;
	}
}

void ReversibleBlock::updateForRevSideHint(float dt)
{
    if (_showRevSideHintEnabled || _showRevSideHintDuration > 0.f)
    {
        updateForLaunchRevSideHint(dt);
    }
    else
    {
		updateForFinishRevSideHint(dt);
    }
    
	if (_showRevSideHintDuration < 0.f)
	{
		_showRevSideHintDuration = 0.f;
	}
	else
	{
		_showRevSideHintDuration -= dt;
	}
}

void ReversibleBlock::updateForLaunchRevSideHint(float dt)
{
	auto revSideHint = dynamic_cast<ProgressTimer *>(getChildByName("RevSideHint"));

	float percentage = revSideHint->getPercentage() + GAMEPLAY_BlockRevHintSpeed * dt;
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

void ReversibleBlock::updateForFinishRevSideHint(float dt)
{
	auto revSideHint = dynamic_cast<ProgressTimer *>(getChildByName("RevSideHint"));

	float percentage = revSideHint->getPercentage() - GAMEPLAY_BlockRevHintSpeed * dt;
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

bool ReversibleBlock::onTouchBegan(Touch* touch, Event* event)
{
    Vec2 touchPoint = this->convertToNodeSpaceAR( touch->getLocation());
    
    Rect myRect = Rect(-_anchorPoint.x * GAMEPLAY_BlockWidth,
                       -_anchorPoint.y * GAMEPLAY_BlockHeight,
                       GAMEPLAY_BlockWidth,
                       GAMEPLAY_BlockHeight);
    
    if (myRect.containsPoint(touchPoint))
    {
        if (!_isFlipToObvSideEnabled)	// BUG
        {
            flipToObvSide();
        }
    }
    
    return false;
}

void ReversibleBlock::setFeverModeEnabled(bool enabled)
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

FiniteTimeAction* ReversibleBlock::removeSelfAction()
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

	return Sequence::create( CallFunc::create(effectCallfunc),
							 DelayTime::create(0.1f),
							 Hide::create(),
							 DelayTime::create(actionDuration-0.1f),
							 CallFunc::create(CC_CALLBACK_0(GameBlock::removeFromBlockManager, this)),
							 nullptr);
}
