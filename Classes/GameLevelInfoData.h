#ifndef __GameLevelInfoData__
#define __GameLevelInfoData__

#include "CsvFileForCocos2d.h"
#include "GameResources.h"

class GameLevelInfoData: public CsvFileForCocos2d
{
public:
	GameLevelInfoData() 
	{ 
		openFile( MAP_LevelInfoCsv );
	}

	// Use getData function with these colIndex.
	enum Field{
		LevelID = 0,
		Rows,
		Cols,
		Time,
		Color1,
		Color2,
		Color3,
		Color4,
		Color5,
		BonusCoin,
		ScoreGoal1,
		ScoreGoal2,
		ScoreGoal3,
		BonusTool1,
		BonusTool2,
		BonusTool3
	};
};

#endif