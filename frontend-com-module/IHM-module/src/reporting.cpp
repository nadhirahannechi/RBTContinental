/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file   reporting.cpp
 *  @brief  Qt Base - IHM_Module.
 *  \author uid1344 Tarek Othmani
 *
 *  \addtogroup frontend-com-module
 *  \{
 */
/*
 * This class is using to generate HTML report
 * and display all the test results for the TC8
*/

#include <QFile>
#include "reporting.h"
#include <QIODevice>
#include <vector>


const char *test_result[] = { "PASSED", "FAILED", "NOT_EXECUTED" };
const char *test_protocol_tab[] = { "NONE", "TCP", "UDP", "ARP", "ICMP", "IPV4", "DHCP", "SOMEIP" };

/*!
 * \brief Reporting::Reporting
 * default constructor of the class Reporting
 * Initiale the attributes to empty string
 */
Reporting::Reporting( TestDataCreator& t):
    testDataCreator(t),
    m_htmlPath(""),
    totalPassed(0),
    totalFailed(0),
    totalNotExecuted(0),
    TCP_prtocol(0),
    UDP_prtocol(0),
    ARP_prtocol(0),
    IPV4_prtocol(0),
    ICMP_prtocol(0),
    DHCP_prtocol(0),
    SOMEIP_prtocol(0)

{
    qDebug()<<"Start Reporting";
    //testDataCreator = t;
}

Reporting::~Reporting()
{
    //Nothing to do
}


/*!
 * \brief Reporting::get_htmlPath
 * \return the path of the generated HTML report
 */
std::string Reporting::gethtmlPath() const
{
    return m_htmlPath;
}

/*!
 * \brief Reporting::setHtmlPath
 * \param htmlPath: html path received from the user (reporter)
 */
void Reporting::setHtmlPath(const std::string &htmlPath)
{
    m_htmlPath = htmlPath;

    //get the path only the path of the html file and
    // set the javascript file name in the same path
    std::size_t found = m_htmlPath.find_last_of("/\\");
    m_javaScriptFile = m_htmlPath.substr(0,found).c_str();
    m_javaScriptFile.append("\\chart_statistic.js");
    std::cout<< "javascript file path "<< m_javaScriptFile;
}

/*!
 * \brief Reporting::createHtml
 * test function to create HTML file from structure
 */
void Reporting::generateHtmlReport()
{


    //html file output
    QFile file_html;

    file_html.setFileName(m_htmlPath.c_str());

    //fichier.flush();
    if (!file_html.open(QIODevice::WriteOnly))
    {
        std::cout<<"open file error: "<<file_html.errorString().toUtf8().constData();
    }
    else
    {
        qDebug()<<"Start generating the report";
        QDomDocument doc( "html" ); // <!DOCTYPE html>
        QDomElement root_html = doc.createElement( "html" ); //<html></html>
        doc.appendChild( root_html );



        /*
         * Create the <head> and the <style> of the html doc
         */

        createHtmlHead(doc, root_html);


        /*
         * create the tag <body>
        */
        QDomElement tag_body = doc.createElement( "body" ); //<body></body>
        root_html.appendChild( tag_body );

        /*
         * Report title
         */
        QDomElement text_header1 = doc.createElement( "h1" );
        QDomText report_title = doc.createTextNode("Report from the Ethernet Test Tool\n");
        text_header1.appendChild( report_title );
        tag_body.appendChild(text_header1);




          //an iterator for the list of test cases
        auto size = testDataCreator.TestSuiteMap.size();
        std::cout<<"\nNumber of tests=:"<< size;
        //split tests in vectors according to their protocol name
        for(auto it = testDataCreator.TestSuiteMap.begin(); it != testDataCreator.TestSuiteMap.end(); it++)
        {
            auto testToReport = it->second;
            std::cout<<"\nGENERATING:"<< testToReport.getTestName();

            // fill in each table with related information from protocol_test_list
            switch(testToReport.getEthernetProtocol())
            {
                case TCP:
                    TCP_prtocol[testToReport.getTestId()] = testToReport;
                    std::cout<<"\n TCP_prtocol:\n";

                break;

                case UDP:
                    UDP_prtocol[testToReport.getTestId()] = testToReport;
                break;

                case ARP:
                    ARP_prtocol[testToReport.getTestId()] = testToReport;

                break;

                case IPV4:
                    IPV4_prtocol[testToReport.getTestId()] = testToReport;
                break;

                case ICMP:
                    ICMP_prtocol[testToReport.getTestId()] = testToReport;
                break;

                case DHCP:
                    DHCP_prtocol[testToReport.getTestId()] = testToReport;
                break;

                case SOMEIP:
                    SOMEIP_prtocol[testToReport.getTestId()] = testToReport;
                break;

                default:
                    std::cout<<"this test does not belong to any protocol";
            }

            // calculate the number of passed, failed and not executed tests

            switch(testToReport.getTestResult())
            {
                case PASSED:
                    totalPassed++;
                break;

                case FAILED:
                    totalFailed++;
                break;

                case NOT_EXECUTED:
                    totalNotExecuted++;
                break;
            }
            //break;
        }

std::cout<<"\n--2\n";
        // create the chart

    createChartForStatistics(doc, tag_body);
    createRecapTableForStatistics(doc, tag_body);



std::cout<<"\n--3\n";
        // if TCP test vector is not empty, then fill the HTML table of the TCP protocol
        if (!TCP_prtocol.empty())
        {std::cout<<"\nTCP_prtocol.empty()\n";

            createHTMLTableForProtocol(doc, tag_body, TCP_prtocol);
        }
std::cout<<"\n--4\n";
        // if UDP test vector is not empty, then fill the HTML table of the TCP protocol
        if (!UDP_prtocol.empty())
        {
            createHTMLTableForProtocol(doc, tag_body, UDP_prtocol);
        }
std::cout<<"\n--5\n";
        // if ARP test vector is not empty, then fill the HTML table of the TCP protocol
        if (!ARP_prtocol.empty())
        {
            createHTMLTableForProtocol(doc, tag_body, ARP_prtocol);
        }
std::cout<<"\n--6\n";
        // if IPV4 test vector is not empty, then fill the HTML table of the TCP protocol
        if (!IPV4_prtocol.empty())
        {
            createHTMLTableForProtocol(doc, tag_body, IPV4_prtocol);
        }
std::cout<<"\n--7\n";
        // if ICMP test vector is not empty, then fill the HTML table of the TCP protocol
        if (!ICMP_prtocol.empty())
        {
            createHTMLTableForProtocol(doc, tag_body, ICMP_prtocol);
        }
std::cout<<"\n--8\n";
        // if DHCP test vector is not empty, then fill the HTML table of the TCP protocol
        if (!DHCP_prtocol.empty())
        {
            createHTMLTableForProtocol(doc, tag_body, DHCP_prtocol);
        }
std::cout<<"\n--9\n";
        // if SOMEIP test vector is not empty, then fill the HTML table of the TCP protocol
        if (!SOMEIP_prtocol.empty())
        {
            createHTMLTableForProtocol(doc, tag_body, SOMEIP_prtocol);
        }

std::cout<<"\n--10\n";


        std::string html = doc.toString().toUtf8().constData();
        QTextStream write_html(&file_html);
        write_html<<html.c_str();
        std::cout<<"\n--11\n";
        file_html.close();
        qDebug() << "Finish reporting";

    }


}

/*!
 * \brief Reporting::createHtmlHead: create the tag <head> and <style> of the html file
 * \param doc : the DomDocument
 * \param root_html : the tag <html>
 */
void Reporting::createHtmlHead(QDomDocument &doc, QDomElement &root_html)
{
    QDomElement tag_head = doc.createElement( "head" ); //<head></head>
    root_html.appendChild( tag_head );

    /*
     * The stylesheet
     */
    QDomElement tag_style = doc.createElement( "style" ); //<style></style>

    QDomText text_style = doc.createTextNode("body {border-style: outset; }\n"  // margin: top right bottom left
                                              "h1 {text-align: center;}"
                                             "table, th, td {border: 1px solid black;border-collapse: collapse;}\n"
                                             "th, td {padding: 5px;}\n"
                                             "th { text-align: left;}\n"
                                             ".resultTab {margin-top: 40px;margin-left: auto; margin-right: auto;}\n"
                                             "tr.noBorder td {border: 0;text-align: center;font-size:20px;font-weight: bold;background-color:#eeeef1}\n"
                                             ".chart {width: 50%;float:right;text-align: center;padding-bottom: 30px;}\n"
                                             ".chart p {font-family: \"Arial\", sans-serif;font-size: 16px;color: #bdc3c7;}\n"
                                             ".recap {width: 50%;float:right;}\n"
                                             ".sumtable {margin-left:75px;position: absolute;top: 50%;transform: translateY(-50%);transform: translateY(-50%);text-align: center;}\n"
                                             ".recap p {text-align: center;font-family: \"Arial\", sans-serif;font-size: 16px;color: #bdc3c7;}\n");

    tag_style.appendChild(text_style);
    tag_head.appendChild( tag_style );
}


/*!
 * \brief Reporting::createTestStatistics: calcul the test result statistics and reprort it in the top of the html doc
 * \param doc
 * \param tag_body
 */
void Reporting::createTestStatistics(QDomDocument &doc, QDomElement &tag_body)
{
    QDomElement tag_data_js = doc.createElement( "script" ); //<script> javascript
    tag_data_js.setAttribute("type", "text/javascript");
    std::string textData = "var passed = " + std::to_string(totalPassed) +
                      "; var failed = "+ std::to_string(totalFailed) +
                      "; var notExecuted = "+ std::to_string(totalNotExecuted) +";";
    QDomText text_data_js = doc.createTextNode(textData.c_str());
    tag_data_js.appendChild(text_data_js);
    tag_body.appendChild( tag_data_js );


    QDomElement tag_script_js = doc.createElement( "script" ); //<script> javascript
    tag_script_js.setAttribute("type", "text/javascript");
    tag_script_js.setAttribute("src", "chart_statistic.js");
    std::string javaScriptCode = "";

    QDomText text_script_js = doc.createTextNode(javaScriptCode.c_str());


    tag_script_js.appendChild(text_script_js);
    tag_body.appendChild( tag_script_js );

    //Load the javascript file in the same path of the html file
    loadScriptJS();
}


/*!
 * \brief Reporting::createChartForStatistics
 * \param doc
 * \param tag_body
 */
void Reporting::createChartForStatistics(QDomDocument &doc, QDomElement &tag_body)
{

    QDomElement tag_div_statistic = doc.createElement( "div" );    //<div>
    tag_div_statistic.setAttribute("class", "chart");
    QDomElement tag_p = doc.createElement( "p" );                  //<p>
    QDomText text_p = doc.createTextNode("TC8 Test Result Statistics");
    tag_p.appendChild(text_p);                                     // <p>TC8 Test Result Statistics</p>
    QDomElement tag_canvas = doc.createElement( "canvas" );
    tag_canvas.setAttribute("width", "800");
    tag_canvas.setAttribute("height", "500");
    tag_canvas.setAttribute("id", "canvas");                       // <canvas id="canvas" width="1000" height="400"></canvas>
    QDomText text_canvas = doc.createTextNode("");
    tag_canvas.appendChild(text_canvas);
    tag_div_statistic.appendChild(tag_p);
    tag_div_statistic.appendChild(tag_canvas);   // </div>
    tag_body.appendChild(tag_div_statistic);

    //create chart javascript
    createTestStatistics(doc, tag_body);


}

/*!
 * \brief Reporting::createHTMLTableForProtocol: create the html table report for each protocol
 * \param doc
 * \param tag_body
 * \param protocol_table: vector with tests data for each protocol
 */
void Reporting::createHTMLTableForProtocol(QDomDocument &doc, QDomElement &tag_body, const std::unordered_map<int, TestData> &protocol_table)
{ std::cout<<"\n createHTMLTableForProtocol\n";
    std::string protocolName;
    for (auto it=protocol_table.begin();it!=protocol_table.end();it++){
        protocolName = test_protocol_tab[it->second.getEthernetProtocol()];
        break;
    }
    std::cout<<"\n createHTMLTableForProtocol for \n"<<protocolName;

     QDomText text_protocol_name = doc.createTextNode((protocolName+" Protocol").c_str());


    /*
     * create the tag <table>
    */
    QDomElement tag_table = doc.createElement( "table" ); //<table></table>
    tag_table.setAttribute("style", "width:90%");
    tag_table.setAttribute("class", "resultTab");
    QDomElement tagToDisplayTheProtocolName = doc.createElement( "tr" );
    tagToDisplayTheProtocolName.setAttribute("class", "noBorder");
    QDomElement tagTableTitle = doc.createElement( "td" );
    tagTableTitle.setAttribute("colspan", "8");
    tagTableTitle.setAttribute("id", protocolName.c_str());
    tagTableTitle.appendChild(text_protocol_name);
    tagToDisplayTheProtocolName.appendChild(tagTableTitle);
    tag_table.appendChild(tagToDisplayTheProtocolName);

    QDomElement tag_table_header = doc.createElement( "tr" );

    /*
     * create the headers of the table TCP
    */
    QDomElement tag_header_test_id = doc.createElement( "th" );
    QDomText header_test_id = doc.createTextNode("TEST ID");
    tag_header_test_id.appendChild(header_test_id);
    tag_table_header.appendChild(tag_header_test_id);

    QDomElement tag_header_test_name = doc.createElement( "th" );
    QDomText header_test_name = doc.createTextNode("TEST NAME");
    tag_header_test_name.appendChild(header_test_name);
    tag_table_header.appendChild(tag_header_test_name);

    QDomElement tag_header_test_type = doc.createElement( "th" );
    QDomText header_test_type = doc.createTextNode("CATEGORY");
    tag_header_test_type.appendChild(header_test_type);
    tag_table_header.appendChild(tag_header_test_type);

    QDomElement tag_header_test_Description = doc.createElement( "th" );
    QDomText header_test_description = doc.createTextNode("DESCRIPTION");
    tag_header_test_Description.appendChild(header_test_description);
    tag_table_header.appendChild(tag_header_test_Description);

    QDomElement tag_header_test_result = doc.createElement( "th" );
    QDomText header_test_result = doc.createTextNode("RESULT");
    tag_header_test_result.appendChild(header_test_result);
    tag_table_header.appendChild(tag_header_test_result);

    QDomElement tag_header_test_cause = doc.createElement( "th" );
    QDomText header_test_cause = doc.createTextNode("CAUSE");
    tag_header_test_cause.appendChild(header_test_cause);
    tag_table_header.appendChild(tag_header_test_cause);

    QDomElement tag_header_test_timestamp = doc.createElement( "th" );
    QDomText header_test_timestamp = doc.createTextNode("TIMETAMP");
    tag_header_test_timestamp.appendChild(header_test_timestamp);
    tag_table_header.appendChild(tag_header_test_timestamp);

    tag_table.appendChild(tag_table_header);

    // Fill test data in the HTML table

    auto it = protocol_table.begin();  //an iterator for the list of test cases

    while(it != protocol_table.end())
    {
        TestData testDataToReport = it->second;

        QDomElement tag_tr_data = doc.createElement( "tr" );

        /*
         * first test case report of TCP protocol
        */

        QDomElement tag_test_id = doc.createElement( "td" );
        QDomText test_id = doc.createTextNode(std::to_string(testDataToReport.getTestId()).c_str());
        tag_test_id.appendChild(test_id);
        tag_tr_data.appendChild(tag_test_id);

        QDomElement tag_test_name = doc.createElement( "td" );
        QDomText test_name = doc.createTextNode(testDataToReport.getTestName().c_str());
        tag_test_name.appendChild(test_name);
        tag_tr_data.appendChild(tag_test_name);

        QDomElement tag_test_type = doc.createElement( "td" );
        QDomText test_type = doc.createTextNode(testDataToReport.getTestCategory().c_str());
        tag_test_type.appendChild(test_type);
        tag_tr_data.appendChild(tag_test_type);

        QDomElement tag_test_description = doc.createElement( "td" );
        QDomText test_description = doc.createTextNode(testDataToReport.getTestDescription().c_str());
        tag_test_description.appendChild(test_description);
        tag_tr_data.appendChild(tag_test_description);

        int test_key = testDataToReport.getTestResult();
        const char* result = test_result[test_key];
        QDomElement tag_test_result = doc.createElement( "td" );
        QDomText test_result = doc.createTextNode(result);
        tag_test_result.appendChild(test_result);
        switch (test_key)
        {
            case 0: //passed
                 tag_tr_data.setAttribute("bgcolor", "#48E63E"); // green
            break;

            case 1: //failed
                tag_tr_data.setAttribute("bgcolor", "#FB8C85"); // red
            break;

            case 2: //not executed
                tag_tr_data.setAttribute("bgcolor", "yellow"); // blue
            break;
        }

        tag_tr_data.appendChild(tag_test_result);

        QDomElement tag_test_cause = doc.createElement( "td" );
        QDomText test_cause = doc.createTextNode(testDataToReport.getTestResultCause().c_str());
        tag_test_cause.appendChild(test_cause);
        tag_tr_data.appendChild(tag_test_cause);

        QDomElement tag_test_timestamp = doc.createElement( "td" );
        QDomText test_timestamp = doc.createTextNode(testDataToReport.getTimestamp().c_str());
        tag_test_timestamp.appendChild(test_timestamp);
        tag_tr_data.appendChild(tag_test_timestamp);

        tag_table.appendChild(tag_tr_data);

        it++;
    }

    tag_body.appendChild(tag_table);
}

/*!
 * \brief Reporting::loadScriptJS: thismethod will load the javascript code
 *       in the same path when exist the html report
 */
void Reporting::loadScriptJS()
{
    //html file output
    QFile javaScriptFile;

    javaScriptFile.setFileName(m_javaScriptFile.c_str());

    //fichier.flush();
    if (!javaScriptFile.open(QIODevice::WriteOnly))
    {
        std::cout<<"open file error: "<< javaScriptFile.errorString().toUtf8().constData();
    }
    else
    {
        std::string javaScriptCode = "var myColor = [\"#39ca74\",\"#e54d42\",\"#c1eefc\"];\n"
                                    "var myData = [passed,failed,notExecuted];\n"
                                    "var myLabel = [\"PASSED\",\"FAILED\",\"NOT EXECUTED\"];\n"

                                    "function getTotal(){\n"
                                    "var myTotal = 0;\n"
                                    "for (var j = 0; j < myData.length; j++) {\n"
                                    "myTotal += (typeof myData[j] == 'number') ? myData[j] : 0;\n"
                                    "}\n"
                                    "return myTotal;\n"
                                    "}\n"

                                    "function plotData() {\n"
                                    "var canvas;\n"
                                    "var ctx;\n"
                                    "var lastend = 0;\n"
                                    "var myTotal = getTotal();\n"
                                    "var doc;\n"
                                    "canvas = document.getElementById(\"canvas\");\n"
                                    "var x = (canvas.width)/2;\n"
                                    "var y = (canvas.height)/2;\n"
                                    "var r = 150;\n"

                                    "ctx = canvas.getContext(\"2d\");\n"
                                    "ctx.clearRect(0, 0, canvas.width, canvas.height);\n"

                                    "for (var i = 0; i < myData.length; i++) {\n"
                                    "if (myData[i] != 0){"
                                    "ctx.fillStyle = myColor[i];\n"
                                    "ctx.beginPath();\n"
                                    "ctx.moveTo(x,y);\n"
                                    "ctx.arc(x,y,r,lastend,lastend+(Math.PI*2*(myData[i]/myTotal)),false);\n"
                                    "ctx.lineTo(x,y);\n"
                                    "ctx.fill();\n"


                                    "ctx.beginPath();\n"
                                    "var start = [];\n"
                                    "var end = [];\n"
                                    "var last = 0;\n"
                                    "var flip = 0;\n"
                                    "var textOffset = 0;\n"
                                    "var precentage = (myData[i]/myTotal)*100;\n"
                                    "start = getPoint(x,y,r-20,(lastend+(Math.PI*2*(myData[i]/myTotal))/2));\n"
                                    "end = getPoint(x,y,r+20,(lastend+(Math.PI*2*(myData[i]/myTotal))/2));\n"
                                    "if(start[0] <= x)\n"
                                    "{\n"
                                      "flip = -1;\n"
                                      "textOffset = -110;\n"
                                    "}\n"
                                    "else\n"
                                    "{\n"
                                      "flip = 1;\n"
                                      "textOffset = 10;\n"
                                    "}\n"
                                    "ctx.moveTo(start[0],start[1]);\n"
                                    "ctx.lineTo(end[0],end[1]);\n"
                                    "ctx.lineTo(end[0]+120*flip,end[1]);\n"
                                    "ctx.strokeStyle = \"#bdc3c7\";\n"
                                    "ctx.lineWidth   = 2;\n"
                                    "ctx.stroke();\n"

                                    "ctx.font=\"17px Arial\";\n"
                                    "ctx.fillText(myLabel[i]+\" \"+precentage.toFixed(2)+\"%\",end[0]+textOffset,end[1]-4);\n"

                                    "lastend += Math.PI*2*(myData[i]/myTotal);\n"

                                    "}\n"
                                    "}\n"
                                     "}\n"
                                    "function getPoint(c1,c2,radius,angle) {\n"
                                    "return [c1+Math.cos(angle)*radius,c2+Math.sin(angle)*radius];\n"
                                    "}\n"

                                    "plotData();\n";

        QTextStream write_html(&javaScriptFile);
        write_html<<javaScriptCode.c_str();
        javaScriptFile.close();

    }
}

void Reporting::createRecapTableForStatistics(QDomDocument &doc, QDomElement &tag_body)
{
    QDomElement tag_div_recap = doc.createElement( "div" );    //<div>
    tag_div_recap.setAttribute("class", "recap");  // <div class="recap">

    QDomElement tag_p = doc.createElement( "p" );                  //<p>
    QDomText text_p = doc.createTextNode("Summary Table");
    tag_p.appendChild(text_p);
    tag_div_recap.appendChild(tag_p);

    QDomElement tag_table = doc.createElement( "table" ); //<table></table>
    tag_table.setAttribute("class", "sumtable");
    tag_table.setAttribute("style", "width:45%");

    //color the columns of the summury table
    QDomElement tag_col_span = doc.createElement( "col" );    //<col>
    tag_col_span.setAttribute("span", "2");
    tag_table.appendChild(tag_col_span);
    QDomElement tag_col_passed = doc.createElement( "col" );    //color the <col> of passed tests in green
    tag_col_passed.setAttribute("span", "2");
    tag_col_passed.setAttribute("style", "background-color: #39ca74;");
    tag_table.appendChild(tag_col_passed);
    QDomElement tag_col_failed = doc.createElement( "col" );    //color the <col> of failed tests in red
    tag_col_failed.setAttribute("span", "2");
    tag_col_failed.setAttribute("style", "background-color: #e54d42;");
    tag_table.appendChild(tag_col_failed);
    QDomElement tag_col_notExecuted = doc.createElement( "col" );    //color the <col> of not executed tests in blue
    tag_col_notExecuted.setAttribute("span", "2");
    tag_col_notExecuted.setAttribute("style", "background-color: #c1eefc;");
    tag_table.appendChild(tag_col_notExecuted);

    QDomElement tag_table_header = doc.createElement( "tr" );

    /*
     * create the headers of the table recap of all the tests
    */
    QDomElement tag_header_test_id = doc.createElement( "th" );
    QDomText header_test_id = doc.createTextNode("Protocol/Results");
    tag_header_test_id.appendChild(header_test_id);
    tag_table_header.appendChild(tag_header_test_id);

    QDomElement tag_header_test_total = doc.createElement( "th" );
    QDomText header_test_total = doc.createTextNode("Total");
    tag_header_test_total.appendChild(header_test_total);
    tag_table_header.appendChild(tag_header_test_total);

    QDomElement tag_head_recap_passed = doc.createElement( "th" );
    //tag_head_recap_passed.setAttribute("style", "font-color:green");
    tag_head_recap_passed.setAttribute("colspan", "2");
    QDomText header_test_passed = doc.createTextNode("Passed");
    tag_head_recap_passed.appendChild(header_test_passed);
    tag_table_header.appendChild(tag_head_recap_passed);

    QDomElement tag_head_recap_failed = doc.createElement( "th" );
    QDomText header_test_failed = doc.createTextNode("Failed");
    tag_head_recap_failed.setAttribute("colspan", "2");
    tag_head_recap_failed.appendChild(header_test_failed);
    tag_table_header.appendChild(tag_head_recap_failed);

    QDomElement tag_head_recap_notExec = doc.createElement( "th" );
    QDomText header_test_notExec = doc.createTextNode("Not executed");
    tag_head_recap_notExec.setAttribute("colspan", "2");
    tag_head_recap_notExec.appendChild(header_test_notExec);
    tag_table_header.appendChild(tag_head_recap_notExec);

    tag_table.appendChild(tag_table_header);

    QDomElement tag_tr_data = doc.createElement( "tr" );

    /*
     * first test case report of TCP protocol
    */

    QDomElement tag_test_empty = doc.createElement( "td" );
    QDomText test_empty = doc.createTextNode("Total");
    tag_test_empty.appendChild(test_empty);
    tag_tr_data.appendChild(tag_test_empty);

    QDomElement tag_total_tests = doc.createElement( "td" );
    QDomText total_tests = doc.createTextNode(std::to_string(testDataCreator.getSize()).c_str());
    tag_total_tests.appendChild(total_tests);
    tag_tr_data.appendChild(tag_total_tests);

    QDomElement tag_total_passed = doc.createElement( "td" );
    tag_total_passed.setAttribute("colspan", "2");
    QDomText total_passed = doc.createTextNode(std::to_string(totalPassed).c_str());
    tag_total_passed.appendChild(total_passed);
    tag_tr_data.appendChild(tag_total_passed);


    QDomElement tag_total_failed = doc.createElement( "td" );
    tag_total_failed.setAttribute("colspan", "2");
    QDomText total_failed = doc.createTextNode(std::to_string(totalFailed).c_str());
    tag_total_failed.appendChild(total_failed);
    tag_tr_data.appendChild(tag_total_failed);

    QDomElement tag_total_notExec = doc.createElement( "td" );
    tag_total_notExec.setAttribute("colspan", "2");
    QDomText total_notExec = doc.createTextNode(std::to_string(totalNotExecuted).c_str());
    tag_total_notExec.appendChild(total_notExec);
    tag_tr_data.appendChild(tag_total_notExec);

    tag_table.appendChild(tag_tr_data);



std::cout<<"\n++++++++++++\n";

    // Fill in the recap table for each protocol

    // if TCP test vector is not empty, then fill the HTML recap table of the TCP protocol
    if (!TCP_prtocol.empty())
    {
        createRecapTableForEachProtocol(doc, tag_table, TCP_prtocol);
    }

    // if UDP test vector is not empty, then fill the HTML recap table of the TCP protocol
    if (!UDP_prtocol.empty())
    {
        createRecapTableForEachProtocol(doc, tag_table, UDP_prtocol);
    }

    // if ARP test vector is not empty, then fill the HTML recap table of the TCP protocol
    if (!ARP_prtocol.empty())
    {
        createRecapTableForEachProtocol(doc, tag_table, ARP_prtocol);
    }

    // if IPV4 test vector is not empty, then fill the HTML recap table of the TCP protocol
    if (!IPV4_prtocol.empty())
    {
        createRecapTableForEachProtocol(doc, tag_table, IPV4_prtocol);
    }

    // if ICMP test vector is not empty, then fill the HTML recap table of the TCP protocol
    if (!ICMP_prtocol.empty())
    {
        createRecapTableForEachProtocol(doc, tag_table, ICMP_prtocol);
    }

    // if DHCP test vector is not empty, then fill the HTML recap table of the TCP protocol
    if (!DHCP_prtocol.empty())
    {
        createRecapTableForEachProtocol(doc, tag_table, DHCP_prtocol);
    }

    // if SOMEIP test vector is not empty, then fill the HTML recap table of the TCP protocol
    if (!SOMEIP_prtocol.empty())
    {
        createRecapTableForEachProtocol(doc, tag_table, SOMEIP_prtocol);
    }

    tag_div_recap.appendChild(tag_table);
    tag_body.appendChild(tag_div_recap);

}

/*!
 * \brief Reporting::createRecapTableForEachProtocol
 * \param doc
 * \param tag_div_recap
 * \param protocol_table
 */
void Reporting::createRecapTableForEachProtocol(QDomDocument &doc, QDomElement &tag_table, const std::unordered_map<int, TestData> protocol_table)
{

    int totalPassed = 0;
    int totalFailed = 0;
    int totalNotExecuted = 0;

    //Calculate the number of passed, failed and not executed test for each protocol
    calculatePassedFailedNotExecuted(totalPassed, totalFailed, totalNotExecuted, protocol_table);

    //std::string protocolName = test_protocol_tab[protocol_table.at(0).getEthernetProtocol()];
    std::string protocolName ;
    for (auto it=protocol_table.begin();it!=protocol_table.end();it++){
        protocolName = test_protocol_tab[it->second.getEthernetProtocol()];
        break;
    }

    std::cout<<"\n++++++++++++\n";
    QDomElement tagLinkToProtocolTable = doc.createElement( "a" );
    std::string link = "#"+protocolName;
    tagLinkToProtocolTable.setAttribute("href", link.c_str());
    QDomText text_protocol_name = doc.createTextNode((protocolName + " Protocol").c_str());
    tagLinkToProtocolTable.appendChild( text_protocol_name );


    QDomElement tag_tr_data = doc.createElement( "tr" );

    /*
     * first test case report of TCP protocol
    */

    QDomElement tag_header_test_id = doc.createElement( "td" );
    //QDomText header_test_id = doc.createTextNode("Total");
    tag_header_test_id.appendChild(tagLinkToProtocolTable);
    tag_tr_data.appendChild(tag_header_test_id);

    QDomElement tag_test_id = doc.createElement( "td" );
    QDomText test_id = doc.createTextNode(std::to_string(protocol_table.size()).c_str());
    tag_test_id.appendChild(test_id);
    tag_tr_data.appendChild(tag_test_id);

    // Display total passed for each protocol in number
    QDomElement tag_total_passed = doc.createElement( "td" );
    QDomText test_passed = doc.createTextNode(std::to_string(totalPassed).c_str());
    tag_total_passed.appendChild(test_passed);
    tag_tr_data.appendChild(tag_total_passed);

    // Display total passed for each protocol in poucentage
    QDomElement tag_tot_passed_poucentage = doc.createElement( "td" );
    std::string poucentage_passed = std::to_string((totalPassed * 100)/protocol_table.size())+"%";
    QDomText tot_passed_pourcentage = doc.createTextNode(poucentage_passed.c_str());
    tag_tot_passed_poucentage.appendChild(tot_passed_pourcentage);
    tag_tr_data.appendChild(tag_tot_passed_poucentage);

    // Display total failed for each protocol in number
    QDomElement tag_test_type = doc.createElement( "td" );
    QDomText test_type = doc.createTextNode(std::to_string(totalFailed).c_str());
    tag_test_type.appendChild(test_type);
    tag_tr_data.appendChild(tag_test_type);

    // Display total failed for each protocol in poucentage
    QDomElement tag_tot_failed_poucentage = doc.createElement( "td" );
    std::string poucentage_failed = std::to_string((totalFailed * 100)/protocol_table.size())+"%";
    QDomText tot_failed_pourcentage = doc.createTextNode(poucentage_failed.c_str());
    tag_tot_failed_poucentage.appendChild(tot_failed_pourcentage);
    tag_tr_data.appendChild(tag_tot_failed_poucentage);

    // Display total not executed for each protocol in number
    QDomElement tag_test_description = doc.createElement( "td" );
    QDomText test_description = doc.createTextNode(std::to_string(totalNotExecuted).c_str());
    tag_test_description.appendChild(test_description);
    tag_tr_data.appendChild(tag_test_description);

    // Display total not executed for each protocol in poucentage
    QDomElement tag_tot_notExec_poucentage = doc.createElement( "td" );
    std::string poucentage_notExec = std::to_string((totalNotExecuted * 100)/protocol_table.size())+"%";
    QDomText tot_notExec_pourcentage = doc.createTextNode(poucentage_notExec.c_str());
    tag_tot_notExec_poucentage.appendChild(tot_notExec_pourcentage);
    tag_tr_data.appendChild(tag_tot_notExec_poucentage);

    tag_table.appendChild(tag_tr_data);

}

void Reporting::calculatePassedFailedNotExecuted(int &totalPassed, int &totalFailed, int &totalNotExecuted, const std::unordered_map<int, TestData> protocol_table)
{
    auto it = protocol_table.begin();
    for(;it != protocol_table.end(); it++)
    {
        switch (it->second.getTestResult())
        {
        case PASSED:
            totalPassed++;
            break;
        case FAILED:
            totalFailed++;
            break;
        case NOT_EXECUTED:
            totalNotExecuted++;
            break;
        }
    }
}





