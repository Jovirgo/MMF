#include "GameBlockManager.h"
#include "GameResources.h"
#include "GameSettings.h"
#include "GameBlocksCountManager.h"

USING_NS_CC;

bool GameBlockManager::init(int rows, int cols)
{
	bool bRet = Node::init();

	srand(time(0));

	CCASSERT(rows > 0 && cols > 0, "");

	_rows = rows;
	_cols = cols;

	for (int i = 0; i != getCols(); i++)
	{
		Vec2 pos;
		pos.x = GAMEPLAY_BlockWidth * (i+0.5f);
		pos.y = GAMEPLAY_BlockHeight * (-0.5f);

		_headBlockOfCol[i] = GameBlock::create();
		_headBlockOfCol[i]->retain();
		_headBlockOfCol[i]->setPosition(pos);

		Vec2 delta = Vec2(0, GAMEPLAY_BlockHeight * (rows+1));

		_tailBlockOfCol[i] = GameBlock::create();
		_tailBlockOfCol[i]->retain();
		_tailBlockOfCol[i]->setPosition(pos + delta);

		_headBlockOfCol[i]->setAboveBlock(_tailBlockOfCol[i]);
		_tailBlockOfCol[i]->setBelowBlock(_headBlockOfCol[i]);
	}

	scheduleUpdate();

	return bRet;
}

void GameBlockManager::update(float dt)
{
	updateForFilling(dt);
	updateForFalling(dt);
}

void GameBlockManager::matchingReversibleBlock(ReversibleBlock* rBlock)
{
	if (isSpecifiedMode())
	{
		matchingInSpecifiedMode(rBlock);
	}
	else
	{
		matchingInPairMode(rBlock);
	}

}

void GameBlockManager::matchingInSpecifiedMode(ReversibleBlock* rBlock)
{
	if (isSpecifiedType( rBlock->getType() ))
	{
		matchSpecifiedTypeCorret(rBlock);
	}
	else
	{
		matchSpecifiedTypeIncorret(rBlock);
	}
}

void GameBlockManager::matchSpecifiedTypeCorret(ReversibleBlock* rBlock)
{
	rBlock->setActive(false);
	rBlock->runAction( Sequence::create(rBlock->removeSelfAction(),
										CallFunc::create( CC_CALLBACK_0(GameBlock::removeFromBlockManager, rBlock)),
										nullptr));
}

void GameBlockManager::matchSpecifiedTypeIncorret(ReversibleBlock* rBlock)
{
	rBlock->flipToRevSide();
}


void GameBlockManager::addSpecifiedType(int type)
{
	_specifiedTypeSet.insert( type );
}

void GameBlockManager::removeSpecifiedType(int type)
{
	_specifiedTypeSet.erase( type );
}

bool GameBlockManager::isSpecifiedType(int type)
{
	return _specifiedTypeSet.count( type ) != 0;
}

void GameBlockManager::matchingInPairMode(ReversibleBlock* rBlock)
{
	if (_1stPairBlock == nullptr)
	{
		set1stPairBlock(rBlock);
	}
	else
	{
		CCASSERT(_1stPairBlock != rBlock, "");

		set2ndPairBlock(rBlock);

		if (_1stPairBlock->getType() == _2ndPairBlock->getType())
		{
			matchPairCorret();
			GameSettings::getInstance()->playSound( GAMEPLAY_CorrectMp3 );
		}
		else
		{
			matchPairIncorret();
			GameSettings::getInstance()->playSound( GAMEPLAY_IncorrectMp3 );
		}

		set1stPairBlock(nullptr);
		set2ndPairBlock(nullptr);
	}
}

void GameBlockManager::matchPairCorret()
{
	_1stPairBlock->setActive(false);
	_1stPairBlock->runAction( Sequence::create(_1stPairBlock->removeSelfAction(),
												CallFunc::create( CC_CALLBACK_0(GameBlock::removeFromBlockManager, _1stPairBlock)),
												nullptr));

	_2ndPairBlock->setActive(false);
	_2ndPairBlock->runAction( Sequence::create(_2ndPairBlock->removeSelfAction(),
												CallFunc::create( CC_CALLBACK_0(GameBlock::removeFromBlockManager, _2ndPairBlock)),
												nullptr));
	
	_eventDispatcher->dispatchCustomEvent( EVENT_PairCorrect );
}

void GameBlockManager::matchPairIncorret()
{
	_1stPairBlock->flipToRevSide();
	_2ndPairBlock->flipToRevSide();

	_eventDispatcher->dispatchCustomEvent( EVENT_PairIncorrect );
}

