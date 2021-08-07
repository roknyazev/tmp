//
// Created by romak on 01.06.2021.
//

#include "PriceModel.h"

PriceModel::PriceModel(Hub *departure, Hub *destination, DistanceModel *distanceModel, double load, double  load_max) :
		AModel(nullptr, departure, destination),
		distanceModel(distanceModel),
		load(load),
		load_max(load_max)
{
	this->distanceModel->changeParams(departure, destination);
}

void PriceModel::changeParams(Hub *departure, Hub *destination, double new_load, double new_load_max)
{
	changeAll(nullptr, departure, destination);
	this->distanceModel->changeParams(departure, destination);
	this->load = new_load;
	this->load_max = new_load_max;
}

int PriceModel::calcWeight() const
{
	auto distance = (double)distanceModel->getWeight();
	return (int)round(distance * load_max / load);
}
