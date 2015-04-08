#ifndef __NewFlipop_ShopScene__
#define __NewFlipop_ShopScene__

#include "cocos2d.h"
#include "ui/cocosGUI.h"
#include "cocostudio/Cocostudio.h"

class ShopScene: public cocos2d::Scene
{
public:
	CREATE_FUNC(ShopScene);

	virtual bool init();

	void initTopBar();
	void initSwitchMenu();

	// initial shop
	void initShop();
	void initGiftsShop();
	void initToolsShop();
	void initDiamondsShop();

	virtual void onEnter();
	virtual void onExit();

	// shop type
	enum ShopType {
		Gifts = 0,
		Tools,
		Diamonds
	};

private:
	void switchItemCallback(cocos2d::Ref* pSender);

	void switchTypeToGifts();
	void switchTypeToTools();
	void swtichTypeToDiamonds();

	CC_SYNTHESIZE(ShopType, _type, Type);

private:
	// shop display and buy
	void buyItemCallback(
		cocos2d::Ref* sender, 
		cocos2d::ui::Widget::TouchEventType type);

	void buyGiftsCallback(cocos2d::Ref* sender);
	void buyToolsCallback(cocos2d::Ref* sender);
	void buyDiamondsCallback(cocos2d::Ref* sender);

	cocos2d::Node* _shopNode[3];

	// for scroll view
	cocos2d::Vec2 _touchBeganPos;	

private:
	void adjustIncreaseCoinsEvent(cocos2d::EventCustom* event);
	void adjustDecreaseCoinsEvent(cocos2d::EventCustom* event);
	void adjustIncreaseDiamondsEvent(cocos2d::EventCustom* event);
	void adjustDecreaseDiamondsEvent(cocos2d::EventCustom* event);
	void adjustIncreasePowersEvent(cocos2d::EventCustom* event);
	void adjustDecreasePowersEvent(cocos2d::EventCustom* event);

	void updateForTimerText(float dt);

	cocos2d::ui::TextBMFont*	_coinsText;
	cocos2d::ui::TextBMFont*	_diamondsText;
	cocos2d::ui::TextBMFont*	_powersText;
	cocos2d::ui::TextBMFont*	_timerText;

private:
	cocos2d::Node* _rootNode;

	ShopScene();
	virtual ~ShopScene();
};

#endif