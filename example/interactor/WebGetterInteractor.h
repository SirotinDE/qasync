#pragma once

#include <qasync/qasync.h>

class WebGetterInteractor {
public:
    qasync<QString> callGet(QString url);
    qasync<void> callVoid(QString url);
};
