//
//  GameConstant.h
//  NewFlipop
//
//  Created by 欧洲 on 15/3/9.
//
//

#ifndef NewFlipop_GameConstant_h
#define NewFlipop_GameConstant_h

#include "cocos2d.h"

const cocos2d::Size MAP_PlateSize(720, 1280);
const float MAP_TouchSensitivity = 5.f;		// for level button

const float	GAMEPLAY_BlockWidth  = 100.f;
const float GAMEPLAY_BlockHeight = 100.f;
const int   GAMEPLAY_BlocksRowMax = 6;
const int   GAMEPLAY_BlocksColMax = 6;
const int   GAMEPLAY_BaseScore = 10;
const int	GAMEPLAY_FeverStep = 7;
const float	GAMEPLAY_FeverDuration = 5.f;

const unsigned int	STATS_PowersMax = 30;
const long			STATS_ChargingDuration = 60;	// seconds

// game

#endif
