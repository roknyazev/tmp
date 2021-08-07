//
// Created by romak on 25.05.2021.
//

#ifndef ROUTER_UAV_H
#define ROUTER_UAV_H
#include "exception"

class UAV
{
public:
	typedef		enum
	{
		smallHub,
		mediumHub,
		largeHub
	}			hubType;
private:
	double meanV;
	hubType type;
public:
	~UAV() = default;
	UAV() = default;
	void  setType(hubType t);
	double getMeanV() const;
};


#endif //ROUTER_UAV_H
