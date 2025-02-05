#include "WebGetterInteractor.h"

#include <functional>

#include <QDebug>
#include <QThread>

#include "../http/HttpClient.h"
#include "../http/HttpReply.h"


qasync<QString> WebGetterInteractor::callGet(QString url)
{
    return {
        [url]() -> QString {
            HttpClient::instance().init(url, 80);
            auto result = HttpClient::instance().get("/",{});
            if (result.isOk()) {
                return result.reply().payload();
            } else {
                return result.error().errorString();
            }
        }};
}

qasync<void> WebGetterInteractor::callVoid(QString url)
{
    return {
        [url]() {
            HttpClient::instance().init(url, 80);

            QString answer;
            auto result = HttpClient::instance().get("/",{});
            if (result.isOk()) {
                answer = result.reply().payload();
            } else {
                answer = result.error().errorString();
            }
            qDebug() << answer;
        }};
}

