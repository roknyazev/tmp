//
// Created by romak on 25.05.2021.
//

#ifndef ROUTER_AMODEL_H
#define ROUTER_AMODEL_H
#include "uav/UAV.h"
#include "hub/Hub.h"


class AModel
{
protected:
	UAV *uav;
	Hub *departure;
	Hub *destination;

	virtual int calcWeight() const = 0;
	void changeUav(UAV *newUav);
	void changeDeparture(Hub *newDeparture);
	void changeDestination(Hub *newDestination);
	void changeAll(UAV *newUav, Hub *newDeparture, Hub *newDestination);
public:
	virtual ~AModel() = default;
	AModel(UAV *uav, Hub *departure, Hub *destination);
	AModel();

	class EmptyModelFieldsException : public std::exception
	{
		const char *what() const noexcept final
		{ return "Model: can not calculate weight because all model fields are nullptr"; }
	};

	int getWeight() const;
};

#endif //ROUTER_AMODEL_H
