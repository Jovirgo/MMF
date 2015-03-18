//
//  GameData.cpp
//  NewFlipop
//
//  Created by 欧洲 on 15/1/6.
//
//

#include "GameData.h"

GameData * GameData::_instance = nullptr;

GameData * GameData::getInstance(void)
{
    if (_instance == nullptr)
    {
        _instance = new GameData();
    }
    
    return _instance;
}

void GameData::resetGameData()
{
	_score = 0;
	_combo = 0;
	_comboMax = _combo;
	_multiply = 1;
	_coins = 0;
	_match = 0;
}

GameData::GameData(void)
: _score(0)
, _combo(0)
, _comboMax(_combo)
, _multiply(1)
, _coins(0)
, _match(0)
{}
