#include "testdatacreator.h"
#include <QDebug>

// Define the static TestData instance
//TestDataCreator* TestDataCreator::testData_instance = NULL;


TestDataCreator::TestDataCreator()
{
    std::cout<<"Create a static object to create test data list\n";
}



TestDataCreator *TestDataCreator::getInstance()
{
    static TestDataCreator* testData_instance;
    if (testData_instance == NULL)
    {std::cout<<"\nCreate a static object to TestDataCreator +++\n";
        testData_instance = new TestDataCreator();
    }
    return(testData_instance);
}


/*!
 * \brief TestData::createTestData: create a test data and put it in the list of the tests
 * \param m_testId
 * \param m_testName
 * \param m_testDescription
 * \param m_expectedResult
 * \param m_testResult
 * \param m_testResultCause
 * \param m_timestamp
 * \param m_resultIsReady
 * \param m_ethernetProtocol
 */
void TestDataCreator::addTestData(int                 TestID,
                                  int                 API_ID,
                                  std::string         testName,
                                  test_protocol         Protocol,
                                  std::string         testDescription,
                                  std::string         testCategory,
                                  std::string         Status,
                                  std::string         testResultCause,
                                  std::string         timestamp,
                                  result          testResult,
                                  bool            resultIsReady)
{
    //create the test data
    TestData *testData = new TestData(TestID,API_ID,testName,Protocol,testDescription,
                                      testCategory,Status,testResultCause,timestamp,
                                      testResult,resultIsReady);

    //add the testdata to the test list
    TestSuiteMap[TestID] = *testData;
    std::cout<<"\n Adding test :"<<testData->getTestName()<<"\n";


}

void TestDataCreator::addTestData(TestData testData)
{

        TestSuiteMap[testData.getTestId()] = testData;
}

int TestDataCreator::removeTestData(int testId)
{
    return TestSuiteMap.erase(testId);
}

int TestDataCreator::getSize()
{
    return TestSuiteMap.size();
}

TestData TestDataCreator::getTest(int testId)
{
        return TestSuiteMap[testId];
}

void TestDataCreator::printTestList()
{
    //std::unordered_map<int, TestData*>::iterator it = TestSuiteMap.begin();
    for(auto it = TestSuiteMap.begin();it != TestSuiteMap.end(); it++)
    {
        std::cout<< "\nTestID: " << it->second.getTestId()<< " /// TestName: " << it->second.getTestName() <<" /// Category: "<< /*it->second.getTestCategory()*/""
                        << " /// Description: "<< it->second.getTestDescription()
                        << " /// Result :"<<it->second.getTestResult() << " /// Cause: "<<it->second.getTestResultCause()<< " /// Timestamp: "<<it->second.getTimestamp()
                        << " /// Protocol: "<<it->second.getEthernetProtocol() << " /// Result is ready: "<< it->second.getResultIsReady()<<"\n\n";

    }

}

int TestDataCreator::updateTest(TestData testData)
{
    auto testToUpdate = TestSuiteMap.find(testData.getTestId());
    if (testToUpdate == TestSuiteMap.end())
        return 0; // test not found

    testToUpdate->second = testData;
    return 1; // test found and updated

}

int TestDataCreator::updateTest(int                 TestID,
                                int                 API_ID,
                                std::string         testName,
                                test_protocol         Protocol,
                                std::string         testDescription,
                                std::string         testCategory,
                                std::string         Status,
                                std::string         testResultCause,
                                std::string         timestamp,
                                result          testResult,
                                bool            resultIsReady)
{
    auto testToUpdate = TestSuiteMap.find(TestID);
    if (testToUpdate == TestSuiteMap.end())
        return 0; // test not found

       testToUpdate->second.setTestAPI(API_ID);
       testToUpdate->second.setTestName(testName);
       testToUpdate->second.setEthernetProtocol(Protocol);
       testToUpdate->second.setTestDescription(testDescription);
       testToUpdate->second.setTestCategory(testCategory);
       testToUpdate->second.setTestStatus(Status);
       testToUpdate->second.setTestResultCause(testResultCause);
       testToUpdate->second.setTimestamp(timestamp);

       testToUpdate->second.setTestResult(testResult);
       testToUpdate->second.setResultIsReady(resultIsReady);
    return 1; // test found and updated
}

int TestDataCreator::updateTestStatus(int testId, result testResult, bool resultIsReady, std::string timestamp, std::string testResultCause)
{
    auto testToUpdate = TestSuiteMap.find(testId);
    if (testToUpdate == TestSuiteMap.end())
        return 0; // test not found

    testToUpdate->second.setTestResultCause(testResultCause);
       testToUpdate->second.setTimestamp(timestamp);
       testToUpdate->second.setTestResult(testResult);
       testToUpdate->second.setResultIsReady(resultIsReady);
    return 1; // test found and updated
}

//remove
