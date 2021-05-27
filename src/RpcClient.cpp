//
// Created by ltc on 2021/5/26.
//

#include "RpcClient.h"
#include <iostream>
RpcClient::RpcClient(const string &address, AddressType addressType):tcpClient(address, addressType) {}

vector<string> RpcClient::callFunction(const string &name, const string &type, vector<string> &args) {
    string cmd = "$START$FUNCTION$";
    cmd += type + ":" + name + "$";
    for (auto &arg : args) {
        cmd += arg + "$";
    }
    cmd += "END$$$$";
    tcpClient.connect();
    tcpClient.write(cmd);
    string result;
    while (true) {
        string temp = tcpClient.read();
        if (temp.empty()){
            return vector<string>();
        }
        result += temp;
        if (result.find("$END$") != string::npos) {
            tcpClient.close();
            break;
        }
    }
    int posStart = result.find("$START$");
    if (posStart == string::npos) {
        return vector<string>();
    }
    int posEnd = result.find("$END$");
    result = result.substr(posStart, posEnd-posStart+5);
    vector<string> split = utils::split(result, '$');
    if (split[1] == "RESULT") {
        return vector<string>(split.begin()+2, split.end()-1);
    }
    return vector<string>();
}
