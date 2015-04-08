#include "PlayerStats.h"
#include "LoadAndSaveDataForCocos2d.h"

USING_NS_CC;
	
#define STATS_ChargingStartSaveKey		"ChargingStart"
#define STATS_ChargingDurationSaveKey	"ChargingDuration"
#define STATS_PowersSaveKey		"Powers"
#define STATS_PowersMaxSaveKey	"PowersMax"
#define STATS_CoinsSaveKey		"Coins"
#define STATS_DiamondsSaveKey	"Diamonds"

time_t getSystemCurTime()
{
	// could not be lated than 2038/1/18 19:14:07
	time_t t;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&t);
#else
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	t = tv.tv_sec;
#endif

	return t;
}

PlayerStats* PlayerStats::_sInstance = nullptr;

PlayerStats* PlayerStats::getInstance()
{
	if (nullptr == _sInstance)
	{
		_sInstance = new PlayerStats();
	}

	return _sInstance;
}

int PlayerStats::getPowersByCalculate()
{
	if (0 != _chargingStart)
	{	
		int powers = (getSystemCurTime() - _chargingStart)/_chargingDuration;
		increasePowers(powers);
	}

	return _powers;
}

int PlayerStats::getPowers()
{
	return _powers;
}

void PlayerStats::increasePowers(int delta)
{
	_powers += delta;

	if (_powers > _powersMax)
	{
		_powers = _powersMax;

		_chargingStart = 0;
	}
	else
	{
		_chargingStart += (delta*_chargingDuration);
	}

	savePowerStats();

	auto director = Director::getInstance();
	director->getEventDispatcher()->dispatchCustomEvent( EVENT_IncreasePowers );
}

bool PlayerStats::decreasePowers(int delta)
{
	auto director = Director::getInstance();

	if (_powers < delta)
	{
		director->getEventDispatcher()->dispatchCustomEvent( EVENT_DecreasePowersFailed );
		return false;
	}
	else
	{
		_powers -= delta;
		_chargingStart = getSystemCurTime();
		savePowerStats();

		director->getEventDispatcher()->dispatchCustomEvent( EVENT_DecreasePowersSuccess );
		return true;
	}

}

void PlayerStats::setChargingSpeed(long duration)
{
	if (_chargingDuration == duration)
	{
		return;
	}
	else
	{
		_chargingDuration = duration;

		savePowerStats();
	}
}

int PlayerStats::getChargingSpeed()
{
	return _chargingDuration;
}

int PlayerStats::getRemainingChargingDur()
{
	if (_chargingStart == 0)
	{
		return 0;
	}

	int chargingEnd = _chargingStart + _chargingDuration * (_powersMax-_powers);
	return chargingEnd - getSystemCurTime();
}

void PlayerStats::savePowerStats()
{
	saveDataByKey_int(STATS_ChargingStartSaveKey, _chargingStart, true);
	saveDataByKey_int(STATS_ChargingDurationSaveKey, _chargingDuration, true);
	saveDataByKey_int(STATS_PowersSaveKey, _powers, true);
	saveDataByKey_int(STATS_PowersMaxSaveKey, _powersMax, true);
	saveDataFlush();
}

void PlayerStats::loadPowerStats()
{
	_chargingStart = loadDataByKey_int(STATS_ChargingStartSaveKey, true);
	_chargingDuration = loadDataByKey_int(STATS_ChargingDurationSaveKey, true);
	_powers = loadDataByKey_int(STATS_PowersSaveKey, true);
	_powersMax = loadDataByKey_int(STATS_PowersMaxSaveKey, true);
}

void PlayerStats::increaseCoins(int delta) 
{ 
	_coins += delta;
	saveCoinStats();

	auto director = Director::getInstance();
	director->getEventDispatcher()->dispatchCustomEvent( EVENT_IncreaseCoins );
}

bool PlayerStats::decreaseCoins(int delta) 
{
	auto director = Director::getInstance();

	if (_coins < delta)
	{
		director->getEventDispatcher()->dispatchCustomEvent( EVENT_DecreaseCoinsFailed );
		return false;
	}
	else
	{
		_coins -= delta; 
		saveCoinStats();

		director->getEventDispatcher()->dispatchCustomEvent( EVENT_DecreaseCoinsSuccess );
		return true;
	}
}

void PlayerStats::saveCoinStats()
{
	saveDataByKey_int(STATS_CoinsSaveKey, _coins, true);
	saveDataFlush();
}

void PlayerStats::loadCoinStats()
{
	_coins = loadDataByKey_int(STATS_CoinsSaveKey, true);
}

void PlayerStats::increaseDiamonds(int delta) 
{ 
	_diamonds += delta; 
	saveDiamondStats();

	auto director = Director::getInstance();
	director->getEventDispatcher()->dispatchCustomEvent( EVENT_IncreaseDiamonds );
}

bool PlayerStats::decreaseDiamonds(int delta)
{
	auto director = Director::getInstance();

	if (_diamonds < delta)
	{
		director->getEventDispatcher()->dispatchCustomEvent( EVENT_DecreaseDiamondsFailed );
		return false;
	}
	else
	{
		_diamonds -= delta;
		saveDiamondStats();

		director->getEventDispatcher()->dispatchCustomEvent( EVENT_DecreaseDiamondsSuccess );
		return true;
	}
}

void PlayerStats::saveDiamondStats()
{
	saveDataByKey_int(STATS_DiamondsSaveKey, _diamonds, true);
	saveDataFlush();
}

void PlayerStats::loadDiamondStats()
{
	_diamonds = loadDataByKey_int(STATS_DiamondsSaveKey, true);
}

PlayerStats::PlayerStats()
	: _powersMax(STATS_PowersMax)
	, _powers(_powersMax)
	, _chargingStart(0)
	, _chargingDuration(STATS_ChargingDuration)
	, _coins(0)
	, _diamonds(0)
{
	const char* haveSavedPlayerStats = "HaveSavedPlayerStats";

	if (loadDataByKey_bool( haveSavedPlayerStats ))
	{
		loadPowerStats();
		loadCoinStats();
		loadDiamondStats();
	}
	else
	{
		saveDataByKey_bool( haveSavedPlayerStats , true);
		
		savePowerStats();
		saveCoinStats();
		saveDiamondStats();
	}

	// just for test
//	_diamonds = 0;
//	_coins = 0;
}