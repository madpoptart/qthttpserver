#include "httpparser.h"

#include <QDebug>

HttpParser::HttpParser(QTcpSocket *socket, QObject *parent):QObject(parent)
{
    tcpSocket = socket;
    initParser();

    connect(tcpSocket,SIGNAL(readyRead()),SLOT(onSocketReadyRead()));
    connect(tcpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
           this,SLOT(onSocketStateChange(QAbstractSocket::SocketState)));

    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(onSocketDisconnected()));
}

void HttpParser::initParser()
{
    settings.on_message_begin = HttpParser::OnMessageBegin;
    settings.on_url = HttpParser::OnUrl;
    settings.on_header_field = HttpParser::OnHeaderField;
    settings.on_header_value = HttpParser::OnHeaderValue;
    settings.on_headers_complete = HttpParser::OnHeadersComplete;
    settings.on_body = HttpParser::OnBody;
    settings.on_message_complete = HttpParser::OnMessageComplete;

    parser = new http_parser();

    qDebug()<<"Parser initialized "<<parser->instance<<endl;
}

void HttpParser::onSocketReadyRead()
{
    size_t nparsed;
    qDebug()<<"HttpSocket: readInputStream"<<endl;

    QTcpSocket *client = (QTcpSocket *)QObject::sender();

    QByteArray socketData;

    while(client->bytesAvailable())
    {
        socketData.append(client->readAll());
    }
    qDebug()<<"HttpSocket: InputStream = "<<socketData<<endl;


    http_parser_init(parser, HTTP_REQUEST);
    parser->instance = this;
    nparsed = http_parser_execute(parser,&settings,socketData.data(),(size_t)socketData.length());

    qDebug()<<"nparsed size is "<<nparsed<<endl;
}

void HttpParser::onSocketDisconnected()
{
    emit this->socketDisconnected();
}


void HttpParser::onSocketStateChange(QAbstractSocket::SocketState socketState)
{
    switch(socketState)
    {
        case QAbstractSocket::UnconnectedState:
            qDebug()<<"Curr Socket Unconnected"<<endl;
            break;
        case QAbstractSocket::HostLookupState:
            qDebug()<<"Curr Socket HostLookup"<<endl;
            break;
        case QAbstractSocket::ConnectingState:
            qDebug()<<"Curr Socket Connecting"<<endl;
            break;
        case QAbstractSocket::ConnectedState:
            qDebug()<<"Curr Socket Connected"<<endl;
            break;
        case QAbstractSocket::BoundState:
            qDebug()<<"Curr Socket Bound"<<endl;
            break;
        case QAbstractSocket::ListeningState:
            qDebug()<<"Curr Socket Listening"<<endl;
            break;
        case QAbstractSocket::ClosingState:
            qDebug()<<"Curr Socket Closing"<<endl;
            break;
    }
}

int HttpParser::onMessageBegin()
{
    qDebug()<<"Message Begin"<<endl;
    headers.clear();
    fieldList.clear();
    valueList.clear();
    return 0;
}

int HttpParser::onUrl(const char *at, size_t length)
{
    QByteArray url(at,length);
    qDebug()<<"onUrl: "<<url<<endl;
    this->url = QString(url);
    return 0;
}

int HttpParser::onHeaderField(const char *at, size_t length)
{
    QByteArray headerField(at,length);
    qDebug()<<"onHeaderField: "<<headerField<<endl;
    fieldList.push_back(QString(headerField));
    return 0;
}

int HttpParser::onHeaderValue(const char *at, size_t length)
{
    QByteArray headerValue(at,length);
    qDebug()<<"onHeaderValue: "<<headerValue<<endl;
    valueList.push_back(QString(headerValue));


    return 0;
}

int HttpParser::onHeadersComplete()
{
    qDebug()<<"onHeadersComplete"<<endl;

    if(valueList.length() == fieldList.length())
    {
        for(int i=0; i<fieldList.length(); i++)
        {
            headers.insert(fieldList.at(i),valueList.at(i));
        }
    }

    return 0;
}

int HttpParser::onBody(const char *at, size_t length)
{
    QByteArray body(at,length);
    return 0;
}

int HttpParser::onMessageComplete()
{
    emit (this->parsingComplete());
    return 0;
}

QString HttpParser::getUrl()
{
    return url;
}

QMap<QString,QString> HttpParser::getHeaders()
{
    return this->headers;
}

void HttpParser::write(QByteArray data)
{
    this->tcpSocket->write(data);
}

HttpParser::~HttpParser()
{
    qDebug()<<"HttpParser: deleting"<<endl;
    delete parser;
}

