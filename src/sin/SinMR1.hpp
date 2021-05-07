//
// Created by ltc on 2/5/21.
//

#ifndef METAMORPHICTESTFRAMEWORK_SINMR1_HPP
#define METAMORPHICTESTFRAMEWORK_SINMR1_HPP

#include "MR.h"
#include <cstdlib>

#define PI 3.141592653589793

using std::shared_ptr;

class SinMR1 : public metamorphicTestFramework::MR<double, double>{
public:
    double genFollowCase(double& sourceCase, double& sourceCaseResult) override{
        return sourceCase+2*PI*rand();
    };

    bool mrRelation(double &sourceCaseResult, double &followCaseResult, double& sourceCase, double& followCase) override{
        return std::abs(sourceCaseResult-followCaseResult) < 1e-3;
    };

    ~SinMR1() noexcept override = default;

    shared_ptr<MR> clone() const & override{
        return shared_ptr<MR>(new SinMR1(*this));
    }

    shared_ptr<MR> clone() && override{
        return shared_ptr<MR>(new SinMR1(*this));
    }
};


#endif //METAMORPHICTESTFRAMEWORK_SINMR1_HPP
