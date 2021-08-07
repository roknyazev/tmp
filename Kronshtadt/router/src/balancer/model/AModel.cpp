//
// Created by romak on 25.05.2021.
//

#include "AModel.h"

AModel::AModel(UAV *uav, Hub *departure, Hub *destination)
{
	this->uav = uav;
	this->departure = departure;
	this->destination = destination;
}

AModel::AModel()
{
	this->uav = nullptr;
	this->departure = nullptr;
	this->destination = nullptr;
}

void AModel::changeUav(UAV *newUav)
{
	this->uav = newUav;
}

void AModel::changeDeparture(Hub *newDeparture)
{
	this->departure = newDeparture;
}

void AModel::changeDestination(Hub *newDestination)
{
	this->destination = newDestination;
}

int AModel::getWeight() const
{
	if (uav == nullptr && departure == nullptr && destination == nullptr)
		throw EmptyModelFieldsException();
	return calcWeight();
}

void AModel::changeAll(UAV *newUav, Hub *newDeparture, Hub *newDestination)
{
	changeUav(newUav);
	changeDeparture(newDeparture);
	changeDestination(newDestination);
}
