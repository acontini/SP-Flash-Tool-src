#include "sendreport.h"

#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
#include "../UI/src/MainWindow.h"

#include <QDebug>
#include <QThread>


SendReport::SendReport(QObject *parent, MainWindow *main_window) :
    QObject(parent),
    isSendThread_running_(false),
    main_window_(main_window)
{
    connect(&sendThread_, SIGNAL(finished()), this, SLOT(releaseResources()));
    connect(this, SIGNAL(stopTimer()), main_window_, SLOT(stopTimer()));

    moveToThread(&sendThread_);
}

void SendReport::setChipID(uint chip)
{
    bbchipID = chip;
}
void SendReport::setErrorID(uint error)
{
    errorID = error;
}

void SendReport::sendErrorReport()
{
    if(isSendThread_running_)
    {
        qDebug()<<"Ongoing sending another report.";
                    return;
    }

    CaptureScreen();

    sendThread_.start();

    isSendThread_running_ = true;

    startSend();
}

void SendReport::startSend()
{
    //Entry point of sending a error report.
    //This function runs in the current (children) thread.
    getSenderID();

    if(!initVariables())
    {
        qDebug()<<QString("Initialize variables failed!");
        releaseResources();
        setSendResult(false);
        return;
    }

    sendReportRequest();
}

void SendReport::setSendResult(bool ret)
{
    qDebug()<<QString("Main, receive thread ret=%1").arg(ret);
    sendThread_.quit();
    isSendThread_running_ = false;

    emit stopTimer();
}

void SendReport::waitSendThreadTimeout()
{
    qDebug()<<"Wait thread timeout";
    sendThread_.quit();
    isSendThread_running_ = false;
}

void SendReport::CaptureScreen()
{
    QFileInfo srcFileInfo;
    QFileInfoList srcList;
    QDir srcDir;

    srcDir.cd(gConstLogFileDir);
    srcDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    srcDir.setSorting(QDir::Time);
    srcList = srcDir.entryInfoList();
    if(srcList.isEmpty())
    {
        qDebug()<<QString("No dirs under %1!").arg(gConstLogFileDir);
        return;
    }
    else
        srcFileInfo = srcList.at(0);

    srcDir.cd(srcFileInfo.fileName());

    QPixmap pic = QPixmap::grabWindow(QApplication::desktop()->winId());
    QString screenFilePath = QString("%1/%2").arg(srcFileInfo.filePath()).arg(gConstScreenShotFileName);

    QFile file(screenFilePath);
    if(file.exists())
        file.remove();
    pic.save(screenFilePath, "png");

    qDebug()<<"Screen shot saved.";
}

void SendReport::releaseResources()
{
    uint i;

    qDebug()<<QString("releaseResources, threadID=%1").arg((uint)QThread::currentThreadId());

    for(i=0 ; i<gConstMaxLogFileNumber ; i++)
    {
        if(logFiles[i] != NULL)
        {
            logFiles[i]->close();
            delete logFiles[i];
            logFiles[i] = NULL;
        }
    }
    if(fileBuf != NULL)
    {
        delete []fileBuf;
        fileBuf = NULL;
    }
    if(socketBuf != NULL)
    {
        delete []socketBuf;
        socketBuf = NULL;
    }
    if(socket != NULL)
    {
        socket->close();
        delete socket;
        socket = NULL;
    }
}


bool SendReport::initVariables()
{
    uint i;

    //Initialize socket related
    socket = NULL;
    socket = new QUdpSocket(this);
    if(socket == NULL)
    {
        return false;
    }
    if(!socket->bind(QHostAddress::Any, gConstClientPort))
    {
        return false;
    }
    if(!connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadData())))
    {
        return false;
    }
    serverAddr = QHostAddress(gConstServerAddr);

    //Initialize file related variables
    socketBuf = NULL;
    socketBuf = new char[gConstPacketBufferSize];
    if(socketBuf == NULL)
    {
        return false;
    }
    fileBuf = NULL;
    fileBuf = new char[gConstFileReadBufferSize];
    if(fileBuf == NULL)
    {
        return false;
    }
    if(!getLatestLogDir())
    {
        return false;
    }
    for(i=0 ; i<gConstMaxLogFileNumber ; i++)
    {
        logFiles[i] = NULL;
        logFilesLength[i] = 0;
        logFilesSentLength[i] = 0;
    }
    currentLogFileIndex = (uint)LFID_BROM_DLL_V5;
    packetIndexInFileBuffer = 0;

    status = THREAD_STATUS_INITIAL;
    return true;
}


bool SendReport::getLatestLogDir()
{
    QFileInfo srcFileInfo;
    QFileInfoList srcList;
    QDir srcDir;

    srcDir.cd(gConstLogFileDir);
    srcDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    srcDir.setSorting(QDir::Time);
    srcList = srcDir.entryInfoList();
    if(srcList.isEmpty())
    {
        qDebug()<<QString("No dirs under %1!").arg(gConstLogFileDir);
        return false;
    }
    else
        srcFileInfo = srcList.at(0);
    srcDir.cd(srcFileInfo.fileName());
    strLatestLogDirName = srcFileInfo.filePath();
    //qDebug()<<QString("Log dir: %1").arg(srcFileInfo.fileName());
    return true;
}


