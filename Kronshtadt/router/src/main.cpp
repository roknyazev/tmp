#include <iostream>
#include "Router.h"

int main()
{
	char buf[1024];

	size_t i1 =1624206005,i2 = 1624206000, i3 = 1624205907, i4 = 1624205900;

	memcpy(&buf[0],&i1, 4);
	memcpy(&(buf[4]),&i2, 4);
	memcpy(&(buf[8]),&i3, 4);
	memcpy(&(buf[12]),&i4, 4);

	unsigned int ii[4];

	memcpy(ii, buf, sizeof(ii));

	Router router;
	router.start();
}
