//
// Created by romak on 25.05.2021.
//

#include "UAV.h"

void UAV::setType(UAV::hubType t)
{
	type = t;

	if (type == smallHub)
	{
		meanV = 20 * 50;
	}

	else if (type == mediumHub)
	{
		meanV = 100 * 50;
	}

	else
	{
		meanV = 500 * 50;
	}
}

double UAV::getMeanV() const
{
	return this->meanV;
}
