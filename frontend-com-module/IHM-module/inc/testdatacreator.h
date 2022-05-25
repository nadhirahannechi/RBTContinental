#ifndef TESTDATACREATOR_H
#define TESTDATACREATOR_H


#include "Structure_type.h"
#include "testdata.h"
#include "unordered_map"

class TestDataCreator
{
public:
    TestDataCreator();

    TestDataCreator* getInstance();
   std::unordered_map<int, TestData> TestSuiteMap;  // this map will contains the list of te test cases parsed from the xml file

   void addTestData(                 int                 TestID,
                                     int                 API_ID,
                                     std::string         testName,
                                     test_protocol         Protocol,
                                     std::string         testDescription,
                                     std::string         testCategory,
                                     std::string         Status,
                                     std::string         testResultCause,
                                     std::string         timestamp,
                                     result          testResult,
                                     bool            resultIsReady);

    void       addTestData(TestData testData);
    int        removeTestData(int testId);
    int        getSize();
    TestData  getTest(int testId);
    void       printTestList();
    int        updateTest(TestData testData);
    int        updateTest(int                 TestID,
                          int                 API_ID,
                          std::string         testName,
                          test_protocol         Protocol,
                          std::string         testDescription,
                          std::string         testCategory,
                          std::string         Status,
                          std::string         testResultCause,
                          std::string         timestamp,
                          result          testResult,
                          bool            resultIsReady);
    int        updateTestStatus(int             testId,
                                result          testResult,
                                bool            resultIsReady, std::string timestamp, std::string testResultCause);


private:
   // TestDataCreator();
    //~TestDataCreator();

    //static TestDataCreator* testData_instance;

};

#endif // TESTDATACREATOR_H
