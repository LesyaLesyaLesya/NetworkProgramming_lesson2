#pragma once

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <windows.h>
//#include <Winsock2.h>
//#include <ws2tcpip.h>

#include "socket_wrapper_impl.h"

#pragma comment(lib, "ws2_32.lib")

namespace socket_wrapper
{

    class SocketWrapperImpl : ISocketWrapperImpl
    {
    public:
        SocketWrapperImpl() : initialized_(false) {}

        void initialize()
        {
            WSADATA wsaData;
            // Initialize Winsock
            auto result = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (result != 0)
            {
                throw std::runtime_error(get_last_error_string());
            }
        }

        bool initialized() const
        {
            return initialized_;
        }

        void deinitialize()
        {
            WSACleanup();
        }

        int get_last_error_code() const
        {
            return WSAGetLastError();
            //return 0;
        }

        std::string get_last_error_string() const
        {
            // return std::strerror(std::errno);
            char* s = NULL;
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, get_last_error_code(), MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPWSTR)&s, 0, NULL);
            std::string result{ s };
            LocalFree(s);

            return result;
        };

    private:
        bool initialized_;
    };

}
