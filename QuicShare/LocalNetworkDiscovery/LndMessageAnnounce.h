#pragma once
#include "LndMessage.h"

#include <string>

struct LndMessageAnnounce : public LndMessage {
    LndMessageAnnounce() {
        type = LndMessageType::Announce;
    }

    std::string peerId;
    uint16_t quicPort = 0;

    JS_OBJ_SUPER(JS_SUPER(LndMessage), peerId, quicPort);
};
