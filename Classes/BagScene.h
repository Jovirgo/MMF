#ifndef __NewFlipop__BagScene__
#define __NewFlipop__BagScene__

#include "cocos2d.h"
#include "cocostudio/Cocostudio.h"
#include "ui/cocosGUI.h"

class BagScene: public cocos2d::Scene
{
public:
	CREATE_FUNC(BagScene);

	virtual bool init();

	void initTopBar();

	virtual void onEnter();
	virtual void onExit();

	void initBagItem(cocos2d::Node* item, 
					 int toolType, 
					 int toolCount);
	
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

	BagScene();
	virtual ~BagScene();
};

#endif