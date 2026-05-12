#pragma once

#if __has_include(<json/json.h>)
#include <json/json.h>
#elif __has_include(<jsoncpp/json/json.h>)
#include <jsoncpp/json/json.h>
#else
#error "JsonCpp headers were not found"
#endif
