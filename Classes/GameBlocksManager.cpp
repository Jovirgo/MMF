//
//  GameBlocksManager.cpp
//  NewFlipop
//
//  Created by OJO on 15/1/2.
//
//

#include "GameBlocksManager.h"
#include "GameBlock.h"
#include "GameBlocksCountManager.h"
#include "GameResources.h"
#include "GameData.h"
#include "GamePlayScene.h"
#include "AnimationReaderForCocos2d.h"

USING_NS_CC;

GameBlocksManager* GameBlocksManager::create(int rows, int cols, const int color[5])
{
	GameBlocksManager* pRet = new GameBlocksManager();
	if (pRet && pRet->init(rows, cols, color))
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

bool GameBlocksManager::init(int rows, int cols, const int color[5])
{
	bool bRet = Node::init();

	srand(time(0));

	CCASSERT(rows > 0 && cols > 0, "");

	_rows = rows;
	_cols = cols;

	for (int i = 0; i != getCols(); i++)
	{
		_headBlockOfCol[i] = GameBlock::create();
		_headBlockOfCol[i]->retain();

		_tailBlockOfCol[i] = GameBlock::create();
		_tailBlockOfCol[i]->retain();

		_headBlockOfCol[i]->setAboveBlock(_tailBlockOfCol[i]);
		_tailBlockOfCol[i]->setBelowBlock(_headBlockOfCol[i]);

	}

	auto countManager = GameBlocksCountManager::getInstance();
	for (int i = 0; i != 5; ++i)
	{
		 countManager->setTypeLaunched(1+i, color[i] == 1);
	}

	_eventDispatcher->addCustomEventListener( EVENT_BeginNextRound,		   [this](Event*){ this->setFillingAndFallingEnabled(true); });
	_eventDispatcher->addCustomEventListener( EVENT_StopFallingAndFilling, [this](Event*){ this->setFillingAndFallingEnabled(false); });

	scheduleUpdate();

	setFillingAndFallingEnabled(true);

	return bRet;
}

void GameBlocksManager::update(float dt)
{
	if (_isFillingAndFalling)
	{
		// filling first
		updateForFilling(dt);
		updateForFalling(dt);
	}
}

void GameBlocksManager::updateForFilling(float dt)
{
	for (int i = 0; i != getCols(); ++i)
	{
		int row = 0;

		for (GameBlock * block = _tailBlockOfCol[i]->getBelowBlock()
			; block != _headBlockOfCol[i]
			; block = block->getBelowBlock())
		{
			row++;
		}

		for (int j = 0; j != getRows()-row; j++)
		{
			auto block = dynamic_cast<GameCard*>(createBlock());
			pushBlockToCol(i, block);
			block->setFeverModeEnabled(_isFeverModeEnabled);
			block->setShowObvSideEnabled(true);
			block->setTouchEnabled(_isTouchEnabled);

			Vec2 pos;
			pos.x = GAMEPLAY_BlockWidth  * (i-float(getCols()-1)/2);
			pos.y = GAMEPLAY_BlockHeight * (j+float(getRows()+1)/2);
			pos.y = MAX(pos.y, block->getBelowBlock()->getPositionY()+GAMEPLAY_BlockHeight);
			block->setPosition(pos);
		}
	}
}

void GameBlocksManager::updateForFalling(float dt)
{
	for (int i = 0; i != getCols(); ++i)
	{
		for (GameBlock* block = _tailBlockOfCol[i]->getBelowBlock()
		; block != _headBlockOfCol[i]
		; block = block->getBelowBlock())
		{
			block->updateForFalling(dt);
		}
	}
}

void GameBlocksManager::setFillingAndFallingEnabled(bool var)
{
	if (_isFillingAndFalling == var)
	{
		return;
	}
	else
	{
		_isFillingAndFalling = var;
	}

	if (_isFillingAndFalling)
	{
		resumeFillingAndFalling();
	}
	else
	{
		pauseFillingAndFalling();
	}
}

void GameBlocksManager::resumeFillingAndFalling()
{
	auto countManager = GameBlocksCountManager::getInstance();
	std::vector<int> blockTypes;

	int totalCount = getCols() * getRows();
	for (int i = 0; i != totalCount/2; ++i)
	{
		int type = countManager->getNewCardTypeAtRandom();
		blockTypes.push_back(type);
		blockTypes.push_back(type);
	}

	std::random_shuffle(blockTypes.begin(), blockTypes.end());

	for (auto iter = blockTypes.begin() ;iter != blockTypes.end() ;++iter)
	{
		countManager->addBlockTypeToPriorityQueue(*iter);
	}

	EventCustom event( EVENT_PauseTimer );
	_eventDispatcher->dispatchEvent(&event);
}

void GameBlocksManager::pauseFillingAndFalling()
{
	auto resumeTimerFunc = [this](float dt) {
		for (int i = 0; i != getCols(); ++i)
		{
			for (auto block = _tailBlockOfCol[i]->getBelowBlock()
				; block != _headBlockOfCol[i]
				; block = block->getBelowBlock())
			{
				dynamic_cast<GameCard*>(block)->hideObvSideColor();
			}
		}

		EventCustom event( EVENT_ResumeTimer );
		_eventDispatcher->dispatchEvent(&event);
	};

	scheduleOnce(resumeTimerFunc, 2.f, "ResumeTimer");
}

GameBlock* GameBlocksManager::createBlock(void)
{
	int type = GameBlocksCountManager::getInstance()->getNewBlockType();

	switch (type)
	{
	case CARD_1: case CARD_2: case CARD_3: 
	case CARD_4: case CARD_5: case CARD_6:
		return GameCard::createWithType(type);
	case CARD_WILD:
		return WildCard::create();
	case TOOL_COIN:
		return ToolCoin::create();
	case TOOL_MULT:
		return ToolMult::create();
	case TOOL_TIME:
		return ToolTime::create();
	default:
		return nullptr;
	}
}

void GameBlocksManager::pushBlockToCol(int col, GameBlock* block)
{
	addChild(block);

	CCAssert(col >= 0 && col < getCols(), "");
	CCAssert(nullptr == block->getBelowBlock() 
		&& nullptr == block->getAboveBlock(), "");

	GameBlock* tail = _tailBlockOfCol[col];

	block->setBelowBlock(tail->getBelowBlock());
	block->setAboveBlock(tail);
	block->getBelowBlock()->setAboveBlock(block);

	tail->setBelowBlock(block);
}

void GameBlocksManager::openTheCard(GameCard* card)
{
	if (_1stCard == nullptr)
	{
		set1stCard(card);
	}
	else
	{
		CCASSERT(_1stCard != card, "");

		set2ndCard(card);

		if (isMatch(_1stCard, _2ndCard))
		{
			pairingSucceed();
		}
		else
		{
			pairingFailed();
		}
	}
}

void GameBlocksManager::showRevSideHintWithDuration(float duration)
{
	for (int i = 0; i != getCols(); ++i)
	{
		for (GameBlock* block = _tailBlockOfCol[i]->getBelowBlock()
		; block != _headBlockOfCol[i]
		; block = block->getBelowBlock())
		{
			dynamic_cast<GameCard*>(block)->showRevSideHintWithDuration(duration);
		}
	}
}

void GameBlocksManager::autoMatchCards(int matchTimes)
{
	int remaining = GameBlocksCountManager::getInstance()->getTotalCountWithRemaining();

	if (remaining < matchTimes * 2)
	{

		GameData* gData = GameData::getInstance();

		for (int i = 0; i != remaining/2; ++i)
		{
			gData->increaseMatch();
			gData->increaseCombo();
		}

		matchWildWithWild(nullptr, nullptr);
		set1stCard(nullptr);
		set2ndCard(nullptr);

		return;
	}

	std::map<GameBlock*, bool> blocksMap;

	for (int i = 0; i != getCols(); ++i)
	{
		for (GameBlock* block = _tailBlockOfCol[i]->getBelowBlock()
		; block != _headBlockOfCol[i]
		; block = block->getBelowBlock())
		{
			blocksMap[block] = false;
		}
	}

	for (int times = 0; times != matchTimes; ++times)
	{
		int col = 1;

		do
		{
			col = getCols() * CCRANDOM_0_1();

		}while (col == getCols());

		// first block
		GameBlock* block1st = nullptr;

		for (int i = 1; i <= getCols(); i++)
		{
			int index = (col-i+getCols()) % getCols();

			auto block = _headBlockOfCol[index]->getAboveBlock();

			for (
				; block != _tailBlockOfCol[index]
				; block = block->getAboveBlock())
			{
				if (block != _tailBlockOfCol[index] && !blocksMap[block])
				{
					blocksMap[block] = true;
					block1st = block;
					break;
				}
			}

			if (block != _tailBlockOfCol[index])
			{
				break;
			}
		}

		// second block
		for (int i = 1; i <= getCols(); i++)
		{
			int index = (col+i) % getCols();

			auto block = _tailBlockOfCol[index]->getBelowBlock();

			for (
				; block != _headBlockOfCol[index]
				; block = block->getBelowBlock())
			{
				if (block != _headBlockOfCol[index] 
				&& block1st->getType() == block->getType()
				&& !blocksMap[block])
				{
					blocksMap[block] = true;
					break;
				}
			}

			if (block != _headBlockOfCol[index])
			{
				break;
			}
		}
	}

	GameData* gData = GameData::getInstance();
	for (int i = 0; i != matchTimes; ++i)
	{
		gData->increaseMatch();
		gData->increaseCombo();
	}

	int score = GameData::getInstance()->getScoreDelta();

	for (auto iter = blocksMap.begin() ; iter != blocksMap.end() ; ++iter)
	{
		if (iter->second)
		{
			auto block = iter->first;

			if (_1stCard == block)
			{
				_1stCard = nullptr;
			}

			if (_2ndCard == block)
			{
				_2ndCard = nullptr;
			}

			dynamic_cast<GameCard*>(block)->showObvSideColorByWildCard();

			// score
			Vec2 wPos = this->convertToWorldSpace(block->getPosition());

			char szData[50];
			snprintf(szData, 50, UDATA_PutScore, score, block->getType(), wPos.x, wPos.y);

			EventCustom event( EVENT_PutScore );
			event.setUserData(szData);
			_eventDispatcher->dispatchEvent(&event);
		}
	}
}

bool GameBlocksManager::isMatch(GameCard const* lCard,
								GameCard const* rCard)
{
	return lCard->getType() == rCard->getType()
		|| lCard->getType() == CARD_WILD
		|| rCard->getType() == CARD_WILD;
}

void GameBlocksManager::pairingSucceed(void)
{
	auto gData = GameData::getInstance();
	gData->increaseCombo();
	gData->increaseMatch();

	if (_1stCard->getType() == CARD_WILD && _2ndCard->getType() == CARD_WILD)
	{
		matchWildWithWild(_1stCard, _2ndCard);
	}
	else if (_1stCard->getType() == CARD_WILD)
	{
		matchWildWithCard(_1stCard, _2ndCard);
	}
	else if (_2ndCard->getType() == CARD_WILD)
	{
		matchWildWithCard(_2ndCard, _1stCard);
	}
	else
	{
		matchCardWithCard(_1stCard, _2ndCard);
	}

	set1stCard(nullptr);
	set2ndCard(nullptr);

	if (!_isFeverModeEnabled && gData->getCombo() > 1)
	{
		EventCustom event( EVENT_FeverIncrease );
		_eventDispatcher->dispatchEvent(&event);
	}
}

void GameBlocksManager::matchCardWithCard(GameCard* lCard, GameCard* rCard)
{
	int score = GameData::getInstance()->getScoreDelta();

	dispatchPutScoreEventWithCard(score, lCard);
	dispatchPutScoreEventWithCard(score, rCard);

	lCard->runAction( lCard->removeSelfAction());
	rCard->runAction( rCard->removeSelfAction());
}

void GameBlocksManager::matchWildWithWild(GameCard* lWild, GameCard* rWild)
{
//	lWild->runAction( lCard->removeSelfAction());
//	rWild->runAction( lCard->removeSelfAction());

	int score = GameData::getInstance()->getScoreDelta();

	for (int i = 0; i != getCols(); ++i)
	{
		for (auto block = _tailBlockOfCol[i]->getBelowBlock()
		; block != _headBlockOfCol[i]
		; block = block->getBelowBlock())
		{
			if (block->isCard())
			{
				auto card = dynamic_cast<GameCard*>(block);
				card->showObvSideColorByWildCard();
				dispatchPutScoreEventWithCard(score, card);
			}
		}
	}
}

void GameBlocksManager::matchWildWithCard(GameCard*  wild, GameCard*  card)
{
	int score = GameData::getInstance()->getScoreDelta();

	dispatchPutScoreEventWithCard(score, wild);
	dispatchPutScoreEventWithCard(score, card);

	wild->runAction( wild->removeSelfAction());
	card->runAction( card->removeSelfAction());

	for (int i = 0; i != getCols(); ++i)
	{
		for (auto block = _tailBlockOfCol[i]->getBelowBlock()
			; block != _headBlockOfCol[i]
			; block = block->getBelowBlock())
		{
			if (block->getType() == card->getType() && block != card)
			{
				auto c = dynamic_cast<GameCard*>(block);
				c->showObvSideColorByWildCard();
				dispatchPutScoreEventWithCard(score, c);
			}
		}
	}
}

void GameBlocksManager::dispatchPutScoreEventWithCard(int score, GameCard* card)
{
	Vec2 wPos = this->convertToWorldSpace(card->getPosition());

	char szData[50];
	snprintf(szData, 50, UDATA_PutScore, score, card->getType(), wPos.x, wPos.y);

	EventCustom event( EVENT_PutScore );
	event.setUserData(szData);
	_eventDispatcher->dispatchEvent(&event);
}

void GameBlocksManager::pairingFailed(void)
{
	GameData::getInstance()->setCombo(0);

	_1stCard->hideObvSideColor();
	_2ndCard->hideObvSideColor();

	set1stCard(nullptr);
	set2ndCard(nullptr);
}

void GameBlocksManager::setFeverModeEnabled(bool isEnabled)
{
	if (_isFeverModeEnabled == isEnabled)
	{
		return;
	}
	else
	{
		_isFeverModeEnabled = isEnabled;
	}

	for (int i = 0; i != getCols(); i++)
	{
		for (GameBlock * block = _tailBlockOfCol[i]->getBelowBlock()
			; block != _headBlockOfCol[i]
			; block = block->getBelowBlock())
		{
			block->setFeverModeEnabled(isEnabled);
		}
	}
}

bool GameBlocksManager::getFeverModeEnabled()
{
	return _isFeverModeEnabled;
}

void GameBlocksManager::setTouchEnabled(bool isEnabled)
{
	if (_isTouchEnabled == isEnabled)
	{
		return;
	}
	else
	{
		_isTouchEnabled = isEnabled;
	}

	for (int i = 0; i != getCols(); ++i)
	{
		for (auto block = _tailBlockOfCol[i]->getBelowBlock()
			; block != _headBlockOfCol[i]
			; block = block->getBelowBlock())
		{
			block->setTouchEnabled(_isTouchEnabled);
		}
	}
}

bool GameBlocksManager::getTouchEnabled(void)
{
	return _isTouchEnabled;
}

GameBlocksManager::GameBlocksManager(void)
	: _isFillingAndFalling(false)
	, _rows(0)
	, _cols(0)
	, _1stCard(nullptr)
	, _2ndCard(nullptr)
	, _isFeverModeEnabled(false)
	, _isTouchEnabled(false)
{
/*
	Animation* animation = nullptr;
	auto cache = AnimationCache::getInstance();

	animation = AnimationReaderForCocos2d::readAnimationFromPlist( GAMEPlAY_CoinPlist, 0.08f, 1, true);
	cache->addAnimation(animation, ANI_Coin);
	animation = AnimationReaderForCocos2d::readAnimationFromPlist( GAMEPLAY_WildObvPlist, 0.08f, 1, true);
	cache->addAnimation(animation, ANI_WildObv);
	animation = AnimationReaderForCocos2d::readAnimationFromPlist( GAMEPLAY_WildRevPlist, 0.08f, 1, true);
	cache->addAnimation(animation, ANI_WildRev);
	animation = AnimationReaderForCocos2d::readAnimationFromPlist( GAMEPLAY_MultPlist, 0.08f, 1, true);
	cache->addAnimation(animation, ANI_Mult);
*/
	for (int i = 0; i != GAMEPLAY_BlocksColMax; i++)
	{
		_headBlockOfCol[i] = nullptr;
		_tailBlockOfCol[i] = nullptr;
	}
}

GameBlocksManager::~GameBlocksManager(void)
{
	CC_SAFE_RELEASE(_1stCard);
	CC_SAFE_RELEASE(_2ndCard);

	for (int i = 0; i != GAMEPLAY_BlocksColMax; i++)
	{
		CC_SAFE_RELEASE(_headBlockOfCol[i]);
		CC_SAFE_RELEASE(_tailBlockOfCol[i]);
	}
/*
	auto cache = AnimationCache::getInstance();
	cache->removeAnimation(ANI_Coin);
	cache->removeAnimation(ANI_WildObv);
	cache->removeAnimation(ANI_WildRev);
	cache->removeAnimation(ANI_Mult);
*/
	_eventDispatcher->removeCustomEventListeners( EVENT_BeginNextRound );
	_eventDispatcher->removeCustomEventListeners( EVENT_StopFallingAndFilling );
}


