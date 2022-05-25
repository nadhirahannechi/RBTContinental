#ifndef REPORTING_H
#define REPORTING_H

#include <QtXml>
#include "Structure_type.h"
#include "testdata.h"
#include "testdatacreator.h"

class Reporting
{
public:
    Reporting(TestDataCreator& t);
    ~Reporting();

    std::string gethtmlPath() const;
    void setHtmlPath(const std::string &htmlPath);

    void generateHtmlReport();
    void createHtmlHead(QDomDocument &doc, QDomElement &root_html);
    void createTestStatistics(QDomDocument &doc, QDomElement &tag_body);
    void createChartForStatistics(QDomDocument &doc, QDomElement &tag_body);
    void createHTMLTableForProtocol(QDomDocument &doc, QDomElement &tag_body, const std::unordered_map<int, TestData>& protocol_table);
    void loadScriptJS();

    void createRecapTableForStatistics(QDomDocument &doc, QDomElement &tag_body);
    void createRecapTableForEachProtocol(QDomDocument &doc, QDomElement &tag_table, const std::unordered_map<int, TestData> protocol_table);
    void calculatePassedFailedNotExecuted(int &totalPassed, int &totalFailed, int& totalNotExecuted, const std::unordered_map<int, TestData> protocol_table);

private:

    // output of the class
    TestDataCreator& testDataCreator;
    std::string m_htmlPath;
    std::string m_javaScriptFile;
    int totalPassed;
    int totalFailed;
    int totalNotExecuted;
    std::unordered_map<int, TestData> TCP_prtocol;
    std::unordered_map<int, TestData> UDP_prtocol;
    std::unordered_map<int, TestData> ARP_prtocol;
    std::unordered_map<int, TestData> IPV4_prtocol;
    std::unordered_map<int, TestData> ICMP_prtocol;
    std::unordered_map<int, TestData> DHCP_prtocol;
    std::unordered_map<int, TestData> SOMEIP_prtocol;

};

#endif // REPORTING_H
