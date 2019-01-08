#include "Utils.h"

/**
 *  Procces input string into picojson::value object.
 * 
 * @param response String with json-formatted text.
 * @return picojson::value Created picojson::value object.
*/
picojson::value getJson(const std::string& str)
{
    /// Variable to store parsed string 
    picojson::value jsonValue;
    /// picojson::pase got in result variable as first param and input string as second
    picojson::parse(jsonValue, str);
    return jsonValue;
}