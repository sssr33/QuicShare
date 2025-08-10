#pragma once
#include "LndMessageType.h"

#include <json_struct/json_struct.h>

struct LndMessage {
    LndMessageType type = LndMessageType::Unknown;

    JS_OBJ(type);
};
