#ifndef HTTPSOCKET_H
#define HTTPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QMap>
#include "joyent-parser/http_parser.h"


class HttpParser : public QObject
{
    Q_OBJECT

public:


    HttpParser(QTcpSocket *socket, QObject *parent=0);

    static int OnMessageBegin(http_parser *p)
    {  if(p->instance) {return ((HttpParser *)(p->instance))->onMessageBegin(); } else return -1; }

    static int OnUrl(http_parser *p, const char *at, size_t length)
    { if(p->instance){ return ((HttpParser *)(p->instance))->onUrl(at,length); } else return -1; }

    static int OnHeaderField(http_parser *p, const char *at, size_t length)
    { if(p->instance){ return ((HttpParser *)(p->instance))->onHeaderField(at,length); } else return -1; }

    static int OnHeaderValue(http_parser *p, const char *at, size_t length)
    { if(p->instance){ return ((HttpParser *)(p->instance))->onHeaderValue(at,length); } else return -1; }

    static int OnHeadersComplete(http_parser *p)
    { if(p->instance) {return ((HttpParser *)(p->instance))->onHeadersComplete(); } else return -1; }

    static int OnBody(http_parser *p, const char *at, size_t length)
    { if(p->instance){ return ((HttpParser *)(p->instance))->onBody(at,length); } else return -1; }

    static int OnMessageComplete(http_parser *p)
    { if(p->instance) {return ((HttpParser *)(p->instance))->onMessageComplete(); } else return -1; }


    int onMessageBegin();
    int onUrl(const char *at, size_t length);
    int onHeaderField(const char *at, size_t length);
    int onHeaderValue(const char *at, size_t length);
    int onHeadersComplete();
    int onBody(const char *at, size_t length);
    int onMessageComplete();
    QString getUrl();
    QMap<QString,QString> getHeaders();

    void write(QByteArray data);

    ~HttpParser();


signals:
    void socketDisconnected();
    void parsingComplete();

public slots:
    void onSocketReadyRead();
    void onSocketStateChange(QAbstractSocket::SocketState);
    void onSocketDisconnected();


private:

    void initParser();

    QTcpSocket *tcpSocket;
    http_parser *parser;
    http_parser_settings settings;

    QMap<QString,QString> headers;
    QList<QString> fieldList;
    QList<QString> valueList;
    QString url;

};

#endif // HTTPSOCKET_H
