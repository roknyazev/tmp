//
// Created by romak on 01.06.2021.
//

#ifndef ROUTER_PRICEMODEL_H
#define ROUTER_PRICEMODEL_H
#include "AModel.h"
#include "DistanceModel.h"

class PriceModel : public AModel
{
private:
	DistanceModel *distanceModel;
	double load;
	double load_max;
	int calcWeight() const override;
public:
	PriceModel(Hub *departure,
			Hub *destination,
			DistanceModel *distanceModel,
			double load,
			double load_max);
	void changeParams(Hub *departure, Hub *destination, double load, double load_max);
};


#endif //ROUTER_PRICEMODEL_H
