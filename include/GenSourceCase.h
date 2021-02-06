//
// Created by ltc on 2/5/21.
//

#ifndef METAMORPHICTESTFRAMEWORK_GENSOURCECASE_H
#define METAMORPHICTESTFRAMEWORK_GENSOURCECASE_H

#include "AutoPoint.h"

namespace metamorphicTestFramework{
    template <typename T>
    class GenSourceCase {
    public:
        virtual T genSourceCase() = 0;
        virtual ~GenSourceCase() = default;
        virtual AutoPoint<GenSourceCase<T>> clone() const & = 0;
        virtual AutoPoint<GenSourceCase<T>> clone() && = 0;
    };
}


#endif //METAMORPHICTESTFRAMEWORK_GENSOURCECASE_H
