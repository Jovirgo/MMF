//
//  GameSettings.cpp
//  NewFlipop
//
//  Created by 欧洲 on 14/12/28.
//
//

#include "GameSettings.h"

USING_NS_CC;

GameSettings * GameSettings::_instance = nullptr;

GameSettings * GameSettings::getInstance(void)
{
    if (_instance == nullptr)
    {
        _instance = new GameSettings();
    }
    
    return _instance;
}

void GameSettings::playMusic(const char *szFileName, bool isLoop)
{
    if (_isMusicEnabled)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(szFileName, isLoop);
    }
}

void GameSettings::stopMusic(void)
{
    if (_isMusicEnabled)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }
}

void GameSettings::playSound(const char* szFileName, bool isLoop)
{
	if (_isSoundEnabled)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(szFileName, isLoop);
	}
}

void GameSettings::setSoundEnabled(bool var)
{
    
}

bool GameSettings::getSoundEnabled(void)
{
    return _isSoundEnabled;
}

void GameSettings::setMusicEnabled(bool var)
{
}

bool GameSettings::getMusicEnabled(void)
{
    return _isMusicEnabled;
}

GameSettings::GameSettings(void)
: _isSoundEnabled(true)
, _isMusicEnabled(true)
{}