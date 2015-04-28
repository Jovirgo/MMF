#ifndef __NewFlipop_PlayerStats__
#define __NewFlipop_PlayerStats__

#include "cocos2d.h"
#include "GameConstant.h"

#define	EVENT_IncreasePowers		"IncreasePowers"
#define EVENT_DecreasePowersSuccess	"DecreasePowersSuccess"
#define EVENT_DecreasePowersFailed	"DecreasePowersFailed"
#define EVENT_IncreaseCoins			"IncreaseCoins"
#define EVENT_DecreaseCoinsSuccess	"DecreaseCoinsSuccess"
#define EVENT_DecreaseCoinsFailed	"DecreaseCoinsFailed"
#define EVENT_IncreaseDiamonds			"IncreaseDiamonds"
#define EVENT_DecreaseDiamondsSuccess	"DecreaseDiamondsSuccess"
#define EVENT_DecreaseDiamondsFailed	"DecreaseDiamondsFailed"
#define EVENT_IncreaseTools			"IncreaseTool"
#define EVENT_DecreaseToolsSuccess	"DecreaseToolSuccess"
#define EVENT_DecreaseToolsFailed	"DecreaseToolFailed"

class PlayerStats	// Powers, Coins and Diamonds
{
public:
	/**
	* Powers
	*/

	int getPowersByCalculate();
	int getPowers();

	void increasePowers(int delta);
	bool decreasePowers(int delta);

	void setChargingSpeed(long duration);
	int  getChargingSpeed();

	int  getRemainingChargingDur();

	void savePowerStats();
	void loadPowerStats();

	bool isFullPower() { return _powers == _powersMax; }
	
	CC_SYNTHESIZE(int, _powersMax, PowersMax);

private:
	int _chargingStart;
	int _chargingDuration;		// duration of charging 1 power

	int _powers;

public:
	/**
	* Coins
	*/
	int getCoins() { return _coins; }

	void increaseCoins(int delta);
	bool decreaseCoins(int delta);

	void saveCoinStats();
	void loadCoinStats();

private:
	int _coins;


public:
	/**
	* Diamonds
	*/
	int getDiamonds() { return _diamonds; }

	void increaseDiamonds(int delta);
	bool decreaseDiamonds(int delta);

	void saveDiamondStats();
	void loadDiamondStats();

private:
	int _diamonds;


public:
	/**
	* Tools
	*/
	enum Tool{
		MoreTime = 0,
		AutoMatch,
		ShowRevHint,
		ShowRevHintShort,
		TotalTypes
	};

	int getToolCountWithType(int type);

	void increaseToolCountWithType(int type, int delta);
	bool decreaseToolCountWithType(int type, int delta);

	void saveToolStatsWithType(int type);
	void saveToolStats();
	void loadToolStats();

private:
	int _toolCount[ TotalTypes ];

public:
	static PlayerStats* getInstance();
private:
	static PlayerStats*  _sInstance;

	PlayerStats();
};

#endif	/* __NewFlipop_PlayerStats */