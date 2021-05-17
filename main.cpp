#include <iostream>
#include <memory>
#include "MetamorphicTest.hpp"
#include "src/sin/SinGen.hpp"
#include "src/sin/SinMR1.hpp"
#include "src/sin/SinProgram.hpp"
#include "modules/time_system/include/TimeSystem.h"
#include "modules/task_system/include/TaskSystem.h"

int main() {
    using namespace metamorphicTestFramework;
    using namespace std;

    ObjPool::init();
    TimeSystem::init();
    TaskSystem::init();

    // sin test
    vector<shared_ptr<MR<double, double>>> sinMrs;
    shared_ptr<GenSourceCase<double>> sinGenerator(new SinGen());
    shared_ptr<MR<double, double>> sinMr1(new SinMR1());
    sinMrs.push_back(sinMr1);
    shared_ptr<Program<double, double>> sinProgram(new SinProgram());
    MetamorphicTest<double, double> sinTest(10000, 20, sinGenerator,sinProgram, sinMrs);
    sinTest.metamorphicTest();
    auto re = sinTest.getMetamorphicTestResult();

    TaskSystem::close();
    TimeSystem::close();
    ObjPool::close();
    cout << "hello world" << endl;
    return 0;
}
