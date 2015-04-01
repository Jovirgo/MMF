#ifndef __NewFlipop_PlayerStats__
#define __NewFlipop_PlayerStats__

#include "cocos2d.h"
#include "GameConstant.h"

class PlayerStats	// Powers, Coins and Diamonds
{
public:
	/**
	* Powers
	*/
	unsigned int getPowers();

	void increasePowers(unsigned int delta);
	void decreasePowers(unsigned int delta);

	void setChargingSpeed(long duration);
	long getChargingSpeed();

	long getRemaingChargingDur();
	
	CC_SYNTHESIZE(unsigned int, _powersMax, PowersMax);

private:
	unsigned int _powers;

	long _chargingStart;
	long _chargingDuration;		// duration of charging 1 power

public:
	/**
	* Coins
	*/
	unsigned int getCoins() { return _coins; }

	void increaseCoins(unsigned int delta) { _coins -= delta; /* save data */ }
	void decreaseCoins(unsigned int delta) { _coins += delta; /* save data */ }

private:
	unsigned int _coins;

public:
	/**
	* Diamonds
	*/
	unsigned int getDiamonds() { return _diamonds; }

	void increaseCoins(unsigned int delta) { _diamonds += delta; /* save data */ }
	void decreaseCoins(unsigned int delta) { _diamonds -= delta; /* save data */ }

private:
	unsigned int _diamonds;


public:
	static PlayerStats* getInstance();
private:
	static PlayerStats*  _sInstance;

	PlayerStats()
	: _powersMax(0)
	, _powers(0)
	, _chargingStart(0)
	, _chargingDuration(0)
	, _coins(0)
	, _diamonds(0)
	{}
};

#endif	/* __NewFlipop_PlayerStats */