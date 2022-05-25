/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file   main.cpp
 *  @brief  Qt Base - IHM_Module.
 *  \author uid1344 Tarek Othmani
 *
 *  \addtogroup frontend-com-module
 *  \{
 */
#include "mainwindow.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString LogMsg;
    switch (type) {
    case QtDebugMsg:
        LogMsg = "Debug: ";
        break;
    case QtInfoMsg:
        LogMsg = "Info: ";
        break;
    case QtWarningMsg:
       // LogMsg = QString::fromStdString("Warning: "+std::string(localMsg.constData()) + "(" + std::string(context.file) + ":" + std::to_string(context.line) + ")" + std::string(context.function) + "\n");
        break;
    case QtCriticalMsg:
        LogMsg = "Critical: ";
        break;
    case QtFatalMsg:
        LogMsg = "Fatal: ";
        abort();
    }
    QFile outFile("./log.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&outFile);
        ts << LogMsg;
        ts << msg ;
        ts << "(";
        ts << context.file;
        ts << ":";
        ts << context.line;
        ts << ")";
        ts << context.function;
        ts <<"\n";
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput); // Install the handler
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}
