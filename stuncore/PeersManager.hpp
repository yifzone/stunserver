//
//  PeersManager.hpp
//  StunServerDemo
//
//  Created by yifzone on 2020/3/25.
//  Copyright © 2020 com.hunantv. All rights reserved.
//

#ifndef PeersManager_hpp
#define PeersManager_hpp

#include <stdio.h>
#include <sys/time.h>

#include <map>
#include <list>
#include <string>

#include "PeerInfo.hpp"

using namespace std;
class PeersManager {
public:
    map<string, list<PeerInfo *> > _answerMap;
    
    static PeersManager *shared() {
        static PeersManager instance;
        return &instance;
    }
    
    static int64_t getMSTimestamp() {
        struct timeval tv = {0};
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000 + tv.tv_usec / 1000;
    }
    
    void updatePeer(const string &peerId, const string &jsonInfo) {
        if (peerId.length() == 0 || jsonInfo.length() == 0) {
            return;
        }
        
        PeerInfo *peer = NULL;
        map<string, PeerInfo *>::iterator iter = _peersMap.find(peerId);
        if (iter != _peersMap.end()) {
            peer = iter->second;
        } else {
            peer = new PeerInfo();
        }
        
        *peer = PeerInfo::fromJsonString(jsonInfo);
        peer->_timestamp = PeersManager::getMSTimestamp();
        
        _peersMap[peerId] = peer;
    }
    
    PeerInfo * queryPeer(const string &peerId) {
        map<string, PeerInfo *>::iterator iter = _peersMap.find(peerId);
        if (iter == _peersMap.end()) {
            return NULL;
        }
        
        return iter->second;
    }
    
    void checkInactivePeers(int64_t currentTimestamp) {
        map<string, PeerInfo *>::iterator iter = _peersMap.begin();
        map<string, PeerInfo *>::iterator end = _peersMap.end();
        for (; iter != end; ) {
            PeerInfo *info = iter->second;
            if (!info || currentTimestamp - info->_timestamp > 1000*60*3) {
                //peer3分钟没有上报心跳则表示peer已经离线，需要移除
                iter = _peersMap.erase(iter);
            } else {
                ++iter;
            }
        }
    }
    
    list<PeerInfo *> getPeersForOffer(string offerPeerId) {
        list<PeerInfo *> peers;
        map<string, PeerInfo *>::iterator iter = _peersMap.begin();
        map<string, PeerInfo *>::iterator end = _peersMap.end();
        for (; iter != end; ++iter) {
            PeerInfo *peer = iter->second;
            if (peer && iter->first != offerPeerId) {
                peers.push_back(peer);
            }
        }
        
        return peers;
    }
    
    string peersToJsonString(list<PeerInfo *> &peers) {
        string ret = "";
        list<PeerInfo *>::iterator iter = peers.begin();
        list<PeerInfo *>::iterator end = peers.end();
        for (; iter != end; ++iter) {
            PeerInfo *peer = *iter;
            if (peer) {
                if (ret.length() > 0) {
                    ret += ",";
                }
                
                ret += peer->toJsonString();
            }
        }
        
        return string("[")+ret+string("]");
    }
    
    void pushAnswerSession(string offerPeerId, list<PeerInfo *> &answerPeers) {
        _answerMap[offerPeerId] = answerPeers;
    }
    
private:
    PeersManager(){};
    ~PeersManager(){};
    
    map<string, PeerInfo *> _peersMap;
};

#endif /* PeersManager_hpp */
