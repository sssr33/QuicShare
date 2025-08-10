#pragma once

// order in which boost sees the correct value of _WIN32_WINNT
// https://stackoverflow.com/questions/9750344/boostasio-winsock-and-winsock-2-compatibility-issue
// https://github.com/boostorg/beast/issues/1895
#ifdef _WIN32
#include <winsock2.h>
#endif
#include <boost/asio.hpp>
