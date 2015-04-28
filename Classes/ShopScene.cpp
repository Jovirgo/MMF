#include "ShopScene.h"
#include "GameResources.h"
#include "GameZoomMenuItem.h"
#include "GameConstant.h"
#include "PlayerStats.h"

USING_NS_CC;

bool ShopScene::init()
{
	bool bRet = Scene::init();

	_rootNode = CSLoader::createNode( SHOP_Csb );
	_rootNode->retain();
	addChild(_rootNode);

	Size winSize = Director::getInstance()->getWinSize();
	_rootNode->setPosition(winSize.width /2,
						   winSize.height/2);

	initSwitchMenu();
	initShop();

	initTopBar();

	auto btnExit = dynamic_cast<ui::Button*>( _rootNode->getChildByName("BtnExit") );
	btnExit->addClickEventListener([](Ref*){
		// may be change
		Director::getInstance()->popScene();
	});

	return bRet;
}

void ShopScene::initTopBar()
{
	auto topBar = _rootNode->getChildByName("StatsBar");

	
	// coins
	auto oldCoinsText = dynamic_cast<ui::TextBMFont*>(topBar->getChildByName("CoinsText"));
    oldCoinsText->removeFromParent();
    
    _coinsText = NumberCountingLabel::create();
    _coinsText->retain();
    _coinsText->setPosition(oldCoinsText->getPosition());
    _coinsText->setAnchorPoint(oldCoinsText->getAnchorPoint());
	_coinsText->setScale(oldCoinsText->getScale());
	_coinsText->setFntFile("fonts/Stats.fnt");
    _coinsText->setCountingDuration(0.2f);
	_coinsText->setScale(oldCoinsText->getScale());
    topBar->addChild(_coinsText, oldCoinsText->getZOrder(), oldCoinsText->getName());

	
	// diamonds
	auto oldDiamondsText = dynamic_cast<ui::TextBMFont*>(topBar->getChildByName("DiamondsText"));
    oldDiamondsText->removeFromParent();
    
    _diamondsText = NumberCountingLabel::create();
    _diamondsText->retain();
    _diamondsText->setPosition(oldDiamondsText->getPosition());
    _diamondsText->setAnchorPoint(oldDiamondsText->getAnchorPoint());
	_diamondsText->setScale(oldDiamondsText->getScale());
	_diamondsText->setFntFile("fonts/Stats.fnt");
    _diamondsText->setCountingDuration(0.2f);
    topBar->addChild(_diamondsText, oldDiamondsText->getZOrder(), oldDiamondsText->getName());

	_powersText = dynamic_cast<ui::TextBMFont*>(topBar->getChildByName("PowersText"));
	_powersText->retain();

	_timerText = dynamic_cast<ui::TextBMFont*>(topBar->getChildByName("TimerText"));
	_timerText->retain();
}

void ShopScene::initSwitchMenu()
{
	char* btnEnabledImages[] = {
		SHOP_GiftsItemEnabled,
		SHOP_ToolsItemEnabled,
		SHOP_DiamondsItemEnabled
	};

	char* btnDisabledImages[] = {
		SHOP_GiftsItemDisabled,
		SHOP_ToolsItemDisabled,
		SHOP_DiamondsItemDisabled,
	};

	auto oldMenu = _rootNode->getChildByName("Menu");
	auto newMenu = Menu::create();
	newMenu->setPosition(oldMenu->getPosition());

	for (int i = 0; i != oldMenu->getChildrenCount(); ++i)
	{
		auto oldItem = oldMenu->getChildByTag(i);
		auto newItem = GameZoomMenuItem::create(Sprite::create(btnEnabledImages[i]),
												Sprite::create(btnEnabledImages[i]),
												Sprite::create(btnDisabledImages[i]),
												CC_CALLBACK_1(ShopScene::switchItemCallback, this));
		newItem->setPosition(oldItem->getPosition());
		newItem->setTag(i);
		newMenu->addChild(newItem);
	}

	_rootNode->removeChild(oldMenu);
	_rootNode->addChild(newMenu, oldMenu->getZOrder(), oldMenu->getName());
}

void ShopScene::initShop()
{
	// init shop
	auto shop = _rootNode->getChildByName("Shop");
	
	int nodeHeight[3];
	nodeHeight[0] = 600.f;
	nodeHeight[1] = 780.f;
	nodeHeight[2] = 660.f;

	_shopNode[0] = createGiftsShopNode();
	_shopNode[0]->retain();
	_shopNode[1] = createToolsShopNode();
	_shopNode[1]->retain();
	_shopNode[2] = createDiamondsShopNode();
	_shopNode[2]->retain();

	for (int i = 0; i != 3; ++i)
	{
		auto background = shop->getChildByTag(i);
		auto scrollView = dynamic_cast<ui::ScrollView*>(background->getChildByName("ScrollView"));
		scrollView->addChild(_shopNode[i]);

		// inner content size
		auto size = scrollView->getContentSize();
		if (size.height < nodeHeight[i])
		{
			size.height = nodeHeight[i];
		}
		scrollView->setInnerContainerSize(size);

		// shopNode position
		_shopNode[i]->setPosition(size.width/2, 0);
	}
}

