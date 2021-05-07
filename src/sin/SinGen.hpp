//
// Created by ltc on 2/5/21.
//

#ifndef METAMORPHICTESTFRAMEWORK_SINGEN_HPP
#define METAMORPHICTESTFRAMEWORK_SINGEN_HPP

#include "GenSourceCase.h"
#include <cstdlib>

using std::shared_ptr;

class SinGen : public metamorphicTestFramework::GenSourceCase<double>{
public:
    double genSourceCase() override{
        return rand()/1000.0;
    };

    ~SinGen() noexcept override = default;
    shared_ptr<GenSourceCase> clone() const & override{
        return shared_ptr<GenSourceCase>(new SinGen(*this));
    };

    shared_ptr<GenSourceCase> clone() && override{
        return shared_ptr<GenSourceCase>(new SinGen(*this));
    };
};


#endif //METAMORPHICTESTFRAMEWORK_SINGEN_HPP
