//
// Created by ltc on 2/4/21.
//

#ifndef METAMORPHICTESTFRAMEWORK_METAMORPHICTEST_HPP
#define METAMORPHICTESTFRAMEWORK_METAMORPHICTEST_HPP

#include <cstddef>
#include <vector>
#include "MR.h"
#include "Program.h"
#include "GenSourceCase.h"
#include "lib_pthread/include/ThreadPool.h"
#include "lib_pthread/include/Latch.h"

namespace metamorphicTestFramework{
    using std::vector;
    using std::move;
    using std::shared_ptr;
    using std::tuple;

    template <typename T, typename U>
    class MetamorphicTest final{
    public:
        MetamorphicTest(size_t sourceCaseNum, size_t followCaseNum,
                        shared_ptr<GenSourceCase<T>>& generator, shared_ptr<Program<T, U>>& program,
                        vector<shared_ptr<MR<T, U>>>& mrs, ThreadPool* threadPool);
        MetamorphicTest(const MetamorphicTest& m) = delete;
        MetamorphicTest(MetamorphicTest&& m) = delete;
        MetamorphicTest& operator=(const MetamorphicTest& m) = delete;
        MetamorphicTest& operator=(MetamorphicTest&& m) = delete;
        ~MetamorphicTest() = default;
        void metamorphicTest();
        vector<double> analyzeResult(vector<double> (*f)(vector<vector<vector<bool>>>&));
        vector<vector<vector<bool>>> getMetamorphicTestResult() const;
    private:
        static void sourceCaseTask(void* arg);
        static void followCaseTask(void* arg);
    private:
        Latch sourceCaseLatch;
        Latch followCaseLatch;
        ThreadPool* threadPool;
        size_t sourceCaseNum;
        size_t followCaseNum;
        shared_ptr<GenSourceCase<T>> generator;
        shared_ptr<Program<T, U>> program;
        vector<shared_ptr<MR<T, U>>> mrs;
        vector<T> sourceCases;
        vector<vector<vector<T>>> followCases;
        vector<U> sourceCaseResults;
        vector<vector<vector<U>>> followCaseResults;
        vector<vector<vector<bool>>> metamorphicTestResults;
    };

    template <typename T, typename U> inline
    MetamorphicTest<T, U>::MetamorphicTest(size_t sourceCaseNum,
                                           size_t followCaseNum,
                                           shared_ptr<GenSourceCase<T>>& generator,
                                           shared_ptr<Program<T, U>>& program,
                                           vector<shared_ptr<MR<T, U>>>& mrs,
                                           ThreadPool* threadPool)
    : sourceCaseNum(sourceCaseNum), followCaseNum(followCaseNum), generator(generator), program(program), mrs(mrs),
      sourceCases(sourceCaseNum), followCases(mrs.size(), vector<vector<T>>(sourceCaseNum, vector<T>(followCaseNum))),
      sourceCaseResults(sourceCaseNum),
      followCaseResults(mrs.size(), vector<vector<U>>(sourceCaseNum, vector<U>(followCaseNum))),
      metamorphicTestResults(mrs.size(), vector<vector<bool>>(sourceCaseNum, vector<bool>(followCaseNum))),
      threadPool(threadPool), sourceCaseLatch(sourceCaseNum), followCaseLatch(mrs.size()*sourceCaseNum*followCaseNum){}

    template <typename T, typename U> inline
    void MetamorphicTest<T, U>::metamorphicTest() {
        for (int i = 0; i < sourceCaseNum; i++){
            auto* arg = new tuple<MetamorphicTest*, int>(this, i);
            threadPool->addTask(&this->sourceCaseTask, arg);
        }
        sourceCaseLatch.wait();
        for (int i = 0; i < mrs.size(); i++){
            for (int j = 0; j < sourceCaseNum; j++){
                for (int k = 0; k < followCaseNum; k++){
                    auto* arg = new tuple<MetamorphicTest*, int, int, int>(this, i, j, k);
                    threadPool->addTask(&this->followCaseTask, arg);
                }
            }
        }
        followCaseLatch.wait();
    }

    template <typename T, typename U> inline
    vector<double> MetamorphicTest<T, U>::analyzeResult(vector<double> (*f)(vector<vector<vector<bool>>> &)) {
        return f(metamorphicTestResults);
    }

    template<typename T, typename U> inline
    vector<vector<vector<bool>>> MetamorphicTest<T, U>::getMetamorphicTestResult() const {
        return metamorphicTestResults;
    }

    template<typename T, typename U>
    void MetamorphicTest<T, U>::sourceCaseTask(void *arg) {
        tuple<MetamorphicTest<T, U>*, int> args = *(tuple<MetamorphicTest<T, U>*, int>*)arg;
        MetamorphicTest* m = std::get<0>(args);
        int i = std::get<1>(args);
        m->sourceCases[i] = m->generator->genSourceCase();
        m->sourceCaseResults[i] = m->program->genResult(m->sourceCases[i]);
        delete (tuple<MetamorphicTest<T, U>*, int>*)arg;
        m->sourceCaseLatch.done();
    }

    template<typename T, typename U>
    void MetamorphicTest<T, U>::followCaseTask(void *arg) {
        tuple<MetamorphicTest<T, U>*, int, int, int> args = *(tuple<MetamorphicTest<T, U>*, int, int, int>*)arg;
        MetamorphicTest* m = std::get<0>(args);
        int i = std::get<1>(args);
        int j = std::get<2>(args);
        int k = std::get<3>(args);
        m->followCases[i][j][k] = m->mrs[i]->genFollowCase(m->sourceCases[j], m->sourceCaseResults[j]);
        m->followCaseResults[i][j][k] = m->program->genResult(m->followCases[i][j][k]);
        m->metamorphicTestResults[i][j][k] = m->mrs[i]->mrRelation(m->sourceCaseResults[j],
                                                             m->followCaseResults[i][j][k],
                                                             m->sourceCases[j],
                                                             m->followCases[i][j][k]);
        delete (tuple<MetamorphicTest<T, U>*, int>*)arg;
        m->followCaseLatch.done();
    }
}


#endif //METAMORPHICTESTFRAMEWORK_METAMORPHICTEST_HPP