Node* ShopScene::createGiftsShopNode()
{
	return CSLoader::createNode( SHOP_GiftsCsb );
}

Node* ShopScene::createToolsShopNode()
{
	auto shopNode = CSLoader::createNode( SHOP_ToolsCsb );

	for (int i = 0; i != 9; ++i)
	{
		char szChildName[20];
		snprintf(szChildName, 20, "BtnPay%d", i);

		auto button = dynamic_cast<ui::Button*>( shopNode->getChildByName(szChildName) );
		button->setSwallowTouches(false);
		button->addTouchEventListener( CC_CALLBACK_2(ShopScene::buyItemCallback, this));
	}

	return shopNode;
}

Node* ShopScene::createDiamondsShopNode()
{
	auto shopNode = CSLoader::createNode( SHOP_DiamondsCsb );

	for (int i = 0; i != 3; ++i)
	{
		auto button = dynamic_cast<ui::Button*>( shopNode->getChildByTag(i) );
		button->setSwallowTouches(false);
		button->addTouchEventListener( CC_CALLBACK_2(ShopScene::buyItemCallback, this));
	}

	return shopNode;
}

void ShopScene::onEnter()
{
	Scene::onEnter();

	auto menu = _rootNode->getChildByName("Menu");

	for (int i = 0; i != menu->getChildrenCount(); ++i)
	{
		auto item = dynamic_cast<GameZoomMenuItem*>(menu->getChildByTag(i));
		item->setEnabled(i != _type);
	}

	auto shop = _rootNode->getChildByName("Shop");
	switch (_type)
	{
	case Gifts: shop->setPositionX(800.f); break;
	case Tools: shop->setPositionX(0.f); break;
	case Diamonds: shop->setPositionX(-800.f); break;
	default:
		break;
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
	
	_eventDispatcher->addCustomEventListener(EVENT_IncreasePowers, CC_CALLBACK_1(ShopScene::adjustIncreasePowersEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_DecreasePowersSuccess, CC_CALLBACK_1(ShopScene::adjustDecreasePowersEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_IncreaseCoins, CC_CALLBACK_1(ShopScene::adjustIncreaseCoinsEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_DecreaseCoinsSuccess, CC_CALLBACK_1(ShopScene::adjustDecreaseCoinsEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_IncreaseDiamonds, CC_CALLBACK_1(ShopScene::adjustIncreaseDiamondsEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_DecreaseDiamondsSuccess, CC_CALLBACK_1(ShopScene::adjustDecreaseDiamondsEvent, this));

	schedule( schedule_selector(ShopScene::updateForTimerText), 1.f);
}

void ShopScene::onExit()
{
	unschedule( schedule_selector(ShopScene::updateForTimerText));

	_eventDispatcher->removeCustomEventListeners( EVENT_IncreaseCoins );
	_eventDispatcher->removeCustomEventListeners( EVENT_DecreaseCoinsSuccess );
	_eventDispatcher->removeCustomEventListeners( EVENT_IncreaseDiamonds );
	_eventDispatcher->removeCustomEventListeners( EVENT_DecreaseDiamondsSuccess );
	_eventDispatcher->removeCustomEventListeners( EVENT_IncreasePowers );
	_eventDispatcher->removeCustomEventListeners( EVENT_DecreasePowersSuccess );

	Scene::onExit();
}

void ShopScene::switchItemCallback(Ref* sender)
{
	auto item = dynamic_cast<GameZoomMenuItem*>(sender);

	auto menu = item->getParent();
	for (int i = 0; i != menu->getChildrenCount(); ++i)
	{
		dynamic_cast<GameZoomMenuItem*>(menu->getChildByTag(i))->setEnabled(true);
	}

	auto newType = static_cast<ShopType>(item->getTag());
	switch (newType)
	{
	case Gifts:
		switchTypeToGifts(); break;
	case Tools:
		switchTypeToTools(); break;
	case Diamonds:
		swtichTypeToDiamonds(); break;
	default:
		break;
	}

	// must at the last
	item->setEnabled(false);
}

void ShopScene::switchTypeToGifts()
{
	auto timeline = CSLoader::createTimeline(SHOP_Csb);

	switch (_type)
	{
	case Tools:
		{
			timeline->play("ToolsToGifts", false);
			break;
		}
	case Diamonds:
		{
			timeline->play("DiamondsToGifts", false);
			break;
		}
	case Gifts:
	default:
		CCASSERT(false, "Invalid type");
	}
	
	_rootNode->runAction(timeline);

	setType( Gifts );
}

void ShopScene::switchTypeToTools()
{
	auto timeline = CSLoader::createTimeline(SHOP_Csb);

	switch (_type)
	{
	case Gifts:
		{
			timeline->play("GiftsToTools", false);
			break;
		}
	case Diamonds:
		{
			timeline->play("DiamondsToTools", false);
			break;
		}
	case Tools:
	default:
		CCASSERT(false, "Invalid type");
	}
	
	_rootNode->runAction(timeline);
	
	setType( Tools );
}

void ShopScene::swtichTypeToDiamonds()
{
	auto timeline = CSLoader::createTimeline(SHOP_Csb);

	switch (_type)
	{
	case Gifts:
		{
			timeline->play("GiftsToDiamonds", false);
			break;
		}
	case Tools:
		{
			timeline->play("ToolsToDiamonds", false);
			break;
		}
	case Diamonds:
	default:
		CCASSERT(false, "Invalid type");
	}
	
	_rootNode->runAction(timeline);

	setType( Diamonds );
}

void ShopScene::buyItemCallback(Ref* sender, ui::Widget::TouchEventType type)
{
	auto button = dynamic_cast<ui::Button*>(sender);
	auto parent = button->getParent();

	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		{
			_touchBeganPos = parent->convertToWorldSpace( button->getPosition());
			break;
		}
	case ui::Widget::TouchEventType::MOVED:
		{
			auto pos = parent->convertToWorldSpace( button->getPosition());

			if (pos.distance(_touchBeganPos) > TOUCH_Sensitivity)
			{
				button->setHighlighted(false);
			}
			break;
		}
	case ui::Widget::TouchEventType::ENDED:
		{
			_touchBeganPos = Vec2::ZERO;

			switch (_type)
			{
			case Gifts:		buyGiftsCallback(sender); break;
			case Tools:		buyToolsCallback(sender); break;
			case Diamonds:  buyDiamondsCallback(sender) ;break;
			default:
				break;
			}

			break;
		}
	default:
		break;
	}
}

void ShopScene::buyGiftsCallback(Ref* sender)
{
}

void ShopScene::buyToolsCallback(Ref* sender)
{
	auto playerStats = PlayerStats::getInstance();

	auto tag = dynamic_cast<Node*>(sender)->getTag();
	
	if ( playerStats->decreaseDiamonds( SHOP_ToolsPrice[tag] ))
	{
		switch (tag)
		{
		case 0: case 1:
			playerStats->increaseToolCountWithType(PlayerStats::Tool::AutoMatch, SHOP_ToolsQuantity[tag]);
			break;
		case 2: 
			playerStats->increaseToolCountWithType(PlayerStats::Tool::ShowRevHint, SHOP_ToolsQuantity[tag]);
			break;
		case 3: case 4:  
			playerStats->increaseToolCountWithType(PlayerStats::Tool::MoreTime, SHOP_ToolsQuantity[tag]);
			break;
		case 5: case 6:
			playerStats->increaseToolCountWithType(PlayerStats::Tool::ShowRevHintShort, SHOP_ToolsQuantity[tag]);
			break;
		case 7: case 8:
			playerStats->increaseCoins( SHOP_ToolsQuantity[tag] ); 
			break;
		default:
			break;
		}
	}
}

void ShopScene::buyDiamondsCallback(Ref* sender)
{
	auto tag = dynamic_cast<Node*>(sender)->getTag();
	PlayerStats::getInstance()->increaseDiamonds( SHOP_DiamondsQuantity[tag] );
}

void ShopScene::adjustIncreaseCoinsEvent(EventCustom* event)
{
	_coinsText->setIsIncreased(true);
	_coinsText->setGoalNumber( PlayerStats::getInstance()->getCoins());
}

void ShopScene::adjustDecreaseCoinsEvent(EventCustom* event)
{
	_coinsText->setIsIncreased(false);
	_coinsText->setGoalNumber( PlayerStats::getInstance()->getCoins());
}

void ShopScene::adjustIncreaseDiamondsEvent(EventCustom* event)
{
	_diamondsText->setIsIncreased(true);
	_diamondsText->setGoalNumber( PlayerStats::getInstance()->getDiamonds());
}

void ShopScene::adjustDecreaseDiamondsEvent(EventCustom* event)
{
	_diamondsText->setIsIncreased(false);
	_diamondsText->setGoalNumber( PlayerStats::getInstance()->getDiamonds());
}

void ShopScene::adjustIncreasePowersEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getPowers());

	_powersText->setString(szText);
}

void ShopScene::adjustDecreasePowersEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getPowers());

	_powersText->setString(szText);
}

void ShopScene::updateForTimerText(float dt)
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

ShopScene::ShopScene()
	: _type(Gifts)
	, _touchBeganPos(Vec2::ZERO)
	, _coinsText(nullptr)
	, _diamondsText(nullptr)
	, _powersText(nullptr)
	, _timerText(nullptr)
	, _rootNode(nullptr)
{
	for (int i = 0; i != 3; ++i)
	{
		_shopNode[i] = nullptr;
	}
}

ShopScene::~ShopScene()
{
	for (int i = 0; i != 3; ++i)
	{
		CC_SAFE_RELEASE(_shopNode[i]);
	}
	
	CC_SAFE_RELEASE(_coinsText);
	CC_SAFE_RELEASE(_diamondsText);
	CC_SAFE_RELEASE(_powersText);
	CC_SAFE_RELEASE(_timerText);

	CC_SAFE_RELEASE(_rootNode);
}