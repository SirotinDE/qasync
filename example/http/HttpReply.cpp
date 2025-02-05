#include "HttpReply.h"

#include <QNetworkReply>
#include <QVariant>

HttpReplyResult::HttpReply::HttpReply(QNetworkReply* reply) : reply_(reply) {}

HttpReplyResult::HttpReply::~HttpReply() {
    reply_->deleteLater();
}

int HttpReplyResult::HttpReply::code() const {
    return reply_->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
}

QByteArray HttpReplyResult::HttpReply::payload() const {
    return reply_->readAll();
}

HttpReplyResult::NetworkError::NetworkError(QNetworkReply* reply) : HttpReply(reply) {}

QString HttpReplyResult::NetworkError::errorString() const {
    QVariant reason = reply_->attribute(QNetworkRequest::HttpReasonPhraseAttribute);
    return reason.isValid() ? QString("%1 %2").arg(reply_->errorString(), reason.toString()) : reply_->errorString();
}

HttpReplyResult::HttpReplyResult(HttpReplyVariant&& v) : result_(std::move(v)) {}

bool HttpReplyResult::isOk() {
    return std::holds_alternative<HttpReplyResult::HttpReply>(result_);
}

HttpReplyResult::HttpReply HttpReplyResult::reply() {
    return std::get<HttpReplyResult::HttpReply>(result_);
}

HttpReplyResult::NetworkError HttpReplyResult::error() {
    return std::get<HttpReplyResult::NetworkError>(result_);
}
