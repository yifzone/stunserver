//
//  PeerInfo.cpp
//  EasyStunClient
//
//  Created by yifzone on 2020/3/26.
//  Copyright © 2020 com.hunantv. All rights reserved.
//

#include "PeerInfo.hpp"
#include "cJSON.h"

string PeerInfo::toJsonString() const {
    char buf[512] = {0};
    snprintf(buf, sizeof(buf), "{\"id\":\"%s\", \"natType\":%d, \"localAddr\":\"%s\", \"mappedAddr\":\"%s\"}",
             _id.c_str(), _natType, _localAddr.toStdString().c_str(), _mappedAddr.toStdString().c_str());
    return string(buf);
}

PeerInfo PeerInfo::fromJsonString(string jsonStr) {
    PeerInfo peer;
    cJSON *root = cJSON_Parse(jsonStr.c_str());
    if (!root) {
        return peer;
    }
    
    cJSON *item = NULL;
    item = cJSON_GetObjectItem(root, "id");
    if (item) {
        peer._id = item->valuestring;
    }
    item = cJSON_GetObjectItem(root, "natType");
    if (item) {
        peer._natType = item->valueint;
    }
    item = cJSON_GetObjectItem(root, "localAddr");
    if (item) {
        string addr = item->valuestring;
        peer._localAddr = CSocketAddress::fromStdString(addr);
    }
    item = cJSON_GetObjectItem(root, "mappedAddr");
    if (item) {
        string addr = item->valuestring;
        peer._mappedAddr = CSocketAddress::fromStdString(addr);
    }
    
    cJSON_Delete(root);
    return peer;
}
