/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file   mainwindow.cpp
 *  @brief  Qt Base - IHM_Module.
 *  \author uid1344 Tarek Othmani
 *
 *  \addtogroup frontend-com-module
 *  \{
 */

/*==================================================================================================
*                                        Include Files
==================================================================================================*/
#include "mainwindow.h"
#include "ui_mainwindow.h"

class IComModuleInstance : public IComModule
{
};

/*==================================================================================================
*                                        Global Variables
==================================================================================================*/

IComModuleInstance Client;


TestDataCreator testDataCreator;
std::vector<test_t> TestSuite;

/** Test campaign variables*/
int TestRuns = 0, TestExecuted = 0, TestFailed = 0, TestSucceed = 0, TestUnavailable = 0;
/** simulation Mode flag*/
int SimulationMode = 0;
/** Tast campaign execution stop flag*/
int StopFlag = 0;
/** report file path*/
std::string LastReportPath = "";
/** Cell last value */
QString CellLastValue ="";
/** Last row selected */
int LastRow;
/** Last table modified */
QTableWidget *LastTable;



/** DHCP report table variables*/
int Total_DHCP=0, Failed_DHCP=0, Passed_DHCP=0, Not_executed_DHCP=0;
/** ICMP report table variables*/
int Total_ICMP=0,Failed_ICMP=0, Passed_ICMP=0, Not_executed_ICMP=0;
/** ARP report table variables*/
int Total_ARP=0, Failed_ARP=0, Passed_ARP=0, Not_executed_ARP=0;
/** TCP report table variables*/
int Total_TCP=0, Failed_TCP=0, Passed_TCP=0, Not_executed_TCP=0;
/** UDP report table variables*/
int Total_UDP=0, Failed_UDP=0, Passed_UDP=0, Not_executed_UDP=0;
/** IPV4 report table variables*/
int Total_IPV4=0, Failed_IPV4=0, Passed_IPV4=0, Not_executed_IPV4=0;
/** SOMEIP report table variables*/
int Total_SOMEIP=0, Failed_SOMEIP=0, Passed_SOMEIP=0, Not_executed_SOMEIP=0;


void MainWindow::on_connection_update(bool connection_status)
{
    if (connection_status == true)
    {
        qDebug() << "Connection Status Updated: state = Connected";
        emit connection_update_status(connection_status);
    }
    else if (connection_status == false)
    {
        ui->actionstop->setEnabled(false);
        ui->actionDisconnect->setEnabled(false);
        ui->actionRun->setEnabled(false);
        ui->actionConnect->setEnabled(true);
        emit connection_update_status(connection_status);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
    , ConnectDock(new QDockWidget(tr("Connection Parameters")))
    , ConnectWidget(new QWidget(this))
    , ConnectionsettingFillRequest(new QLabel(tr("Please enter a valid parameters"), ConnectDock))
    , TesterAddresslabel(new QLabel(tr("Tester Adress"), ConnectDock))
    , TesterPortlabel(new QLabel(tr("Tester Port"), ConnectDock))
    , TesterAddressline(new QLineEdit(tr("192.168.20.155"), ConnectDock))
    , TesterPortline(new QLineEdit(tr("9876"), ConnectDock))
    , ConnectButton(new QPushButton("Connect", ConnectDock))
    , ConnectGBox(new QGroupBox(tr("Tester Connection")))
    , ConnectMsg(new QLabel(tr(""), ConnectDock))
{
    ui->setupUi(this);
    connect(ConnectButton, &QAbstractButton::clicked, this, &MainWindow::EstablishConnection);
    connect(this, SIGNAL(transmit_to_progressbar(int)), this, SLOT(receive_from_ui(int)));
    connect(this, SIGNAL(transmit_to_cell(int)), this, SLOT(inprogressadd(int)));
    connect(this, SIGNAL(report_enable()), this, SLOT(enable_report()));
    connect(this, SIGNAL(error_detected(int)), this, SLOT(error_message(int)));
    connect(this, SIGNAL(connection_update_status(bool)), this, SLOT(update_connection_status(bool)));
    connect(this, SIGNAL(UpdateReportView(int)), this, SLOT(ReportViewUpdating(int)));
    connect(this, SIGNAL(InvalidConf(int, int,QTableWidget*)), this, SLOT(InvalidConfMsg(int, int,QTableWidget*)));
    MainWindowInit();
    emit connection_update_status(false);

}

MainWindow::~MainWindow()
{
    generate_report();
    delete ui;
}

/***************************************************************
                MainWindow Functions
***************************************************************/

void MainWindow::on_actionopen_file_triggered()
{
}

void MainWindow::on_actionConnect_triggered()
{
    if (!ConnectDock->isHidden())
    {
        qDebug() << "check if Connect Dialog is opened already";
        ConnectDock->close();
    }
    qDebug() << "creating Connect dialog box";
    ConnectDock->setWindowIcon((QIcon) ":/images/images/Connect1.png");
    TesterAddresslabel->setMinimumWidth(80);
    TesterAddresslabel->setStyleSheet("background-color:  rgb(244, 244, 244);");
    TesterPortlabel->setMinimumWidth(80);
    TesterPortlabel->setStyleSheet("background-color:  rgb(244, 244, 244);");
    TesterAddressline->setMinimumWidth(202);
    TesterAddressline->setStyleSheet("background-color: rgb(255, 255, 255)");
    TesterPortline->setMinimumWidth(202);
    TesterPortline->setStyleSheet("background-color: rgb(255, 255, 255)");
    ConnectDock->setMinimumSize(320, 200);
    ConnectButton->setStyleSheet("QPushButton {border: 2px solid #8f8f91;background-color:  rgb(244, 244, 244);max-width: 80px;min-height: 20px;}QPushButton:pressed {background-color:  rgb(205, 232, 255);}QPushButton:hover { background-color: rgb(229, 243, 255);};");
    ConnectButton->setLayoutDirection(Qt::RightToLeft);
    ConnectMsg->setMaximumHeight(20);
    ConnectionsettingFillRequest->setMaximumHeight(20);
    ConnectGBox->setStyleSheet("QGroupBox{border: 2px solid #8f8f91;background-color:  rgb(244, 244, 244);}");
    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(TesterAddresslabel);
    layout1->addWidget(TesterAddressline);
    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->addWidget(TesterPortlabel);
    layout2->addWidget(TesterPortline);
    QVBoxLayout *layout3 = new QVBoxLayout;
    layout3->addWidget(ConnectionsettingFillRequest);
    layout3->addLayout(layout1);
    layout3->addLayout(layout2);
    layout3->addWidget(ConnectMsg);
    layout3->addWidget(ConnectButton);
    ConnectGBox->setLayout(layout3);
    QHBoxLayout *layout4 = new QHBoxLayout;
    layout4->addWidget(ConnectGBox);
    ConnectWidget->setLayout(layout4);
    ConnectDock->setWidget(ConnectWidget);
    ConnectDock->setStyleSheet("background-color: rgb(226, 226, 226)");
    ConnectDock->show();
}

void MainWindow::on_actionDisconnect_triggered()
{
    qDebug() << "Disconnection from tester";
    QDateTime now = QDateTime::currentDateTime();
    qDebug() << "adding Disconnection log";
    ui->LogsPText->insertPlainText(now.toString() + ": Disconnection request started\n");
    qDebug() << "Enable/Disable buttons";
    ui->actionstop->setEnabled(false);
    ui->actionDisconnect->setEnabled(false);
    ui->actionRun->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    qDebug() << "IComModule disconnect function trigger";
    Client.DisConnect();
    QDateTime now1 = QDateTime::currentDateTime();
    ui->LogsPText->insertPlainText(now1.toString() + ": Disconnected from tester\n");
}

void MainWindow::on_actionRun_triggered()
{
    qDebug() << "Action Run triggered";
    ui->MainTab->setCurrentIndex(0);
    qDebug() << "checkin data validity";
    bool ValidConf = ValidityDoubleCheck();
    if (ValidConf == true)
    {
        if (SimulationMode != 1)
        {
            qDebug() << "seeting general configuration...";
            set_abs_conf();
            Sleep(500);
            qDebug() << "general configuration has been set successfully!!\nsetting TCP configuration...";
            set_tcp_conf();
            Sleep(500);
            qDebug() << "TCP configuration has been set successfully!!\nsetting ARP configuration...";
            set_arp_conf();
            Sleep(500);
            qDebug() << "ARP configuration has been set successfully!!\nsetting ICMP configuration";
            set_icmp_conf();
            Sleep(500);
            qDebug() << "ICMP configuration has been set successfully!!\nsetting UDP configuration";
            set_udp_conf();
            Sleep(500);
            qDebug() << "UDP configuration has been set successfully!!\nsetting IPv4 setting...";
            set_ipv4_conf();
            Sleep(500);
            qDebug()<<"IPv4 configuration has been set successfully!!";
        }

        TestSucceed = 0;TestFailed = 0;TestUnavailable = 0;TestExecuted = 0;
        /** DHCP report table variables*/
        Total_DHCP=0, Failed_DHCP=0, Passed_DHCP=0, Not_executed_DHCP=0;
        /** ICMP report table variables*/
        Total_ICMP=0,Failed_ICMP=0, Passed_ICMP=0, Not_executed_ICMP=0;
        /** ARP report table variables*/
        Total_ARP=0, Failed_ARP=0, Passed_ARP=0, Not_executed_ARP=0;
        /** TCP report table variables*/
        Total_TCP=0, Failed_TCP=0, Passed_TCP=0, Not_executed_TCP=0;
        /** UDP report table variables*/
        Total_UDP=0, Failed_UDP=0, Passed_UDP=0, Not_executed_UDP=0;
        /** IPV4 report table variables*/
        Total_IPV4=0, Failed_IPV4=0, Passed_IPV4=0, Not_executed_IPV4=0;
        /** SOMEIP report table variables*/
        Total_SOMEIP=0, Failed_SOMEIP=0, Passed_SOMEIP=0, Not_executed_SOMEIP=0;
        ui->SucceededPercentage->setText("0%");
        ui->FailedPercentage->setText("0%");
        ui->UnavailablePercentage->setText("0%");
        ui->TestRunspBar->setValue(0);
        ui->actionstop->setEnabled(true);
        ui->actionRun->setEnabled(false);
        ui->actionClear->setEnabled(false);
        ui->TestCasesTree->setEnabled(false);
        ui->MainTab->setTabEnabled(1, false);

        std::thread t([=]()
                      {
                          uint16_t test_result;
                          int VTestDuration;
                          QTableWidgetItem *StatusItem = new QTableWidgetItem;
                          QTableWidgetItem *ResultCodeItem = new QTableWidgetItem;
                          QFont font;
                          font.setBold(true);
                          font.setPointSize(10);
                          for (int counter = 0; counter < ui->TestSetTable->rowCount(); counter++)
                          {
                              uint16_t _test_id = ui->TestSetTable->item(counter, 0)->text().toInt();

                              qDebug() << "actaul test ID" << _test_id;
                              emit transmit_to_cell(counter);
                              QTableWidgetItem *LastRunTime = ui->TestSetTable->item(counter, 4);
                              QDateTime now = QDateTime::currentDateTime();
                              QString LastTime = now.toString("dd/MM/yyyy hh:mm:ss");
                              LastRunTime->setText(LastTime);
                              qDebug() << LastTime + ": test " + ui->TestSetTable->item(counter, 1)->text() + " started\n";
                              ui->LogsPText->insertPlainText(LastTime + ": test " + ui->TestSetTable->item(counter, 1)->text() + " started\n");
                              if (SimulationMode == 1)
                              {
                                  test_result = QRandomGenerator::global()->bounded(3);
                                  VTestDuration = QRandomGenerator::global()->bounded(100, 5000);
                                  Sleep(VTestDuration);
                              }
                              else
                              {
                                  test_t CurrentTest;
                                  for (int counter = 0; counter < (int)TestSuite.size(); counter++)
                                  {
                                      if (TestSuite.at(counter).ID == _test_id)
                                      {
                                          CurrentTest = TestSuite.at(counter);
                                          break;
                                      }
                                  }
                                  test_result = Client.StartTest(_test_id, CurrentTest.API);
                              }
                              TestExecuted++;
                              qDebug() << "removing In progress widget";
                              ui->TestSetTable->removeCellWidget(counter, 2);
                              qDebug() << "In progress widget removed";
                              if (test_result == 0)
                              {
                                  qDebug() << "test succeeded";
                                  StatusItem = ui->TestSetTable->item(counter, 2);
                                  StatusItem->setText("Succeeded");
                                  StatusItem->setForeground(QBrush(QColor(0, 200, 0)));
                                  StatusItem->setFont(font);
                                  ResultCodeItem = ui->TestSetTable->item(counter, 3);
                                  ResultCodeItem->setText(QString::fromStdString(std::to_string(test_result)));
                                  TestSucceed++;
                                  time_t rawtime;
                                  struct tm *timeinfo;
                                  char buffer[80];

                                  time(&rawtime);
                                  timeinfo = localtime(&rawtime);

                                  strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
                                  std::string str(buffer);
                                  testDataCreator.updateTestStatus((int)_test_id, PASSED, true, str, "Test Passed OK");
                              }
                              else if (test_result == 2)
                              {
                                  qDebug() << "test unavailable";
                                  StatusItem = ui->TestSetTable->item(counter, 2);
                                  StatusItem->setText("Unavailable");
                                  StatusItem->setForeground(QBrush(QColor(251, 147, 24)));
                                  StatusItem->setFont(font);
                                  //StatusItem->setBackground(Qt::yellow);
                                  ResultCodeItem = ui->TestSetTable->item(counter, 3);
                                  ResultCodeItem->setText(QString::fromStdString(std::to_string(test_result)));
                                  TestUnavailable++;
                                  time_t rawtime;
                                  struct tm *timeinfo;
                                  char buffer[80];

                                  time(&rawtime);
                                  timeinfo = localtime(&rawtime);
                                  strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
                                  std::string str(buffer);
                                  testDataCreator.updateTestStatus((int)_test_id, NOT_EXECUTED, true, str, "Test Unavailable");

                              }
                              else
                              {
                                  qDebug() << "test failed";
                                  StatusItem = ui->TestSetTable->item(counter, 2);
                                  StatusItem->setText("Failed");
                                  StatusItem->setForeground(QBrush(QColor(230, 0, 0)));
                                  StatusItem->setFont(font);
                                  ResultCodeItem = ui->TestSetTable->item(counter, 3);
                                  ResultCodeItem->setText(QString::fromStdString(std::to_string(test_result)));
                                  TestFailed++;
                                  time_t rawtime;
                                  struct tm *timeinfo;
                                  char buffer[80];

                                  time(&rawtime);
                                  timeinfo = localtime(&rawtime);
                                  strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
                                  std::string str(buffer);
                                  testDataCreator.updateTestStatus((int)_test_id, FAILED, true, str, "Test Failed");
                              }
                              TestExecuted = counter + 1;
                              emit UpdateReportView(_test_id);
                              QDateTime testendtime = QDateTime::currentDateTime();
                              ui->LogsPText->insertPlainText(testendtime.toString() + ": test " + ui->TestSetTable->item(counter, 1)->text() + " ended. Exit Code = " + QString::fromStdString(std::to_string(test_result)) + "\n\n");
                              ui->SucceededValLabel->setText(QString::fromStdString(std::to_string(TestSucceed)));
                              ui->SucceededPercentage->setText(QString::fromStdString(std::to_string((int)(((float)TestSucceed / (float)TestRuns) * 100)) + "%"));
                              ui->FailedValLabel->setText(QString::fromStdString(std::to_string(TestFailed)));
                              ui->FailedPercentage->setText(QString::fromStdString(std::to_string((int)(((float)TestFailed / (float)TestRuns) * 100)) + "%"));
                              ui->UnavailableLabel->setText(QString::fromStdString(std::to_string(TestUnavailable)));
                              ui->UnavailablePercentage->setText(QString::fromStdString(std::to_string((int)(((float)TestUnavailable / (float)TestRuns) * 100)) + "%"));

                              emit transmit_to_progressbar((int)(((float)(TestExecuted) / (float)TestRuns) * 100));
                              if (StopFlag == 1)
                              {
                                  qDebug() << "stop button triggered";
                                  emit error_detected(1);
                                  ui->actionstop->setEnabled(false);
                                  ui->actionRun->setEnabled(true);
                                  ui->actionClear->setEnabled(true);
                                  ui->TestCasesTree->setEnabled(true);
                                  ui->MainTab->setTabEnabled(1, true);
                                  StopFlag = 0;
                                  break;
                              }
                          }
                          ui->actionstop->setEnabled(false);
                          ui->actionRun->setEnabled(true);
                          ui->actionClear->setEnabled(true);
                          ui->TestCasesTree->setEnabled(true);
                          ui->MainTab->setTabEnabled(1, true);
                          generate_report();
                          emit report_enable();
                      });
        qDebug() << "thread detaching";
        t.detach();
    }
    else
    {
        qDebug() << "Invalid configuration detected";
        emit error_detected(2);
    }
    qDebug() << "quiting ActionRun";
}

void MainWindow::on_actionstop_triggered()
{
    StopFlag = 1;
}

void MainWindow::on_actionReport_triggered()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(LastReportPath.c_str()));
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actiondesc_triggered()
{
    ui->DescDock->show();
}

void MainWindow::on_actionlogo_triggered()
{
    QDesktopServices::openUrl(QUrl("https://www.focus-corporation.com/"));
}

void MainWindow::on_actionSysInfo_triggered()
{
    QDockWidget *sysinfodock = new QDockWidget(tr("System Information"), this);
    QPlainTextEdit *SysInfo = new QPlainTextEdit();
    SysInfo->width();
    SysInfo->height();
    SysInfo->setStyleSheet("background : white;");
    std::string currentarch = QSysInfo::currentCpuArchitecture().toStdString();
    SysInfo->insertPlainText((std::string("\nCurrent Architecture:\t") + currentarch).c_str());
    std::string ProdType = QSysInfo::prettyProductName().toStdString();
    SysInfo->insertPlainText((std::string("\nProduct Type:\t") + ProdType).c_str());
    std::string KernelType = QSysInfo::kernelType().toStdString();
    SysInfo->insertPlainText((std::string("\nKernel Type:\t") + KernelType).c_str());
    std::string KernelVers = QSysInfo::kernelVersion().toStdString();
    SysInfo->insertPlainText((std::string("\nKernel Version:\t") + KernelVers).c_str());
    std::string MachineID = QSysInfo::machineHostName().toStdString();
    SysInfo->insertPlainText((std::string("\nMachine ID:\t") + MachineID).c_str());
    std::string NetInterface = "", IP = "", MAC = "";
    foreach (QNetworkInterface networkInterface, QNetworkInterface::allInterfaces())
    {
        if (networkInterface.flags().testFlag(QNetworkInterface::IsUp) && !networkInterface.flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            foreach (QNetworkAddressEntry entry, networkInterface.addressEntries())
            {
                if (entry.ip().toString().contains("."))
                {
                    NetInterface = networkInterface.name().toStdString();
                    IP = entry.ip().toString().toStdString();
                    MAC = networkInterface.hardwareAddress().toStdString();
                    SysInfo->insertPlainText((std::string("\nNetwork Interface: ") + NetInterface).c_str());
                    SysInfo->insertPlainText((std::string("\tIP: ") + IP).c_str());
                    SysInfo->insertPlainText((std::string("\tMAC: ") + MAC).c_str());
                }
            }
        }
    }

    QVBoxLayout *sysinfolayout = new QVBoxLayout;
    SysInfo->setLayout(sysinfolayout);
    QVBoxLayout *docklayout = new QVBoxLayout;
    sysinfodock->setLayout(docklayout);
    sysinfodock->setWidget(SysInfo);
    sysinfodock->setFloating(true);
    sysinfodock->show();
}

void MainWindow::on_actionTest_and_Configuration_triggered()
{
    ui->TestConfDock->show();
}

void MainWindow::on_actionSystem_Logs_triggered()
{
    ui->LogsDock->show();
}

void MainWindow::on_actionSaveLog_triggered()
{
    QDateTime now = QDateTime::currentDateTime();
    QString filename = "C:/Users/Tarek-PC/Desktop/log" + now.toString("ddMMyyyy_hh_mm_ss") + ".txt";
    qDebug() << filename;
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << ui->LogsPText->toPlainText();
    }
    ui->LogsPText->insertPlainText(now.toString() + ": log Saved\n");
}

void MainWindow::on_ConfigTree_doubleClicked(const QModelIndex &index)
{
    ui->MainTab->setCurrentIndex(1);
    if (index.data().toString() == "General Configuration")
    {
        qDebug() << "General Config Clicked";
        ui->ConfigurationTabWidget->setCurrentIndex(0);
    }
    else if (index.data().toString() == "TCP")
    {
        qDebug() << "TCP Clicked";
        ui->ConfigurationTabWidget->setCurrentIndex(1);
    }
    else if (index.data().toString() == "UDP")
    {
        qDebug() << "UDP Clicked";
        ui->ConfigurationTabWidget->setCurrentIndex(2);
    }
    else if (index.data().toString() == "ARP")
    {
        qDebug() << "ARP Clicked";
        ui->ConfigurationTabWidget->setCurrentIndex(3);
    }
    else if (index.data().toString() == "DHCP")
    {
        qDebug() << "DHCP Clicked";
        ui->ConfigurationTabWidget->setCurrentIndex(4);
    }
    else if (index.data().toString() == "IPv4")
    {
        qDebug() << "IPv4 Clicked";
        ui->ConfigurationTabWidget->setCurrentIndex(5);
    }
    else if (index.data().toString() == "ICMPv4")
    {
        qDebug() << "ICMPv4 Clicked";
        ui->ConfigurationTabWidget->setCurrentIndex(6);
    }
    else if (index.data().toString() == "SOMEIP")
    {
        qDebug() << "SOMEIP Clicked";
        ui->ConfigurationTabWidget->setCurrentIndex(7);
    }
}

void MainWindow::on_TestCasesTree_itemClicked(QTreeWidgetItem *item)
{
    for (int counter = 0; counter < (int)TestSuite.size(); counter++)
    {
        if (TestSuite.at(counter).Name == item->text(0).toStdString())
        {
            ui->DescriptionPText->setPlainText(QString::fromStdString(TestSuite.at(counter).Name + "\n" + TestSuite.at(counter).Description));
            break;
        }
    }
}

void MainWindow::on_TestCasesTree_itemChanged(QTreeWidgetItem *item)
{

    if (item->checkState(0) == Qt::Checked)
    {
        if (item->childCount() > 0)
        {
            qDebug() << "childcount is greater than 0";
            for (int counter = 0; counter < item->childCount(); counter++)
            {
                qDebug() << "checking childs";
                item->child(counter)->setCheckState(0, Qt::Checked);
            }
            if ((item->text(0) != "TCP") && (item->text(0) != "UDP") && (item->text(0) != "ARP") && (item->text(0) != "DHCP") && (item->text(0) != "IPv4") && (item->text(0) != "ICMP") && (item->text(0) != "SOMEIP"))
            {
                bool ChildsChecked = true;
                QTreeWidgetItem *parent = new QTreeWidgetItem();
                parent = item->parent();
                for (int counter = 0; counter < parent->childCount(); counter++)
                {
                    if (parent->child(counter)->checkState(0) == Qt::Unchecked)
                    {
                        ChildsChecked = false;
                        break;
                    }
                }

                if (ChildsChecked == true)
                {
                    parent->setCheckState(0, Qt::Checked);
                }
            }
        }
        else
        {
            bool ChildsChecked = true;
            QTreeWidgetItem *parent = new QTreeWidgetItem();
            parent = item->parent();
            for (int counter = 0; counter < parent->childCount(); counter++)
            {
                if (parent->child(counter)->checkState(0) == Qt::Unchecked)
                {
                    ChildsChecked = false;
                    break;
                }
            }

            if (ChildsChecked == true)
            {
                parent->setCheckState(0, Qt::Checked);
            }
            for (int counter = 0; counter < (int)TestSuite.size(); counter++)
            {
                if (QString::fromStdString(TestSuite.at(counter).Name) == item->text(0))
                {
                    ui->TestSetTable->insertRow(ui->TestSetTable->rowCount());
                    QTableWidgetItem *ID = new QTableWidgetItem(QString::fromStdString(std::to_string(TestSuite.at(counter).ID)));
                    ID->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
                    ui->TestSetTable->setItem(ui->TestSetTable->rowCount() - 1, 0, ID);
                    QTableWidgetItem *Name = new QTableWidgetItem(QString::fromStdString(TestSuite.at(counter).Name));
                    Name->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
                    ui->TestSetTable->setItem(ui->TestSetTable->rowCount() - 1, 1, Name);
                    QTableWidgetItem *Status = new QTableWidgetItem("Not executed");
                    Status->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
                    ui->TestSetTable->setItem(ui->TestSetTable->rowCount() - 1, 2, Status);
                    QTableWidgetItem *TestResult = new QTableWidgetItem("N/A");
                    TestResult->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
                    ui->TestSetTable->setItem(ui->TestSetTable->rowCount() - 1, 3, TestResult);
                    QTableWidgetItem *ExecTime = new QTableWidgetItem("N/A");
                    ExecTime->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
                    ui->TestSetTable->setItem(ui->TestSetTable->rowCount() - 1, 4, ExecTime);

                    TestSuite.at(counter).Status = "Checked";
                    for (int columncounter = 0; columncounter < ui->TestSetTable->columnCount(); columncounter++)
                    {
                        if ((ui->TestSetTable->rowCount() - 1) % 2 == 0)
                        {
                            ui->TestSetTable->item(ui->TestSetTable->rowCount() - 1, columncounter)->setBackground(QColor(240, 240, 240));
                        }
                    }
                    std::string testName = TestSuite.at(counter).Name;
                    int testId = TestSuite.at(counter).ID;
                    int ApiId = TestSuite.at(counter).API;
                    test_protocol Protocol = NONE;
                    std::string pstr = TestSuite.at(counter).Protocol;
                    if (pstr == "TCP")
                    {
                        Protocol = TCP;
                    }
                    else if (pstr == "UDP")
                    {
                        Protocol = UDP;
                    }
                    else if (pstr == "ARP")
                    {
                        Protocol = ARP;
                    }
                    else if (pstr == "ICMP")
                    {
                        Protocol = ICMP;
                    }
                    else if (pstr == "IPv4")
                    {
                        Protocol = IPV4;
                    }
                    else if (pstr == "DHCP")
                    {
                        Protocol = DHCP;
                    }
                    else if (pstr == "SOMEIP")
                    {
                        Protocol = SOMEIP;
                    }
                    std::string testDescription = TestSuite.at(counter).Description;
                    std::string testcategory = TestSuite.at(counter).Subprotocol;
                    std::string testStatus = TestSuite.at(counter).Status;
                    std::string expectedResult = "Passed";
                    std::string testResultCause = "";
                    time_t rawtime;
                    struct tm *timeinfo;
                    char buffer[80];
                    time(&rawtime);
                    timeinfo = localtime(&rawtime);
                    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
                    std::string str(buffer);
                    std::string timestamp = str;
                    result testResult = NOT_EXECUTED;
                    bool resultIsReady = false;

                    testDataCreator.addTestData(testId, ApiId, testName, Protocol, testDescription, testcategory, testStatus, testResultCause, timestamp, testResult, resultIsReady);
                    break;
                }
            }
        }
    }
    else if (!item->checkState(0))
    {
        if (item->childCount() > 0)
        {
            for (int counter = 0; counter < item->childCount(); counter++)
            {
                item->child(counter)->setCheckState(0, Qt::Unchecked);
            }

            if ((item->text(0) != "TCP") && (item->text(0) != "UDP") && (item->text(0) != "ARP") && (item->text(0) != "DHCP") && (item->text(0) != "IPv4") && (item->text(0) != "ICMP") && (item->text(0) != "SOMEIP"))
            {

                if (item->parent()->checkState(0) == Qt::Checked)
                {
                    item->parent()->setCheckState(0, Qt::PartiallyChecked);
                }
            }
        }
        else
        {
            if (item->parent()->checkState(0) == Qt::Checked)
            {
                item->parent()->setCheckState(0, Qt::PartiallyChecked);
            }

            QList<QTableWidgetItem *> items = ui->TestSetTable->findItems(item->text(0), Qt::MatchExactly);
            for (QTableWidgetItem *item : items)
            {
                int row = item->row();
                uint16_t testId = ui->TestSetTable->item(row, 0)->text().toInt();
                for (int counter = 0; counter < (int)TestSuite.size(); counter++)
                {
                    if (TestSuite.at(counter).ID == testId)
                    {
                        TestSuite.at(counter).Status = "Not_Checked";
                        break;
                    }
                }
                testDataCreator.removeTestData(testId);
                ui->TestSetTable->removeRow(row);
            }
        }
    }
    TestRuns = ui->TestSetTable->rowCount();
    ui->TestRunsValLabbel->setText(QString::fromStdString(std::to_string(TestRuns)));
}

void MainWindow::on_actionClear_triggered()
{
    ui->TestRunspBar->setValue(0);
    TestSucceed = 0;
    TestFailed = 0;
    TestUnavailable = 0;
    ui->SucceededValLabel->setText(QString::fromStdString(std::to_string(TestSucceed)));
    ui->SucceededPercentage->setText(QString::fromStdString("0%"));
    ui->FailedValLabel->setText(QString::fromStdString(std::to_string(TestFailed)));
    ui->FailedPercentage->setText(QString::fromStdString("0%"));
    ui->UnavailableLabel->setText(QString::fromStdString(std::to_string(TestUnavailable)));
    ui->UnavailablePercentage->setText(QString::fromStdString("0%"));
    TestSuite.clear();
    ui->TestCasesTree->clear();
    ui->DescriptionPText->clear();
    filePath = DEFAULT_XML_PATH;
    QFile *file = new QFile(filePath);
    Open(file);
    TreeviewDataInsertion();
}

void MainWindow::on_LogClearbutton_clicked()
{
    ui->LogsPText->clear();
}

void MainWindow::on_SaveLogButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Log"), "",
                                                    tr("Text Files (*.txt);;All Files (*)"));
    QFile file(fileName);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << ui->LogsPText->toPlainText();
    }
}

