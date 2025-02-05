#include "CppApi.h"

#include <QDebug>

#include "interactor/WebGetterInteractor.h"

CppApi::CppApi(WebGetterInteractor* webGetterInteractor, QObject* parent)
    : QObject(parent), webGetterInteractor_(webGetterInteractor) {}

void CppApi::getExample() {
    webGetterInteractor_->callGet("http://example.com").then(this, [](QString s) {
        qDebug() << s;
    });
}

void CppApi::getExampleVoid() {
    webGetterInteractor_->callVoid("http://example.com")
        .then(this, [](){
        qDebug() << "void call!";
    });;
}
