#include "httpserver.h"
#include <QDebug>

HttpServer::HttpServer(QObject *parent) : QObject(parent)
{ 
    tcpServer = new QTcpServer(this);
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(onNewConnection()));

    tcpServer->listen(QHostAddress::Any,8080);
}

void HttpServer::onNewConnection()
{
    qDebug()<<"New connection"<<endl;
    HttpParser *client = new HttpParser(tcpServer->nextPendingConnection(),this);

    connect(client,SIGNAL(socketDisconnected()),this,SLOT(onSocketDisconnect()));
    connect(client,SIGNAL(parsingComplete()),this,SLOT(onParsingComplete()));
}

void HttpServer::onSocketDisconnect()
{
    HttpParser *parser = (HttpParser *)QObject::sender();
    parser->deleteLater();
}

void HttpServer::onParsingComplete()
{
    HttpParser *client = (HttpParser *)QObject::sender();

    qDebug()<<"HttpServer: Parsing complete"<<endl;
    qDebug()<<"HttpServer: "<< client->getHeaders()<<endl;
    qDebug()<<"HttpServer: "<<client->getUrl()<<endl;

    QByteArray message = "<html> <body><h1>Hello World!</h1></body></html>";

    QByteArray header = "HTTP/1.0 200 OK \n";
    header.append("Content-Type: text/html\n");
    header.append("Content-Length: ");
    header.append(QByteArray::number(message.length()));
    header.append("\n\n");

    client->write(header);
    client->write(message);

}

HttpServer::~HttpServer()
{

}

