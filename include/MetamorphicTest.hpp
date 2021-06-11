//
// Created by ltc on 2/4/21.
//

#ifndef METAMORPHICTESTFRAMEWORK_METAMORPHICTEST_HPP
#define METAMORPHICTESTFRAMEWORK_METAMORPHICTEST_HPP

#include <cstddef>
#include <vector>
#include <memory>
#include "MR.h"
#include "Program.h"
#include "GenSourceCase.h"
#include "my_pthread/include/Latch.h"
#include "mem_pool/include/ObjPool.hpp"
#include "task_system/include/TaskSystem.h"
#include "utils/include/ProgressBar.h"
#include "my_sql/include/MySql.h"
#include "utils/include/Serialize.h"

namespace metamorphicTestFramework{
    using std::vector;
    using std::shared_ptr;
    using std::tuple;

    template <typename T, typename U>
    class MetamorphicTest final{
    public:
        MetamorphicTest(const string& name, size_t sourceCaseNum, size_t followCaseNum,
                        shared_ptr<GenSourceCase<T>>& generator, shared_ptr<Program<T, U>>& program,
                        vector<shared_ptr<MR<T, U>>>& mrs, shared_ptr<MySql> mysql=nullptr);
        MetamorphicTest(int testId, shared_ptr<MySql> mysql);
        MetamorphicTest(const MetamorphicTest& m) = delete;
        MetamorphicTest(MetamorphicTest&& m) = delete;
        MetamorphicTest& operator=(const MetamorphicTest& m) = delete;
        MetamorphicTest& operator=(MetamorphicTest&& m) = delete;
        ~MetamorphicTest() = default;
        void metamorphicTest();
        vector<double> analyzeResult(vector<double> (*f)(vector<vector<vector<bool>>>&));
        vector<vector<vector<bool>>> getMetamorphicTestResult() const;
    private:
        void createTables();
        void getTestId();
        void insertTestInfo();
        void insertSourceCaseInfo(int caseId, T& sourceCase, U& sourceCaseResult);
        void insertFollowCaseInfo(int mrId, int sourceCaseId, int followCaseId, T& followCase, U& followCaseResult);
        void insertMTestInfo(int mrId, int sourceCaseId, int followCaseId, bool result);
        int getTestInfo();
        void getSourceCaseInfo(int caseId);
        void getFollowCaseInfo(int mrId, int sourceCaseId, int followCaseId);
        void getMTestInfo(int mrId, int sourceCaseId, int followCaseId);
        static void sourceCaseTask(shared_ptr<void> arg);
        static void followCaseTask(shared_ptr<void> arg);
    private:
        string name;
        Latch sourceCaseLatch;
        Latch followCaseLatch;
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
        shared_ptr<ProgressBar> barSource;
        shared_ptr<ProgressBar> barFollow;
        Mutex mutex;
        shared_ptr<MySql> mysql;
        int testId;
    };

    template <typename T, typename U> inline
    MetamorphicTest<T, U>::MetamorphicTest(const string& name, size_t sourceCaseNum,
                                           size_t followCaseNum,
                                           shared_ptr<GenSourceCase<T>>& generator,
                                           shared_ptr<Program<T, U>>& program,
                                           vector<shared_ptr<MR<T, U>>>& mrs, shared_ptr<MySql> mysql)
    : sourceCaseNum(sourceCaseNum), followCaseNum(followCaseNum), generator(generator), program(program), mrs(mrs),
      sourceCases(sourceCaseNum), followCases(mrs.size(), vector<vector<T>>(sourceCaseNum, vector<T>(followCaseNum))),
      sourceCaseResults(sourceCaseNum),
      followCaseResults(mrs.size(), vector<vector<U>>(sourceCaseNum, vector<U>(followCaseNum))),
      metamorphicTestResults(mrs.size(), vector<vector<bool>>(sourceCaseNum, vector<bool>(followCaseNum))),
      sourceCaseLatch(sourceCaseNum), followCaseLatch(mrs.size()*sourceCaseNum*followCaseNum), mysql(mysql), name(name){
          barSource = ObjPool::allocate<ProgressBar>("source test case running", sourceCaseNum);
          barFollow = ObjPool::allocate<ProgressBar>("follow test case running", mrs.size()*sourceCaseNum*followCaseNum);
          createTables();
          insertTestInfo();
          getTestId();
      }

