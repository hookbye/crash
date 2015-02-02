#include "AnimalEle.h"

AnimalEle* AnimalEle::create(const char* pzName)
{
	AnimalEle* ani = new AnimalEle();
	if(ani && ani->initWithFile(pzName))
	{
		ani->autorelease();
		return ani;
	}
	CC_SAFE_DELETE(ani);
	return NULL;
}