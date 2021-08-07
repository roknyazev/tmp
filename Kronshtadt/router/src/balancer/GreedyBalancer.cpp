//
// Created by romak on 31.05.2021.
//

#include "GreedyBalancer.h"

GreedyBalancer::GreedyBalancer(ABalancer::HubList &list, DistanceModel *distanceModel, PriceModel *priceModel,
							   TimeModel *timeModel) : ABalancer(list, distanceModel, priceModel, timeModel)
{
}

void GreedyBalancer::k_paths(ListDigraph::NodeIt dep,
							 ListDigraph::NodeIt dst)
{
	Dijkstra<ListDigraph> dij(graph, distanceArcMap);
	ListDigraph::ArcMap<bool> filter(graph, true);

	Path<ListDigraph> p;
	Path<ListDigraph> candidate;
	std::vector<Path<ListDigraph> > tmp_paths;
	Path<ListDigraph> tmp_path;
	int tmp_length;


	dij.run(dep);
	if (!dij.processed(dst))
		return;

	paths[0] = dij.path(dst);
	for (int i = 1; i < K_SHORTEST_PATHS; ++i)
	{
		candidate = paths[i - 1];
		p = paths[i - 1];
		int min_length = INT32_MAX;
		for (Path<ListDigraph>::ArcIt it(p); it != INVALID; ++it)
		{
			filter[it] = false;
			dijkstra(filterArcs(graph, filter), distanceArcMap).path(tmp_path).run(dep, dst);

			tmp_length = 0;

			for (Path<ListDigraph>::ArcIt it_tmp(tmp_path); it_tmp != INVALID; ++it_tmp)
				tmp_length += distanceArcMap[it_tmp];
			if (tmp_length > 0 && tmp_length < min_length)
			{
				min_length = tmp_length;
				candidate = tmp_path;
			}
			filter[it] = true;
		}
		paths[i] = candidate;
	}
}

void GreedyBalancer::calcProductPath(Product::ProductPath &pr_path,
									 ListDigraph::NodeIt dep,
									 ListDigraph::NodeIt dst,
									 double weight)
{
	Path<ListDigraph> path_candidate;

	std::list<slot_elem *> slot_candidate;
	std::list<slot_elem *> slot_tmp;

	std::list<size_t> dst_times_candidate;
	std::list<size_t> dst_times_tmp;

	int min_price = INT32_MAX;
	int price;
	size_t start_time = std::time(nullptr);
	size_t dest_time;
	slot_elem *q;
	ListDigraph::Node dep_node;
	ListDigraph::Node dst_node;

	k_paths(dep, dst);
	for (auto & path : paths)
	{
		price = 0;
		dest_time = start_time;
		dep_node = dep;
		slot_tmp.clear();
		dst_times_tmp.clear();

		dst_times_tmp.push_back(dest_time);
		for (Path<ListDigraph>::ArcIt it(path); it != INVALID; ++it)
		{
			q = loadArcMap[it].getLoad(dest_time, weight);
			dest_time = q->time;
			slot_tmp.push_back(q);

			dst_node = graph.oppositeNode(dep_node, it);

			priceModel->changeParams(hubMap[dep_node], hubMap[dst_node], q->load ,
																 loadArcMap[it].weight_capacity);
			price += priceModel->getWeight();

			timeModel->changeParams(&(loadArcMap[it].uav), hubMap[dep_node], hubMap[dst_node]);
			dest_time += timeModel->getWeight();
			dst_times_tmp.push_back(dest_time);

			dep_node = dst_node;
		}
		if (price < min_price)
		{
			path_candidate = path;
			min_price = price;
			slot_candidate = slot_tmp;
			dst_times_candidate = dst_times_tmp;
		}
	}
	ListDigraph::Node node;
	node = dep;

	auto it_slot = slot_candidate.begin();
	auto it_dest_time = dst_times_candidate.begin();
	for (Path<ListDigraph>::ArcIt it(path_candidate); it != INVALID; ++it)
	{
		pr_path.push_back({hubMap[node], *it_dest_time, (*it_slot)->time});
		node = graph.oppositeNode(node, it);
		loadArcMap[it].setLoad(*it_slot, weight);
		it_slot++;
		it_dest_time++;
	}
	pr_path.push_back({hubMap[dst], *it_dest_time, 0});

//for (auto i = time_candidate.begin(); i != time_candidate.end(); ++i)
//	std::cout << *i << "  ";
//	std::cout <<  std::endl;



}
