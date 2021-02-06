//
// Created by ltc on 2/5/21.
//

#ifndef METAMORPHICTESTFRAMEWORK_MR1_HPP
#define METAMORPHICTESTFRAMEWORK_MR1_HPP

#include "MR.h"
#include <cstdlib>

#define PI 3.141592653589793
class MR1 : public metamorphicTestFramework::MR<double, double>{
public:
    double genFollowCase(double& sourceCase, double& sourceCaseResult) override{
        return sourceCase+2*PI*rand();
    };

    bool mrRelation(double &sourceCaseResult, double &followCaseResult) override{
        return abs(sourceCaseResult-followCaseResult) < 1e-3;
    };

    ~MR1() noexcept override = default;

    AutoPoint<MR> clone() const & override{
        return AutoPoint<MR>(new MR1(*this));
    }

    AutoPoint<MR> clone() && override{
        return AutoPoint<MR>(new MR1(*this));
    }
};


#endif //METAMORPHICTESTFRAMEWORK_MR1_HPP
