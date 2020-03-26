//
//  PeerInfo.hpp
//  EasyStunClient
//
//  Created by yifzone on 2020/3/26.
//  Copyright © 2020 com.hunantv. All rights reserved.
//

#ifndef PeerInfo_hpp
#define PeerInfo_hpp

#include <string>

#include "commonincludes.hpp"
#include "socketaddress.h"

using namespace std;
class PeerInfo {
public:
    PeerInfo(){};
    PeerInfo(const PeerInfo &info) {
        _natType = info._natType;
        _localAddr = info._localAddr;
        _mappedAddr = info._mappedAddr;
        _id = info._id;
    }
    
    int32_t _natType = 0;
    CSocketAddress _localAddr;
    CSocketAddress _mappedAddr;
    string _id = "";
    int64_t _timestamp = 0;
    
    bool isValid() {
        return (_localAddr.GetPort() != 0 || _mappedAddr.GetPort() != 0);
    }
    
    string toJsonString() const;
    static PeerInfo fromJsonString(string jsonStr);
};

#endif /* PeerInfo_hpp */
