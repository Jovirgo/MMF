#include "PlayerStats.h"

USING_NS_CC;

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

unsigned int PlayerStats::getPowers()
{
	if (0 != _chargingStart)
	{	
		int powers = (getSystemCurTime() - _chargingStart)/_chargingDuration;
		increasePowers(powers);
	}

	return _powers;
}

void PlayerStats::increasePowers(unsigned int delta)
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

	// save data
}

void PlayerStats::decreasePowers(unsigned int delta)
{
	_powers -= delta;
	
	_chargingStart = getSystemCurTime();

	// save data
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

		// save data
	}
}

long PlayerStats::getChargingSpeed()
{
	return _chargingDuration;
}

long PlayerStats::getRemaingChargingDur()
{
	long chargingEnd = _chargingStart + _chargingDuration * (_powersMax-_powers);
	return chargingEnd - getSystemCurTime();
}