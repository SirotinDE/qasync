#pragma once

#include <QByteArray>
#include <QJsonObject>
#include <QScopedPointer>
#include <QString>
#include <QVector>
#include <functional>

#include "HttpReply.h"

class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;

class HttpClient {
    HttpClient();

public:
    struct Header {
        QString key;
        QString value;
    };

    ~HttpClient() = default;
    static HttpClient& instance();

    void init(const QString& host, int port);

    HttpReplyResult get(const QString& endpoint, const QVector<Header>& headers);

private:
    QNetworkRequest getRequest(const QString& endpoint, const QVector<Header>& headers);

    QString host_;
    QScopedPointer<QNetworkAccessManager> web_;
};
