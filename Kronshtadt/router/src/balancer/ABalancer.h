//
// Created by romak on 25.05.2021.
//

#ifndef ROUTER_ABALANCER_H
#define ROUTER_ABALANCER_H

#include "model/DistanceModel.h"
#include "model/PriceModel.h"
#include "model/TimeModel.h"
#include "model/uav/UAV.h"
#include <vector>
#include "iostream"
#include "lemon/list_graph.h"
#include "lemon/graph_to_eps.h"
#include "model/product/Product.h"
#include "lemon/dijkstra.h"
#include <list>
#include <vector>
#include <lemon/adaptors.h>
#include "Scheduler.h"
#include <iomanip>

#define START_TIME 1622758300 //977616000 2021.01.01 00::00

using namespace lemon;

class ABalancer
{
public:
	typedef std::vector<Hub *> HubList;
	typedef dim2::Point<int> Point;
protected:
	ListDigraph graph;
	ListDigraph::NodeMap<Hub *> hubMap;
	ListDigraph::ArcMap<int> distanceArcMap;
	ListDigraph::ArcMap<Scheduler> loadArcMap;

	DistanceModel *distanceModel;
	PriceModel *priceModel;
	TimeModel *timeModel;

	ListDigraph::NodeMap<Color> nodeColorMap;
	ListDigraph::NodeMap<Point> nodeCoordMap;

	int hubCount;

	virtual void calcProductPath(	Product::ProductPath &path,
								  		ListDigraph::NodeIt dep,
								  		ListDigraph::NodeIt dst,
								  		double weight) = 0;
public:
	virtual ~ABalancer();
	ABalancer(HubList &list, DistanceModel *distanceModel, PriceModel *priceModel, TimeModel *timeModel);

	void setProductPath(Product &product);
	void paintGraph(const std::string &path);
	const ListDigraph::ArcMap<Scheduler> *getLoadMap() const;
	const ListDigraph::ArcMap<int> *getDistMap() const;
	const ListDigraph& getHubGraph() const;
};


#endif //ROUTER_ABALANCER_H