void MainWindow::on_actionExport_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Export Test Campaign"), DEFAULT_CAMPAIGN_EXPORT_PATH,
                                                    tr("xml Files (*.xml);;All Files (*)"));
    QFile file(fileName);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Test_Suite>\n";
        for (int counter = 0; counter < (int)TestSuite.size(); counter++)
        {
            stream << "\t<Test>\n\t\t<ID>" << TestSuite.at(counter).ID << "</ID>\n\t\t";
            stream << "<API>" << TestSuite.at(counter).API << "</API>\n\t\t";
            stream << "<Name>" << QString::fromStdString(TestSuite.at(counter).Name) << "</Name>\n\t\t";
            stream << "<Protocol>" << QString::fromStdString(TestSuite.at(counter).Protocol) << "</Protocol>\n\t\t";
            stream << "<Description>" << QString::fromStdString(TestSuite.at(counter).Description) << "</Description>\n\t\t";
            stream << "<Subprotocol>" << QString::fromStdString(TestSuite.at(counter).Subprotocol) << "</Subprotocol>\n\t\t";
            stream << "<Status>" << QString::fromStdString(TestSuite.at(counter).Status) << "</Status>\n\t</Test>\n";
        }
        stream << "</Test_Suite>";
    }
}

void MainWindow::on_actionImport_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Import Test Campaign"), DEFAULT_CAMPAIGN_IMPORT_PATH,
                                                    tr("xml Files (*.xml);;All Files (*)"));
    QFile file(fileName);
    TestSuite.clear();
    ui->TestCasesTree->clear();
    int res = Open(&file);
    if (res == 0)
    {
        TreeviewDataInsertion();
    }
    else
    {
        filePath = DEFAULT_XML_PATH;
        QFile *XmlFile = new QFile(filePath);
        Open(XmlFile);
        TreeviewDataInsertion();
    }
}

