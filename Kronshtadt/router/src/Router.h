//
// Created by romak on 25.05.2021.
//

#ifndef ROUTER_ROUTER_H
#define ROUTER_ROUTER_H
#include "balancer/GreedyBalancer.h"
#include <cstring>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <unistd.h>
#include <csignal>
#include <fstream>
#include <iostream>
#include <cstdlib>

class Router
{
private:
    DistanceModel distModel;
    PriceModel priceModel;
    TimeModel timeModel;
	GreedyBalancer *balancer;
    ABalancer::HubList list;
public:
	~Router();
	Router();
	void start();
};


#endif //ROUTER_ROUTER_H
