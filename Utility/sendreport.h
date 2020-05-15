#ifndef SENDREPORT_H
#define SENDREPORT_H

#include <QObject>
#include <QUdpSocket>
#include <QFile>
#include <QThread>
#include <QTimer>

//--------------------------------------------------------------------------------
//----<Global const variables.>---------------------------------------------------
//--------------------------------------------------------------------------------
const uint gConstPacketSize = 480; //From QT documents, "Sending datagrams larger than 512 bytes is in general disadvised."
const uint gConstPacketBufferSize = 1024; //Buffer size is larger than real used.

const uint gConstPacketNumberInFileBuffer = 2048;
const uint gConstFileReadSize = gConstPacketSize * gConstPacketNumberInFileBuffer; //Read 960KB from a file one time.
const uint gConstFileReadBufferSize = 1024 * 1024; //As above, buffer size is larger.

const uint gConstMaxFileSizeToSend = 4*1024*1024; //Only send the last 4MB of log files.

//Define how many packets will be sent before waiting for a ACK from server.
//Increase this value, sending speed maybe increased.
//Decrease this value, packet loss maybe decreased.
const uint gConstPacketsBundle = 8;

const uint gConstMaxLogFileNumber = 3; //Now there are 2 log files, and 1 screen shot file.
const QString gConstScreenShotFileName("screenshot.png");
const QString gConstLogFileNames[gConstMaxLogFileNumber] = {
    "BROM_DLL_V5.log",
    "QT_FLASH_TOOL.log",
    "screenshot.png"
};


const QString gConstServerAddr = "172.27.153.27";
const quint16 gConstServerControlPort = 8001; //To transfer report request
const quint16 gConstServerDataPort = 8002; //To transfer log request
const quint16 gConstClientPort = 8001;


const QString gConstSearchSenderIDDir = "C:/Documents and Settings";

#ifdef _WIN32
const QString gConstLogFileDir= "C:/Documents and Settings/All Users/Application Data/SP_FT_Logs";
#else
const QString gConstLogFileDir = "/tmp/SP_FT_Logs";
#endif

const uint gConstResponseACK = 0x1234;

//--------------------------------------------------------------------------------
//----<Structures used between client/server.>------------------------------------
//--------------------------------------------------------------------------------
typedef enum {
    MT_ERROR_REPORT = 0x10,
    MT_ERROR_LOG_DATA = 0x11,
    MT_UNKNOWN = 0xFFFFFFFF
} MessageType;

//This LogFileID _MUST_ start from 0x0, as it will be used as index to array.
typedef enum {
    LFID_BROM_DLL_V5 = 0x0,
    LFID_QT_FLASH_TOOL = 0x1,
    LFID_SCREEN_SHOT = 0x2,
    LFID_UNKNOWN = 0xFFFFFFFF
} LogFileID;

typedef struct _MessageHeader
{
    MessageType type;
    uint length;
} MessageHeader;

typedef struct _ErrorReportRequest
{
    uint senderID;
    uint errorID;
    uint bbchipID;
    uint logFileNumber;
    uint logFileLength[gConstMaxLogFileNumber];
} ErrorReportRequest;

typedef struct _ErrorReportResponse
{
    uint ack;
} ErrorReportResponse;

typedef struct _ErrorLogRequest
{
    uint senderID;
    uint fileID;
    uint sequence;
    char data[1];
} ErrorLogRequest;

typedef struct _ErrorLogResponse
{
    uint fileID;
    uint sequence;
    uint ack;
} ErrorLogResponse;


//--------------------------------------------------------------------------------
//----<Class ReportThread definition.>--------------------------------------------
//--------------------------------------------------------------------------------

//Currently status is quite straight.
//At first, it's in INITIAL.
//After sending report request, goto SEND_REPORT_REQUEST and wait for ACK.
//After receiving REPORT_REQUEST_ACK, goto SEND_LOG_FILE and wait for ACK.
//After all ACK for log files received, goto COMPLETED.
typedef enum {
    THREAD_STATUS_INITIAL = 0x0,
    THREAD_STATUS_SEND_REPORT_REQUEST = 0x1,
    THREAD_STATUS_SEND_LOG_FILE = 0x2,
    THREAD_STATUS_SEND_COMPLETED = 0x3,
    THREAD_STATUS_UNKNOWN = 0xFFFFFFFF
} SendStatus;

class MainWindow;

class SendReport : public QObject
{
    Q_OBJECT
public:
    explicit SendReport(QObject *parent, MainWindow* main_window);

    void setChipID(uint chip);
    void setErrorID(uint error);
    void sendErrorReport();

signals:
    void stopTimer();

public slots:
    void startSend();
    void releaseResources();
    void setSendResult(bool ret);
    void waitSendThreadTimeout();

private slots:
    void socketReadData(); //Read data from server

private:
    uint bbchipID, errorID, senderID;

    QUdpSocket *socket;
    QHostAddress serverAddr;
    char *socketBuf; //Size is 1KB, to hold a packet.
    char *fileBuf; //Size is 1MB (gConstFileReadBufferSize), to hold 2048 packets.
    QThread sendThread_;
    bool isSendThread_running_;
    QTimer *thread_timer_;
    MainWindow *main_window_;

    bool getSenderID(); //If can't find dir name similar to "mtkxxxxx" under gConstSearchSenderIDDir, return false.
    QString strLatestLogDirName;
    bool getLatestLogDir(); //If can't find dir under gConstLogFileDir, return false.
    void CaptureScreen();

    SendStatus status;
    uint currentLogFileIndex; //Which file the thread is sending.
    QFile *logFiles[gConstMaxLogFileNumber];
    qint64 logFilesLength[gConstMaxLogFileNumber];
    qint64 logFilesSentLength[gConstMaxLogFileNumber]; //How many bytes are sent.


    //As data is not guaranteed to be received on UDP,
    //here we send every gConstPacketsBundle(8) packets, with data size of 480 bytes,
    //and wait for ACK from server to send next gConstPacketsBundle(8) packets.
    void sendPackets();
    uint packetIndexInFileBuffer; //Increase gConstPacketsBundle(8) after calling sendPackets().

    void readFile(uint fileID); //Read gConstFileReadSize from file to fileBuf.


    bool initVariables();


    bool sendReportRequest();
    void processLogResponse(ErrorLogResponse *logResponse);
};

#endif // SENDREPORT_H
