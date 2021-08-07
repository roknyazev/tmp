//
// Created by romak on 25.05.2021.
//

#include "DistanceModel.h"
#include "iostream"

int DistanceModel::calcWeight() const
{
	double ph1 = departure->posY;
	double lm1 = departure->posX;
	double ph2 = destination->posY;
	double lm2 = destination->posX;
	double delta_lm = lm2 - lm1;
	int result;

	double delta_sigma;

	double tmp1 = sqrt(pow((cos(ph2) * sin(delta_lm)), 2) + pow((cos(ph1) * sin(ph2) - sin(ph1) * cos(ph2) * cos(delta_lm)), 2));
	double tmp2 = sin(ph1) * sin(ph2) + cos(ph1) * cos(ph2) * cos(delta_lm);
	delta_sigma = atan2(tmp1, tmp2);
	result = (int)round(delta_sigma * EARTH_RADIUS);
	return result;
}

DistanceModel::DistanceModel(Hub *departure, Hub *destination) : AModel(nullptr, departure, destination)
{
}

void DistanceModel::changeParams(Hub *departure, Hub *destination)
{
	changeAll(nullptr, departure, destination);
}