void MainWindow::on_MainTab_tabBarClicked(int index)
{
    if (index == 0)
    {
        ui->TestCasesTab->setCurrentIndex(0);
    }
    else if (index == 1)
    {
        ui->TestCasesTab->setCurrentIndex(1);
    }
}

void MainWindow::on_TestCasesTab_tabBarClicked(int index)
{
    if (index == 0)
    {
        ui->MainTab->setCurrentIndex(0);
    }
    else if (index == 1)
    {
        ui->MainTab->setCurrentIndex(1);
    }
}

void MainWindow::on_GenrConfTable_itemChanged(QTableWidgetItem *item)
{
    if (item->column() == 1)
    {
        int CurrentRow = item->row();
        int CurrentColumn = item->column();
        QString Value = item->text();
        QString Conf = ui->GenrConfTable->item(CurrentRow, CurrentColumn - 1)->text();
        ValidityCheck(ui->GenrConfTable, &Conf, &Value, CurrentRow);
    }
}

void MainWindow::on_TcpConfTable_itemChanged(QTableWidgetItem *item)
{
    if (item->column() == 1)
    {
        int CurrentRow = item->row();
        int CurrentColumn = item->column();
        QString Value = item->text();
        QString Conf = ui->TcpConfTable->item(CurrentRow, CurrentColumn - 1)->text();
        ValidityCheck(ui->TcpConfTable, &Conf, &Value, CurrentRow);
    }
}

void MainWindow::on_UdpConfTable_itemChanged(QTableWidgetItem *item)
{
    if (item->column() == 1)
    {
        int CurrentRow = item->row();
        int CurrentColumn = item->column();
        QString Value = item->text();
        QString Conf = ui->UdpConfTable->item(CurrentRow, CurrentColumn - 1)->text();
        ValidityCheck(ui->UdpConfTable, &Conf, &Value, CurrentRow);
    }
}

void MainWindow::on_ArpConfTable_itemChanged(QTableWidgetItem *item)
{
    if (item->column() == 1)
    {
        int CurrentRow = item->row();
        int CurrentColumn = item->column();
        QString Value = item->text();
        QString Conf = ui->ArpConfTable->item(CurrentRow, CurrentColumn - 1)->text();
        ValidityCheck(ui->ArpConfTable, &Conf, &Value, CurrentRow);
    }
}

void MainWindow::on_DhcpConfTable_itemChanged(QTableWidgetItem *item)
{
    if (item->column() == 1)
    {
        int CurrentRow = item->row();
        int CurrentColumn = item->column();
        QString Value = item->text();
        QString Conf = ui->DhcpConfTable->item(CurrentRow, CurrentColumn - 1)->text();
        ValidityCheck(ui->DhcpConfTable, &Conf, &Value, CurrentRow);
    }
}

void MainWindow::on_Ipv4ConfTable_itemChanged(QTableWidgetItem *item)
{
    if (item->column() == 1)
    {
        int CurrentRow = item->row();
        int CurrentColumn = item->column();
        QString Value = item->text();
        QString Conf = ui->Ipv4ConfTable->item(CurrentRow, CurrentColumn - 1)->text();
        ValidityCheck(ui->Ipv4ConfTable, &Conf, &Value, CurrentRow);
    }
}

void MainWindow::on_IcmpConfTable_itemChanged(QTableWidgetItem *item)
{
    if (item->column() == 1)
    {
        int CurrentRow = item->row();
        int CurrentColumn = item->column();
        QString Value = item->text();
        QString Conf = ui->IcmpConfTable->item(CurrentRow, CurrentColumn - 1)->text();
        ValidityCheck(ui->IcmpConfTable, &Conf, &Value, CurrentRow);
    }
}

void MainWindow::on_SomeIpConfTable_itemChanged(QTableWidgetItem *item)
{
    if (item->column() == 1)
    {
        int CurrentRow = item->row();
        int CurrentColumn = item->column();
        QString Value = item->text();
        QString Conf = ui->SomeIpConfTable->item(CurrentRow, CurrentColumn - 1)->text();
        ValidityCheck(ui->SomeIpConfTable, &Conf, &Value, CurrentRow);
    }
}

/**************************************************************
 *            data validity check function
 *************************************************************/

bool isDigits(const std::string &str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}

bool isValidIpAddress(const std::string ipAddress)
{
    QHostAddress address(ipAddress.c_str());
    return (QAbstractSocket::IPv4Protocol == address.protocol() && ipAddress.length() > 6);
}

bool isValidMACAddress(std::string str)
{
    bool valid = false;
    // Regex to check valid MAC address
    const std::regex pattern(
        "^([0-9A-Fa-f]{2}[:-]){5}"
        "([0-9A-Fa-f]{2})|([0-9a-"
        "fA-F]{4}\\.[0-9a-fA-F]"
        "{4}\\.[0-9a-fA-F]{4})$");

    // If the MAC address
    // is empty return false
    if (str.empty())
    {
        valid = false;
    }

    // Return true if the MAC address
    // matched the ReGex
    if (regex_match(str, pattern))
    {
        valid = true;
    }
    else
    {
        valid = false;
    }
    return valid;
}

bool isValidHex(std::string s)
{
    bool valid = false;
    if (s.substr(0, 2) == "0x")
    {
        // Size of string
        int n = s.length();

        // Iterate over string
        for (int i = 2; i < n; i++)
        {
            char ch = s[i];

            // Check if the character
            // is invalid
            if ((ch < '0' || ch > '9') && (ch < 'A' || ch > 'F'))
            {
                valid = false;
                break;
            }
            else
            {
                valid = true;
            }
        }
    }
    else
    {
        valid = false;
    }
    return valid;
}

bool isValidPort(int Port)
{
    bool valid = false;
    if (Port > 1000 && Port < 100000)
    {
        valid = true;
    }
    else
    {
        valid = false;
    }
    return valid;
}

void MainWindow::ValidityCheck(QTableWidget *ConfTable, QString *Conf, QString *Value, int CurrentRow)
{
    if (Conf->contains("IP_ADDR", Qt::CaseInsensitive))
    {
        if (!isValidIpAddress(Value->toStdString()))
        {
            emit InvalidConf(1,CurrentRow, ConfTable);
            //ConfTable->setItem(CurrentRow, 2, InvalidIPitem);
            ConfTable->item(CurrentRow,1)->setText(CellLastValue);
        }
    }

    if (Conf->contains("MAC_ADDR", Qt::CaseInsensitive))
    {
        if (!isValidMACAddress(Value->toStdString()))
        {
            emit InvalidConf(2,CurrentRow, ConfTable);
            //ConfTable->setItem(CurrentRow, 2, InvalidMacitem);
            ConfTable->item(CurrentRow,1)->setText(CellLastValue);
        }
    }
    if (Conf->contains("_HEX", Qt::CaseInsensitive))
    {
        if (!isValidHex(Value->toStdString()))
        {
            emit InvalidConf(3,CurrentRow, ConfTable);
            //ConfTable->setItem(CurrentRow, 2, InvalidHexitem);
            ConfTable->item(CurrentRow,1)->setText(CellLastValue);
        }
    }

    if (Conf->contains("_PORT", Qt::CaseInsensitive))
    {
        if (!isValidPort(Value->toInt()))
        {
            emit InvalidConf(4,CurrentRow, ConfTable);
            //ConfTable->setItem(CurrentRow, 2, InvalidPortitem);
            ConfTable->item(CurrentRow,1)->setText(CellLastValue);
        }
    }
}

