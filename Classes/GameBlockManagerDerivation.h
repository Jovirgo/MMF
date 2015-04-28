#ifndef __GameBlockManagerDerivation__
#define __GameBlockManagerDerivation__

#include "GameBlockManager.h"

class GameBlockManagerDerivation: public GameBlockManager
{
public:
	static GameBlockManagerDerivation* create(int rows, int cols);

public:
	/**
	* Tools
	*/

	// Auto pair
	void autoPairCard(int pairs);

	// Reverse side hint short
	void showRevSideHintShort(float duration);

	// Reverse side hint long
	void launchRevSideHint();
	void finishRevSideHint();



public:
	void flipAllBlocksToRevSide();

protected:	
	bool isActiveBlockExists();


};

#endif