//
// Created by ltc on 2/4/21.
//

#ifndef METAMORPHICTESTFRAMEWORK_METAMORPHICTEST_H
#define METAMORPHICTESTFRAMEWORK_METAMORPHICTEST_H

#include <cstddef>
#include <vector>
#include "MR.h"
#include "Program.h"
#include "GenSourceCase.h"
#include "lib_pthread/include/ThreadPool.h"

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
        MetamorphicTest(const MetamorphicTest& m);
        MetamorphicTest(MetamorphicTest&& m) noexcept;
        MetamorphicTest& operator=(const MetamorphicTest& m);
        MetamorphicTest& operator=(MetamorphicTest&& m) noexcept;
        ~MetamorphicTest() = default;
        void metamorphicTest();
        vector<double> analyzeResult(vector<double> (*f)(vector<vector<vector<bool>>>&));
        vector<vector<vector<bool>>> getMetamorphicTestResult() const;
    private:
        static void sourceCaseTask(void* arg);
        static void followCaseTask(void* arg);
    private:
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
      threadPool(threadPool){}

    template<typename T, typename U> inline
    MetamorphicTest<T, U>::MetamorphicTest(const MetamorphicTest& m)
            : sourceCaseNum(m.sourceCaseNum), followCaseNum(m.followCaseNum), mrs(m.mrs),
              generator(m.generator->clone()), program(m.program->clone()),
              sourceCases(m.sourceCases), followCases(m.followCases),
              sourceCaseResults(m.sourceCaseResults), followCaseResults(m.followCaseResults),
              metamorphicTestResults(m.metamorphicTestResults),threadPool(m.threadPool){
        for (int i = 0; i < mrs.size(); i++) {
            mrs[i] = m.mrs[i]->clone();
        }
    }

    template<typename T, typename U> inline
    MetamorphicTest<T, U>::MetamorphicTest(MetamorphicTest &&m) noexcept
    : sourceCaseNum(m.sourceCaseNum), followCaseNum(m.followCaseNum), generator(move(m.generator)),
      program(move(m.program)), mrs(move(m.mrs)), sourceCases(move(m.sourceCases)),
      followCases(move(m.followCases)), sourceCaseResults(move(m.sourceCaseResults)),
      followCaseResults(move(m.followCaseResults)), metamorphicTestResults(move(m.metamorphicTestResults)),
      threadPool(m.threadPool){}

    template<typename T, typename U> inline
    MetamorphicTest<T, U> &MetamorphicTest<T, U>::operator=(const MetamorphicTest &m) {
        if(this == &m){
            return *this;
        }
        sourceCaseNum = m.sourceCaseNum;
        followCaseNum = m.followCaseNum;
        sourceCases = m.sourceCases;
        followCases = m.followCases;
        sourceCaseResults = m.sourceCaseResults;
        followCaseResults = m.followCaseResults;
        metamorphicTestResults = m.metamorphicTestResults;
        mrs = m.mrs;
        threadPool = m.threadPool;
        for (int i = 0; i < mrs.size(); i++) {
            mrs[i] = m.mrs[i]->clone();
        }
        generator = m.generator->clone();
        program = m.program->clone();
        return *this;
    }

    template<typename T, typename U> inline
    MetamorphicTest<T, U> &MetamorphicTest<T, U>::operator=(MetamorphicTest &&m) noexcept {
        sourceCaseNum = m.sourceCaseNum;
        followCaseNum = m.followCaseNum;
        generator = move(m.generator);
        program = move(m.program);
        mrs = move(m.mrs);
        sourceCases = move(m.sourceCases);
        followCases = move(m.followCases);
        sourceCaseResults = move(m.sourceCaseResults);
        followCaseResults = move(m.followCaseResults);
        metamorphicTestResults = move(m.metamorphicTestResults);
        threadPool = m.threadPool;
        return *this;
    }

    template <typename T, typename U> inline
    void MetamorphicTest<T, U>::metamorphicTest() {
        for (int i = 0; i < sourceCaseNum; i++){
            auto* arg = new tuple<MetamorphicTest*, int>(this, i);
            threadPool->addTask(&this->sourceCaseTask, arg);
        }
        while (threadPool->getRunningNum() != 0){}
        for (int i = 0; i < mrs.size(); i++){
            for (int j = 0; j < sourceCaseNum; j++){
                for (int k = 0; k < followCaseNum; k++){
                    auto* arg = new tuple<MetamorphicTest*, int, int, int>(this, i, j, k);
                    threadPool->addTask(&this->followCaseTask, arg);
                }
            }
        }
        threadPool->join();
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
    }
}


#endif //METAMORPHICTESTFRAMEWORK_METAMORPHICTEST_H
