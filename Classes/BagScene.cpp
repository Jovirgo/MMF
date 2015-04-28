#include "BagScene.h"
#include "GameResources.h"
#include "PlayerStats.h"

USING_NS_CC;

bool BagScene::init()
{
	bool bRet = Scene::init();

	_rootNode = CSLoader::createNode( BAG_Csb );
	_rootNode->retain();
	addChild(_rootNode);

	Size winSize = Director::getInstance()->getWinSize();
	_rootNode->setPosition(winSize.width /2,
						   winSize.height/2);

	auto btnExit = dynamic_cast<ui::Button*>(_rootNode->getChildByName("BtnExit")); 
	btnExit->addClickEventListener([](Ref*){
		// may be change
		Director::getInstance()->popScene();
	});

	initTopBar();

	return bRet;
}

void BagScene::initTopBar()
{
	auto topBar = _rootNode->getChildByName("StatsBar");

	_coinsText = dynamic_cast<ui::TextBMFont*>(topBar->getChildByName("CoinsText"));
	_coinsText->retain();

	_diamondsText = dynamic_cast<ui::TextBMFont*>(topBar->getChildByName("DiamondsText"));
	_diamondsText->retain();

	_powersText = dynamic_cast<ui::TextBMFont*>(topBar->getChildByName("PowersText"));
	_powersText->retain();

	_timerText = dynamic_cast<ui::TextBMFont*>(topBar->getChildByName("TimerText"));
	_timerText->retain();
}

void BagScene::onEnter()
{
	Scene::onEnter();
	
	auto bag = _rootNode->getChildByName("Bag");

	auto playerStats = PlayerStats::getInstance();

	int itemIdx = 0;
	int typeIdx = 0;

	for (; typeIdx < PlayerStats::Tool::TotalTypes; ++typeIdx)
	{
		auto item = bag->getChildByTag(itemIdx);

		int toolCount = playerStats->getToolCountWithType(typeIdx);
		if (toolCount > 0)
		{
			initBagItem(item, typeIdx, toolCount);

			++itemIdx;
		}
	}

	for (; itemIdx < bag->getChildrenCount(); ++itemIdx)
	{
		auto item = bag->getChildByTag(itemIdx);

		item->getChildByName("Text")->setVisible(false);
		item->getChildByName("Icon")->setVisible(false);
	}

	// stats
	auto stats = PlayerStats::getInstance();

	char szText[20];
	snprintf(szText, 20, "%d", stats->getCoins());
	_coinsText->setString(szText);
	snprintf(szText, 20, "%d", stats->getDiamonds());
	_diamondsText->setString(szText);
	snprintf(szText, 20, "%d", stats->getPowersByCalculate());
	_powersText->setString(szText);

	if (stats->isFullPower())
	{
		_timerText->setVisible(false);
	}
	else
	{
		int duration = stats->getRemainingChargingDur();
		int min = duration / 60;
		int sec = duration % 60;
		snprintf(szText, 10, "%d:%02d", min, sec);
		_timerText->setString(szText);
	}
	
	_eventDispatcher->addCustomEventListener(EVENT_IncreasePowers, CC_CALLBACK_1(BagScene::adjustIncreasePowersEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_DecreasePowersSuccess, CC_CALLBACK_1(BagScene::adjustDecreasePowersEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_IncreaseCoins, CC_CALLBACK_1(BagScene::adjustIncreaseCoinsEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_DecreaseCoinsSuccess, CC_CALLBACK_1(BagScene::adjustDecreaseCoinsEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_IncreaseDiamonds, CC_CALLBACK_1(BagScene::adjustIncreaseDiamondsEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_DecreaseDiamondsSuccess, CC_CALLBACK_1(BagScene::adjustDecreaseDiamondsEvent, this));
	
	schedule( schedule_selector(BagScene::updateForTimerText), 1.f);
}

void BagScene::onExit()
{
	unschedule( schedule_selector(BagScene::updateForTimerText));

	_eventDispatcher->removeCustomEventListeners( EVENT_IncreaseCoins );
	_eventDispatcher->removeCustomEventListeners( EVENT_DecreaseCoinsSuccess );
	_eventDispatcher->removeCustomEventListeners( EVENT_IncreaseDiamonds );
	_eventDispatcher->removeCustomEventListeners( EVENT_DecreaseDiamondsSuccess );
	_eventDispatcher->removeCustomEventListeners( EVENT_IncreasePowers );
	_eventDispatcher->removeCustomEventListeners( EVENT_DecreasePowersSuccess );

	Scene::onExit();
}

void BagScene::initBagItem(Node* item, int toolType, int toolCount)
{
	// icon
	auto icon = dynamic_cast<Sprite*>(item->getChildByName("Icon"));

	switch (toolType)
	{
	case PlayerStats::Tool::MoreTime:
		{
			icon->setTexture( TextureCache::getInstance()->addImage( BAG_ItemIconMoreTime ));
			break;
		}
	case PlayerStats::Tool::AutoMatch:
		{
			icon->setTexture( TextureCache::getInstance()->addImage( BAG_ItemIconAutoMatch ));
			break;
		}
	case PlayerStats::Tool::ShowRevHintShort:
		{
			icon->setTexture( TextureCache::getInstance()->addImage( BAG_ItemIconShowRevHintShort ));
			break;
		}
	case PlayerStats::Tool::ShowRevHint:
		{
			icon->setTexture( TextureCache::getInstance()->addImage( BAG_ItemIconShowRevHint ));
			break;
		}
	default:
		break;
	}

	// text
	char szToolCount[10];
	snprintf(szToolCount, 10, "x%d", toolCount);

	auto text = dynamic_cast<ui::TextBMFont*>(item->getChildByName("Text"));
	text->setString(szToolCount);
}

void BagScene::adjustIncreaseCoinsEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getCoins());

	_coinsText->setString(szText);
}

void BagScene::adjustDecreaseCoinsEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getCoins());

	_coinsText->setString(szText);
}

void BagScene::adjustIncreaseDiamondsEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getDiamonds());

	_diamondsText->setString(szText);
}

void BagScene::adjustDecreaseDiamondsEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getDiamonds());

	_diamondsText->setString(szText);
}

void BagScene::adjustIncreasePowersEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getPowers());

	_powersText->setString(szText);
}

void BagScene::adjustDecreasePowersEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getPowers());

	_powersText->setString(szText);
}

void BagScene::updateForTimerText(float dt)
{
	auto playerStats = PlayerStats::getInstance();

	if (playerStats->isFullPower())
	{
		_timerText->setVisible(false);
		return;
	}
	else
	{
		_timerText->setVisible(true);
	}

	// display
	char szText[10];
	
	int duration = PlayerStats::getInstance()->getRemainingChargingDur();
	int min = duration / 60;
	int sec = duration % 60;
	snprintf(szText, 10, "%d:%02d", min, sec);
	_timerText->setString(szText);

	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getPowersByCalculate());
	_powersText->setString(szText);
}

BagScene::BagScene()
	: _rootNode(nullptr)
{}

BagScene::~BagScene()
{
	CC_SAFE_RELEASE(_rootNode);
}