#pragma once

#include <variant>

#include <QByteArray>

class QNetworkReply;

struct HttpReplyResult {
    struct HttpReply {
    public:
        HttpReply(QNetworkReply* reply);
        ~HttpReply();

        int code() const;
        QByteArray payload() const;

    protected:
        QNetworkReply* reply_{nullptr};
    };

    struct NetworkError : public HttpReply {
        NetworkError(QNetworkReply* reply);
        QString errorString() const;
    };

    using HttpReplyVariant = std::variant<HttpReply, NetworkError>;

    HttpReplyResult() = delete;
    HttpReplyResult(HttpReplyVariant&& v);

    bool isOk();
    HttpReply reply();
    NetworkError error();

private:
    HttpReplyVariant result_;
};
