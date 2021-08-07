//
// Created by romak on 08.06.2021.
//

#include "Scheduler.h"

Scheduler::Scheduler()
{
	slots.emplace_back(START_TIME, 0);
}

slot_elem *Scheduler::getLoad(size_t dest_t, double weight)
{
	size_t last_time;
	size_t cur_time = time(nullptr);


	if (dest_t <= cur_time)
		cur_time = dest_t - 1;

	last_time = slots.back().time;
	while (last_time <= dest_t)
	{
		last_time += dt;
		slots.emplace_back(last_time, 0);
	}
	std::list<slot_elem>::iterator first_candidate;
	auto last_outdated_slot = slots.end();

	bool available_slots = false;
	for (auto it = slots.begin(); it != slots.end(); it++)
	{
		if ((*it).time <= cur_time)
			last_outdated_slot = it;
		if ((*it).time >= dest_t && weight + (*it).load <= weight_capacity)
		{

			first_candidate = it;
			available_slots = true;
			break;
		}
	}


	if (last_outdated_slot != slots.end())
	{
		slots.erase(slots.begin(), last_outdated_slot);
	}

	if (!available_slots)
	{
		slots.emplace_back(last_time + dt, 0);
		auto it_last = slots.end();
		--it_last;
		first_candidate = it_last;
	}

	auto candidate = first_candidate;

	for (auto it = first_candidate; it != slots.end(); it++)
		if ((*it).time >= dest_t && weight + (*it).load <= weight_capacity)
		{
			if ((*it).load > (*candidate).load)
				candidate = it;
		}
	return &(*candidate);
}

size_t Scheduler::setLoad(slot_elem *slot, double weight)
{
	slot->load += weight;
	return slot->time;
}

const std::list<slot_elem> &Scheduler::getQ() const
{
	return this->slots;
}
