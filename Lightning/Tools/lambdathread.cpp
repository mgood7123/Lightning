#include "lambdathread.h"

void LambdaThread::run() {
    func();
}

LambdaThread::LambdaThread(std::function<void ()> func) {
    this->func = func;
}

LambdaThread::~LambdaThread() {}

void LambdaThread::setFunc(const std::function<void ()> &func)
{
    this->func = func;
}

const std::function<void ()> &LambdaThread::getFunc() const
{
    return func;
}

LambdaThreadHelper::LambdaThreadHelper(std::function<void ()> &func) {
    this->func = func;
}

QThread *LambdaThreadHelper::threadGetThread() {
    return workerThread->thread();
}

void LambdaThreadHelper::threadCreate() {
    if (workerThread != nullptr) {
        qFatal("attempting to create a thread without first destroying");
    }
    workerThread = new LambdaThread(func);
    connect(workerThread, &LambdaThread::started, this, &LambdaThreadHelper::private_threadHasStarted, Qt::DirectConnection);
    connect(workerThread, &LambdaThread::finished, this, &LambdaThreadHelper::private_threadHasFinished, Qt::DirectConnection);
    if (context != nullptr) {
        context->moveToThread(workerThread->thread());
    }
    workerThread->start();
}

void LambdaThreadHelper::threadCreate(const std::function<void ()> &func) {
    threadCreate(nullptr, func);
}

void LambdaThreadHelper::threadCreate(QOpenGLContext * context, const std::function<void ()> &func) {
    this->func = func;
    this->context = context;
    threadCreate();
}

void LambdaThreadHelper::threadDestroy()
{
    threadWaitForFinish();
    disconnect();
    delete workerThread;
    workerThread = nullptr;
}

void LambdaThreadHelper::threadWaitForFinish() {
    workerThread->wait();
    started = false;
    finished = true;
}

bool LambdaThreadHelper::threadExists() {
    return workerThread != nullptr;
}

bool LambdaThreadHelper::threadHasStarted() {
    return started;
}

bool LambdaThreadHelper::threadHasFinished() {
    return finished;
}

void LambdaThreadHelper::threadSetFunc(const std::function<void ()> &func)
{
    this->func = func;
}

const std::function<void ()> &LambdaThreadHelper::threadGetFunc() const
{
    return func;
}

LambdaThreadHelper::~LambdaThreadHelper() {
    if (threadExists()) {
        threadWaitForFinish();
        threadDestroy();
    }
}

void LambdaThreadHelper::private_threadHasStarted() {
    finished = false;
    started = true;
}

void LambdaThreadHelper::private_threadHasFinished() {
    started = false;
    finished = true;
}
