//
// Created by ltc on 2/5/21.
//

#ifndef METAMORPHICTESTFRAMEWORK_GEN_HPP
#define METAMORPHICTESTFRAMEWORK_GEN_HPP

#include "GenSourceCase.h"
#include <cstdlib>

class Gen : public metamorphicTestFramework::GenSourceCase<double>{
public:
    double genSourceCase() override{
        return rand()/1000.0;
    };

    ~Gen() noexcept override = default;
    AutoPoint<GenSourceCase> clone() const & override{
        return AutoPoint<GenSourceCase>(new Gen(*this));
    };

    AutoPoint<GenSourceCase> clone() && override{
        return AutoPoint<GenSourceCase>(new Gen(*this));
    };
};


#endif //METAMORPHICTESTFRAMEWORK_GEN_HPP
