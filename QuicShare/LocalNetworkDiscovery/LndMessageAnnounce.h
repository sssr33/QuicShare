#pragma once
#include "LndMessage.h"

#include <string>

struct LndMessageAnnounce : public LndMessage {
    LndMessageAnnounce() {
        type = LndMessageType::Announce;
    }

    std::string peerId;

    JS_OBJ_SUPER(JS_SUPER(LndMessage), peerId);
};
