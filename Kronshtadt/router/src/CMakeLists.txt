cmake_minimum_required(VERSION 3.10)
project(router)

set(CMAKE_CXX_STANDARD 14)

add_executable(router main.cpp Router.cpp Router.h balancer/ABalancer.cpp balancer/ABalancer.h balancer/model/AModel.cpp balancer/model/AModel.h balancer/model/uav/UAV.cpp balancer/model/uav/UAV.h balancer/model/hub/Hub.cpp balancer/model/hub/Hub.h balancer/model/DistanceModel.cpp balancer/model/DistanceModel.h balancer/model/TimeModel.cpp balancer/model/TimeModel.h balancer/model/product/Product.cpp balancer/model/product/Product.h balancer/GreedyBalancer.cpp balancer/GreedyBalancer.h balancer/PrimitiveBalancer.cpp balancer/PrimitiveBalancer.h balancer/model/PriceModel.cpp balancer/model/PriceModel.h balancer/Scheduler.cpp balancer/Scheduler.h)
