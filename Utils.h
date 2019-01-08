#pragma once

#include "picojson.h"

#include <string>

picojson::value getJson(const std::string& str); ///< Get parse string to picojson::value