//SenderID is a unique 5-digit number for every MTK colleagues.
//For OA PC, login name is "mtkxxxxx",
//  which means there is a dir under gConstSearchSenderIDDir.
//So we make use of this, rather than call Windows API to get login name.
//Maybe there are multiple "mtkxxxxx" dirs (multi users logged into the PC),
//  we use the one of them.
bool SendReport::getSenderID()
{
    senderID = 0xFFFFFFFF; //Set to default value.

    QDir srcDir;
    QFileInfo srcFileInfo;
    QFileInfoList srcList;

    srcDir.cd(gConstSearchSenderIDDir);
    srcDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    srcList = srcDir.entryInfoList();
    if(srcList.isEmpty())
    {
        qDebug()<<QString("No dirs under %1.").arg(gConstSearchSenderIDDir);
        return false;
    }
    else
    {
        for(int i=0 ; i<srcList.size() ; i++)
        {
            srcFileInfo = srcList.at(i);
            if(srcFileInfo.fileName().contains("mtk", Qt::CaseInsensitive))
            {
                senderID = srcFileInfo.fileName().right(5).toInt();
            }
        }
    }

    //qDebug()<<"senderID: "<<senderID;
    return true;
}


void SendReport::readFile(uint fileIndex)
{
    //qint64 readLength;
    memset(fileBuf, 0, gConstFileReadSize);
    logFiles[fileIndex]->read(fileBuf, gConstFileReadSize);
    //readLength = logFiles[fileIndex]->read(fileBuf, gConstFileReadSize);
    //qDebug()<<QString("File id=%1, readLength=%2").arg(fileIndex).arg(readLength);
    //qDebug()<<QString("File id=%1, data: %2 %3 %4").arg(fileIndex).arg(fileBuf[0]).arg(fileBuf[1]).arg(fileBuf[2]);
    //qDebug()<<QString("SentLength=%1").arg(logFilesSentLength[fileIndex]);
}


void SendReport::sendPackets()
{
    MessageHeader *header = (MessageHeader *)socketBuf;
    ErrorLogRequest *logRequest = (ErrorLogRequest *)(socketBuf + sizeof(MessageHeader));
    quint64 dataLength = gConstPacketSize, writeLength;
    bool lastPacket = false;

    memset(socketBuf, 0, gConstPacketBufferSize);

    header->type = MT_ERROR_LOG_DATA;
    logRequest->senderID = senderID;
    logRequest->fileID = currentLogFileIndex;
    logRequest->sequence = packetIndexInFileBuffer;

    //qDebug()<<QString("Sent: %1").arg(packetIndexInFileBuffer);

    for(unsigned int i=0 ; i<gConstPacketsBundle ; i++)
    {
        if(logFilesSentLength[currentLogFileIndex]+(i+1)*gConstPacketSize >= logFilesLength[currentLogFileIndex]) //This is last packet
        {
            dataLength = logFilesLength[currentLogFileIndex] - logFilesSentLength[currentLogFileIndex] - i*gConstPacketSize;
            lastPacket = true;
        }

        memcpy(logRequest->data, &fileBuf[(packetIndexInFileBuffer+i)*gConstPacketSize], dataLength);
        header->length = sizeof(ErrorLogRequest) + dataLength - 4;
        writeLength = socket->writeDatagram(socketBuf, header->length+sizeof(MessageHeader), serverAddr, gConstServerDataPort);
        if(writeLength != header->length+sizeof(MessageHeader))
        {
            qDebug()<<QString("Send file id=%1, index=%2 failed.").arg(i);
        }

        if(lastPacket)
            break;
    }
}


