/////////////////////////////////////////////////////////////////////////////
// File: cei2c.h
// Description: ceI2C class to use i2c communication
// WebSite: http://cool-emerald.blogspot.com
// MIT License (https://opensource.org/licenses/MIT)
// Copyright (c) 2018 Yan Naing Aye

// Reference : http://elinux.org/Interfacing_with_I2C_Devices
/////////////////////////////////////////////////////////////////////////////

#ifndef CEI2C_H
#define CEI2C_H

#define CE_DBG_PRINT 0 // print dbg mes

#ifndef CE_MACROS_H
#define CE_MACROS_H

#if defined(_WIN64) || defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__) || defined(__CYGWIN__)
#ifndef CE_WINDOWS
#define CE_WINDOWS
#endif
#elif defined(__linux__) || defined(unix) || defined(__unix) || defined(__unix__)
#ifndef CE_LINUX
#define CE_LINUX
#endif
#else
#ifndef CE_NOS
#define CE_NOS
#endif
#endif

#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64__) || defined(__amd64)
#ifndef CE_x86_64
#define CE_x86_64
#endif
#elif defined(__arm__) || defined(_M_ARM)
#ifndef CE_ARM
#define CE_ARM
#endif
#else
#ifndef CE_NARCH
#define CE_NARCH
#endif
#endif

#endif // CE_MACROS_H

#ifdef CE_LINUX
    #include <unistd.h>
    #include <linux/i2c-dev.h>
    #include <sys/ioctl.h>
    #include <fcntl.h>
#endif

#include <string>
#include <cstdio>
#include <cstdlib>
#include <sstream>

namespace ce {
class ceI2C
{
    public:
        ceI2C();
        ceI2C(int bus_id,int slave_address);
        ~ceI2C();
        bool Begin(int bus_id,int slave_address);
        bool Write(char* buf,int n);
        bool Read(char* buf,int n);
		void Close();
        template <typename T>
            std::string ToString(T Number);
    private:
        int fd;
};

/////////////////////////////////////////////////////////////////////////////
// Implementation

template <typename T>
std::string ceI2C::ToString(T a)
{
	std::ostringstream ss;
	ss << a;
	return ss.str();
}

inline ceI2C::ceI2C()
{
	//ctor
}

inline ceI2C::ceI2C(int bus_id, int slave_address)
{
	Begin(bus_id, slave_address);
}

inline ceI2C::~ceI2C()
{
	//dtor
#ifdef CE_LINUX
	close(fd);
#else
#endif
}

inline bool ceI2C::Begin(int bus_id, int slave_address)
{
	std::string filename = "/dev/i2c-";
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

inline bool ceI2C::Write(char* buf, int n)
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

inline bool ceI2C::Read(char* buf, int n)
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

inline void ceI2C::Close()
{
#ifdef CE_LINUX
	close(fd);
#else
#endif
}

/////////////////////////////////////////////////////////////////////////////

} // namespace ce 
#endif // CEI2C_H
