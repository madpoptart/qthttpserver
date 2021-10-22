#ifndef HTTPSERVER_H
#define HTTPSERVER_H


#include <QObject>
#include <QTcpServer>
#include <QByteArray>
#include <QSharedPointer>
#include <QNetworkInterface>
#include <QTimer>
#include <QList>
#include "httpparser.h"


class HttpServer : public QObject
{
    Q_OBJECT
public:
    explicit HttpServer(QObject *parent = 0);

    ~HttpServer();

signals:

public slots:
    void onNewConnection();
    void onSocketDisconnect();
    void onParsingComplete();

private:

    QTcpServer *tcpServer;
};

#endif // HTTPSERVER_H
