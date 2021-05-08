//
// Created by ltc on 2/4/21.
//

#ifndef METAMORPHICTESTFRAMEWORK_MR_H
#define METAMORPHICTESTFRAMEWORK_MR_H


namespace metamorphicTestFramework{
    template <typename T, typename U>
    class MR {
    public:
        virtual T genFollowCase(T& sourceCase, U& sourceCaseResult) = 0;
        virtual bool mrRelation(U& sourceCaseResult, U& followCaseResult, T& sourceCase, T& followCase) = 0;
        virtual ~MR() = default;
    };
}


#endif //METAMORPHICTESTFRAMEWORK_MR_H
