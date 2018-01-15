#pragma once

#pragma warning( disable : 4503 )
#pragma warning( disable : 4290 )


#define MSVC_PUSH_DISABLE_WARNING(n) __pragma(warning(push)) \
                                     __pragma(warning(disable:n))
#define MSVC_POP_WARNING() __pragma(warning(pop))
#define ALLOW_THIS_IN_INITIALIZER_LIST(code) MSVC_PUSH_DISABLE_WARNING(4355) \
                                             code \
                                             MSVC_POP_WARNING()


#define WIN32_LEAN_AND_MEAN

#define NOMINMAX

#define _USE_MATH_DEFINES


#include <assert.h>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream> 
#include <stdio.h>
#include <string>
