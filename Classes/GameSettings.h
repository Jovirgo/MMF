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
    void playMusic(const char * szFileName, bool isLoop = true);
    void stopMusic(void);
    
    CC_PROPERTY(bool, _isSoundEnabed, SoundEnabed);
    CC_PROPERTY(bool, _isMusicEnabed, MusicEnabed);
};

#endif /* defined(__NewFlipop__GameSettings__) */
