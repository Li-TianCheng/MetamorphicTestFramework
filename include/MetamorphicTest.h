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
#include "AutoPoint.h"

namespace metamorphicTestFramework{
    using std::vector;
    using std::move;

    template <typename T, typename U>
    class MetamorphicTest final{
    public:
        MetamorphicTest(size_t sourceCaseNum, size_t followCaseNum,
                        AutoPoint<GenSourceCase<T>>& generator, AutoPoint<Program<T, U>>& program,
                        vector<AutoPoint<MR<T, U>>>& mrs);
        MetamorphicTest(const MetamorphicTest& m);
        MetamorphicTest(MetamorphicTest&& m) noexcept;
        MetamorphicTest& operator=(const MetamorphicTest& m);
        MetamorphicTest& operator=(MetamorphicTest&& m) noexcept;
        ~MetamorphicTest() = default;
        void metamorphicTest();
        vector<double> analyzeResult(vector<double> (*f)(vector<vector<vector<bool>>>&));
        vector<vector<vector<bool>>> getMetamorphicTestResult() const;
    private:
        size_t sourceCaseNum;
        size_t followCaseNum;
        AutoPoint<GenSourceCase<T>> generator;
        AutoPoint<Program<T, U>> program;
        vector<AutoPoint<MR<T, U>>> mrs;
        vector<T> sourceCases;
        vector<vector<vector<T>>> followCases;
        vector<U> sourceCaseResults;
        vector<vector<vector<U>>> followCaseResults;
        vector<vector<vector<bool>>> metamorphicTestResults;
    };

    template <typename T, typename U> inline
    MetamorphicTest<T, U>::MetamorphicTest(size_t sourceCaseNum,
                                           size_t followCaseNum,
                                           AutoPoint<GenSourceCase<T>>& generator,
                                           AutoPoint<Program<T, U>>& program,
                                           vector<AutoPoint<MR<T, U>>>& mrs)
    : sourceCaseNum(sourceCaseNum), followCaseNum(followCaseNum), generator(generator), program(program), mrs(mrs){
        sourceCases = vector<T>(sourceCaseNum);
        followCases = vector<vector<vector<T>>>(mrs.size(),
                                                vector<vector<T>>(sourceCaseNum,
                                                        vector<T>(followCaseNum)));
        sourceCaseResults = vector<U>(sourceCaseNum);
        followCaseResults = vector<vector<vector<U>>>(mrs.size(),
                                                      vector<vector<U>>(sourceCaseNum,
                                                              vector<U>(followCaseNum)));
        metamorphicTestResults = vector<vector<vector<bool>>>(mrs.size(),
                                                              vector<vector<bool>>(sourceCaseNum,
                                                                      vector<bool>(followCaseNum)));
    }

    template<typename T, typename U> inline
    MetamorphicTest<T, U>::MetamorphicTest(const MetamorphicTest& m)
            : sourceCaseNum(m.sourceCaseNum), followCaseNum(m.followCaseNum), mrs(m.mrs),
              generator(m.generator->clone()), program(m.program->clone()),
              sourceCases(m.sourceCases), followCases(m.followCases),
              sourceCaseResults(m.sourceCaseResults), followCaseResults(m.followCaseResults),
              metamorphicTestResults(m.metamorphicTestResults){
        for (int i = 0; i < mrs.size(); i++) {
            mrs[i] = m.mrs[i]->clone();
        }
    }

    template<typename T, typename U> inline
    MetamorphicTest<T, U>::MetamorphicTest(MetamorphicTest &&m) noexcept
    : sourceCaseNum(m.sourceCaseNum), followCaseNum(m.followCaseNum), generator(move(m.generator)),
      program(move(m.program)), mrs(move(m.mrs)), sourceCases(move(m.sourceCases)),
      followCases(move(m.followCases)), sourceCaseResults(move(m.sourceCaseResults)),
      followCaseResults(move(m.followCaseResults)), metamorphicTestResults(move(m.metamorphicTestResults)){}

    template<typename T, typename U> inline
    MetamorphicTest<T, U> &MetamorphicTest<T, U>::operator=(const MetamorphicTest &m) {
        sourceCaseNum = m.sourceCaseNum;
        followCaseNum = m.followCaseNum;
        sourceCases = m.sourceCases;
        followCases = m.followCases;
        sourceCaseResults = m.sourceCaseResults;
        followCaseResults = m.followCaseResults;
        metamorphicTestResults = m.metamorphicTestResults;
        mrs = m.mrs;
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
        return *this;
    }

    template <typename T, typename U> inline
    void MetamorphicTest<T, U>::metamorphicTest() {
        for (int i = 0; i < sourceCaseNum; i++){
            // TODO: 多线程优化
            sourceCases[i] = generator->genSourceCase();
            sourceCaseResults[i] = program->genResult(sourceCases[i]);
        }
        for (int i = 0; i < mrs.size(); i++){
            for (int j = 0; j < sourceCaseNum; j++){
                for (int k = 0; k < followCaseNum; k++){
                    // TODO: 多线程优化
                    followCases[i][j][k] = mrs[i]->genFollowCase(sourceCases[j], sourceCaseResults[j]);
                    followCaseResults[i][j][k] = program->genResult(followCases[i][j][k]);
                    metamorphicTestResults[i][j][k] = mrs[i]->mrRelation(sourceCaseResults[j],
                                                                        followCaseResults[i][j][k]);
                }
            }
        }
    }

    template <typename T, typename U> inline
    vector<double> MetamorphicTest<T, U>::analyzeResult(vector<double> (*f)(vector<vector<vector<bool>>> &)) {
        return f(metamorphicTestResults);
    }

    template<typename T, typename U> inline
    vector<vector<vector<bool>>> MetamorphicTest<T, U>::getMetamorphicTestResult() const {
        return metamorphicTestResults;
    }
}


#endif //METAMORPHICTESTFRAMEWORK_METAMORPHICTEST_H
