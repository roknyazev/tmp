//
// Created by romak on 31.05.2021.
//

#ifndef ROUTER_GREEDYBALANCER_H
#define ROUTER_GREEDYBALANCER_H
#include "ABalancer.h"

#define K_SHORTEST_PATHS 1

class GreedyBalancer : public ABalancer
{
private:
	void calcProductPath(	Product::ProductPath &path,
							 ListDigraph::NodeIt dep,
							 ListDigraph::NodeIt dst,
							 double weight) override;
	Path<ListDigraph> paths[K_SHORTEST_PATHS];
public:
	GreedyBalancer(HubList &list, DistanceModel *distanceModel, PriceModel *priceModel, TimeModel *timeModel);
	void k_paths(ListDigraph::NodeIt dep,
				 ListDigraph::NodeIt dst);
};


#endif //ROUTER_GREEDYBALANCER_H