void GameBlockManager::updateForFalling(float dt)
{
	if (!_isFallingEnabled)
	{
		return;
	}

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

void GameBlockManager::setFallingEnabled(bool var)
{
	_isFallingEnabled = var;
}

bool GameBlockManager::isFallingEnabled()
{
	return _isFallingEnabled;
}

void GameBlockManager::updateForFilling(float dt)
{
	if (!_isFillingEnabled)
	{
		return;
	}

	for (int col = 0; col != getCols(); ++col)
	{
		int rowOfTopBlock = _tailBlockOfCol[col]->getBelowBlock()->getRowByRelation();
		if (rowOfTopBlock < getRows() - 1)
		{
			int type = GameBlocksCountManager::getInstance()->getNewBlockType();

			auto block = ReversibleBlock::createWithType(type);
			block->setActive(true);
			block->setFlipToObvSideEnabled(_isObvSideDefault);
			addBlockToTailOfCol(block,  col);

			if (_isRevSideHint)
			{
				block->showRevSideHint();
			}
			else
			{
				block->hideRevSideHint();
			}

			float top = (getRows() + 0.5f) * GAMEPLAY_BlockHeight;
			if (block->getPositionY() < top)
			{
				block->setPositionY(top);
			}
		}
	}
}

void GameBlockManager::setFillingEnabled(bool var)
{
	_isFillingEnabled = var;
}

bool GameBlockManager::isFillingEnabled()
{
	return _isFillingEnabled;
}

void GameBlockManager::addBlockToTailOfCol(GameBlock* block, int col)
{
	addChild(block);

	auto above = _tailBlockOfCol[col];
	auto below = _tailBlockOfCol[col]->getBelowBlock();
	
	block->setPosition(below->getPosition() + Vec2(0, GAMEPLAY_BlockHeight));
	block->setAboveBlock(above);
	block->setBelowBlock(below);

	above->setBelowBlock(block);
	below->setAboveBlock(block);
}

void GameBlockManager::removeBlock(GameBlock* block)
{
	GameBlock* below = block->getBelowBlock();
	GameBlock* above = block->getAboveBlock();

    if (below)
    {
        below->setAboveBlock(above);
    }
    
    if (above)
    {
        above->setBelowBlock(below);
    }
    
    removeChild(block);

	// Maybe check the children count here
}

GameBlock* GameBlockManager::getBlockWithPositionRow(int row, int col)
{
	auto block = _headBlockOfCol[col]->getAboveBlock();

	for (; block != _tailBlockOfCol[col]; block = block->getAboveBlock())
	{
		int r = block->getRowByPosition();
		if (r > row)
		{
			return nullptr;
		}
		else if (r == row)
		{
			return block;
		}

		block = block->getAboveBlock();
	}

	return nullptr;
}

GameBlock* GameBlockManager::getBlockWithRelationRow(int row, int col)
{
	auto block = _headBlockOfCol[col]->getAboveBlock();

	for (int i = 0 ; i != row ; ++i)
	{
		if (block == _tailBlockOfCol[col])
		{
			break;
		}

		block = block->getAboveBlock();
	}

	if (block == _tailBlockOfCol[col])
	{
		return nullptr;
	}
	else
	{
		return block;
	}
}

void GameBlockManager::replaceBlock(GameBlock* oldBlock, GameBlock* newBlock)
{
	removeChild(oldBlock);

	auto below = oldBlock->getBelowBlock();
	auto above = oldBlock->getAboveBlock();

	newBlock->setPosition(oldBlock->getPosition());
	newBlock->setBelowBlock(below);
	newBlock->setAboveBlock(above);

	below->setAboveBlock(newBlock);
	above->setBelowBlock(newBlock);

	oldBlock->setBelowBlock(nullptr);
	oldBlock->setAboveBlock(nullptr);

	addChild(newBlock);
}

void GameBlockManager::setTouchEnabled(bool isEnabled)
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

bool GameBlockManager::getTouchEnabled(void)
{
	return _isTouchEnabled;
}

GameBlockManager::~GameBlockManager()
{
	for (int idx = 0; idx != GAMEPLAY_BlocksColMax; ++idx)
	{
		CC_SAFE_RELEASE(_headBlockOfCol[idx]);
		CC_SAFE_RELEASE(_tailBlockOfCol[idx]);
	}

	CC_SAFE_RELEASE(_1stPairBlock);
	CC_SAFE_RELEASE(_2ndPairBlock);
}