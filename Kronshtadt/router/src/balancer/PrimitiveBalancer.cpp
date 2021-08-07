//
// Created by romak on 01.06.2021.
//

#include "PrimitiveBalancer.h"

PrimitiveBalancer::PrimitiveBalancer(ABalancer::HubList &list, DistanceModel *distanceModel, TimeModel *timeModel) :
									ABalancer(list, distanceModel, nullptr, timeModel)
{
}

void PrimitiveBalancer::calcProductPath(Product::ProductPath &pr_path,
										ListDigraph::NodeIt dep,
										ListDigraph::NodeIt dst,
										double weight)
{
	Dijkstra<ListDigraph> dij(graph, distanceArcMap);
	Path<ListDigraph> p;
	ListDigraph::Node dep_node;
	ListDigraph::Node dst_node;
	slot_elem *q;
	size_t dest_time = std::time(nullptr);

	dij.run(dep);
	if (!dij.processed(dst))
		return;
	p = dij.path(dst);

	dep_node = dep;
	pr_path.push_back({hubMap[dep_node], 0, 0}); //FIXME
	for (Path<ListDigraph>::ArcIt it(p); it != INVALID; ++it)
	{
		dst_node = graph.oppositeNode(dep_node, it);
		pr_path.push_back({hubMap[dst_node], 0, 0});

		q = loadArcMap[it].getLoad(dest_time, weight);
		dest_time = q->time;

		timeModel->changeParams(&(loadArcMap[it].uav), hubMap[dep_node], hubMap[dst_node]);
		dest_time += timeModel->getWeight();

		loadArcMap[it].setLoad(q, weight);
		dep_node = dst_node;
	}


}


