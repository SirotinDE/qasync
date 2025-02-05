# qasync

qasync - helper for running functions in parallel thread with callbacks in main thread for Qt5

## Usage

Return qasync with a function as an argument

```cpp
// WebGetterInteractor.h

class WebGetterInteractor {
public:
    qasync<QString> callGet(QString url);
};

// WebGetterInteractor.cpp

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

```

If a callback is needed, qasync has a method "then"

```cpp

void CppApi::getExample() {
    webGetterInteractor_->callGet("http://example.com")
        .then(this, [](QString s) {
            qDebug() << s;
    });
}

```