bool MainWindow::ValidityDoubleCheck()
{
    bool AllValid = true;
    for (int rowCounter = 0; rowCounter < ui->GenrConfTable->rowCount(); rowCounter++)
    {
        qDebug() << "Validity point 2 reached";
        ;
        if (ui->GenrConfTable->item(rowCounter, ui->GenrConfTable->columnCount() - 1))
        {
            qDebug() << "invalid General Conf";
            AllValid = false;
            break;
        }
    }
    if (AllValid == true)
    {
        for (int rowCounter = 0; rowCounter < ui->TcpConfTable->rowCount(); rowCounter++)
        {
            if (ui->TcpConfTable->item(rowCounter, 2))
            {
                qDebug() << "invalid TCP Conf";
                AllValid = false;
                break;
            }
        }
        if (AllValid == true)
        {
            for (int rowCounter = 0; rowCounter < ui->UdpConfTable->rowCount(); rowCounter++)
            {
                if (ui->UdpConfTable->item(rowCounter, 2))
                {
                    qDebug() << "invalid UDP Conf";
                    AllValid = false;
                    break;
                }
            }

            if (AllValid == true)
            {
                for (int rowCounter = 0; rowCounter < ui->ArpConfTable->rowCount(); rowCounter++)
                {
                    if (ui->ArpConfTable->item(rowCounter, 2))
                    {
                        qDebug() << "invalid ARP Conf";
                        AllValid = false;
                        break;
                    }
                }

                if (AllValid == true)
                {
                    for (int rowCounter = 0; rowCounter < ui->Ipv4ConfTable->rowCount(); rowCounter++)
                    {
                        if (ui->Ipv4ConfTable->item(rowCounter, 2))
                        {
                            qDebug() << "invalid IPv4 Conf";
                            AllValid = false;
                            break;
                        }
                    }

                    if (AllValid == true)
                    {
                        for (int rowCounter = 0; rowCounter < ui->IcmpConfTable->rowCount(); rowCounter++)
                        {
                            if (ui->IcmpConfTable->item(rowCounter, 2))
                            {
                                qDebug() << "invalid ICMP Conf";
                                AllValid = false;
                                break;
                            }
                        }

                        if (AllValid == true)
                        {
                        }
                        else
                        {
                            AllValid = false;
                        }
                    }
                    else
                    {
                        AllValid = false;
                    }
                }
                else
                {
                    AllValid = false;
                }
            }
            else
            {
                AllValid = false;
            }
        }
        else
        {
            AllValid = false;
        }
    }
    else
    {
        AllValid = false;
    }
    return AllValid;
}

/**************************************************************
                       Added Function
**************************************************************/

int MainWindow::EstablishConnection()
{
    QDateTime now = QDateTime::currentDateTime();
    ui->LogsPText->insertPlainText(now.toString() + ": Connection request started\n");
    QMessageBox *Msgbox = new QMessageBox(ConnectDock);
    if (TesterAddressline->text().isEmpty() || TesterPortlabel->text().isEmpty())
    {
        std::cout << "parameter not set\n";
        ConnectMsg->setText("Missing parameters detected");
    }
    else
    {

        if (isDigits(TesterPortline->text().toStdString()) && isValidIpAddress(TesterAddressline->text().toStdString()))
        {
            ConnectMsg->setText("Valid Parameters");
            int iResult = Client.ConnectToTester(TesterAddressline->text().toLocal8Bit().constData(), TesterPortline->text().split(" ")[0].toInt());
            switch (iResult)
            {
            case 0:
            {
                std::cout << "connection established successfully\n";
                QDateTime now = QDateTime::currentDateTime();
                ui->LogsPText->insertPlainText(now.toString() + ": Connection established successfully\n");
                ui->actionConnect->setEnabled(false);
                ui->actionDisconnect->setEnabled(true);
                ui->actionRun->setEnabled(true);
                ConnectDock->close();
                break;
            }
            case -1:
            {
                std::cout << "connection can't be established!!\n error while reading config file\n";
                QDateTime now = QDateTime::currentDateTime();
                ui->LogsPText->insertPlainText(now.toString() + ": Connection request Failed\n");
                Msgbox->setText("Error while reading config file\nFailed to establish connection with " + TesterAddressline->text());
                Msgbox->exec();
                break;
            }
            case -2:
            {
                std::cout << "connection can't be established!!\n error while initializing socket\n";
                QDateTime now = QDateTime::currentDateTime();
                ui->LogsPText->insertPlainText(now.toString() + ": Connection request Failed\n");
                Msgbox->setText("Error while initializing socket\nFailed to establish connection with " + TesterAddressline->text());
                Msgbox->exec();
                break;
            }
            case -3:
            {
                std::cout << "connection can't be established!!\n error while creating socket\n";
                QDateTime now = QDateTime::currentDateTime();
                ui->LogsPText->insertPlainText(now.toString() + ": Connection request Failed\n");
                Msgbox->setText("Error while creating socket!\nFailed to establish connection with " + TesterAddressline->text());
                Msgbox->exec();
                break;
            }
            case -4:
            {
                std::cout << "connection can't be established!!\n error while connecting socket\n";
                QDateTime now = QDateTime::currentDateTime();
                ui->LogsPText->insertPlainText(now.toString() + ": Connection request Failed\n");
                Msgbox->setText("Error while connecting socket!\nFailed to establish connection with " + TesterAddressline->text());
                Msgbox->exec();
                break;
            }
            case -5:
            {
                std::cout << "error sending start frame!!!\n";
                QDateTime now = QDateTime::currentDateTime();
                ui->LogsPText->insertPlainText(now.toString() + ": Connection request Failed\n");
                Msgbox->setText("Error sending start frame!!!\n");
                Msgbox->exec();
                break;
            }
            case -6:
            {
                std::cout << "recieve failed\n";
                QDateTime now = QDateTime::currentDateTime();
                ui->LogsPText->insertPlainText(now.toString() + ": Connection request failed\n");
                Msgbox->setText("Failed to recieve start frame\n");
                Msgbox->exec();
                break;
            }
            }
        }
        else if (isDigits(TesterPortline->text().toStdString()) && !isValidIpAddress(TesterAddressline->text().toStdString()))
        {
            ConnectMsg->setStyleSheet("font-weight:600; color:#ff0000;");
            ConnectMsg->setText("Invalid IP address: " + TesterAddressline->text());
        }
        else if (!isDigits(TesterPortline->text().toStdString()) && isValidIpAddress(TesterAddressline->text().toStdString()))
        {
            ConnectMsg->setStyleSheet("font-weight:600; color:#ff0000;");
            ConnectMsg->setText("Invalid Port: " + TesterPortline->text());
        }
        else
        {
            ConnectMsg->setStyleSheet("font-weight:600; color:#ff0000;");
            ConnectMsg->setText("Invalid parameters, please check again!");
        }
    }
    Client.RegisterTesterConnection(std::bind(&MainWindow::on_connection_update, this, _1));
    return 0;
}

int MainWindow::Open(QFile *file)
{
    int ret = 1;
    test_t test_struct;
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << "Couldn't open xml file.xml to load settings for tests import";
        emit error_detected(4);
        ret = 1;
    }
    else
    {
        QXmlStreamReader *xmlReader = new QXmlStreamReader(file);
        //Parse the XML until we reach end of it
        if (!xmlReader->hasError())
        {
            while (!xmlReader->atEnd() && !xmlReader->hasError())
            {
                // Read next element
                QXmlStreamReader::TokenType token = xmlReader->readNext();
                //If token is just StartDocument - go to next
                if (token == QXmlStreamReader::StartDocument)
                {
                    continue;
                }
                //If token is StartElement - read it
                if (token == QXmlStreamReader::StartElement)
                {

                    if (xmlReader->name().toString() == "ID")
                    {
                        QString qstr = xmlReader->readElementText();
                        //qDebug() << qstr;
                        test_struct.ID = qstr.toInt();
                    }

                    else if (xmlReader->name().toString() == "API")
                    {
                        QString qstr = xmlReader->readElementText();
                        //qDebug() << qstr;
                        test_struct.API = qstr.toInt();
                    }

                    else if (xmlReader->name().toString() == "Name")
                    {
                        QString qstr = xmlReader->readElementText();
                        //qDebug() << qstr;
                        test_struct.Name = qstr.toStdString();
                    }

                    else if (xmlReader->name().toString() == "Protocol")
                    {
                        QString qstr = xmlReader->readElementText();
                        //qDebug() << qstr;
                        test_struct.Protocol = qstr.toStdString();
                    }

                    else if (xmlReader->name().toString() == "Description")
                    {
                        QString qstr = xmlReader->readElementText();
                        //qDebug() << qstr;
                        test_struct.Description = qstr.toStdString();
                    }

                    else if (xmlReader->name().toString() == "Subprotocol")
                    {
                        QString qstr = xmlReader->readElementText();
                        //qDebug() << qstr;
                        test_struct.Subprotocol = qstr.toStdString();
                    }

                    else if (xmlReader->name().toString() == "Status")
                    {
                        QString qstr = xmlReader->readElementText();
                        //qDebug() << qstr;
                        test_struct.Status = qstr.toStdString();
                        TestSuite.push_back(test_struct);
                    }
                }
            }
            ret = 0;
        }

        else
        {
            qCritical() << "error while parsing Xml file";
            emit error_detected(5);
            ret = 2;
        }

        //close reader and flush file
        xmlReader->clear();
    }
    file->close();
    return ret;
}

void MainWindow::TreeviewDataInsertion()
{
    qDebug() << "Clearing treeview";
    ui->TestCasesTree->clear();
    QTreeWidgetItem *topLevelItem = NULL;
    qDebug() << "Treeview insertion";
    for (int i = 0; i < (int)TestSuite.size(); i++)
    {
        test_t ActualTest = TestSuite.at(i);
        QList<QTreeWidgetItem*> Protocol = ui->TestCasesTree->findItems(QString::fromStdString(ActualTest.Protocol), Qt::MatchFixedString);
        if (Protocol.isEmpty())
        {
            topLevelItem = new QTreeWidgetItem;
            topLevelItem->setText(0, QString::fromStdString(ActualTest.Protocol));
            topLevelItem->setCheckState(0, Qt::Unchecked);
            ui->TestCasesTree->addTopLevelItem(topLevelItem);
        }
        else
        {
            topLevelItem = Protocol.at(0);
        }

        QTreeWidgetItem *parentItem = topLevelItem;
        // iterate through children of parentItem to see if this directory exists
        bool thisSubprotocolExists = false;
        for (int j = 0; j < parentItem->childCount(); ++j)
        {
            if (QString::fromStdString(TestSuite.at(i).Subprotocol) == parentItem->child(j)->text(0))
            {
                thisSubprotocolExists = true;
                parentItem = parentItem->child(j);
                break;
            }
        }

        if (!thisSubprotocolExists)
        {
            parentItem = new QTreeWidgetItem(parentItem);
            parentItem->setText(0, QString::fromStdString(TestSuite.at(i).Subprotocol));
            parentItem->setCheckState(0, Qt::Unchecked);
        }
        // iterate through children of parentItem to see if this directory exists
        bool thisNameExists = false;
        for (int j = 0; j < parentItem->childCount(); ++j)
        {
            if (QString::fromStdString(TestSuite.at(i).Name) == parentItem->child(j)->text(0))
            {
                thisNameExists = true;
                parentItem = parentItem->child(j);
                break;
            }
        }

        if (!thisNameExists)
        {
            parentItem = new QTreeWidgetItem(parentItem);
            parentItem->setText(0, QString::fromStdString(TestSuite.at(i).Name));
            if (TestSuite.at(i).Status == "Checked")
            {
                parentItem->setCheckState(0, Qt::Checked);
            }
            else
            {
                parentItem->setCheckState(0, Qt::Unchecked);
            }
        }
    }
}

