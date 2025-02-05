#pragma once

#include <functional>
#include <cassert>

#include <QDebug>
#include <QFutureWatcher>
#include <QThreadPool>
#include <QtConcurrent/QtConcurrent>

class qasync_thread_pool: public QObject {
    Q_OBJECT
    QThreadPool pool_;
    qasync_thread_pool();
public:
    static QThreadPool* pool();
    static qasync_thread_pool *instance();
};

// RAII wraper around qasync_task
template <class ReturnType>
class qasync {
private:
    template <class T>
    class qasync_task : public QFutureWatcher<T> {
        friend class qasync;

    public:
        qasync_task(std::function<T()> task) : QFutureWatcher<T>(qasync_thread_pool::instance()), task_(task) {
            assert(task != nullptr);
        }

        void then(QObject* receiver, std::function<void(T)> cb) {
            assert(cb_ == nullptr); // then allready set

            receiver_ = receiver;
            cb_ = cb;
        }

    private:
        void run() {
            if (receiver_ && cb_) {
                QObject::connect(this, &QFutureWatcherBase::finished, receiver_, [this]() {
                    try {
                        if (cb_) {
                            cb_(this->result());
                        }
                    } catch (...) {
                        qWarning() << "Error!" << task_.target_type().name();
                    }
                    this->deleteLater();
                });
            } else {
                this->connect(this, &QFutureWatcherBase::finished, this, &QFutureWatcherBase::deleteLater);
            }
            QObject::connect(this, &QFutureWatcherBase::canceled, this, &QFutureWatcherBase::deleteLater);
            this->setFuture(QtConcurrent::run(qasync_thread_pool::instance()->pool(), task_));
        }

    private:
        std::function<T()> task_;
        QObject* receiver_{nullptr};
        std::function<void(T)> cb_{nullptr};
    };

public:
    // qasync_task creates with qasync_thread_pool::instance() as a QObject parent inside
    qasync(std::function<ReturnType()> task)
        : task_(new qasync_task<ReturnType>(task)) {
        assert(task != nullptr);
    }

    ~qasync() {
        task_->run();
    }

    void then(QObject* receiver, std::function<void(ReturnType)> cb) {
        assert(receiver != nullptr);
        assert(cb != nullptr);
        task_->then(receiver, cb);
    }

private:
    qasync_task<ReturnType>* task_{nullptr};
};

template <>
class qasync<void> {
private:
    class qasync_task_void : public QFutureWatcher<void> {
        friend class qasync;

    public:
        qasync_task_void(std::function<void()> task)
            : QFutureWatcher<void>(qasync_thread_pool::instance()), task_(task)
        {}

        void then(QObject* receiver, std::function<void()> cb) {
            assert(cbVoid_ == nullptr); // then allready set

            receiver_ = receiver;
            cbVoid_ = cb;
        }

    private:
        void run() {
            if (receiver_ && cbVoid_) {
                QObject::connect(this, &QFutureWatcherBase::finished, receiver_, [this]() {
                    try {
                        if (cbVoid_) {
                            cbVoid_();
                        }
                    } catch (...) {
                        qWarning() << "Error!" << task_.target_type().name();
                    }
                    this->deleteLater();
                });
            } else {
                this->connect(this, &QFutureWatcherBase::finished, this, &QFutureWatcherBase::deleteLater);
            }
            QObject::connect(this, &QFutureWatcherBase::canceled, this, &QFutureWatcherBase::deleteLater);
            this->setFuture(QtConcurrent::run(qasync_thread_pool::instance()->pool(), task_));
        }

    private:
        std::function<void()> task_;
        QObject* receiver_{nullptr};
        std::function<void()> cbVoid_{nullptr};
    };

public:
    // qasync_task_void creates with qasync_thread_pool::instance() as a QObject parent inside
    qasync(std::function<void()> task)
        : task_(new qasync_task_void(task)) {
        assert(task != nullptr);
    }

    ~qasync() {
        task_->run();
    }

    void then(QObject* receiver, std::function<void()> cb) {
        assert(receiver != nullptr);
        assert(cb != nullptr);
        task_->then(receiver, cb);
    }

private:
    qasync_task_void* task_{nullptr};
};

