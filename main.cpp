#include <iostream>
#include <memory>
#include "MetamorphicTest.hpp"
#include "src/sin/SinGen.hpp"
#include "src/sin/SinMR1.hpp"
#include "src/sin/SinProgram.hpp"
#include "Modules.h"
#include "my_sql/include/MySql.h"

int main() {
    using namespace metamorphicTestFramework;
    using namespace std;

    modules::init();
    const string userName = "root";
    const string password = "ltc970722";
    const string database = "metamorphic_test_result";
    const string host = "localhost";
    MySql mysql(userName, password, database, host);
    mysql.connect();

    // sin test
    vector<shared_ptr<MR<double, double>>> sinMrs;
    shared_ptr<GenSourceCase<double>> sinGenerator(new SinGen());
    shared_ptr<MR<double, double>> sinMr1(new SinMR1());
    sinMrs.push_back(sinMr1);
    shared_ptr<Program<double, double>> sinProgram(new SinProgram());
    MetamorphicTest<double, double> sinTest("sin", 10, 20, sinGenerator,sinProgram, sinMrs, &mysql);
    sinTest.metamorphicTest();
    auto re = sinTest.getMetamorphicTestResult();
    mysql.close();
    modules::close();
    return 0;
}
