// File: ceI2C.h
// Description: ceI2C class to use i2c communication
// WebSite: http://cool-emerald.blogspot.com
// MIT License (https://opensource.org/licenses/MIT)
// Copyright (c) 2018 Yan Naing Aye

// Reference : http://elinux.org/Interfacing_with_I2C_Devices

#ifndef CEI2C_H
#define CEI2C_H

#if defined(_WIN64) || defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)
    #ifndef CE_WINDOWS
        #define CE_WINDOWS
    #endif
#else
    #ifndef CE_LINUX
        #define CE_LINUX
    #endif
    #include <unistd.h>
    #include <linux/i2c-dev.h>
    #include <sys/ioctl.h>
    #include <fcntl.h>
#endif

#include <string>
#include <stdio.h>
#include <stdlib.h>
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

} // namespace ce 
#endif // CEI2C_H
