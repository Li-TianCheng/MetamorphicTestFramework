//
// Created by ltc on 2/5/21.
//

#ifndef METAMORPHICTESTFRAMEWORK_SINPROGRAM_HPP
#define METAMORPHICTESTFRAMEWORK_SINPROGRAM_HPP

#include "Program.h"
#include <cmath>
#include "RpcClient.h"

class SinProgram : public metamorphicTestFramework::Program<double, double> {
public:
    double genResult(double &testCase) override{
        vector<string> result;
        for (int i = 0; i < 5; i++) {
            RpcClient client("localhost:8000", IPV4);
            client.connect();
            vector<string> args;
            args.push_back("--i "+to_string(testCase));
            result = client.callFunction("sin", "python", args);
            client.close();
            if (!result.empty()){
                break;
            }
        }
        if (result.empty()) {
            throw std::runtime_error("call function failed");
        }
        return stod(result[0]);
    };

    ~SinProgram() noexcept override = default;
};


#endif //METAMORPHICTESTFRAMEWORK_SINPROGRAM_HPP
