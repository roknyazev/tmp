//
// Created by romak on 01.06.2021.
//

#ifndef ROUTER_PRIMITIVEBALANCER_H
#define ROUTER_PRIMITIVEBALANCER_H
#include "ABalancer.h"

class PrimitiveBalancer : public ABalancer
{
private:
	void calcProductPath(	Product::ProductPath &pr_path,
							 ListDigraph::NodeIt dep,
							 ListDigraph::NodeIt dst,
							 double weight) override;
public:
	PrimitiveBalancer(ABalancer::HubList &list, DistanceModel *distanceModel, TimeModel *timeModel);
};


#endif //ROUTER_PRIMITIVEBALANCER_H
