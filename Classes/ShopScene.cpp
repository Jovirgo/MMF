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

	_coinsText = dynamic_cast<ui::TextBMFont*>(topBar->getChildByName("CoinsText"));
	_coinsText->retain();

	_diamondsText = dynamic_cast<ui::TextBMFont*>(topBar->getChildByName("DiamondsText"));
	_diamondsText->retain();

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

	// init menu
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
	initGiftsShop();
	initToolsShop();
	initDiamondsShop();

	// init shop
	auto shop = _rootNode->getChildByName("Shop");
	
	int nodeHeight[3];
	nodeHeight[0] = 600.f;
	nodeHeight[1] = 780.f;
	nodeHeight[2] = 660.f;

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

void ShopScene::initGiftsShop()
{
	_shopNode[0] = CSLoader::createNode( SHOP_GiftsCsb ); 
	_shopNode[0]->retain();
}

void ShopScene::initToolsShop()
{
	_shopNode[1] = CSLoader::createNode( SHOP_ToolsCsb ); 
	_shopNode[1]->retain();

	for (int i = 0; i != 9; ++i)
	{
		char szChildName[20];
		snprintf(szChildName, 20, "BtnPay%d", i);

		auto button = dynamic_cast<ui::Button*>( _shopNode[1]->getChildByName(szChildName) );
		button->setSwallowTouches(false);
		button->addTouchEventListener( CC_CALLBACK_2(ShopScene::buyItemCallback, this));
	}

}

void ShopScene::initDiamondsShop()
{
	_shopNode[2] = CSLoader::createNode( SHOP_DiamondsCsb ); 
	_shopNode[2]->retain();

	for (int i = 0; i != 3; ++i)
	{
		auto button = dynamic_cast<ui::Button*>( _shopNode[2]->getChildByTag(i) );
		button->setSwallowTouches(false);
		button->addTouchEventListener( CC_CALLBACK_2(ShopScene::buyItemCallback, this));
	}
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

	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getCoins());
	_coinsText->setString(szText);
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getDiamonds());
	_diamondsText->setString(szText);
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getPowersByCalculate());
	_powersText->setString(szText);
	
	_eventDispatcher->addCustomEventListener(EVENT_IncreasePowers, CC_CALLBACK_1(ShopScene::adjustIncreasePowersEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_DecreasePowersSuccess, CC_CALLBACK_1(ShopScene::adjustIncreasePowersEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_IncreaseCoins, CC_CALLBACK_1(ShopScene::adjustDecreasePowersEvent, this));
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
		case 0: case 1: case 2:
		case 3: case 4: case 5:
		case 6:
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
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getCoins());

	_coinsText->setString(szText);
}

void ShopScene::adjustDecreaseCoinsEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getCoins());

	_coinsText->setString(szText);
}

void ShopScene::adjustIncreaseDiamondsEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getDiamonds());

	_diamondsText->setString(szText);
}

void ShopScene::adjustDecreaseDiamondsEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getDiamonds());

	_diamondsText->setString(szText);
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