//
// Created by romak on 25.05.2021.
//

#ifndef ROUTER_HUB_H
#define ROUTER_HUB_H
#include "cmath"
#include "string"


struct Hub
{
	typedef		enum
	{
		smallHub,
		mediumHub,
		largeHub
	}			hubType;

	hubType type;
	double posX;
	double posY;
	double lat;
	double lon;
	double max_distance;
	double max_load_capacity;
	int color[3];
	unsigned int hub_id;
	size_t step;

	~Hub() = default;
	Hub(hubType type, double x, double y, unsigned int hub_id);
};


#endif //ROUTER_HUB_H
