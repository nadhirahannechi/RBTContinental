#ifndef TESTDATA_H
#define TESTDATA_H


#include "Structure_type.h"
#include <iostream>

class TestData
{
public:
    TestData(){}

    TestData(   int                 TestID,
                int                 API_ID,
                std::string         testName,
                test_protocol       Protocol,
                std::string         testDescription,
                std::string         testCategory,
                std::string         Status,
                std::string         testResultCause,
                std::string         timestamp,
                result          testResult,
                bool            resultIsReady);

    ~TestData();


    int getTestId() const;
    void setTestId(int testId);

    int getTestAPI() const;
    void setTestAPI(int ApiId);

    std::string getTestName() const;
    void setTestName(const std::string &testName);

    std::string getTestDescription() const;
    void setTestDescription(const std::string &testDescription);

    std::string getSubprotocol() const;
    void setSubprotocol(const std::string &Subprotocol);

    std::string getTestStatus() const;
    void setTestStatus(const std::string &TestStatus);

    result getTestResult() const;
    void setTestResult(const result &testResult);

    std::string getTestResultCause() const;
    void setTestResultCause(const std::string &testResultCause);

    std::string getTimestamp() const;
    void setTimestamp(const std::string &timestamp);

    bool getResultIsReady() const;
    void setResultIsReady(bool resultIsReady);

    test_protocol getEthernetProtocol() const;
    void setEthernetProtocol(test_protocol &ethernetProtocol);

    std::string getTestCategory() const;
    void setTestCategory(const std::string &testCategory);

private:

    int                 m_TestID=0;              // test case ID
    int                 m_API_ID=0;
    std::string         m_testName;            // test case name
    test_protocol       m_ethernetProtocol;    // the Ethernet protocol
    std::string         m_testDescription;     // test case description
    std::string         m_testCategory;         // test case subprotocol
    std::string         m_Status;              // Test Case Status
    std::string         m_testResultCause;     // the cause of the verdict/result
    std::string         m_timestamp;           // the test execution timestamp
    result              m_testResult;          // the actual test result
    bool                m_resultIsReady;       // the test result is ready or not
};

#endif // TESTDATA_H
