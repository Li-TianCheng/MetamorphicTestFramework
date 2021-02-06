//
// Created by ltc on 2/5/21.
//

#ifndef METAMORPHICTESTFRAMEWORK_SINPROGRAM_HPP
#define METAMORPHICTESTFRAMEWORK_SINPROGRAM_HPP

#include "Program.h"
#include <cmath>

class SinProgram : public metamorphicTestFramework::Program<double, double> {
public:
    double genResult(double &testCase) override{
        return sin(testCase);
    };

    ~SinProgram() noexcept override = default;

    AutoPoint<Program> clone() const & override{
        return AutoPoint<Program>(new SinProgram(*this));
    };
    AutoPoint<Program> clone() && override{
        return AutoPoint<Program>(new SinProgram(*this));
    };
};


#endif //METAMORPHICTESTFRAMEWORK_SINPROGRAM_HPP
