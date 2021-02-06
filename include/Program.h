//
// Created by ltc on 2/4/21.
//

#ifndef METAMORPHICTESTFRAMEWORK_PROGRAM_H
#define METAMORPHICTESTFRAMEWORK_PROGRAM_H

#include "AutoPoint.h"

namespace metamorphicTestFramework{
    template <typename T, typename U>
    class Program {
    public:
        virtual U genResult(T& testCase) = 0;
        virtual ~Program() = default;
        virtual AutoPoint<Program<T, U>> clone() const & = 0;
        virtual AutoPoint<Program<T, U>> clone() && = 0;
    };
}


#endif //METAMORPHICTESTFRAMEWORK_PROGRAM_H