    template <typename T, typename U> inline
    void MetamorphicTest<T, U>::metamorphicTest() {
        barSource->start();
        for (int i = 0; i < sourceCaseNum; i++){
            auto arg = ObjPool::allocate<tuple<MetamorphicTest*, int>>(this, i);
            TaskSystem::addTask(&this->sourceCaseTask, arg);
        }
        sourceCaseLatch.wait();
        barSource->done();
        barFollow->start();
        for (int i = 0; i < mrs.size(); i++){
            for (int j = 0; j < sourceCaseNum; j++){
                for (int k = 0; k < followCaseNum; k++){
                    auto arg = ObjPool::allocate<tuple<MetamorphicTest*, int, int, int>>(this, i, j, k);
                    TaskSystem::addTask(&this->followCaseTask, arg);
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

    template<typename T, typename U> inline
    void MetamorphicTest<T, U>::sourceCaseTask(shared_ptr<void> arg) {
        tuple<MetamorphicTest<T, U>*, int> args = *static_pointer_cast<tuple<MetamorphicTest<T, U>*, int>>(arg);
        MetamorphicTest* m = std::get<0>(args);
        int i = std::get<1>(args);
        T sourceCase = m->generator->genSourceCase();
        U sourceCaseResult = m->program->genResult(sourceCase);
        m->insertSourceCaseInfo(i, sourceCase, sourceCaseResult);
        m->mutex.lock();
        m->sourceCases[i] = sourceCase;
        m->sourceCaseResults[i] = sourceCaseResult;
        m->mutex.unlock();
        m->barSource->done();
        m->sourceCaseLatch.done();
    }

    template<typename T, typename U> inline
    void MetamorphicTest<T, U>::followCaseTask(shared_ptr<void> arg) {
        tuple<MetamorphicTest<T, U>*, int, int, int> args = *static_pointer_cast<tuple<MetamorphicTest<T, U>*, int, int, int>>(arg);
        MetamorphicTest* m = std::get<0>(args);
        int i = std::get<1>(args);
        int j = std::get<2>(args);
        int k = std::get<3>(args);
        T followCase = m->mrs[i]->genFollowCase(m->sourceCases[j], m->sourceCaseResults[j]);
        U followCaseResult = m->program->genResult(followCase);
        m->insertFollowCaseInfo(i, j, k, followCase, followCaseResult);
        bool mr = m->mrs[i]->mrRelation(m->sourceCaseResults[j], followCaseResult,
                                        m->sourceCases[j], followCase);
        m->insertMTestInfo(i, j, k, mr);
        m->mutex.lock();
        m->followCases[i][j][k] = followCase;
        m->followCaseResults[i][j][k] = followCaseResult;
        m->metamorphicTestResults[i][j][k] = mr;
        m->mutex.unlock();
        m->barFollow->done();
        m->followCaseLatch.done();
    }

    template<typename T, typename U>
    void MetamorphicTest<T, U>::createTables() {
        if (mysql != nullptr) {
            string sql = "CREATE TABLE IF NOT EXISTS `test_info`("
                         "`test_id` INT UNSIGNED AUTO_INCREMENT,"
                         "`test_name` VARCHAR(50) NOT NULL,"
                         "`mr_num` INT UNSIGNED NOT NULL,"
                         "`source_case_num` INT UNSIGNED NOT NULL,"
                         "`follow_case_num` INT UNSIGNED NOT NULL,"
                         "PRIMARY KEY(`test_id`)"
                         ")ENGINE=InnoDB DEFAULT CHARSET=utf8;"
                         "CREATE TABLE IF NOT EXISTS `source_case_info`("
                         "`id` INT UNSIGNED AUTO_INCREMENT,"
                         "`test_id` INT UNSIGNED NOT NULL,"
                         "`source_case_id` INT UNSIGNED NOT NULL,"
                         "`source_case` VARCHAR(1024) NOT NULL,"
                         "`source_case_result` VARCHAR(1024) NOT NULL,"
                         "PRIMARY KEY(`id`)"
                         ")ENGINE=InnoDB DEFAULT CHARSET=utf8;"
                         "CREATE TABLE IF NOT EXISTS `follow_case_info`("
                         "`id` INT UNSIGNED AUTO_INCREMENT,"
                         "`test_id` INT UNSIGNED NOT NULL,"
                         "`mr_id` INT UNSIGNED NOT NULL,"
                         "`source_case_id` INT UNSIGNED NOT NULL,"
                         "`follow_case_id` INT UNSIGNED NOT NULL,"
                         "`follow_case` VARCHAR(1024) NOT NULL,"
                         "`follow_case_result` VARCHAR(1024) NOT NULL,"
                         "PRIMARY KEY(`id`)"
                         ")ENGINE=InnoDB DEFAULT CHARSET=utf8;"
                         "CREATE TABLE IF NOT EXISTS `metamorphic_test_info`("
                         "`id` INT UNSIGNED AUTO_INCREMENT,"
                         "`test_id` INT UNSIGNED NOT NULL,"
                         "`mr_id` INT UNSIGNED NOT NULL,"
                         "`source_case_id` INT UNSIGNED NOT NULL,"
                         "`follow_case_id` INT UNSIGNED NOT NULL,"
                         "`metamorphic_test_result` BOOLEAN NOT NULL,"
                         "PRIMARY KEY(`id`)"
                         ")ENGINE=InnoDB DEFAULT CHARSET=utf8;";
            mysql->executeSQL(sql);
        }
    }

    template<typename T, typename U>
    void MetamorphicTest<T, U>::insertSourceCaseInfo(int caseId, T &sourceCase, U &sourceCaseResult) {
        if (mysql != nullptr) {
            string sql = "INSERT INTO source_case_info (test_id, source_case_id, source_case, source_case_result) "
                         "VALUES (" + to_string(testId) + "," + to_string(caseId) + ",'" +
                         serialize(sourceCase) + "','"
                         + serialize(sourceCaseResult) + "');";
            mysql->executeSQL(sql);
        }
    }

    template<typename T, typename U>
    void MetamorphicTest<T, U>::insertFollowCaseInfo(int mrId, int sourceCaseId, int followCaseId, T &followCase,
                                                     U &followCaseResult) {
        if (mysql != nullptr) {
            string sql = "INSERT INTO follow_case_info (test_id, mr_id, source_case_id, follow_case_id, follow_case, follow_case_result) "
                         "VALUES (" + to_string(testId) + "," + to_string(mrId) + "," +
                         to_string(sourceCaseId) + "," + to_string(followCaseId) + ",'" +
                         serialize(followCase) + "','" +
                         serialize(followCaseResult) + "');" ;
            mysql->executeSQL(sql);
        }
    }

    template<typename T, typename U>
    void MetamorphicTest<T, U>::insertMTestInfo(int mrId, int sourceCaseId, int followCaseId, bool result) {
        if (mysql != nullptr) {
            string sql = "INSERT INTO metamorphic_test_info (test_id, mr_id, source_case_id, follow_case_id, metamorphic_test_result) "
                         "VALUES (" + to_string(testId) + "," + to_string(mrId) + "," +
                         to_string(sourceCaseId) + "," + to_string(followCaseId) + ","
                         + to_string(result) + ");";
            mysql->executeSQL(sql);
        }
    }

    template<typename T, typename U>
    void MetamorphicTest<T, U>::insertTestInfo() {
        if (mysql != nullptr) {
            string sql = "INSERT INTO `test_info` ("
                         "`test_name`,`mr_num`,`source_case_num`,`follow_case_num`)"
                         "VALUES (\"" + name + "\"," + to_string(mrs.size()) + "," + to_string(sourceCaseNum)
                         + "," + to_string(followCaseNum) + ");";
            mysql->executeSQL(sql);
        }
    }

    template<typename T, typename U>
    void MetamorphicTest<T, U>::getTestId() {
        if (mysql != nullptr) {
            string sql = "SELECT test_id FROM test_info WHERE test_name=\"" + name + "\" AND mr_num=" + to_string(mrs.size()) + " AND source_case_num=" + to_string(sourceCaseNum) + " AND follow_case_num=" +to_string(followCaseNum) + " ORDER BY test_id DESC LIMIT 1";
            vector<vector<unordered_map<string,string>>> result = mysql->queryData(sql);
            testId = stoi(result[0][0]["test_id"]);
        }
    }

    template<typename T, typename U>
    MetamorphicTest<T, U>::MetamorphicTest(int testId, shared_ptr<MySql> mysql) : testId(testId), mysql(mysql), sourceCaseLatch(0), followCaseLatch(0), barSource(nullptr), barFollow(nullptr) {
        int mrNum = getTestInfo();
        sourceCases = vector<T>(sourceCaseNum);
        sourceCaseResults = vector<U>(sourceCaseNum);
        followCases = vector<vector<vector<T>>>(mrNum, vector<vector<T>>(sourceCaseNum, vector<T>(followCaseNum)));
        followCaseResults = vector<vector<vector<U>>>(mrNum, vector<vector<U>>(sourceCaseNum, vector<U>(followCaseNum)));
        metamorphicTestResults = vector<vector<vector<bool>>>(mrNum, vector<vector<bool>>(sourceCaseNum, vector<bool>(followCaseNum)));
        for (int i = 0; i < sourceCaseNum; i++) {
            getSourceCaseInfo(i);
        }
        for (int i = 0; i < mrNum; i++) {
            for (int j = 0; j < sourceCaseNum; j++) {
                for (int k = 0; k < followCaseNum; k++) {
                    getFollowCaseInfo(i, j, k);
                    getMTestInfo(i, j, k);
                }
            }
        }
    }

    template<typename T, typename U>
    int MetamorphicTest<T, U>::getTestInfo() {
        string sql = "SELECT * FROM test_info WHERE test_id=" + to_string(testId) + ";";
        vector<vector<unordered_map<string,string>>> result = mysql->queryData(sql);
        name = result[0][0]["test_name"];
        int mrNum = stoi(result[0][0]["mr_num"]);
        sourceCaseNum = stoi(result[0][0]["source_case_num"]);
        followCaseNum = stoi(result[0][0]["follow_case_num"]);
        return mrNum;
    }

    template<typename T, typename U>
    void MetamorphicTest<T, U>::getSourceCaseInfo(int caseId) {
        string sql = "SELECT * FROM source_case_info WHERE test_id=" + to_string(testId) + " AND source_case_id=" + to_string(caseId) + ";";
        vector<vector<unordered_map<string,string>>> result = mysql->queryData(sql);
        sourceCases[caseId] = deserialize<T>(result[0][0]["source_case"]);
        sourceCaseResults[caseId] = deserialize<U>(result[0][0]["source_case_result"]);
    }

    template<typename T, typename U>
    void MetamorphicTest<T, U>::getFollowCaseInfo(int mrId, int sourceCaseId, int followCaseId) {
        string sql = "SELECT * FROM follow_case_info WHERE test_id=" + to_string(testId) + " AND mr_id=" + to_string(mrId) +
                     " AND source_case_id=" + to_string(sourceCaseId) + " AND follow_case_id=" + to_string(followCaseId) + ";";
        vector<vector<unordered_map<string,string>>> result = mysql->queryData(sql);
        followCases[mrId][sourceCaseId][followCaseId] = deserialize<T>(result[0][0]["follow_case"]);
        followCaseResults[mrId][sourceCaseId][followCaseId] = deserialize<U>(result[0][0]["follow_case_result"]);
    }

    template<typename T, typename U>
    void MetamorphicTest<T, U>::getMTestInfo(int mrId, int sourceCaseId, int followCaseId) {
        string sql = "SELECT * FROM metamorphic_test_info WHERE test_id=" + to_string(testId) + " AND mr_id=" + to_string(mrId) +
                     " AND source_case_id=" + to_string(sourceCaseId) + " AND follow_case_id=" + to_string(followCaseId) + ";";
        vector<vector<unordered_map<string,string>>> result = mysql->queryData(sql);
        metamorphicTestResults[mrId][sourceCaseId][followCaseId] = stoi(result[0][0]["metamorphic_test_result"]);
    }
}


#endif //METAMORPHICTESTFRAMEWORK_METAMORPHICTEST_HPP
