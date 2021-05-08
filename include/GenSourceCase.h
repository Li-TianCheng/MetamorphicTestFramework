//
// Created by ltc on 2/5/21.
//

#ifndef METAMORPHICTESTFRAMEWORK_GENSOURCECASE_H
#define METAMORPHICTESTFRAMEWORK_GENSOURCECASE_H


namespace metamorphicTestFramework{
    template <typename T>
    class GenSourceCase {
    public:
        virtual T genSourceCase() = 0;
        virtual ~GenSourceCase() = default;
    };
}


#endif //METAMORPHICTESTFRAMEWORK_GENSOURCECASE_H
