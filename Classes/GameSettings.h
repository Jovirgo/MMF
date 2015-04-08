//
//  GameSettings.h
//  NewFlipop
//
//  Created by 欧洲 on 14/12/28.
//
//

#ifndef __NewFlipop__GameSettings__
#define __NewFlipop__GameSettings__

#include "SimpleAudioEngine.h"
#include "cocos2d.h"

class GameSettings
{
public:
    static GameSettings * getInstance(void);
    
private:
    static GameSettings * _instance;
    
    GameSettings(void);
    
public:
    void playMusic(const char* szFileName, bool isLoop = true);
    void stopMusic(void);

	void playSound(const char* szFileName, bool isLoop = false);
    
    CC_PROPERTY(bool, _isSoundEnabled, SoundEnabled);
    CC_PROPERTY(bool, _isMusicEnabled, MusicEnabled);
};

#endif /* defined(__NewFlipop__GameSettings__) */
