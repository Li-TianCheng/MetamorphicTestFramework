//
// Created by ltc on 2/4/21.
//

#ifndef METAMORPHICTESTFRAMEWORK_PROGRAM_H
#define METAMORPHICTESTFRAMEWORK_PROGRAM_H


namespace metamorphicTestFramework{
    template <typename T, typename U>
    class Program {
    public:
        virtual U genResult(T& testCase) = 0;
        virtual ~Program() = default;
    };
}


#endif //METAMORPHICTESTFRAMEWORK_PROGRAM_H
