#include "DataCollisionAvoidence.h"

DataCollisionAvoidence::DataCollisionAvoidence(void)
{
}

DataCollisionAvoidence::~DataCollisionAvoidence(void)
{
}

int DataCollisionAvoidence::CollisionAvoidence(int iMaxDelay_ms)
{
	int iDelay = rand() % iMaxDelay_ms + 1;
	srand((unsigned int)time(NULL));
	Sleep(iDelay);

	return iDelay;
}