void MainWindow::receive_from_ui(int value)
{
    qDebug() << "Progress bar update";
    ui->TestRunspBar->setValue(value);
    ui->percentage->setText(QString::fromStdString(std::to_string(ui->TestRunspBar->value())) + "%");
    QString pBarStyle = "QProgressBar:horizontal {border: 1px solid gray;border-radius: 10px;background: white;padding: 1px;}";
    if ((TestSucceed == 0) && (TestFailed == 0) && (TestUnavailable != 0))
    {
        pBarStyle += "QProgressBar::chunk:horizontal {border-radius: 8px;background-color: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5,stop: 0 orange, stop: 1 orange)}";
    }
    else if ((TestSucceed == 0) && (TestFailed != 0) && (TestUnavailable == 0))
    {
        pBarStyle += "QProgressBar::chunk:horizontal {border-radius: 8px;background-color: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5,stop: 0 red, stop: 1 red)}";
    }
    else if ((TestSucceed != 0) && (TestFailed == 0) && (TestUnavailable == 0))
    {
        pBarStyle += "QProgressBar::chunk:horizontal {border-radius: 8px;background-color: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5,stop: 0 green, stop: 1 green)}";
    }
    else if ((TestSucceed != 0) && (TestFailed != 0) && (TestUnavailable == 0))
    {
        qDebug() << QString::fromStdString(std::to_string((float)TestSucceed / (float)TestExecuted));
        pBarStyle += "QProgressBar::chunk:horizontal {border-radius: 8px;background-color: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5,stop: 0 green, stop: " + QString::fromStdString(std::to_string((float)TestSucceed / (float)TestExecuted)) + " green,stop:" + QString::fromStdString(std::to_string(((float)TestSucceed / (float)TestExecuted) + 0.0001)) + " red, stop: 1 red)}";
    }
    else if ((TestSucceed == 0) && (TestFailed != 0) && (TestUnavailable != 0))
    {
        qDebug() << QString::fromStdString(std::to_string((float)TestUnavailable / (float)TestExecuted));
        pBarStyle += "QProgressBar::chunk:horizontal {border-radius: 8px;background-color: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5,stop: 0 orange, stop: " + QString::fromStdString(std::to_string((float)TestUnavailable / (float)TestExecuted)) + " orange,stop:" + QString::fromStdString(std::to_string(((float)TestUnavailable / (float)TestExecuted) + 0.0001)) + " red, stop: 1 red)}";
    }
    else if ((TestSucceed != 0) && (TestFailed == 0) && (TestUnavailable != 0))
    {
        qDebug() << QString::fromStdString(std::to_string((float)TestSucceed / (float)TestExecuted));
        pBarStyle += "QProgressBar::chunk:horizontal {border-radius: 8px;background-color: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5,stop: 0 green, stop: " + QString::fromStdString(std::to_string((float)TestSucceed / (float)TestExecuted)) + " green,stop:" + QString::fromStdString(std::to_string(((float)TestSucceed / (float)TestExecuted) + 0.0001)) + " orange, stop: 1 orange)}";
    }
    else if ((TestSucceed != 0) && (TestFailed != 0) && (TestUnavailable != 0))
    {
        qDebug() << QString::fromStdString(std::to_string((float)TestSucceed / (float)TestExecuted));
        qDebug() << QString::fromStdString(std::to_string(((float)TestSucceed + (float)TestUnavailable) / (float)TestExecuted));
        pBarStyle += "QProgressBar::chunk:horizontal {border-radius: 8px;background-color: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5,stop: 0 green, stop: " + QString::fromStdString(std::to_string((float)TestSucceed / (float)TestExecuted)) + " green,stop: " + QString::fromStdString(std::to_string(((float)TestSucceed / (float)TestExecuted) + 0.00001)) + " orange, stop: " + QString::fromStdString(std::to_string(((float)TestSucceed + (float)TestUnavailable) / (float)TestExecuted)) + " orange,stop: " + QString::fromStdString(std::to_string((((float)TestSucceed + (float)TestUnavailable) / (float)TestExecuted) + 0.00001)) + " red, stop: 1 red)}";
    }
    ui->TestRunspBar->setStyleSheet(pBarStyle);
    qDebug() << "Progress bar updated";
}

void MainWindow::inprogressadd(int row)
{
    qDebug() << "In progress label enabling signal emitted";
    QWidget *ProgressWidget = new QWidget();
    QLabel *ProgressLabel = new QLabel();
    QLabel *ProgressLabel1 = new QLabel();
    ProgressLabel1->setText("In Progress");
    ProgressLabel1->setStyleSheet("font-weight:300;color:rgb(0,0,230)");
    QMovie *movie = new QMovie(":/OtherImages/images/inprogress.gif");
    movie->start();
    movie->setScaledSize(QSize(15, 15));
    ProgressLabel->setMovie(movie);
    QHBoxLayout *Layout = new QHBoxLayout();
    Layout->addWidget(ProgressLabel1);
    Layout->addWidget(ProgressLabel);
    ProgressWidget->setLayout(Layout);
    if (row % 2 == 0)
    {
        ProgressWidget->setStyleSheet("background-color:rgb(240,240,240)");
    }
    ui->TestSetTable->setCellWidget(row, 2, ProgressWidget);
    qDebug() << "In progress widget is set successfully";
}

void MainWindow::on_TestConfDock_visibilityChanged(bool visible)
{
    if (visible == false)
    {
        ui->actionTest_and_Configuration->setIconVisibleInMenu(false);
    }
    else
    {
        ui->actionTest_and_Configuration->setIconVisibleInMenu(true);
    }
}

void MainWindow::on_DescDock_visibilityChanged(bool visible)
{
    if (visible == false)
    {
        ui->actiondesc->setIconVisibleInMenu(false);
    }
    else
    {
        ui->actiondesc->setIconVisibleInMenu(true);
    }
}

void MainWindow::on_LogsDock_visibilityChanged(bool visible)
{
    if (visible == false)
    {
        ui->actionSystem_Logs->setIconVisibleInMenu(false);
    }
    else
    {
        ui->actionSystem_Logs->setIconVisibleInMenu(true);
    }
}

void MainWindow::on_actionSimulationMode_triggered()
{
    QMessageBox *Msgbox = new QMessageBox();
    if (SimulationMode == 0)
    {
        SimulationMode = 1;
        Msgbox->setText("Simulation mode activated!");
        Msgbox->exec();
        ui->actionRun->setEnabled(true);
        ui->actionConnect->setEnabled(false);
        emit connection_update_status(true);
    }
    else
    {
        SimulationMode = 0;
        Msgbox->setText("Simulation mode deactivated!");
        Msgbox->exec();
        ui->actionRun->setEnabled(false);
        ui->actionConnect->setEnabled(true);
        emit connection_update_status(false);
    }
}

int MainWindow::load_config_file(std::string filepath, QTableWidget *ConfTable, std::string ConfType)
{
    int ret = 1;
    std::cout << "loading config file\n";
    std::ifstream file(filepath);

    if (file.is_open())
    {
        std::string str;
        std::getline(file, str);
        if (str.find(ConfType) != std::string::npos)
        {
            int counter = 0;
            while (std::getline(file, str))
            {
                if (str.find('#') == std::string::npos && str.find('=') != std::string::npos)
                {
                    std::string ConfName = str.substr(0, str.find('='));
                    std::string value = str.substr(str.find('=') + 1, str.length());
                    QString conf = QString::fromUtf8(ConfName.c_str());
                    QString val = QString::fromUtf8(value.c_str());
                    QTableWidgetItem *Confitem = new QTableWidgetItem(conf);
                    QTableWidgetItem *Valitem = new QTableWidgetItem(val);
                    ConfTable->insertRow(ConfTable->rowCount());
                    ConfTable->setItem(ConfTable->rowCount() - 1, 0, Confitem);
                    ConfTable->setItem(ConfTable->rowCount() - 1, 1, Valitem);
                    std::cout << "ConfName=" << ConfName << "\n";
                    std::cout << "value=" << value << "\n";
                    std::cout << "***************************\n";
                    for (int columncounter = 0; columncounter < ConfTable->columnCount() - 1; columncounter++)
                    {
                        if ((ConfTable->rowCount() - 1) % 2 == 0)
                        {
                            ConfTable->item(ConfTable->rowCount() - 1, columncounter)->setBackground(QColor(240, 240, 240));
                        }
                    }
                    counter++;
                }
            }
            ret = 0;
        }
        else
        {
            qDebug() << "Invalid" << QString::fromStdString(ConfType) << "file has been selected";
            emit error_detected(3);
            ret = 1;
        }
    }
    else
    {
        std::cout << filepath << " not found!!\n";
        ret = 2;
    }
    return ret;
}

