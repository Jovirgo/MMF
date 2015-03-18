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
    if (_isMusicEnabed)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(szFileName, isLoop);
    }
}

void GameSettings::stopMusic(void)
{
    if (_isMusicEnabed)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }
}

void GameSettings::setSoundEnabed(bool var)
{
    
}

bool GameSettings::getSoundEnabed(void)
{
    return _isSoundEnabed;
}

void GameSettings::setMusicEnabed(bool var)
{
}

bool GameSettings::getMusicEnabed(void)
{
    return _isMusicEnabed;
}

GameSettings::GameSettings(void)
: _isSoundEnabed(true)
, _isMusicEnabed(true)
{}