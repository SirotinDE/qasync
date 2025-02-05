#include "HttpClient.h"

#include <QDebug>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>

static QVector<HttpClient::Header> contentJsonHeader() {
    return {{"Content-Type", "application/json"}};
}

HttpClient::HttpClient() : web_(new QNetworkAccessManager) {}

HttpClient& HttpClient::instance() {
    static HttpClient httpClient;
    return httpClient;
}

void HttpClient::init(const QString& host, int port) {
    host_ = QString("%1:%2").arg(host, QString::number(port));
}

HttpReplyResult HttpClient::get(const QString& endpoint, const QVector<Header>& headers) {
    auto request = getRequest(endpoint, headers);

    QEventLoop loop;
    QTimer timer;
    timer.start(20000);
    web_->connect(web_.data(), SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    timer.connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    auto reply = web_->get(request);
    loop.exec();

    if (reply->error()) {
        return HttpReplyResult{HttpReplyResult::NetworkError(reply)};
    }
    return HttpReplyResult{HttpReplyResult::HttpReply(reply)};
}

QNetworkRequest HttpClient::getRequest(const QString& endpoint, const QVector<Header>& headers) {
    QNetworkRequest request;
    request.setUrl(QUrl(host_ + endpoint));

    for (const auto& h : headers) {
        request.setRawHeader(h.key.toUtf8(), h.value.toUtf8());
    }
    return request;
}
