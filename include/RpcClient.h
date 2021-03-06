//
// Created by ltc on 2021/5/26.
//

#ifndef METAMORPHICTESTFRAMEWORK_RPCCLIENT_H
#define METAMORPHICTESTFRAMEWORK_RPCCLIENT_H

#include "net/include/TcpClient.h"

class RpcClient : public TcpClient {
public:
    RpcClient(const string& address, AddressType addressType);
    vector<string> callFunction(const string &name, const string& type, vector<string>& args);
};


#endif //METAMORPHICTESTFRAMEWORK_RPCCLIENT_H
