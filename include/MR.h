//
// Created by ltc on 2/4/21.
//

#ifndef METAMORPHICTESTFRAMEWORK_MR_H
#define METAMORPHICTESTFRAMEWORK_MR_H

#include "AutoPoint.h"

namespace metamorphicTestFramework{
    template <typename T, typename U>
    class MR {
    public:
        virtual T genFollowCase(T& sourceCase, U& sourceCaseResult) = 0;
        virtual bool mrRelation(U& sourceCaseResult, U& followCaseResult) = 0;
        virtual ~MR() = default;
        virtual AutoPoint<MR<T, U>> clone() const & = 0;
        virtual AutoPoint<MR<T, U>> clone() && = 0;
    };
}


#endif //METAMORPHICTESTFRAMEWORK_MR_H
