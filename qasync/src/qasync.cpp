#include <qasync/qasync.h>

qasync_thread_pool::qasync_thread_pool() {
    pool_.setMaxThreadCount(1);
}

QThreadPool* qasync_thread_pool::pool() {
    return &instance()->pool_;
}

qasync_thread_pool* qasync_thread_pool::instance() {
    static qasync_thread_pool c;
    return &c;
}