bool SendReport::sendReportRequest()
{
    MessageHeader *header = (MessageHeader *)socketBuf;
    ErrorReportRequest *reportRequest = (ErrorReportRequest *)(socketBuf + sizeof(MessageHeader));
    qint64 writeLength;

    if(status != THREAD_STATUS_INITIAL)
    {
        qDebug()<<"Someone called sendReportRequest(), but we're not in INITIAL state.";
        return false;
    }

    memset(socketBuf, 0, gConstPacketBufferSize);

    header->type = MT_ERROR_REPORT;
    header->length = sizeof(ErrorReportRequest);
    reportRequest->senderID = senderID;
    reportRequest->bbchipID = bbchipID;
    reportRequest->errorID = errorID;
    reportRequest->logFileNumber = gConstMaxLogFileNumber;


    QFileInfo srcFileInfo;
    QFileInfoList srcList;
    QDir srcDir;
    srcDir.cd(strLatestLogDirName);
    srcDir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    srcList = srcDir.entryInfoList();
    if(srcList.isEmpty())
    {
        qDebug()<<QString("No files in %1").arg(strLatestLogDirName);
        return false;
    }
    else
    {
        for(int i=0 ; i<srcList.size() ; i++)
        {
            srcFileInfo = srcList.at(i);
            if(srcFileInfo.fileName() == gConstLogFileNames[i]) //Ignore files whose names we don't know.
            {
                logFiles[i] = new QFile(srcFileInfo.filePath());
                if(!logFiles[i]->open(QIODevice::ReadOnly))
                {
                    qDebug()<<QString("Open file id=%1 name=%2 failed.").arg(i).arg(srcFileInfo.filePath());
                    return false;
                }
                if(srcFileInfo.size() > gConstMaxFileSizeToSend)
                {
                    logFilesLength[i] = srcFileInfo.size();
                    logFilesSentLength[i] = srcFileInfo.size() - gConstMaxFileSizeToSend;
                    logFiles[i]->seek(logFilesSentLength[i]);
                    reportRequest->logFileLength[i] = gConstMaxFileSizeToSend;
                    //qDebug()<<QString("total:%1, seek:%2").arg(srcFileInfo.size()).arg(logFilesSentLength[i]);
                }
                else
                {
                    logFilesLength[i] = srcFileInfo.size();
                    reportRequest->logFileLength[i] = logFilesLength[i];
                    //qDebug()<<QString("total:%1").arg(logFilesLength[i]);
                }


            }
        }
    }

    writeLength = socket->writeDatagram(socketBuf, sizeof(MessageHeader)+sizeof(ErrorReportRequest), serverAddr, gConstServerControlPort);
    if(writeLength != (sizeof(MessageHeader)+sizeof(ErrorReportRequest)))
    {
        qDebug()<<"Send reportRequest failed!";
        return false;
    }

    status = THREAD_STATUS_SEND_REPORT_REQUEST;
    qDebug()<<"Send request finished.";
    return true;
}


//Some presumptions:
//1. The socket read logic is simple: wait specific response at specific state.
//2. At a time, only one response is waited.
//3. The size of all response messages is small, far less than 1024B.
void SendReport::socketReadData()
{
    ErrorReportResponse *reportResponse = NULL;
    ErrorLogResponse *logResponse = NULL;
    QHostAddress sender;
    quint16 senderPort;
    qint64 /*readLength, */pendingLength;

    memset(socketBuf, 0, gConstPacketBufferSize);

    while (socket->hasPendingDatagrams())
    {
        pendingLength = socket->pendingDatagramSize();
        socket->readDatagram(socketBuf, pendingLength, &sender, &senderPort);
        //readLength = socket->readDatagram(socketBuf, pendingLength, &sender, &senderPort);
        //No readLength and pendingLength comparison, as we trust QUdpSocket.

        //qDebug()<<QString("Server, addr=%1, port=%2").arg(sender.toString()).arg(senderPort);

        switch(senderPort)
        {
        case gConstServerControlPort:
            reportResponse = (ErrorReportResponse *)socketBuf;
            if((reportResponse->ack==gConstResponseACK) && (status==THREAD_STATUS_SEND_REPORT_REQUEST))
            {
                qDebug()<<"Received request report ACK.";
                readFile(currentLogFileIndex);
                sendPackets();
                status = THREAD_STATUS_SEND_LOG_FILE;
            }
            break;

        case gConstServerDataPort:
            logResponse = (ErrorLogResponse *)socketBuf;
            processLogResponse(logResponse);
            break;

        default:
            qDebug()<<QString("Unknown message from %1_%2").arg(sender.toString()).arg(senderPort);
            break;
        }

   }
}


void SendReport::processLogResponse(ErrorLogResponse *logResponse)
{
    if((logResponse->ack == gConstResponseACK)
        && (status == THREAD_STATUS_SEND_LOG_FILE)
        && (logResponse->fileID == currentLogFileIndex))
    {
        //qDebug()<<QString("Log response, ack=%1, fileID=%2, sequence=%3")
        //          .arg(logResponse->ack).arg(logResponse->fileID).arg(logResponse->sequence);
        //qDebug()<<QString("index=%1").arg(packetIndexInFileBuffer);
        if(logResponse->sequence == packetIndexInFileBuffer) //Send next 8 packets
        {
            logFilesSentLength[currentLogFileIndex] += (gConstPacketsBundle*gConstPacketSize);
            if(logFilesSentLength[currentLogFileIndex] >= logFilesLength[currentLogFileIndex])
            {
                //Start to send next file.
                //qDebug()<<QString("File id=%1 sent finished len=%2.").arg(currentLogFileIndex)
                //          .arg(logFilesSentLength[currentLogFileIndex]);
                currentLogFileIndex++;

                if(currentLogFileIndex == gConstMaxLogFileNumber)
                {
                    qDebug()<<QString("Finished sending 2 files.");
                    status = THREAD_STATUS_SEND_COMPLETED;
                    setSendResult(true);
                    return;
                }
                readFile(currentLogFileIndex);
                packetIndexInFileBuffer = 0;
            }
            else
            {
                packetIndexInFileBuffer += gConstPacketsBundle;
                if(packetIndexInFileBuffer == gConstPacketNumberInFileBuffer)
                {
                    readFile(currentLogFileIndex);
                    packetIndexInFileBuffer = 0;
                }
            }
        }
        //If packetIndexInFileBuffer not changed, re-send last 8 packets.
        sendPackets();
    }
}
