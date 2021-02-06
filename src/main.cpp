#include <iostream>
#include "MetamorphicTest.h"
#include "./sin/Gen.hpp"
#include "./sin/MR1.hpp"
#include "./sin/SinProgram.hpp"

int main() {
    using namespace metamorphicTestFramework;
    vector<AutoPoint<MR<double, double>>> mrs;
    AutoPoint<GenSourceCase<double>> generator(new Gen());
    AutoPoint<MR<double, double>> mr1(new MR1());
    mrs.push_back(mr1);
    AutoPoint<Program<double, double>> program(new SinProgram());
    MetamorphicTest<double, double> m(10, 20, generator, program, mrs);
    MetamorphicTest<double, double> m1(move(m));
    m1.metamorphicTest();
    return 0;
}
