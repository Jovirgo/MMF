#include "GameBlockManagerDerivation.h"


GameBlockManagerDerivation* GameBlockManagerDerivation::create(int rows, int cols)
{
	GameBlockManagerDerivation* pRet = new GameBlockManagerDerivation();

	if (pRet && pRet->init(rows, cols))
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

void GameBlockManagerDerivation::autoPairCard(int pairs)
{
	std::vector<ReversibleBlock*> autoPairCards;

	while (pairs > 0 && isActiveBlockExists())
	{
		int startCol = int(getCols() * CCRANDOM_0_1());
		int type	 = GB_NULL;

		for (int i = 0; i != getCols(); ++i)
		{
			int		index = (startCol + i) % getCols();
			auto	block = _tailBlockOfCol[index]->getBelowBlock();

			for (; block != _headBlockOfCol[index]
				 ; block = block->getBelowBlock())
			{
				auto revBlock = dynamic_cast<ReversibleBlock*>(block);
				if (revBlock && revBlock->isActive())
				{
					revBlock->setActive(false);
					autoPairCards.push_back(revBlock);
					type = revBlock->getType();
					break;
				}
			}
				
			if (block != _headBlockOfCol[index])
			{
				break;
			}
		}

		for (int i = 0; i != getCols(); ++i)
		{
			int		index = (getCols() + startCol - i) % getCols();
			auto	block = _tailBlockOfCol[index]->getBelowBlock();

			for (; block != _headBlockOfCol[index]
				 ; block = block->getBelowBlock())
			{
				auto revBlock = dynamic_cast<ReversibleBlock*>(block);
				if (revBlock 
					&& revBlock->isActive()
					&& revBlock->getType() == type)
				{
					revBlock->setActive(false);
					autoPairCards.push_back(revBlock);
					break;
				}
			}

			if (block != _headBlockOfCol[index])
			{
				break;
			}
		}

		--pairs;
	}

	for (auto iter = autoPairCards.begin()
		; iter != autoPairCards.end()
		; ++iter)
	{
		(*iter)->flipToObvSideAndRemoveSelf();
	}
}

void GameBlockManagerDerivation::showRevSideHintShort(float duration)
{
	for (int col = 0; col != getCols(); ++col)
	{
		for (auto block = _tailBlockOfCol[col]->getBelowBlock()
			; block != _headBlockOfCol[col]
			; block = block->getBelowBlock())
		{
			auto revBlock = dynamic_cast<ReversibleBlock*>(block);
			if (revBlock != nullptr)
			{
				revBlock->showRevSideHintWithDuration( duration );
			}
		}
	}
}

void GameBlockManagerDerivation::launchRevSideHint()
{
	setRevSideHint(true);

	for (int col = 0; col != getCols(); ++col)
	{
		for (auto block = _tailBlockOfCol[col]->getBelowBlock()
			; block != _headBlockOfCol[col]
			; block = block->getBelowBlock())
		{
			auto revBlock = dynamic_cast<ReversibleBlock*>(block);
			if (revBlock != nullptr)
			{
				revBlock->showRevSideHint();
			}
		}
	}
}

void GameBlockManagerDerivation::finishRevSideHint()
{
	setRevSideHint(false);

	for (int col = 0; col != getCols(); ++col)
	{
		for (auto block = _tailBlockOfCol[col]->getBelowBlock()
			; block != _headBlockOfCol[col]
			; block = block->getBelowBlock())
		{
			auto revBlock = dynamic_cast<ReversibleBlock*>(block);
			if (revBlock != nullptr)
			{
				revBlock->hideRevSideHint();
			}
		}
	}
}

void GameBlockManagerDerivation::flipAllBlocksToRevSide()
{
	for (int col = 0; col != getCols(); ++col)
	{
		for (auto block = _tailBlockOfCol[col]->getBelowBlock()
			; block != _headBlockOfCol[col]
			; block = block->getBelowBlock())
		{
			auto revBlock = dynamic_cast<ReversibleBlock*>(block);
			if (revBlock != nullptr)
			{
				revBlock->flipToRevSide();
			}
		}
	}
}

bool GameBlockManagerDerivation::isActiveBlockExists()
{
	for (int col = 0; col != getCols(); ++col)
	{
		for (auto block = _tailBlockOfCol[col]->getBelowBlock()
			; block != _headBlockOfCol[col]
			; block = block->getBelowBlock())
		{
			if (block->isActive())
			{
				return true;
			}
		}
	}

	return false;
}