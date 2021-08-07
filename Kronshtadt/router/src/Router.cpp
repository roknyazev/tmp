//
// Created by romak on 25.05.2021.
//

#include "Router.h"

std::vector<std::string> split(const std::string &s, char delim)
{
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	while (std::getline(ss, item, delim))
	{
		if (item != "")
			elems.push_back(item);
	}
	return elems;
}

void fill_hub_list(ABalancer::HubList *list)
{
	//srand(12345);
	Hub::hubType sh = Hub::smallHub;
	Hub::hubType mh = Hub::mediumHub;
	Hub::hubType lh = Hub::largeHub;
//
	//double xl;
	//double yl;
//
	//double am;
	//double dm;
	//double xm;
	//double ym;
//
	//double as;
	//double ds;
	//double xs;
	//double ys;
//
	//unsigned int hub_id = 0;
	//for (int i = 0; i < 7; i++)
	//{
	//	xl = rand() % 9000 - 3000;
	//	yl = rand() % 9000 - 3000;
	//	list->push_back(new Hub(lh, xl, yl, hub_id++));
	//	for (int j = 0; j < 4; j++)
	//	{
	//		am = rand() % 360;
	//		dm = rand() % 500 + 500;
	//		xm = xl + cos(am) * dm;
	//		ym = yl + sin(am) * dm;
	//		list->push_back(new Hub(mh, xm, ym, hub_id++));
	//		for (int k = 0; k < 6; k++)
	//		{
	//			as = rand() % 360;
	//			ds = rand() % 100 + 100;
	//			xs = xm + cos(as) * ds;
	//			ys = ym + sin(as) * ds;
	//			list->push_back(new Hub(sh, xs, ys, hub_id++));
	//		}
	//	}
	//}
	//for (auto & it : *list)
	//	std::cout << it->hub_id << " " << it->posX << " " << it->posY << std::endl;
//
	//list->push_back(new Hub(lh, 62.027116, 129.731981, 0));
//
	//list->push_back(new Hub(mh, 62.192133, 130.713196, 1));
	//list->push_back(new Hub(sh, 62.188151, 130.673728, 2));
	//list->push_back(new Hub(sh, 62.221784, 130.684686, 3));
	//list->push_back(new Hub(sh, 62.257617, 130.720085, 4));
//
	//list->push_back(new Hub(mh, 62.530460, 129.762779, 5));
	//list->push_back(new Hub(sh, 62.585172, 129.770292, 6));
	//list->push_back(new Hub(sh, 62.627082, 129.714927, 7));
	//list->push_back(new Hub(sh, 62.665896, 129.704122, 8));
	//list->push_back(new Hub(sh, 62.680782, 129.911464, 9));
//
	//list->push_back(new Hub(mh, 61.536701, 129.182589, 10));
	//list->push_back(new Hub(sh, 61.614186, 129.228230, 11));
	//list->push_back(new Hub(sh, 61.484245, 129.148212, 12));
	//list->push_back(new Hub(sh, 61.535072, 129.411281, 13));
	//list->push_back(new Hub(sh, 61.486290, 129.320305, 14));

	int i = 0;
	int type;

	std::ifstream file("../../hubs.txt");
	std::cout << i << std::endl;
	std::string str;
	std::vector<std::string> data;
	int flag = 0;
	while(getline(file,str))
	{
		data = split(str, ' ');
		type = std::stoi(data[0]);
		//if (flag && type != 2)
		//{
		//	flag--;
		//	continue;
		//}



		if (type == 0)
			list->push_back(new Hub(sh, std::stod(data[1]), std::stod(data[2]), 0));
		if (type == 1)
			list->push_back(new Hub(mh, std::stod(data[1]), std::stod(data[2]), 0));
		if (type == 2)
			list->push_back(new Hub(lh, std::stod(data[1]), std::stod(data[2]), 0));
		i++;
		std::cout << i << std::endl;
		flag = 3;
	}
}

Router::~Router()
{

}

Router::Router() :
	priceModel(nullptr, nullptr, &distModel, 0, 0),
	timeModel(nullptr, nullptr, nullptr, &distModel)

{
	fill_hub_list(&list);
	balancer = new GreedyBalancer(list, &distModel, &priceModel, &timeModel);
	balancer->paintGraph("../../graph.eps");
}

struct productData
{
	double weight;
	int first_hub;
	int last_hub;
};

void Router::start()
{
	int sock, listener;
	struct sockaddr_in saddr{};
	int n = sizeof(double) + sizeof(int) + sizeof(int);
	char buf_res[n], buf_send[1024];
	productData data{};
	Product product;
	Product::ProductPath path;

	memset(buf_res, 0, n);
	size_t bytes_read;
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0)
	{
		perror("socket");
		exit(1);
	}
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(12345);
	saddr.sin_addr.s_addr = htonl(2130706433);
	if (bind(listener, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
	{
		perror("bind");
		exit(2);
	}
	listen(listener, 1);

	std::cout << "Server started!" << std::endl << std::endl;
	while (true)
	{
		sock = accept(listener, nullptr, nullptr);
		if(sock < 0)
		{
			perror("accept");
			break;
		}
		while (true)
		{

			bytes_read = recv(sock, buf_res, n, 0);
			if (bytes_read <= 0)
				break;
			memcpy(&data, buf_res, n);
			product.weight = (int)data.weight;
			product.dep = list[data.first_hub];
			product.dst = list[data.last_hub];

			balancer->setProductPath(product);
			path = product.getPath();
			int it_buf = 0;
            for (auto & it : path) {
                memcpy(&buf_send[it_buf], &(it.hub->hub_id), 4);
                std::cout << "Product: " << it.hub->hub_id << " " << it.dest_time << "  " << it.dep_time << std::endl;
                it_buf+=4;
                memcpy(&buf_send[it_buf], &(it.dest_time), 4);
                it_buf+=4;
                memcpy(&buf_send[it_buf], &(it.dep_time), 4);
                it_buf+=4;

            }
			std::cout << std::endl;
			send(sock, buf_send, it_buf, 0);
		}
		close(sock);
	}
}
