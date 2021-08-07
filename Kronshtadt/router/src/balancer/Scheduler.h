//
// Created by romak on 08.06.2021.
//

#ifndef ROUTER_SCHEDULER_H
#define ROUTER_SCHEDULER_H
#include "list"
#include "model/uav/UAV.h"
#include "iostream"

#define START_TIME 1623171300 //977616000 2021.01.01 00::00

struct slot_elem
{
	size_t time;
	double load;
	slot_elem(size_t time, double load) : time(time), load(load) {};
	slot_elem() = default;
};

class Scheduler
{
private:
	std::list<slot_elem> slots;
public:
	UAV uav;
	double weight_capacity;
	size_t dt;

	Scheduler();
	slot_elem *getLoad(size_t dest_t, double weight);
	size_t setLoad(slot_elem *slot, double weight);
	const std::list<slot_elem> &getQ() const;
};


#endif //ROUTER_SCHEDULER_H
