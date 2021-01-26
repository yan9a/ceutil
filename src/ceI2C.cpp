// File: ceI2C.h
// Description: ceI2C class to use i2c communication
// WebSite: http://cool-emerald.blogspot.com
// MIT License (https://opensource.org/licenses/MIT)
// Copyright (c) 2018 Yan Naing Aye

#include "ce/ceI2C.h"
using namespace std;

namespace ce {
template <typename T>
string ceI2C::ToString(T a)
{
	ostringstream ss;
	ss << a;
	return ss.str();
}

ceI2C::ceI2C()
{
	//ctor
}

ceI2C::ceI2C(int bus_id, int slave_address)
{
	Begin(bus_id, slave_address);
}

ceI2C::~ceI2C()
{
	//dtor
#ifdef CE_LINUX
	close(fd);
#else
#endif
}

bool ceI2C::Begin(int bus_id, int slave_address)
{
	string filename = "/dev/i2c-";
	filename += ToString(bus_id);
#ifdef CE_LINUX
	if ((fd = open(filename.c_str(), O_RDWR)) < 0) {
		perror("Failed to open the i2c bus\n");
		return false;
	}
	if (ioctl(fd, I2C_SLAVE, slave_address) < 0) {
		perror("Failed to acquire bus access and/or talk to slave.\n");
		return false;
	}
#else
#endif
	return true;
}

bool ceI2C::Write(char* buf, int n)
{
#ifdef CE_LINUX
	if (write(fd, buf, n) != n) {
		perror("Failed to write to the i2c bus.\n");
		return false;
	}
#else
#endif
	return true;
}

bool ceI2C::Read(char* buf, int n)
{
#ifdef CE_LINUX
	if (read(fd, buf, n) != n) {
		perror("Failed to read from the i2c bus.\n");
		return false;
	}
#else
#endif
	return true;
}

void ceI2C::Close()
{
#ifdef CE_LINUX
	close(fd);
#else
#endif
}

} // namespace ce 