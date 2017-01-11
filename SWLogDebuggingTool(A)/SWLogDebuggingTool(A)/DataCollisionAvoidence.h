#include <cstdlib>
#include <ctime>
#include <windows.h>

#pragma once

class DataCollisionAvoidence
{
	public:
		DataCollisionAvoidence(void);
		~DataCollisionAvoidence(void);

		int CollisionAvoidence(int iMaxDelay_ms);
};
