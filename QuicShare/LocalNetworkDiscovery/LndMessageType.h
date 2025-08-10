#pragma once

#include <cstdint>
#include <json_struct/json_struct.h>

JS_ENUM(LndMessageType, Unknown, Announce)
JS_ENUM_DECLARE_STRING_PARSER(LndMessageType)
