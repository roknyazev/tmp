//
// Created by romak on 25.05.2021.
//

#include "ABalancer.h"

ABalancer::~ABalancer()
{
}

ABalancer::ABalancer(HubList &list, DistanceModel *distanceModel, PriceModel *priceModel, TimeModel *timeModel) :
		hubMap(graph),
		distanceArcMap(graph),
		loadArcMap(graph),
		distanceModel(distanceModel),
		priceModel(priceModel),
		timeModel(timeModel),
		nodeColorMap(graph),
		nodeCoordMap(graph)
{
	ListDigraph::Node node;
	ListDigraph::Arc arc1;
	ListDigraph::Arc arc2;
	int dist;

	std::vector<ListDigraph::NodeIt> isolated_nodes;

	hubCount = (int)list.size();
	for (int i = 0; i < hubCount; i++)
	{
		node = graph.addNode();
		hubMap[node] = list[i];
	}


	int count;
	int count2;
	for (ListDigraph::NodeIt dep(graph); dep != INVALID; ++dep)
	{
		count = 0;
		count2 = 0;
		for (ListDigraph::NodeIt dst(graph); dst != INVALID; ++dst)
			if (dep != dst)
			{
				distanceModel->changeParams(hubMap[dep], hubMap[dst]);

				dist = distanceModel->getWeight();
				if (dist == INT32_MAX)
					dist = dist - 1;
				if (dist <= hubMap[dep]->max_distance && dist <= hubMap[dst]->max_distance
					&& (abs(hubMap[dep]->type - hubMap[dst]->type) < 2))
				{
					if (hubMap[dep]->type == 0 && hubMap[dst]->type == 1)
						count2++;
					count++;
				}
			}
		if (count < 1)
			isolated_nodes.push_back(dep);
	}
	for (auto &isolatedNode : isolated_nodes)
	{
		hubMap[isolatedNode]->hub_id = 0;
		graph.erase(isolatedNode);
	}


	for (ListDigraph::NodeIt dep(graph); dep != INVALID; ++dep)
	{
		count = 0;
		for (ListDigraph::NodeIt dst(graph); dst != INVALID; ++dst)
			if (dep != dst)
			{
				distanceModel->changeParams(hubMap[dep], hubMap[dst]);

				dist = distanceModel->getWeight();
				if (dist == INT32_MAX)
					dist = dist - 1;
				if (dist <= hubMap[dep]->max_distance && dist <= hubMap[dst]->max_distance
					&& (abs(hubMap[dep]->type - hubMap[dst]->type) < 2))
				{
					arc1 = graph.addArc(dep,dst);
					distanceArcMap[arc1] = dist;

					loadArcMap[arc1].weight_capacity = std::min(hubMap[dep]->max_load_capacity,
																hubMap[dst]->max_load_capacity);
					loadArcMap[arc1].dt = std::min(hubMap[dep]->step,
												   hubMap[dst]->step);
					loadArcMap[arc1].uav.setType((UAV::hubType)std::min(hubMap[dep]->type,
																		hubMap[dst]->type));
					count++;
				}
			}
	}


	std::ofstream out;
	out.open("../../validated_hubs.txt");
	std::cout.setf(std::ios::fixed);
	int i = 0;
	list.clear();
	for (ListDigraph::NodeIt it(graph); it != INVALID; ++it)
	{
		list.push_back(hubMap[it]);
		hubMap[it]->hub_id = i;
		std::cout << list[i]->hub_id << " ";
		out << std::setw(1) << hubMap[it]->type << std::setw(15) << hubMap[it]->lat << std::setw(15) << hubMap[it]->lon << std::setw(15) << hubMap[it]->hub_id << std::endl;
		i++;
	}
	hubCount = i;
	std::cout << std::endl;

	for (ListDigraph::NodeIt it(graph); it != INVALID; ++it)
	{
		nodeColorMap[it] = Color(hubMap[it]->color[0], hubMap[it]->color[1], hubMap[it]->color[2]);
		nodeCoordMap[it] = Point((int)(hubMap[it]->lon * 1000), (int)(hubMap[it]->lat * 1000));
	}
}

void ABalancer::paintGraph(const std::string &path)
{
	graphToEps(graph, path).coords(nodeCoordMap).nodeColors(nodeColorMap).run();
}

void ABalancer::setProductPath(Product &product)
{
	ListDigraph::NodeIt dep = INVALID;
	ListDigraph::NodeIt dst = INVALID;

	Product::ProductPath productPath;
	for (ListDigraph::NodeIt it(graph); it != INVALID; ++it)
	{
		if (product.dep == hubMap[it])
			dep = it;
		if (product.dst == hubMap[it])
			dst = it;
	}
	if (dep == INVALID || dst == INVALID)
		return; // TODO Выбросить исключение
	calcProductPath(productPath, dep, dst, product.weight);
	product.setPath(productPath);
}

const ListDigraph::ArcMap<Scheduler> *ABalancer::getLoadMap() const
{
	return &(this->loadArcMap);
}

const ListDigraph &ABalancer::getHubGraph() const
{
	return (this->graph);
}

const ListDigraph::ArcMap<int> *ABalancer::getDistMap() const
{
	return &(this->distanceArcMap);
}
