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
#include "AnimationReaderForCocos2d.h"

USING_NS_CC;

GameBlocksManager* GameBlocksManager::create(int rows, int cols, int colors)
{
	GameBlocksManager* pRet = new GameBlocksManager();
	if (pRet && pRet->init(rows, cols, colors))
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

bool GameBlocksManager::init(int rows, int cols, int cardTypes)
{
	bool bRet = Node::init();

	srand(time(0));

	_rows = rows;
	_cols = cols;

	for (int i = 0; i != _cols; i++)
	{
		_headBlockOfCol[i] = GameBlock::create();
		_headBlockOfCol[i]->retain();

		_tailBlockOfCol[i] = GameBlock::create();
		_tailBlockOfCol[i]->retain();

		_headBlockOfCol[i]->setAboveBlock(_tailBlockOfCol[i]);
		_tailBlockOfCol[i]->setBelowBlock(_headBlockOfCol[i]);

	}

	setFillingAndFallingEnabled(true);

	auto countManager = GameBlocksCountManager::getInstance();
	countManager->setCardTypes(cardTypes);

	_eventDispatcher->addCustomEventListener( "enabledFallingAndFilling",	[this](Event*){ this->setFillingAndFallingEnabled(true); });
	_eventDispatcher->addCustomEventListener("disabledFallingAndFilling",	[this](Event*){ this->setFillingAndFallingEnabled(false); });

	return bRet;
}

void GameBlocksManager::update(float dt)
{
	// filling first
	updateForFilling(dt);
	updateForFalling(dt);
}

void GameBlocksManager::updateForFilling(float dt)
{
	for (int i = 0; i != _cols; i++)
	{
		int row = 0;

		for (GameBlock * block = _tailBlockOfCol[i]->getBelowBlock()
			; block != _headBlockOfCol[i]
		; block = block->getBelowBlock())
		{
			row++;
		}

		for (int j = 0; j != _rows-row; j++)
		{
			auto block = dynamic_cast<GameCard*>(createBlock());
			pushBlockToCol(i, block);
			block->setFeverModeEnabled(_isFeverModeEnabled);
			block->setShowObvSideEnabled(true);


			Vec2 pos;
			pos.x = BLOCK_WIDTH  * (i-float(_cols-1)/2);
			pos.y = BLOCK_HEIGHT * (j+float(_rows+1)/2);
			pos.y = MAX(pos.y, block->getBelowBlock()->getPositionY()+BLOCK_HEIGHT);
			block->setPosition(pos);
		}
	}
}

void GameBlocksManager::updateForFalling(float dt)
{
	for (int i = 0; i != _cols; i++)
	{
		for (GameBlock* block = _tailBlockOfCol[i]->getBelowBlock()
			; block != _headBlockOfCol[i]
		; block = block->getBelowBlock())
		{
			block->updateForFalling(dt);
		}

		// when the tool at the bottom, activate it.
		GameBlock* bottom = _headBlockOfCol[i]->getAboveBlock();

		if (bottom->isTool() && !bottom->isFalling())
		{
			activateTheTool(dynamic_cast<GameTool*>(bottom));
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

		auto countManager = GameBlocksCountManager::getInstance();
		std::vector<int> existingTypes;

		int totalCount = _cols * _rows;
		for (int i = 0; i != totalCount/2; ++i)
		{
			int type = countManager->getNewCardTypeAtRandom();
			existingTypes.push_back(type);
			existingTypes.push_back(type);
		}

		std::random_shuffle(existingTypes.begin(), existingTypes.end());

		for (auto iter = existingTypes.begin() ; iter != existingTypes.end() ; ++iter)
		{
			countManager->addBlockTypeToPriorityQueue(*iter);
		}

		scheduleUpdate();

		EventCustom event("PauseTimerEvent");
		_eventDispatcher->dispatchEvent(&event);
	}
	else
	{
		unscheduleUpdate();

		auto resumeTimerFunc = [this](float dt) {
			for (int i = 0; i != _cols; ++i)
			{
				for (auto block = _tailBlockOfCol[i]->getBelowBlock()
					; block != _headBlockOfCol[i]
				; block = block->getBelowBlock())
				{
					auto card = dynamic_cast<GameCard*>(block);
					card->hideObvSideColor();
				}
			}

			EventCustom event("ResumeTimerEvent");
			_eventDispatcher->dispatchEvent(&event);
		};

		scheduleOnce(resumeTimerFunc, 2.f, "ResumeTimer");
	}
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

	CCAssert(col >= 0 && col < _cols, "");
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
		if (_1stCard == card)
		{
			return;
		}
		else
		{
			set2ndCard(card);
		}

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

	/* 配对类型 */
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
		EventCustom event("FeverIncreaseEvent");
		_eventDispatcher->dispatchEvent(&event);
	}
}

void GameBlocksManager::matchCardWithCard(GameCard* lCard, GameCard* rCard)
{
	int score = GameData::getInstance()->getScoreDelta();

	Vec2 wPos_1 = this->convertToWorldSpace(lCard->getPosition());
	Vec2 wPos_2 = this->convertToWorldSpace(rCard->getPosition());

	char szData_1[50];
	snprintf(szData_1, 50, "%d,%d,%f,%f", score, lCard->getType(), wPos_1.x, wPos_1.y);
	char szData_2[50];
	snprintf(szData_2, 50, "%d,%d,%f,%f", score, rCard->getType(), wPos_2.x, wPos_2.y);

	EventCustom eventOne("PutScoreEvent");
	eventOne.setUserData(szData_1);
	_eventDispatcher->dispatchEvent(&eventOne);

	EventCustom eventTwo("PutScoreEvent");
	eventTwo.setUserData(szData_2);
	_eventDispatcher->dispatchEvent(&eventTwo);

	lCard->removeFromBlocksManager();
	rCard->removeFromBlocksManager();
}

void GameBlocksManager::matchWildWithWild(GameCard* lWild, GameCard* rWild)
{
	lWild->removeFromBlocksManager();
	rWild->removeFromBlocksManager();

	int score = GameData::getInstance()->getScoreDelta();

	for (int i = 0; i != _cols; ++i)
	{
		for (auto block = _tailBlockOfCol[i]->getBelowBlock()
			; block != _headBlockOfCol[i]
		; block = block->getBelowBlock())
		{
			if (block->isCard())
			{
				dynamic_cast<GameCard*>(block)->showObvSideColorByWildCard();

				// score
				Vec2 wPos = this->convertToWorldSpace(block->getPosition());

				char szData[50];
				snprintf(szData, 50, "%d,%d,%f,%f", score, block->getType(), wPos.x, wPos.y);

				EventCustom event("PutScoreEvent");
				event.setUserData(szData);
				_eventDispatcher->dispatchEvent(&event);
			}
		}
	}
}

void GameBlocksManager::matchWildWithCard(GameCard*  wild, GameCard*  card)
{

	wild->removeFromBlocksManager();
	card->removeFromBlocksManager();

	// score
	int score = GameData::getInstance()->getScoreDelta();

	Vec2 wPos_1 = this->convertToWorldSpace(wild->getPosition());
	Vec2 wPos_2 = this->convertToWorldSpace(card->getPosition());

	char szData_1[50];
	snprintf(szData_1, 50, "%d,%d,%f,%f", score, wild->getType(), wPos_1.x, wPos_1.y);
	char szData_2[50];
	snprintf(szData_2, 50, "%d,%d,%f,%f", score, card->getType(), wPos_2.x, wPos_2.y);

	EventCustom eventOne("PutScoreEvent");
	eventOne.setUserData(szData_1);
	_eventDispatcher->dispatchEvent(&eventOne);

	EventCustom eventTwo("PutScoreEvent");
	eventTwo.setUserData(szData_2);
	_eventDispatcher->dispatchEvent(&eventTwo);

	for (int i = 0; i != _cols; ++i)
	{
		for (auto block = _tailBlockOfCol[i]->getBelowBlock()
			; block != _headBlockOfCol[i]
		; block = block->getBelowBlock())
		{
			if (block->getType() == card->getType() && block != card)
			{
				dynamic_cast<GameCard*>(block)->showObvSideColorByWildCard();

				// score
				Vec2 wPos = this->convertToWorldSpace(block->getPosition());

				char szData[50];
				snprintf(szData, 50, "%d,%d,%.1f,%.1f", score, block->getType(), wPos.x, wPos.y);

				EventCustom event("PutScoreEvent");
				event.setUserData(szData);
				_eventDispatcher->dispatchEvent(&event);
			}
		}
	}
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

	for (int i = 0; i != _cols; i++)
	{
		for (GameBlock * block = _tailBlockOfCol[i]->getBelowBlock()
			; block != _headBlockOfCol[i]
		; block = block->getBelowBlock())
		{
			block->setFeverModeEnabled(isEnabled);
		}
	}
}

bool GameBlocksManager::getFeverModeEnabled(void)
{
	return _isFeverModeEnabled;
}

void GameBlocksManager::activateTheTool(GameTool* tool)
{
	switch (tool->getType()) {
	case TOOL_COIN:
		adjustForActivateTheCoin(tool); break;
	case TOOL_MULT:
		adjustForActivateTheMult(tool); break;
	case TOOL_TIME:
		adjustForActivateTheTime(tool); break;
	default:
		break;
	}

	tool->removeFromBlocksManager();
}

void GameBlocksManager::adjustForActivateTheCoin(GameTool* coin)
{
	auto instance = GameBlocksCountManager::getInstance();

	switch (instance->getClearCountWithType(TOOL_COIN)%3)
	{
	case 0:
		{
			instance->addBlockTypeToPriorityQueue(CARD_WILD);
			instance->addBlockTypeToPriorityQueue(instance->getNewCardTypeInMinimum());
			break;
		}
	case 1:
		{
			instance->addBlockTypeToPriorityQueue(TOOL_MULT);
			instance->addBlockTypeToPriorityQueue(instance->getNewCardTypeInMinimum());
			break;
		}
	case 2:
		{
			instance->addBlockTypeToPriorityQueue(TOOL_TIME);
			instance->addBlockTypeToPriorityQueue(instance->getNewCardTypeInMinimum());
			break;
		}
	default:
		break;
	}

	Vec2 wPos = convertToWorldSpace(coin->getPosition());
	char szData[15];
	snprintf(szData, 15, "%.1f,%.1f", wPos.x, wPos.y);

	EventCustom event("PutCoinsEvent");
	event.setUserData(szData);
	_eventDispatcher->dispatchEvent(&event);
}

void GameBlocksManager::adjustForActivateTheMult(GameTool* mult)
{}

void GameBlocksManager::adjustForActivateTheTime(GameTool* time)
{
	Vec2 wPos = convertToWorldSpace(time->getPosition());
	char szData[15];
	snprintf(szData, 15, "%.1f,%.1f", wPos.x, wPos.y);

	EventCustom event("PutTimerEvent");
	event.setUserData(szData);
	_eventDispatcher->dispatchEvent(&event);
}

void GameBlocksManager::addNewBlocks()
{
	updateForFilling(0);
}

GameBlocksManager::GameBlocksManager(void)
	: _rows(0)
	, _cols(0)
	, _1stCard(nullptr)
	, _2ndCard(nullptr)
	, _isFeverModeEnabled(false)
{
	Animation* animation = nullptr;

	animation = AnimationReaderForCocos2d::readAnimationFromPlist("GameBlock/CoinAnimation.plist", 0.08f, 1, true);
	AnimationCache::getInstance()->addAnimation(animation, COIN_ANIMATION);
	animation = AnimationReaderForCocos2d::readAnimationFromPlist("GameBlock/WildCardObv.plist", 0.08f, 1, true);
	AnimationCache::getInstance()->addAnimation(animation, WILD_OBV_ANIMATION);
	animation = AnimationReaderForCocos2d::readAnimationFromPlist("GameBlock/WildCardRev.plist", 0.08f, 1, true);
	AnimationCache::getInstance()->addAnimation(animation, WILD_REV_ANIMATION);
	animation = AnimationReaderForCocos2d::readAnimationFromPlist("GameBlock/MultiplyAnimation.plist", 0.08f, 1, true);
	AnimationCache::getInstance()->addAnimation(animation, MULT_ANIMATION);

	for (int i = 0; i != BLOCKS_COL_MAX; i++)
	{
		_headBlockOfCol[i] = nullptr;
		_tailBlockOfCol[i] = nullptr;
	}
}

GameBlocksManager::~GameBlocksManager(void)
{
	CC_SAFE_RELEASE(_1stCard);
	CC_SAFE_RELEASE(_2ndCard);

	for (int i = 0; i != BLOCKS_COL_MAX; i++)
	{
		CC_SAFE_RELEASE(_headBlockOfCol[i]);
		CC_SAFE_RELEASE(_tailBlockOfCol[i]);
	}

	auto cache = AnimationCache::getInstance();
	cache->removeAnimation(COIN_ANIMATION);
	cache->removeAnimation(WILD_OBV_ANIMATION);
	cache->removeAnimation(WILD_REV_ANIMATION);
	cache->removeAnimation(MULT_ANIMATION);
}