void MainWindow::Save_Config_file(QTableWidget *ConfTable, std::string ConfType)
{
    QString textData;
    textData = "#" + QString::fromStdString(ConfType) + "_Conf\n#Default configuration\n#ConfName=Value\n";
    int rows = ConfTable->rowCount();
    int columns = ConfTable->columnCount() - 1;

    for (int i = 0; i < rows; i++)
    {
        bool b = false;
        for (int j = 0; j < columns; j++)
        {

            textData += ConfTable->item(i, j)->text();
            if (!b)
            {
                textData += "="; // for .conf file format
                b = 1;
            }
        }

        textData += "\n";
    }

    QString filename = QFileDialog::getSaveFileName(this, "Save Txt", "./doc/", "CONF files (*.conf)");
    QFile txtFile(filename);
    if (txtFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {

        QTextStream out(&txtFile);
        out << textData;

        txtFile.close();
    }
}

void MainWindow::set_arp_conf()
{
    ARPConfig_t arpconf;
    arpconf.DYNAMIC_ARP_CACHETIMEOUT = ui->ArpConfTable->item(0, 1)->text().toInt();
    strcpy((char *)arpconf.HOST_1_IP, ui->ArpConfTable->item(1, 1)->text().toStdString().c_str());
    strcpy((char *)arpconf.DIface_0_IP, ui->ArpConfTable->item(2, 1)->text().toStdString().c_str());
    arpconf.ParamListenTime = ui->ArpConfTable->item(3, 1)->text().toInt();
    strcpy((char *)arpconf.MAC_ADDR1, ui->ArpConfTable->item(4, 1)->text().toStdString().c_str());
    strcpy((char *)arpconf.MAC_ADDR2, ui->ArpConfTable->item(5, 1)->text().toStdString().c_str());
    strcpy((char *)arpconf.MAC_ADDR3, ui->ArpConfTable->item(6, 1)->text().toStdString().c_str());
    strcpy((char *)arpconf.DIFACE_O_MAC_ADDR, ui->ArpConfTable->item(7, 1)->text().toStdString().c_str());
    strcpy((char *)arpconf.ARBIT_MAC_ADDR, ui->ArpConfTable->item(8, 1)->text().toStdString().c_str());
    arpconf.ARP_HARDWARE_TYPE_UNKNOWN = ui->ArpConfTable->item(9, 1)->text().toInt();
    arpconf.UNKNOWN_HW_ADDR_LEN = ui->ArpConfTable->item(10, 1)->text().toInt();
    arpconf.ARP_PROTOCOL_UNKNOWN = ui->ArpConfTable->item(11, 1)->text().toInt();
    arpconf.UNKNOWN_PROTCOL_ADDR_LEN = ui->ArpConfTable->item(12, 1)->text().toInt();
    arpconf.ARP_TOLERANCE_TIME = ui->ArpConfTable->item(13, 1)->text().toInt();
    arpconf.ETHERNET_ADDR_LEN = ui->ArpConfTable->item(14, 1)->text().toInt();
    arpconf.ARP_HARDWARE_ETHERNET = ui->ArpConfTable->item(15, 1)->text().toInt();
    arpconf.IP_ADDR_LEN = ui->ArpConfTable->item(16, 1)->text().toInt();
    arpconf.ARP_PROTOCOL_IP = ui->ArpConfTable->item(17, 1)->text().toInt();
    arpconf.OPERATION_REQUEST = ui->ArpConfTable->item(18, 1)->text().toInt();
    strcpy((char *)arpconf.all_zeroes, ui->ArpConfTable->item(19, 1)->text().toStdString().c_str());
    strcpy((char *)arpconf.ETHERNET_BROADCAST_ADDR, ui->ArpConfTable->item(20, 1)->text().toStdString().c_str());
    strcpy((char *)arpconf.IP_FIRST_UNUSED_ADDR_INTERFACE_1, ui->ArpConfTable->item(21, 1)->text().toStdString().c_str());
    arpconf.OPERATION_RESPONSE = ui->ArpConfTable->item(22, 1)->text().toInt();
    Client.SetARPConfig(arpconf);
}
void MainWindow::set_udp_conf()
{
    UDPConfig_t udpconf;
    udpconf.testerUDPPort = ui->UdpConfTable->item(0, 1)->text().toInt();
    udpconf.testerUDPPort2 = ui->UdpConfTable->item(1, 1)->text().toInt();
    udpconf.unusedUDPDstPort1 = ui->UdpConfTable->item(2, 1)->text().toInt();
    udpconf.unusedUDPSrcPort = ui->UdpConfTable->item(3, 1)->text().toInt();
    udpconf.DUTUDPPort = ui->UdpConfTable->item(4, 1)->text().toInt();
    udpconf.udpUserDataSize = ui->UdpConfTable->item(5, 1)->text().toInt();
    udpconf.calculatedUDPChecksum = ui->UdpConfTable->item(6, 1)->text().toInt();
    udpconf.incorrectUDPChecksum = ui->UdpConfTable->item(7, 1)->text().toInt();
    strcpy((char *)udpconf.UDPDefaultData, ui->UdpConfTable->item(8, 1)->text().toStdString().c_str());
    udpconf.UDPDefaultDataLen = ui->UdpConfTable->item(9, 1)->text().toInt();
    strcpy((char *)udpconf.UDPData, ui->UdpConfTable->item(10, 1)->text().toStdString().c_str());
    udpconf.UDPDataLen = ui->UdpConfTable->item(11, 1)->text().toInt();
    strcpy((char *)udpconf.Dst_IP_addr, ui->UdpConfTable->item(12, 1)->text().toStdString().c_str());
    strcpy((char *)udpconf.Host_1_IP, ui->UdpConfTable->item(13, 1)->text().toStdString().c_str());
    strcpy((char *)udpconf.Host_2_IP, ui->UdpConfTable->item(14, 1)->text().toStdString().c_str());
    strcpy((char *)udpconf.AIface_0_BcastIP, ui->UdpConfTable->item(15, 1)->text().toStdString().c_str());
    strcpy((char *)udpconf.allSystemMCastAddr, ui->UdpConfTable->item(16, 1)->text().toStdString().c_str());

    Client.SetUDPConfig(udpconf);
}
void MainWindow::set_ipv4_conf()
{
    IPv4_config_t ipv4conf;
    strcpy((char *)ipv4conf.DUT_IP, ui->Ipv4ConfTable->item(0, 1)->text().toStdString().c_str());
    qDebug()<<"hna 1";
    strcpy((char *)ipv4conf.TESTER_IP, ui->Ipv4ConfTable->item(1, 1)->text().toStdString().c_str());
    qDebug()<<"hna 2";
    ipv4conf.DUT_PORT = ui->Ipv4ConfTable->item(2, 1)->text().toInt();
    qDebug()<<"hna 3";
    ipv4conf.TESTER_PORT = ui->Ipv4ConfTable->item(3, 1)->text().toInt();
    qDebug()<<"hna 4";
    ipv4conf.INVALID_CHECKSUM = ui->Ipv4ConfTable->item(4, 1)->text().toInt();
    qDebug()<<"hna 5";
    ipv4conf.TTL = ui->Ipv4ConfTable->item(5, 1)->text().toInt();
    qDebug()<<"hna 6";
    ipv4conf.LARGE_TTL_VALUE = ui->Ipv4ConfTable->item(6, 1)->text().toInt();
    qDebug()<<"hna 7";
    ipv4conf.LOW_TTL_VALUE = ui->Ipv4ConfTable->item(7, 1)->text().toInt();
    qDebug()<<"hna 8";
    strcpy((char *)ipv4conf.DIRECTED_BROADCAST_ADDRESS, ui->Ipv4ConfTable->item(8, 1)->text().toStdString().c_str());
    qDebug()<<"hna 9";
    strcpy((char *)ipv4conf.LIMITED_BROADCAST_ADDRESS, ui->Ipv4ConfTable->item(9, 1)->text().toStdString().c_str());
    qDebug()<<"hna 10";
    ipv4conf.LISTEN_TIME = ui->Ipv4ConfTable->item(10, 1)->text().toInt();
    qDebug()<<"hna 11";
    ipv4conf.IP_INI_REASSEMBLE_TIMEOUT = ui->Ipv4ConfTable->item(11, 1)->text().toInt();
    qDebug()<<"hna 12";
    ipv4conf.FRAGMENT_REASSEMBLY_TIMEOUT = ui->Ipv4ConfTable->item(12, 1)->text().toInt();
    qDebug()<<"hna 13";
    ipv4conf.IP_VERSION = ui->Ipv4ConfTable->item(13, 1)->text().toInt();
    qDebug()<<"hna 14";
    ipv4conf.IP_TYPE_ICMP = ui->Ipv4ConfTable->item(14, 1)->text().toInt();
    qDebug()<<"hna 15";
    ipv4conf.IP_TYPE_TCP = ui->Ipv4ConfTable->item(15, 1)->text().toInt();
    qDebug()<<"hna 16";
    ipv4conf.MTU = ui->Ipv4ConfTable->item(16, 1)->text().toInt();
    qDebug()<<"hna 17";
    Client.SetIPV4Config(ipv4conf);
}
void MainWindow::set_icmp_conf()
{
    ICMPv4Config_t icmpconf;
    strcpy((char *)icmpconf.DUT_IP, ui->IcmpConfTable->item(0, 1)->text().toStdString().c_str());
    strcpy((char *)icmpconf.TESTER_IP, ui->IcmpConfTable->item(1, 1)->text().toStdString().c_str());
    icmpconf.ICMP_IDENTIFIER = ui->IcmpConfTable->item(2, 1)->text().toInt();
    icmpconf.ICMP_SEQUENCE_NUMBER = ui->IcmpConfTable->item(3, 1)->text().toInt();
    icmpconf.INVALID_CHECKSUM = ui->IcmpConfTable->item(4, 1)->text().toInt();
    icmpconf.FRAGMENT_REASSEMBLY_TIMEOUT = ui->IcmpConfTable->item(5, 1)->text().toInt();
    icmpconf.LISTEN_TIME = ui->IcmpConfTable->item(6, 1)->text().toInt();
    icmpconf.UNSUPPORTED_PROTOCOL = ui->IcmpConfTable->item(7, 1)->text().toInt();
    icmpconf.INVALID_ICMP_TYPE = ui->IcmpConfTable->item(8, 1)->text().toInt();
    strcpy((char *)icmpconf.BROADCAST_ADDRESS, ui->IcmpConfTable->item(8, 1)->text().toStdString().c_str());
    icmpconf.ORINGIN_TIMESTAMP_VALUE = ui->IcmpConfTable->item(8, 1)->text().toInt();
    Client.SetICMPConfig(icmpconf);
}
void MainWindow::set_tcp_conf()
{
    TCP_config_t tcpconf;
    tcpconf.DUT_Port = ui->TcpConfTable->item(0, 1)->text().toInt();
    tcpconf.Tester_Port = ui->TcpConfTable->item(1, 1)->text().toInt();
    strcpy(tcpconf.TESTER_IP, ui->TcpConfTable->item(2, 1)->text().toStdString().c_str());
    strcpy(tcpconf.DUT_IP, ui->TcpConfTable->item(3, 1)->text().toStdString().c_str());
    tcpconf.Maxcon = ui->TcpConfTable->item(4, 1)->text().toInt();
    tcpconf.ISN = ui->TcpConfTable->item(5, 1)->text().toInt();
    tcpconf.mss = ui->TcpConfTable->item(6, 1)->text().toInt();
    tcpconf.window = ui->TcpConfTable->item(7, 1)->text().toInt();
    tcpconf.nss = ui->TcpConfTable->item(8, 1)->text().toInt();
    tcpconf.ssz = ui->TcpConfTable->item(9, 1)->text().toInt();
    tcpconf.tmx = ui->TcpConfTable->item(10, 1)->text().toInt();
    tcpconf.tfn = ui->TcpConfTable->item(11, 1)->text().toInt();
    tcpconf.wnp = ui->TcpConfTable->item(12, 1)->text().toInt();
    tcpconf.uopt = ui->TcpConfTable->item(13, 1)->text().toInt();
    tcpconf.msl = ui->TcpConfTable->item(14, 1)->text().toInt();
    tcpconf.PORT1 = ui->TcpConfTable->item(15, 1)->text().toInt();
    tcpconf.SEQ1 = ui->TcpConfTable->item(16, 1)->text().toInt();
    Client.SetTCPConfig(tcpconf);
}
void MainWindow::set_abs_conf()
{
    NetAPIConf_t absconf;
    absconf.S_interface = ui->GenrConfTable->item(0, 1)->text().toStdString();
    absconf.S_DUT_HW_Address = ui->GenrConfTable->item(1, 1)->text().toStdString();
    absconf.S_DUT_IP_Address = ui->GenrConfTable->item(2, 1)->text().toStdString();
    absconf.S_Tester_HW_Address = ui->GenrConfTable->item(3, 1)->text().toStdString();
    absconf.S_Tester_IP_Address = ui->GenrConfTable->item(4, 1)->text().toStdString();
    std::cout << "wwwwwwwwwwwwwwwwwwww\n"
              << absconf.S_DUT_HW_Address << "\n";
    Client.SetNetAPIConfig(absconf);
}

void MainWindow::on_LoadConfButton_clicked()
{
    std::cout << "loading config file\n";
    QString filename = QFileDialog::getOpenFileName(NULL, "chose", DEFAULT_CONF_PATH);
    if (ui->TcpConfTable->isVisible())
    {
        ui->TcpConfTable->setRowCount(0);
        int res = load_config_file(filename.toStdString(), ui->TcpConfTable, "TCP");
        if (res != 0)
        {
            std::string filepath_Tcp = std::string(DEFAULT_CONF_PATH) + "Config_Tcp.conf";
            load_config_file(filepath_Tcp, ui->TcpConfTable, "TCP");
        }
    }
    else if (ui->UdpConfTable->isVisible())
    {
        ui->UdpConfTable->setRowCount(0);
        int res = load_config_file(filename.toStdString(), ui->UdpConfTable, "UDP");
        if (res != 0)
        {
            std::string filepath_Udp = std::string(DEFAULT_CONF_PATH) + "Config_Udp.conf";
            load_config_file(filepath_Udp, ui->UdpConfTable, "UDP");
        }
    }
    else if (ui->ArpConfTable->isVisible())
    {
        ui->ArpConfTable->setRowCount(0);
        int res = load_config_file(filename.toStdString(), ui->ArpConfTable, "ARP");
        if (res != 0)
        {
            std::string filepath_Arp = std::string(DEFAULT_CONF_PATH) + "Config_Arp.conf";
            load_config_file(filepath_Arp, ui->ArpConfTable, "ARP");
        }
    }
    else if (ui->Ipv4ConfTable->isVisible())
    {
        ui->Ipv4ConfTable->setRowCount(0);
        int res = load_config_file(filename.toStdString(), ui->Ipv4ConfTable, "IPv4");
        if (res != 0)
        {
            std::string filepath_Ipv4 = std::string(DEFAULT_CONF_PATH) + "Config_Ipv4.conf";
            load_config_file(filepath_Ipv4, ui->Ipv4ConfTable, "IPv4");
        }
    }
    else if (ui->IcmpConfTable->isVisible())
    {
        ui->IcmpConfTable->setRowCount(0);
        int res = load_config_file(filename.toStdString(), ui->IcmpConfTable, "ICMP");
        if (res != 0)
        {
            std::string filepath_Icmp = std::string(DEFAULT_CONF_PATH) + "Config_Icmp.conf";
            load_config_file(filepath_Icmp, ui->IcmpConfTable, "ICMP");
        }
    }
    else if (ui->GenrConfTable->isVisible())
    {
        ui->GenrConfTable->setRowCount(0);
        int res = load_config_file(filename.toStdString(), ui->GenrConfTable, "General");
        if (res != 0)
        {
            std::string filepath_Abs = std::string(DEFAULT_CONF_PATH) + "configuration.conf";
            load_config_file(filepath_Abs, ui->GenrConfTable, "General");
        }
    }
}

void MainWindow::on_SaveConfButton_clicked()
{
    if (ui->TcpConfTable->isVisible())
    {

        Save_Config_file(ui->TcpConfTable, "TCP");
    }
    else if (ui->UdpConfTable->isVisible())
    {

        Save_Config_file(ui->UdpConfTable, "UDP");
    }
    else if (ui->ArpConfTable->isVisible())
    {

        Save_Config_file(ui->ArpConfTable, "ARP");
    }
    else if (ui->Ipv4ConfTable->isVisible())
    {

        Save_Config_file(ui->IcmpConfTable, "IPv4");
    }
    else if (ui->IcmpConfTable->isVisible())
    {

        Save_Config_file(ui->IcmpConfTable, "ICMP");
    }
    else if (ui->GenrConfTable->isVisible())
    {
        Save_Config_file(ui->GenrConfTable, "General");
    }
}

void MainWindow::generate_report()
{

    testDataCreator.printTestList();
    Reporting *report = new Reporting(testDataCreator);
    QString filename = QDateTime::currentDateTime().toString(DEFAULT_REPORT_SAVE_PATH);

    std::string path = filename.toStdString();
    LastReportPath = path;
    std::cout << path << "\n";

    report->setHtmlPath(path);
    report->generateHtmlReport();
    //std::cout<<"indice";
    std::cout << report->gethtmlPath();

    delete report;
}

void MainWindow::enable_report()
{
    if (ui->actionReport->isEnabled() == false)
        ui->actionReport->setEnabled(true);
}

void MainWindow::error_message(int ErrCode)
{
    switch (ErrCode)
    {
    case 1:
    {
        QMessageBox::information(this, tr("Stop Message"),
                                 tr("Test Suite Execution Stopped! (User Request)"),
                                 QMessageBox::Ok);
        break;
    }
    case 2:
    {
        QMessageBox::critical(this, tr("Invalid parameters"),
                              tr("Invalid Parameters detected!! Cannot proceed tests execution.\nPlease re-check your configuration."),
                              QMessageBox::Ok);
        break;
    }
    case 3:
    {
        QMessageBox::critical(this, tr("Invalid configuration file"),
                              tr("Cannot load configuration file!! Invalid file selected\nDefault Configuration file will be reloaded."),
                              QMessageBox::Ok);
        break;
    }
    case 4:
    {
        QMessageBox::critical(this, "XML File Problem",
                              "Couldn't open xml file.xml!! Test cases cannot be imported\nLoading default file",
                              QMessageBox::Ok);
        break;
    }
    case 5:
    {
        QMessageBox::critical(this, "xml Parse Error",
                              "Error while parsing Xml file!! cannot proceed to import Test cases\nLoading default file",
                              QMessageBox::Ok);
        break;
    }
    default:
    {
        break;
    }
    }
}

void MainWindow::update_connection_status(bool status)
{
    if (status == true)
    {
        ConnectionIcon->setPixmap(QPixmap(":/images/images/connected.png").scaledToHeight(ui->statusbar->height()/2));
        ConnectionLabel->setText("Connected");
        ConnectionLabel->setStyleSheet("font-weight:600");
        ConnectionLabel->setMaximumHeight(ui->statusbar->height()/2);
        ui->statusbar->addWidget(ConnectionIcon,0);
        ui->statusbar->addWidget(ConnectionLabel,0);
    }
    else
    {
        ConnectionIcon->setPixmap(QPixmap(":/images/images/Disconnected.png").scaledToHeight(ui->statusbar->height()/2));
        ConnectionLabel->setText("Disconnected");
        ConnectionLabel->setStyleSheet("font-weight:600");
        ConnectionLabel->setMaximumHeight(ui->statusbar->height()/2);
        ui->statusbar->addWidget(ConnectionIcon,0);
        ui->statusbar->addWidget(ConnectionLabel,0);
    }
}

void MainWindow::ConfTablesInit(QTableWidget *ConfTable)
{
    ConfTable->setSelectionMode(QTableWidget::NoSelection);
    ConfTable->setFocusPolicy(Qt::NoFocus);
    ConfTable->setColumnCount(3);
    ConfTable->setHorizontalHeaderLabels(QStringList{"Configuration", "Value", " "});
    int w = ConfTable->horizontalHeader()->width();
    ConfTable->horizontalHeader()->resizeSection(0, 2.5 * w);
    ConfTable->horizontalHeader()->resizeSection(1, 2.5 * w);
    ConfTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ConfTable->horizontalHeader()->setStretchLastSection(true);
    ConfTable->horizontalHeader()->setStyleSheet("QHeaderView::section {font-weight:800;background-color: rgb(24,160,251);color: white;padding-left: 4px;border: 1px solid #6c6c6c;}");
}

void  MainWindow::SummaryTableInit()
{
    ui->SummaryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->SummaryTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->SummaryTable->verticalHeader()->setVisible(false);
    ui->SummaryTable->horizontalHeader()->setVisible(false);
    ui->SummaryTable->setSpan(0,1,1,2);
    ui->SummaryTable->setSpan(0,3,1,2);
    ui->SummaryTable->setSpan(0,5,1,2);
    ui->SummaryTable->setSpan(0,7,1,2);
    ui->SummaryTable->setSpan(1,1,1,2);
    ui->SummaryTable->setSpan(1,3,1,2);
    ui->SummaryTable->setSpan(1,5,1,2);
    ui->SummaryTable->setSpan(1,7,1,2);


     for (int row=0;row<9 ;row++ )

      {

          ui->SummaryTable->item(row,3)->setBackground(QColor(57, 212, 116));
          ui->SummaryTable->item(row,5)->setBackground(QColor(229, 77, 66));
          ui->SummaryTable->item(row,7)->setBackground(QColor(248,180,0));



     if(row>=2)
     {
      ui->SummaryTable->item(row,4)->setBackground(QColor(57, 212, 116));
      ui->SummaryTable->item(row,6)->setBackground(QColor(229, 77, 66));
      ui->SummaryTable->item(row,8)->setBackground(QColor(248,180,0));
      ui->SummaryTable->hideRow(row);
     }

     }



}

void MainWindow::SummaryTableSizeAdjust()
{
int h = 0;

h += ui->SummaryTable->contentsMargins().top()
        + ui->SummaryTable->contentsMargins().bottom();

for (int i=0; i<ui->SummaryTable->rowCount(); ++i)
    h += ui->SummaryTable->rowHeight(i);

ui->SummaryTable->setMinimumHeight(h);
ui->SummaryTable->setMaximumHeight(h);
}

void MainWindow::RportTableSizeAdjust(QTableWidget *table)
{
int  h = 8;
table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


h += table->contentsMargins().top()
        + table->contentsMargins().bottom();

h += table->horizontalHeader()->height();

for (int i=0; i<table->rowCount(); ++i)
    h += table->rowHeight(i);


table->setMinimumHeight(h);
table->setMaximumHeight(h);


}

void MainWindow::InitReportTable(QTableWidget *table)
{
    int w = table->horizontalHeader()->width();
    table->horizontalHeader()->resizeSection(0, w);
    table->horizontalHeader()->resizeSection(1,  2*w);
    table->horizontalHeader()->resizeSection(2, 2*w);
    table->horizontalHeader()->resizeSection(3, 6*w);

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    table->horizontalHeader()->setStretchLastSection(true);
    table->hide();
}

void MainWindow::ShowReportTable(QTableWidget *Summary_table,QTableWidget *Protocol_table,QLabel *label,int ind)
{
    if(Protocol_table->isHidden())
    {
    Summary_table->showRow(ind);
    Protocol_table->show();
    label->show();
    }
}

void MainWindow::AddTestReport(QTableWidget *table,int &test_id,int row,int &total_protocol,int &failed_protocol,int &passed_protocol,int &Not_executed_protocol)
{
    table->setRowCount(total_protocol+1);

    RportTableSizeAdjust(table);
    TestData Test = testDataCreator.getTest(test_id);

    QTableWidgetItem *testId = new QTableWidgetItem(QString::number(Test.getTestId()));
    testId->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    QTableWidgetItem *TestName = new QTableWidgetItem(QString::fromStdString(Test.getTestName()));
    TestName->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    QTableWidgetItem *TestCategory = new QTableWidgetItem(QString::fromStdString(Test.getTestCategory()));
    TestCategory->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    QTableWidgetItem *TestDescription = new QTableWidgetItem(QString::fromStdString(Test.getTestDescription()));
    TestDescription->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    QTableWidgetItem *ResultCause = new QTableWidgetItem(QString::fromStdString(Test.getTestResultCause()));
    ResultCause->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    QTableWidgetItem *TimeStamp = new QTableWidgetItem(QString::fromStdString(Test.getTimestamp()));
    TimeStamp->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    QTableWidgetItem *ExitResult = new QTableWidgetItem();
    ExitResult->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);

   table->setItem(total_protocol, 0, testId);
   table->setItem(total_protocol, 1, TestName);
   table->setItem(total_protocol, 2, TestCategory);
   table->setItem(total_protocol, 3, TestDescription);
   table->setItem(total_protocol, 5, ResultCause);
   table->setItem(total_protocol, 6, TimeStamp);
   int TestResult = Test.getTestResult();

   switch (TestResult)
   {
   case PASSED:
   {
       passed_protocol++;
       ExitResult->setText("Passed");
       table->setItem(total_protocol, 4, ExitResult);
       for (int column=0;column<table->columnCount() ;column++ )
       {
           table->item(total_protocol,column)->setBackground(QColor(72, 230, 62));
       }
       break;
   }
   case FAILED:
   {
       failed_protocol++;
       ExitResult->setText("Failed");
       table->setItem(total_protocol, 4, ExitResult);
       for (int column=0;column<table->columnCount() ;column++ )
       {
           table->item(total_protocol,column)->setBackground(QColor(251, 140, 133));
       }
       break;
   }
   case NOT_EXECUTED:
   {
       Not_executed_protocol++;
       ExitResult->setText("Not Executed");
       table->setItem(total_protocol, 4, ExitResult);
       for (int column=0;column<table->columnCount() ;column++ )
       {
           table->item(total_protocol,column)->setBackground(QColor(248,180,0));
       }
       break;
   }
   default:
       break;
   }

   total_protocol++;
   QString Stotal= QString::number(TestExecuted);
   QString SPassed= QString::number(TestSucceed);
   QString SFailed= QString::number(TestFailed);
   QString Snot_executed= QString::number(TestUnavailable);
   QString Stotal_protocol= QString::number(total_protocol);
   QString Spassed_protocol= QString::number(passed_protocol);
   QString Sfailed_protocol= QString::number(failed_protocol);
   QString SNot_executed_protocol= QString::number(Not_executed_protocol);
   QTableWidgetItem *Total= new QTableWidgetItem();
   Total->setText(Stotal);
   Total->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
   QTableWidgetItem *TotalProtocol= new QTableWidgetItem();
   TotalProtocol->setText(Stotal_protocol);
   TotalProtocol->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
   QTableWidgetItem *Spassed_item= new QTableWidgetItem();
   Spassed_item->setBackground(QColor(57, 212, 116));
   Spassed_item->setText(SPassed);
   Spassed_item->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
   QTableWidgetItem *Spassed_protocol_item= new QTableWidgetItem();
   Spassed_protocol_item->setBackground(QColor(57, 212, 116));
   Spassed_protocol_item->setText(Spassed_protocol);
   Spassed_protocol_item->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
   QTableWidgetItem *Sfailed_item= new QTableWidgetItem();
   Sfailed_item->setBackground(QColor(229, 77, 66));
   Sfailed_item->setText(SFailed);
   Sfailed_item->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
   QTableWidgetItem *Sfailed_protocol_item= new QTableWidgetItem();
   Sfailed_protocol_item->setBackground(QColor(229, 77, 66));
   Sfailed_protocol_item->setText(Sfailed_protocol);
   Sfailed_protocol_item->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
   QTableWidgetItem *Snot_executed_item= new QTableWidgetItem();
   Snot_executed_item->setBackground(QColor(248,180,0));
   Snot_executed_item->setText(Snot_executed);
   Snot_executed_item->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
   QTableWidgetItem *Snot_executed_protocol_item= new QTableWidgetItem();
   Snot_executed_protocol_item->setBackground(QColor(248,180,0));
   Snot_executed_protocol_item->setText(SNot_executed_protocol);
   Snot_executed_protocol_item->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);


     ui->SummaryTable->setItem(1, 1,Total);
     ui->SummaryTable->setItem(row, 1, TotalProtocol);
     ui->SummaryTable->setItem(1, 3, Spassed_item);
     ui->SummaryTable->setItem(row, 3, Spassed_protocol_item);
     ui->SummaryTable->setItem(1, 5, Sfailed_item);
     ui->SummaryTable->setItem(row, 5,Sfailed_protocol_item);
     ui->SummaryTable->setItem(1, 7, Snot_executed_item);
     ui->SummaryTable->setItem(row, 7, Snot_executed_protocol_item);
     for (int i=0;i<4 ;i++ )
     {
         int value=ui->SummaryTable->item(row,1+2*i)->data(Qt::DisplayRole).toInt();
         int pourcentage=value*100/total_protocol;
          QString data= QString::number(pourcentage);
         data= data+"%";

          ui->SummaryTable->setItem(row, 2+2*i, new QTableWidgetItem(data));


     }
     ui->SummaryTable->item(row,4)->setBackground(QColor(57, 212, 116));
     ui->SummaryTable->item(row,6)->setBackground(QColor(229, 77, 66));
     ui->SummaryTable->item(row,8)->setBackground(QColor(248,180,0));




}
void MainWindow::UpdateReportTables(int test_id)
{
    float pourcentagePassed=(float)(TestSucceed)*100/(float)TestExecuted;
     QString Spassed= QString::number(pourcentagePassed);
    Spassed= "Passed "+ Spassed+"%";
    float pourcentageFailed=(float)(TestFailed)*100/(float)TestExecuted;
     QString Sfailed= QString::number(pourcentageFailed);
    Sfailed= "Failed "+Sfailed+"%";
    float pourcentageUnavailable=(float)(TestUnavailable)*100/(float)TestExecuted;
     QString Sunavailable= QString::number(pourcentageUnavailable);
    Sunavailable="unavailable "+ Sunavailable+"%";

    QPieSeries *series = new QPieSeries();
    series->append(Spassed,TestSucceed);
    series->append(Sfailed,TestFailed);
    series->append(Sunavailable,TestUnavailable);



    QPieSlice *slice1 = series->slices().at(0);
    slice1->setBrush(QColor(57, 212, 116));
    if(TestSucceed>0){
    slice1->setLabelVisible(true);
    }
    slice1->setLabelColor(QColor(57, 212, 116));
    slice1->setLabelFont( QFont("Times", 16, QFont::Bold));
    QPieSlice *slice2 = series->slices().at(1);
    slice2->setBrush(QColor(229, 77, 66));
    if(TestFailed>0){
     slice2->setLabelVisible(true);
    }
     slice2->setLabelColor(QColor(229, 77, 66));
     slice2->setLabelFont( QFont("Times", 16, QFont::Bold));
     QPieSlice *slice3 = series->slices().at(2);
     slice3->setBrush(QColor(248,180,0));
     if(TestUnavailable>0){
      slice3->setLabelVisible(true);
     }
      slice3->setLabelColor(QColor(248,180,0));
      slice3->setLabelFont( QFont("Times", 16, QFont::Bold));


     QChart *chart= new QChart;
     chart->addSeries(series);
     chart->setTitle("TC8 Test Result Statistics");
     chart->setTitleFont(QFont("Times", 16, QFont::Bold));
     chart->setFont(QFont("Times", 16, QFont::Bold));
     ChartView->setChart(chart);

     ChartView->setRenderHint(QPainter::Antialiasing);

     ui->ChartLayout->addWidget(ChartView, 0, 0);
     SummaryTableSizeAdjust();

        int Protocol = testDataCreator.getTest(test_id).getEthernetProtocol();

        switch (Protocol)
        {
        case TCP:
        {
            ShowReportTable(ui->SummaryTable,ui->TCPTable,ui->TCPLabel,4);
            AddTestReport(ui->TCPTable,test_id,4,Total_TCP,Failed_TCP,Passed_TCP,Not_executed_TCP);
            break;
        }
        case UDP:
        {
            ShowReportTable(ui->SummaryTable,ui->UDPTable,ui->UDPLabel,5);
            AddTestReport(ui->UDPTable,test_id,5,Total_UDP,Failed_UDP,Passed_UDP,Not_executed_UDP);
            break;
        }
        case ARP:
        {
            ShowReportTable(ui->SummaryTable,ui->ARPTable,ui->ARPLabel,6);
            AddTestReport(ui->ARPTable,test_id,6,Total_ARP,Failed_ARP,Passed_ARP,Not_executed_ARP);
            break;
        }
        case ICMP:
        {
            ShowReportTable(ui->SummaryTable,ui->ICMPTable,ui->ICMPLabel,2);
            AddTestReport(ui->ICMPTable,test_id,2,Total_ICMP,Failed_ICMP,Passed_ICMP,Not_executed_ICMP);
            break;
        }
        case IPV4:
        {
            ShowReportTable(ui->SummaryTable,ui->IPV4Table,ui->IPV4Label,7);
            AddTestReport(ui->IPV4Table,test_id,7,Total_IPV4,Failed_IPV4,Passed_IPV4,Not_executed_IPV4);
            break;
        }
        case DHCP:
        {
            ShowReportTable(ui->SummaryTable,ui->DHCPTable,ui->DHCPLabel,3);
            AddTestReport(ui->DHCPTable,test_id,3,Total_DHCP,Failed_DHCP,Passed_DHCP,Not_executed_DHCP);
            break;
        }
        case SOMEIP:
        {
            ShowReportTable(ui->SummaryTable,ui->SOMEIPTable,ui->SOMEIPLabel,8);
            AddTestReport(ui->SOMEIPTable,test_id,8,Total_SOMEIP,Failed_SOMEIP,Passed_SOMEIP,Not_executed_SOMEIP);
            break;
        }
        default:
            break;
        }






           }




