#include "BasicEle.h"


bool BasicEle::init()
{
	if(!CCLayerColor::initWithColor(ccc4(202,81,0,255)))
		return false;
	this->ignoreAnchorPointForPosition(false);
	this->setContentSize(CCSizeMake(50,50));
	return true;
}
