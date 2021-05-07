#include <iostream>
#include <memory>
#include "MetamorphicTest.h"
#include "src/sin/SinGen.hpp"
#include "src/sin/SinMR1.hpp"
#include "src/sin/SinProgram.hpp"

int main() {
    using namespace metamorphicTestFramework;
    using namespace std;

    const int threadNum = 12;
    const int querySize = 1000;
    ThreadPool threadPool(threadNum, querySize);
    // sin test
    vector<shared_ptr<MR<double, double>>> sinMrs;
    shared_ptr<GenSourceCase<double>> sinGenerator(new SinGen());
    shared_ptr<MR<double, double>> sinMr1(new SinMR1());
    sinMrs.push_back(sinMr1);
    shared_ptr<Program<double, double>> sinProgram(new SinProgram());
    MetamorphicTest<double, double> sinTest(1000, 2000, sinGenerator,
                                            sinProgram, sinMrs, &threadPool);
    sinTest.metamorphicTest();
    auto re = sinTest.getMetamorphicTestResult();
    cout << "hello world" << endl;
    return 0;
}
