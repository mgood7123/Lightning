#ifndef LAMBDATHREAD_H
#define LAMBDATHREAD_H

#include <QOpenGLContext>
#include <QThread>

class LambdaThread : public QThread
{
    std::function<void()> func;
    Q_OBJECT
    void run() override;

public:
    LambdaThread () = default;
    LambdaThread (std::function<void()> func);

    virtual ~LambdaThread();;
    void setFunc(const std::function<void()> & func);
    const std::function<void()> &getFunc() const;
};

class LambdaThreadHelper : public QObject {
    std::function<void()> func;
    LambdaThread * workerThread = nullptr;
    QOpenGLContext * context = nullptr;
    bool started = false;
    bool finished = false;
    bool terminated = true;
    Q_OBJECT
public:
    LambdaThreadHelper() = default;
    LambdaThreadHelper(std::function<void()> & func);
    QThread * threadGetThread();
    void threadCreate();
    void threadCreate(const std::function<void()> & func);
    void threadCreate(QOpenGLContext * context, const std::function<void()> & func);
    void threadDestroy();
    void threadWaitForFinish();
    bool threadExists();
    bool threadHasStarted();
    bool threadHasFinished();
    void threadSetFunc(const std::function<void()> & func);
    const std::function<void()> &threadGetFunc() const;
    virtual ~LambdaThreadHelper();
private slots:
    void private_threadHasStarted();
    void private_threadHasFinished();
};

#endif // LAMBDATHREAD_H
