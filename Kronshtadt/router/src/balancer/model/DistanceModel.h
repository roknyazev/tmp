//
// Created by romak on 25.05.2021.
//

#ifndef ROUTER_DISTANCEMODEL_H
#define ROUTER_DISTANCEMODEL_H
#include "AModel.h"
#define EARTH_RADIUS 6371

class DistanceModel : public AModel
{
private:
	int calcWeight() const override;
public:
	DistanceModel(Hub *departure, Hub *destination);
	DistanceModel() = default;
	void changeParams(Hub *departure, Hub *destination);
};

#endif //ROUTER_DISTANCEMODEL_H
