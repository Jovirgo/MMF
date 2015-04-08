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

// for button in scroll view
const float TOUCH_Sensitivity = 5.f;		

const cocos2d::Size MAP_PlateSize(720, 1280);


/**
* Gameplay
*/
const float	GAMEPLAY_BlockWidth  = 100.f;
const float GAMEPLAY_BlockHeight = 100.f;
const int   GAMEPLAY_BlocksRowMax = 6;
const int   GAMEPLAY_BlocksColMax = 6;
const int   GAMEPLAY_BaseScore = 10;
const int	GAMEPLAY_FeverStep = 7;
const float	GAMEPLAY_FeverDuration = 5.f;


/**
* Shop
*/
const int	SHOP_DiamondsQuantity[3] = {
	10,
	90,
	150
};

const int	SHOP_ToolsPrice[9] = {
	100,	200,	300,
	100,	200,	300,
	100,	200,	300
};

const int SHOP_ToolsQuantity[9] = {
	1,		1,		1,	// tools
	1,		1,		1,	// tools
	1,		1000,	50000	// tool, coin, coin
};


/**
* Stats
*/
const int STATS_PowersMax = 30;
const int STATS_ChargingDuration = 60;	// seconds


/**
* Ready
*/
const int READY_PowersDecrease = 5;

// game

#endif
