//
// Created by romak on 25.05.2021.
//

#include "TimeModel.h"

int TimeModel::calcWeight() const
{
	double distance = distanceModel->getWeight();
	double meanV = uav->getMeanV();
	return (int)round(distance * 3600. / meanV);
}

TimeModel::TimeModel(UAV *uav, Hub *departure, Hub *destination, DistanceModel *distanceModel) :
AModel(uav, departure, destination), distanceModel(distanceModel)
{
	this->distanceModel->changeParams(departure, destination);
}

void TimeModel::changeParams(UAV *uav, Hub *departure, Hub *destination)
{
	changeAll(uav, departure, destination);
	this->distanceModel->changeParams(departure, destination);
}