void MainWindow::ReportTablesInit()
{
    ui->ReportTablesFrame->setStyleSheet(".QFrame{background-color: rgb(232, 232, 232) ; border:1px solid black; border-radius: 20;}");
     ui->ReportTablesFrame->setStyleSheet(".QFrame{background-color: rgb(232, 232, 232) ;}");

    ui->ScrollArea->setFrameShape( QFrame::NoFrame );


  SummaryTableInit();
  InitReportTable(ui->ICMPTable);
  InitReportTable(ui->ARPTable);
  InitReportTable(ui->DHCPTable);
  InitReportTable(ui->IPV4Table);
  InitReportTable(ui->TCPTable);
  InitReportTable(ui->UDPTable);
  InitReportTable(ui->SOMEIPTable);




  ui->ChartLayout->addWidget(ChartView, 0, 0);

  ui->SummaryTable->hide();
  ui->SummaryTableLabel->hide();
  ui->ICMPLabel->hide();
  ui->ARPLabel->hide();
  ui->DHCPLabel->hide();
  ui->IPV4Label->hide();
  ui->TCPLabel->hide();
  ui->SOMEIPLabel->hide();
  ui->UDPLabel->hide();
  ui->ReportTablesFrame->hide();
}

void MainWindow::ReportViewUpdating(int _test_id)
{
    ui->SummaryTable->show();
    ui->SummaryTableLabel->show();
    ui->ReportTablesFrame->show();
    SummaryTableSizeAdjust();
    UpdateReportTables(_test_id);
}

