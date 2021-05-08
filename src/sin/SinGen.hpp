//
// Created by ltc on 2/5/21.
//

#ifndef METAMORPHICTESTFRAMEWORK_SINGEN_HPP
#define METAMORPHICTESTFRAMEWORK_SINGEN_HPP

#include "GenSourceCase.h"

class SinGen : public metamorphicTestFramework::GenSourceCase<double>{
public:
    double genSourceCase() override{
        return rand()/1000.0;
    };

    ~SinGen() noexcept override = default;
};


#endif //METAMORPHICTESTFRAMEWORK_SINGEN_HPP
