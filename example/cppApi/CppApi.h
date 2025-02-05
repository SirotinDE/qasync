#pragma once

#include <QObject>

class WebGetterInteractor;

class CppApi : public QObject {
    Q_OBJECT
public:
    explicit CppApi(WebGetterInteractor* webGetterInteractor, QObject* parent = nullptr);

    Q_INVOKABLE void getExample();
    Q_INVOKABLE void getExampleVoid();

private:
    WebGetterInteractor* webGetterInteractor_{nullptr};
};
