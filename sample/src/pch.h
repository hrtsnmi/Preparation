#pragma once

//disable all warnings
#ifdef _HAS_EXCEPTIONS
#undef _HAS_EXCEPTIONS
#endif

#ifndef _HAS_EXCEPTIONS
#define _HAS_EXCEPTIONS 0
#endif


// include stl headers
#include <string>
#include <vector>
#include <array>
#include <set>
#include <unordered_set>
#include <list>
#include <forward_list>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>

#include <functional> // need for std::greater
#include <memory> // smart pointers

#include <iostream>
#include <fstream>
#include <sstream>

#include <stdio.h>
#include <stdarg.h>

// std threads
#include <thread>
#include <mutex>

#include <Windows.h>

// include math
#include "Math/int2.h"
#include "Math/float3.h"

using Math::float3;


class Application;
extern Application* kApp;