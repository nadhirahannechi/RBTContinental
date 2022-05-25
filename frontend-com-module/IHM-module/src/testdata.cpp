#include "testdata.h"
#include <iostream>
#include <qdebug.h>
/*!
 * \brief TestData::TestData
 */
TestData::TestData(   int                 m_TestID,
                      int                 m_API_ID,
                      std::string         m_testName,
                      test_protocol       m_Protocol,
                      std::string         m_testDescription,
                      std::string         m_testCategory,
                      std::string         m_Status,
                      std::string         m_testResultCause,
                      std::string         m_timestamp,
                      result              m_testResult,
                      bool                m_resultIsReady):
    m_TestID(m_TestID),
    m_API_ID(m_API_ID),
    m_testName(m_testName),
    m_ethernetProtocol(m_Protocol),
    m_testDescription(m_testDescription),
    m_testCategory(m_testCategory),
    m_Status(m_Status),
    m_testResultCause(m_testResultCause),
    m_timestamp(m_timestamp),
    m_testResult(m_testResult),
    m_resultIsReady(m_resultIsReady)

{


    std::cout<<"Create a test data";
}

TestData::~TestData()
{
    qDebug()<<"delete a test data";
}



/*!
 * \brief TestData::testId
 * \return
 */
int TestData::getTestId() const
{
    return m_TestID;
}

/*!
 * \brief TestData::setTestId
 * \param testId
 */
void TestData::setTestId(int testId)
{
    m_TestID = testId;
}

/*!
 * \brief TestData::testId
 * \return
 */
int TestData::getTestAPI() const
{
    return m_API_ID;
}

/*!
 * \brief TestData::setTestId
 * \param testId
 */
void TestData::setTestAPI(int ApiId)
{
    m_API_ID = ApiId;
}

std::string TestData::getTestName() const
{
    return m_testName;
}

void TestData::setTestName(const std::string &testName)
{
    m_testName = testName;
}

std::string TestData::getTestDescription() const
{
    return m_testDescription;
}

void TestData::setTestDescription(const std::string &testDescription)
{
    m_testDescription = testDescription;
}

std::string TestData::getTestCategory() const
{
    return m_testCategory;
}

void TestData::setTestCategory(const std::string &testCategory)
{
    m_testCategory = testCategory;
}


std::string TestData::getTestStatus() const
{
    return m_Status;
}

void TestData::setTestStatus(const std::string &TestStatus)
{
    m_Status = TestStatus;
}


result TestData::getTestResult() const
{
    return m_testResult;
}

void TestData::setTestResult(const result &testResult)
{
    m_testResult = testResult;
}

std::string TestData::getTestResultCause() const
{
    return m_testResultCause;
}

void TestData::setTestResultCause(const std::string &testResultCause)
{
    m_testResultCause = testResultCause;
}

std::string TestData::getTimestamp() const
{
    return m_timestamp;
}

void TestData::setTimestamp(const std::string &timestamp)
{
    m_timestamp = timestamp;
}

bool TestData::getResultIsReady() const
{
    return m_resultIsReady;
}

void TestData::setResultIsReady(bool resultIsReady)
{
    m_resultIsReady = resultIsReady;
}

test_protocol TestData::getEthernetProtocol() const
{
    return m_ethernetProtocol;
}

void TestData::setEthernetProtocol(test_protocol &ethernetProtocol)
{
    m_ethernetProtocol = ethernetProtocol;
}



