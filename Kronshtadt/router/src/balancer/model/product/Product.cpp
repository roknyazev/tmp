//
// Created by romak on 31.05.2021.
//

#include "Product.h"

Product::Product() : weight(weight), dep(dep), dst(dst)
{
}

void Product::setPath(Product::ProductPath &pPath)
{
	this->path = pPath;
}

const Product::ProductPath &Product::getPath() const
{
	return this->path;
}
