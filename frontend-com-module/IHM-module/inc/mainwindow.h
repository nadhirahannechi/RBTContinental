/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file   mainwindow.h
 *  @brief  Qt Base - IHM_Module.
 *  \author uid1344 Tarek Othmani
 *
 *  \addtogroup frontend-com-module
 *  \{
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/*==================================================================================================
*                                        Include Files
==================================================================================================*/
#include <QApplication>
#include <QtGlobal>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QXmlStreamReader>
#include <QTableWidget>
#include <QDesktopServices>
#include <QLabel>
#include <QPlainTextEdit>
#include <iostream>
#include <QSysInfo>
#include <QStorageInfo>
#include <QNetworkInterface>
#include <QSettings>
#include <QProcess>
#include <QDateTime>
#include <QPushButton>
#include <QLineEdit>
#include <QTreeWidgetItem>
#include <QGroupBox>
#include <QHostAddress>
#include <QtWidgets>
#include <QDomDocument>
#include <unordered_map>
#include <QColor>
#include <QRandomGenerator>
#include <fstream>
#include <regex>
#include <QStatusBar>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QChart>
#include <QTimer>
#include <math.h>
#include "IComModule.h"
#include "testdata.h"
#include "testdatacreator.h"
#include "reporting.h"
/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

//       development variables

#define DEFAULT_XML_PATH "../IHM-module/doc/TestSuite.xml"
#define DEFAULT_CONF_PATH "../IHM-module/doc/"
#define DEFAULT_REPORT_SAVE_PATH "'../IHM-module/TestReports/Report_'yyyy_MM_dd-hh_mm'.html'"
#define DEFAULT_CAMPAIGN_EXPORT_PATH "../IHM-module/doc"
#define DEFAULT_CAMPAIGN_IMPORT_PATH "../IHM-module/doc"

//   deployement Variables
/*
#define DEFAULT_XML_PATH "./doc/TestSuite.xml"
#define DEFAULT_CONF_PATH "./doc/"
#define DEFAULT_REPORT_SAVE_PATH "'./TestReports/Report_'yyyy_MM_dd-hh_mm'.html'"
#define DEFAULT_CAMPAIGN_EXPORT_PATH "./doc"
#define DEFAULT_CAMPAIGN_IMPORT_PATH "./doc"
*/

/*==================================================================================================
*                                             TYPES
==================================================================================================*/

typedef struct
{
    uint16_t ID;
    uint16_t API;
    std::string Name;
    std::string Protocol;
    std::string Description;
    std::string Subprotocol;
    std::string Status;
} test_t;
struct configuration
{

    std::string S_interface;
    std::string S_DUT_HW_Address;
    std::string S_DUT_IP_Address;
    std::string S_Tester_HW_Address;
    std::string S_Tester_IP_Address;
};

/*==================================================================================================
*                                        Global Variables
==================================================================================================*/

/*==================================================================================================
*                                     Local Functions prototypes
==================================================================================================*/

/*==================================================================================================
*                                     Global Functions prototypes
==================================================================================================*/

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /*==================================================================================================
    *                                              Signals
    ==================================================================================================*/
signals:
    /***************************************************************************************************
    *
    *   SIGNAL NAME: transmit_to_progressbar
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This signal is connected to a slot, its emission will trigger it in order to
    *  update the progress bar on GUI.
    *
    *
    * @par
    * @param[in] value   : Type int
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void transmit_to_progressbar(int value);

    /***************************************************************************************************
    *
    *   SIGNAL NAME: transmit_to_cell
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This signal is connected to a slot, its emission will trigger this slot in order to
    *  add In progress widget after starting a test.
    *
    *
    * @par
    * @param[in] row   : Type int
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void transmit_to_cell(int row);

    /***************************************************************************************************
    *
    *   SIGNAL NAME: report_enable
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This signal is connected to a slot, its emission will trigger this slot in order to
    *  enable action report button on GUI.
    *
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void report_enable();

    /***************************************************************************************************
    *
    *   SIGNAL NAME: error_detected
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This signal is connected to a slot, its emission will trigger this slot in order to
    *  display a message box indicating the current detected error on runtime.
    *  Error list:
    *  Error 1:
    *  Error 2:
    *  Error 3:
    *  Error 4:
    *  Error 5:
    *
    *
    * @par
    * @param[in] ErrCode   : Type int
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void error_detected(int ErrCode);

    /***************************************************************************************************
    *
    *   SIGNAL NAME: connection_update_status
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This signal is connected to a slot, its emission will trigger this slot in order to
    *  update the tester connection status at run time
    *
    *
    *
    * @par
    * @param[in] ErrCode   : Type int
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void connection_update_status(bool status);

    /***************************************************************************************************
    *
    *   SIGNAL NAME: UpdateReportView
    *
    ***************************************************************************************************/
    /**
    * @brief
    *
    *
    *
    *
    *
    * @par
    * @param[in] ErrCode   : Type int
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -TODO
    *
    ***************************************************************************************************/
    void UpdateReportView(int _test_id);

    /***************************************************************************************************
    *
    *   SIGNAL NAME: InvalidConf
    *
    ***************************************************************************************************/
    /**
    * @brief
    *
    *
    *
    *
    *
    * @par
    * @param[in] ErrCode   : Type int
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -TODO
    *
    ***************************************************************************************************/
    void InvalidConf(int,int, QTableWidget*);

private slots:

    /******************Mainwindow Init Functions*******************/

    /***************************************************************************************************
    *
    *   FUNCTION NAME: ConfTablesInit
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will init Tables stylesheet in GUI.
    *
    * @par
    * @param[in] ConfTable : Type QTableWidget : TableWidget to be initialized
    *
    * @return TP_ResultID_t : Termination status of the executed function
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void ConfTablesInit(QTableWidget *ConfTable);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: MainWindowInit
    *
    ***************************************************************************************************/
    /**
    * @brief
    *   This slot will load needed parameters, set tables settings and importing data
    *   at GUI launch.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void MainWindowInit();

    /******************MainWindow lOAD Configurations**************/

    /***************************************************************************************************
    *
    *   FUNCTION NAME: load_config_file
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will open configuration files,parse it and fill Configurations tables.
    *
    * @par
    * @param[in] filepath : Type std::string : Configuration file path.
    * @param[in] ConfTable : Type QTableWidget : Configuration Table to be filled
    * @param[in] ConfType : Type std::string : Configuration Type
    *
    * @return int status
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    int load_config_file(std::string filepath, QTableWidget *ConfTable, std::string ConfType);

    /******************MainWindow Save Configurations**************/

    /***************************************************************************************************
    *
    *   FUNCTION NAME: Save_Config_file
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will save  configuration  parsed from Configurations tables which will be
    *  specified by ConfType into a file choosen by user.
    *
    * @par
    * @param[in] ConfTable : Type QTableWidget : Configuration Table
    * @param[in] ConfType : Type std::string : Configuration Type
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void Save_Config_file(QTableWidget *ConfTable, std::string ConfType);

    /******************MainWindow Set Configurations**************/

    /***************************************************************************************************
    *
    *   FUNCTION NAME: set_arp_conf
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will parse config from ARP table and assign it to ARP conf struct.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void set_arp_conf();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: set_udp_conf
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will parse config from UDP table and assign it to UDP conf struct.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void set_udp_conf();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: set_icmp_conf
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will parse config from ICMP table and assign it to ICMP conf struct.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void set_icmp_conf();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: set_tcp_conf
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will parse config from TCP table and assign it to TCP conf struct.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void set_tcp_conf();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: set_abs_conf
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will parse config from Abstraction table and assign it to Abstraction conf struct.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void set_abs_conf();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: set_ipv4_conf
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will parse config from ipv4 table and assign it to ipv4 conf struct.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void set_ipv4_conf();

    /******************MainWindow Buttons Functions**************/

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_actionConnect_triggered
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will detect Connect button trigger and launch connection procedure(connection to tester).
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_actionConnect_triggered();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_actionDisconnect_triggered
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will detect Disconnect button trigger and launch disconnection procedure(disconnection from tester).
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_actionDisconnect_triggered();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_actionopen_file_triggered
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will detect Open button trigger and launch open procedure.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_actionopen_file_triggered();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_actionRun_triggered
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will detect Run button trigger and launch run procedure(test suite execution).
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_actionRun_triggered();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_actionstop_triggered
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will detect Stop button trigger and launch stop procedure(stop test suite execution).
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_actionstop_triggered();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_actionReport_triggered
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will detect Report button trigger and launch report procedure(open generated
    *  report file in default browser).
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_actionReport_triggered();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_actionExit_triggered
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will detect Exit button trigger and launch exit procedure(close app).
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_actionExit_triggered();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_actiondesc_triggered
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will show/hide description dock widget.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_actiondesc_triggered();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_actionlogo_triggered
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will redirect user to the official company website.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_actionlogo_triggered();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_actionSysInfo_triggered
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will show the user its own system information.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_actionSysInfo_triggered();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_actionTest_and_Configuration_triggered
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will show/hide test and configuration dock widget.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_actionTest_and_Configuration_triggered();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_actionSystem_Logs_triggered
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will show/hide system log dock widget.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_actionSystem_Logs_triggered();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_actionSaveLog_triggered
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will open dialog file in order to choose your file path and save system log in it.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_actionSaveLog_triggered();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_TestCasesTree_itemChanged
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will add/remove test from Test suite view.
    *
    * @par
    * @param[in] item : Type QTreeWidgetItem : Modified item(checked/unchecked).
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_TestCasesTree_itemChanged(QTreeWidgetItem *item);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_TestCasesTree_itemClicked
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will show Synopsis(Description) of a test after clicking on it.
    *
    * @par
    * @param[in] item : Type QTreeWidgetItem : clicked item.
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_TestCasesTree_itemClicked(QTreeWidgetItem *item);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_ConfigTree_doubleClicked
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will show config tab related to the item double clicked on
    *  Test and configuration->configuration tab.
    *
    * @par
    * @param[in] index : Type QModelIndex : index of double clicked item.
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_ConfigTree_doubleClicked(const QModelIndex &index);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_actionClear_triggered
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will clear view and reset GUI view and parameters.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_actionClear_triggered();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_LogClearbutton_clicked
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will clear Log on Log dock widget.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_LogClearbutton_clicked();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_SaveLogButton_clicked
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will save log generated in log dock widget at run time.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_SaveLogButton_clicked();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_TestConfDock_visibilityChanged
    *
    ***************************************************************************************************/
    /**
    * @brief
    *
    * @par
    * @param[in] visible : Type bool
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_TestConfDock_visibilityChanged(bool visible);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_DescDock_visibilityChanged
    *
    ***************************************************************************************************/
    /**
    * @brief
    *
    * @par
    * @param[in] visible : Type bool
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_DescDock_visibilityChanged(bool visible);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_LogsDock_visibilityChanged
    *
    ***************************************************************************************************/
    /**
    * @brief
    *
    * @par
    * @param[in] visible : Type bool
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_LogsDock_visibilityChanged(bool visible);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_actionSimulationMode_triggered
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will activate/deactivate simulation mode.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_actionSimulationMode_triggered();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_LoadConf_clicked
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will check if config is valid and then import and set it in Conf table.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_LoadConfButton_clicked();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_SaveConf_clicked
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will save the current shown conf table parameters into a given file.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_SaveConfButton_clicked();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_actionExport_triggered
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will export test campaign defined by user as an xml file.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_actionExport_triggered();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_actionImport_triggered
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will import test campaign from a given file.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_actionImport_triggered();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_MainTab_tabBarClicked
    *
    ***************************************************************************************************/
    /**
    * @brief
    *
    * @par
    * @param[in] index : Type int : index of clicked tab
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_MainTab_tabBarClicked(int index);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_TestCasesTab_tabBarClicked
    *
    ***************************************************************************************************/
    /**
    * @brief
    *
    * @par
    * @param[in] index : Type int : index of clicked tab
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_TestCasesTab_tabBarClicked(int index);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_GenrConfTable_itemChanged
    *
    ***************************************************************************************************/
    /**
    * @brief
    * This slot will detect any change in config table and check it's validity.
    *
    * @par
    * @param[in] item : Type QTableWidgetItem : changed item in GenrConfTable
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_GenrConfTable_itemChanged(QTableWidgetItem *item);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_TcpConfTable_itemChanged
    *
    ***************************************************************************************************/
    /**
    * @brief
    * This slot will detect any change in config table and check it's validity.
    *
    * @par
    * @param[in] item : Type QTableWidgetItem : changed item in TcpConfTable
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_TcpConfTable_itemChanged(QTableWidgetItem *item);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_UdpConfTable_itemChanged
    *
    ***************************************************************************************************/
    /**
    * @brief
    * This slot will detect any change in config table and check it's validity.
    *
    * @par
    * @param[in] item : Type QTableWidgetItem : changed item in UdpConfTable
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_UdpConfTable_itemChanged(QTableWidgetItem *item);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_ArpConfTable_itemChanged
    *
    ***************************************************************************************************/
    /**
    * @brief
    * This slot will detect any change in config table and check it's validity.
    *
    * @par
    * @param[in] item : Type QTableWidgetItem : changed item in ArpConfTable
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_ArpConfTable_itemChanged(QTableWidgetItem *item);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_DhcpConfTable_itemChanged
    *
    ***************************************************************************************************/
    /**
    * @brief
    * This slot will detect any change in config table and check it's validity.
    *
    * @par
    * @param[in] item : Type QTableWidgetItem : changed item in DhcpConfTable
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_DhcpConfTable_itemChanged(QTableWidgetItem *item);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_Ipv4ConfTable_itemChanged
    *
    ***************************************************************************************************/
    /**
    * @brief
    * This slot will detect any change in config table and check it's validity.
    *
    * @par
    * @param[in] item : Type QTableWidgetItem : changed item in Ipv4ConfTable
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_Ipv4ConfTable_itemChanged(QTableWidgetItem *item);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_IcmpConfTable_itemChanged
    *
    ***************************************************************************************************/
    /**
    * @brief
    * This slot will detect any change in config table and check it's validity.
    *
    * @par
    * @param[in] item : Type QTableWidgetItem : changed item in IcmpConfTable
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_IcmpConfTable_itemChanged(QTableWidgetItem *item);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: on_SomeIpConfTable_itemChanged
    *
    ***************************************************************************************************/
    /**
    * @brief
    * This slot will detect any change in config table and check it's validity.
    *
    * @par
    * @param[in] item : Type QTableWidgetItem : changed item in SomeIpConfTable
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void on_SomeIpConfTable_itemChanged(QTableWidgetItem *item);

    /**************************Other Functions*******************/

    /***************************************************************************************************
    *
    *   FUNCTION NAME: EstablishConnection
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will trigger Connection function from IComModule class.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    int EstablishConnection();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: Open
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will open, read, check validity and import data of a given xml file.
    *
    * @par
    * @param[in] file : Type QFile
    *
    * @return int status
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    int Open(QFile *file);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: TreeviewDataInsertion
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will read a vector content and insert it as a treeview.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void TreeviewDataInsertion();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: receive_from_ui
    *
    ***************************************************************************************************/
    /**
    * @brief
    *
    * @par
    * @param[in] value : Type int
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void receive_from_ui(int value);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: inprogressadd
    *
    ***************************************************************************************************/
    /**
    * @brief
    *
    * @par
    * @param[in] row : Type int
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void inprogressadd(int row);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: generate_report
    *
    ***************************************************************************************************/
    /**
    * @brief
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void generate_report();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: enable_report
    *
    ***************************************************************************************************/
    /**
    * @brief
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void enable_report();

    /***************************************************************************************************
    *
    *   FUNCTION NAME: ValidityCheck
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will check data validity in conf tables.
    *
    * @par
    * @param[in] ConfTable : Type QTableWidget
    * @param[in] Conf : Type QString
    * @param[in] Value : Type QString
    * @param[in] CurrentRow : Type int
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void ValidityCheck(QTableWidget *ConfTable, QString *Conf, QString *Value, int CurrentRow);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: error_message
    *
    ***************************************************************************************************/
    /**
    * @brief
    *
    * @par
    * @param[in] int
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    void error_message(int);

    /***************************************************************************************************
    *
    *   FUNCTION NAME: ValidityDoubleCheck
    *
    ***************************************************************************************************/
    /**
    * @brief
    *  This slot will make a global Conf check to make sure that all Conf are valid and can be sent.
    *
    * @par
    * @param[in] void
    *
    * @return void
    *
    * @note
    * -
    *
    * @warning
    * -
    *
    ***************************************************************************************************/
    bool ValidityDoubleCheck();

    void update_connection_status(bool);

    void on_connection_update(bool connection_status);

    void SummaryTableSizeAdjust();

    void RportTableSizeAdjust(QTableWidget *table);

    void ShowReportTable(QTableWidget *Summary_table,QTableWidget *Protocol_table,QLabel *label,int ind);

    void AddTestReport(QTableWidget *table,int &indice,int row,int &total_protocol,int &failed_protocol,int &passed_protocol,int &Not_executed_protocol);

    void UpdateReportTables(int test_id);

    void ReportViewUpdating(int _test_id);

    void on_GenrConfTable_itemSelectionChanged();

    void  SummaryTableInit();

    void InitReportTable(QTableWidget *table);

    void ReportTablesInit();

    void InvalidConfMsg(int, int, QTableWidget*);


    void on_GenrConfTable_cellDoubleClicked(int row, int column);

    void on_TcpConfTable_cellDoubleClicked(int row, int column);

    void on_UdpConfTable_cellDoubleClicked(int row, int column);

    void on_ArpConfTable_cellDoubleClicked(int row, int column);

    void on_DhcpConfTable_cellDoubleClicked(int row, int column);

    void on_Ipv4ConfTable_cellDoubleClicked(int row, int column);

    void on_IcmpConfTable_cellDoubleClicked(int row, int column);

    void on_SomeIpConfTable_cellDoubleClicked(int row, int column);

    void RemoveWarning();

private:
    Ui::MainWindow *ui;

    /********** private variables************/

    /*************xml handling*******/
    QDomNode xml_node;
    QStandardItem *item;
    QStandardItemModel *model;
    QWidget *widget;
    QItemSelectionModel *selectModel;
    QDomDocument doc;
    QString filePath;
    /********Connect Dock***********/
    QDockWidget *ConnectDock = nullptr;
    QWidget *ConnectWidget = nullptr;
    QLabel *ConnectionsettingFillRequest = nullptr;
    QLabel *TesterAddresslabel = nullptr;
    QLabel *TesterPortlabel = nullptr;
    QLineEdit *TesterAddressline = nullptr;
    QLineEdit *TesterPortline = nullptr;
    QPushButton *ConnectButton = nullptr;
    QGroupBox *ConnectGBox = nullptr;
    QLabel *ConnectMsg = nullptr;
    QChart *Chart = new QChart();
    QChartView *ChartView = new QChartView(Chart);
    QLabel *ConnectionIcon = new QLabel();
    QLabel *ConnectionLabel = new QLabel();

    /*********General Config Tab****************/
    /*QTabWidget *GeneralConfigTab = nullptr;
    QLabel *  interfacelabel= nullptr;
    QLineEdit *  interfaceline= nullptr;
    QLabel *  DUT_HW_Addresslabel= nullptr;
    QLineEdit * DUT_HW_Addressline = nullptr;
    QLabel * S_DUT_IP_Addresslabel = nullptr;
    QLineEdit * S_DUT_IP_Addressline = nullptr;
    QLabel * S_Tester_HW_Addresslabel = nullptr;
    QLineEdit * S_Tester_HW_Addressline = nullptr;
    QLabel * S_Tester_IP_Addresslabel = nullptr;
    QLineEdit * S_Tester_IP_Addressline = nullptr;*/
};
#endif // MAINWINDOW_H
