#include <ctime>
#include <iostream>
#include <string>
#include "json.hpp"
#include "avm_common.h"

#ifdef __cplusplus // be wary of compiling with a C++ compiler
extern "C" {
#endif
    void json_parse_message(char *s);

#ifdef __cplusplus
}
#endif

using json = nlohmann::json;
void json_parse_message(char *s)
{
    std::string str(s);
    try {
        json j = json::parse(s);
        std::string type = j.count("type") ? j["type"].get<std::string>() : "";
        std::string value = j.count("value") ? j["value"].get<std::string>() : "";
        int direction = j.count("direction") ? j["direction"].get<int>() : 0;

        if (type == "layout") {
            std::cout << "type:" << type  << " value:" << value << " direction:" << direction << std::endl;
            if (value == "default") {
                avm_ui_layout = AVM_UI_LAYOUT_DEFAULT;
            } else if (value == "bird") {
                avm_ui_layout = AVM_UI_LAYOUT_BIRD;
            } else if (value == "3d") {
                avm_ui_layout = AVM_UI_LAYOUT_3D;
            }
            avm_ui_direction = direction;
            if (avm_ui_direction > AVM_UI_DIRECTION_RIGHT)
                avm_ui_direction = AVM_UI_DIRECTION_RIGHT;
        }
    } catch (nlohmann::detail::parse_error e) {
        std::cout << "parse error!" << std::endl;
    }
}
