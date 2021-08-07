//
// Created by romak on 31.05.2021.
//

#ifndef ROUTER_PRODUCT_H
#define ROUTER_PRODUCT_H
#include "../hub/Hub.h"
#include "list"

struct Schedule
{
	Hub* hub;
	size_t dest_time;
	size_t dep_time;
};

class Product
{
public:
	typedef std::list<Schedule> ProductPath;
private:
	ProductPath path;
public:
	Hub *dep;
	Hub *dst;
	int weight;
	uint32_t price;
	~Product() = default;
	Product();
	void setPath(ProductPath &pPath);
	const ProductPath &getPath() const;
};


#endif //ROUTER_PRODUCT_H
