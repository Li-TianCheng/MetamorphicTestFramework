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
        virtual std::shared_ptr<GenSourceCase<T>> clone() const & = 0;
        virtual std::shared_ptr<GenSourceCase<T>> clone() && = 0;
    };
}


#endif //METAMORPHICTESTFRAMEWORK_GENSOURCECASE_H