void MainWindow::InvalidConfMsg(int ConfTypeID,int CurrentRow, QTableWidget *ConfTable)
{
    QTableWidgetItem *Msg = new QTableWidgetItem();
    Msg->setFont(QFont("font-weight:800"));
    Msg->setForeground(Qt::red);
    switch (ConfTypeID)
    {
    case 1:
    {
        Msg->setText("IP_address has the following format: x . x . x . x where x must be a decimal value between 0 and 255");
        break;
    }
    case 2:
    {
        Msg->setText("MAC_address has the following format: xx:xx:xx:xx:xx:xx where x must be a hexadecimal value between 0 and f");
        break;
    }
    case 3:
    {
        Msg->setText("hexadecimal number is reprented by the letters A–F and numerals 0–9 and preceded by 0x ");
        break;
    }
    case 4:
    {
        Msg->setText("port must be a decimal value between 1000 and 99999");
        break;
    }
    default:
        break;
    }
    ConfTable->setItem(CurrentRow, 2, Msg);
    QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::RemoveWarning);
        timer->start(6000);
}

void MainWindow::MainWindowInit()
{
    qDebug() << "App started";

    qDebug() << "checking if log file exsist already";

    QFile LogFile("log.txt");
    if (LogFile.exists())
    {
        LogFile.remove();
        qDebug() << "log file removed";
    }
    else
    {
        qDebug() << "no log file found";
    }

    /**
     *
     *
     * MainWindow component stylesheet set
     *
     *
    */

    /************** tables set*******************/

    ui->TestRunspBar->setValue(0);
    ui->TestRunspBar->setTextVisible(false);
    resizeDocks({ui->DescDock, ui->LogsDock}, {11, 40}, Qt::Horizontal);
    ui->TestSetTable->setShowGrid(false);
    ui->TestSetTable->setSelectionMode(QTableWidget::NoSelection);
    ui->TestSetTable->setFocusPolicy(Qt::NoFocus);
    ui->TestSetTable->setColumnCount(5);
    ui->TestSetTable->setHorizontalHeaderLabels(QStringList{"Test identifier", "Test name", "Test status", "Exit Code", "Last run time"});
    int w = ui->TestSetTable->horizontalHeader()->width();
    ui->TestSetTable->horizontalHeader()->resizeSection(0, w);
    ui->TestSetTable->horizontalHeader()->resizeSection(1, 6 * w);
    ui->TestSetTable->horizontalHeader()->resizeSection(2, 2 * w);
    ui->TestSetTable->horizontalHeader()->resizeSection(3, 2 * w);
    ui->TestSetTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->TestSetTable->horizontalHeader()->setStretchLastSection(true);
    ui->TestSetTable->horizontalHeader()->setStyleSheet("QHeaderView::section {font-weight:800;background-color: rgb(24,160,251);color: white;padding-left: 4px;border: 1px solid #6c6c6c;}");

    ConfTablesInit(ui->ArpConfTable);
    ConfTablesInit(ui->GenrConfTable);
    ConfTablesInit(ui->TcpConfTable);
    ConfTablesInit(ui->DhcpConfTable);
    ConfTablesInit(ui->IcmpConfTable);
    ConfTablesInit(ui->SomeIpConfTable);
    ConfTablesInit(ui->UdpConfTable);
    ConfTablesInit(ui->Ipv4ConfTable);
    ReportTablesInit();


    /*ui->Summary_Table->hide();
    ui->SummaryTableLabel->hide();
    ui->frame_3->hide();*/

    MainWindow::setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);

    /**
     *
     *
     * @brief Data Load functions
     *
     *
     */

    std::string filepath_Udp = std::string(DEFAULT_CONF_PATH) + "Config_Udp.conf";
    std::string filepath_Arp = std::string(DEFAULT_CONF_PATH) + "Config_Arp.conf";
    std::string filepath_Ipv4 = std::string(DEFAULT_CONF_PATH) + "Config_Ipv4.conf";
    std::string filepath_Icmp = std::string(DEFAULT_CONF_PATH) + "Config_Icmp.conf";
    std::string filepath_Tcp = std::string(DEFAULT_CONF_PATH) + "Config_Tcp.conf";
    std::string filepath_Abs = std::string(DEFAULT_CONF_PATH) + "configuration.conf";
    load_config_file(filepath_Abs, ui->GenrConfTable, "General");
    load_config_file(filepath_Udp, ui->UdpConfTable, "UDP");
    load_config_file(filepath_Arp, ui->ArpConfTable, "ARP");
    load_config_file(filepath_Ipv4, ui->Ipv4ConfTable, "IPv4");
    load_config_file(filepath_Icmp, ui->IcmpConfTable, "ICMP");
    load_config_file(filepath_Tcp, ui->TcpConfTable, "TCP");
    filePath = DEFAULT_XML_PATH;
    QFile *XmlFile = new QFile(filePath);
    Open(XmlFile);
    TreeviewDataInsertion();
    qDebug() << "the vector contains" << TestSuite.size() << "element";

    /************** Log set*******************/

    QDateTime now = QDateTime::currentDateTime();
    ui->LogsPText->insertPlainText(now.toString() + ": Application started\n");
    std::string NetInterface = "", IP = "", MAC = "";
    ui->LogsPText->insertPlainText("Available interfaces:\n\n");
    foreach (QNetworkInterface networkInterface, QNetworkInterface::allInterfaces())
    {
        if (networkInterface.flags().testFlag(QNetworkInterface::IsUp) && !networkInterface.flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            foreach (QNetworkAddressEntry entry, networkInterface.addressEntries())
            {
                if (entry.ip().toString().contains("."))
                {
                    NetInterface = networkInterface.name().toStdString();
                    IP = entry.ip().toString().toStdString();
                    MAC = networkInterface.hardwareAddress().toStdString();
                    ui->LogsPText->insertPlainText((NetInterface + "\n").c_str());
                    ui->LogsPText->insertPlainText((std::string("IP: ") + IP + "\n").c_str());
                    ui->LogsPText->insertPlainText((std::string("MAC: ") + MAC + "\n----\n").c_str());
                }
            }
        }
    }

    /************** Buttons set*******************/

    qDebug() << "App path : " << qApp->applicationDirPath();
    ui->actionstop->setEnabled(false);
    ui->actionDisconnect->setEnabled(false);
    ui->actionRun->setEnabled(false);
    ui->actionReport->setEnabled(false);
}

void MainWindow::on_GenrConfTable_itemSelectionChanged()
{
    if (ui->actionCopy->Trigger)
    {
        QTextEdit().copy();
    }
}

void MainWindow::on_GenrConfTable_cellDoubleClicked(int row, int column)
{
    CellLastValue = ui->GenrConfTable->item(row,column)->text();
    LastRow = row;
    LastTable = ui->GenrConfTable;
    if (ui->actionCopy->Trigger)
    {
        QTextEdit().copy();
    }
    if (ui->actionPaste->Trigger)
    {
        QTextEdit().paste();
    }
}


void MainWindow::on_TcpConfTable_cellDoubleClicked(int row, int column)
{
    LastRow = row;
    LastTable = ui->TcpConfTable;
    CellLastValue = ui->TcpConfTable->item(row,column)->text();
}


void MainWindow::on_UdpConfTable_cellDoubleClicked(int row, int column)
{
    LastRow = row;
    LastTable = ui->UdpConfTable;
    CellLastValue = ui->UdpConfTable->item(row,column)->text();
}


void MainWindow::on_ArpConfTable_cellDoubleClicked(int row, int column)
{
    LastRow = row;
    LastTable = ui->ArpConfTable;
    CellLastValue = ui->ArpConfTable->item(row,column)->text();
}


void MainWindow::on_DhcpConfTable_cellDoubleClicked(int row, int column)
{
    LastRow = row;
    LastTable = ui->DhcpConfTable;
    CellLastValue = ui->DhcpConfTable->item(row,column)->text();
}


void MainWindow::on_Ipv4ConfTable_cellDoubleClicked(int row, int column)
{
    LastRow = row;
    LastTable = ui->Ipv4ConfTable;
    CellLastValue = ui->Ipv4ConfTable->item(row,column)->text();
}


void MainWindow::on_IcmpConfTable_cellDoubleClicked(int row, int column)
{
    LastRow = row;
    LastTable = ui->IcmpConfTable;
    CellLastValue = ui->IcmpConfTable->item(row,column)->text();
}


void MainWindow::on_SomeIpConfTable_cellDoubleClicked(int row, int column)
{
    LastRow = row;
    LastTable = ui->SomeIpConfTable;
    CellLastValue = ui->SomeIpConfTable->item(row,column)->text();
}

void MainWindow::RemoveWarning()
{
    LastTable->takeItem(LastRow,2);
}

