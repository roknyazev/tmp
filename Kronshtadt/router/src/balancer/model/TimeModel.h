//
// Created by romak on 25.05.2021.
//

#ifndef ROUTER_TIMEMODEL_H
#define ROUTER_TIMEMODEL_H
#include "DistanceModel.h"

class TimeModel : public AModel
{
private:
	DistanceModel *distanceModel;
	int calcWeight() const override;
public:
	TimeModel(UAV *uav, Hub *departure, Hub *destination, DistanceModel *distanceModel);
	void changeParams(UAV *uav, Hub *departure, Hub *destination);
};


#endif //ROUTER_TIMEMODEL_H
