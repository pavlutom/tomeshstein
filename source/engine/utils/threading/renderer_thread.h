//
// Created by tomesh on 6/17/20.
//

#ifndef TOMESHSTEIN_RENDERER_THREAD_H
#define TOMESHSTEIN_RENDERER_THREAD_H

#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>


class CRendererThread {
private:
    typedef std::unique_lock<std::mutex> u_lock;
    typedef std::lock_guard<std::mutex> lock;
    typedef std::function<void()> job;
    typedef std::queue<job> job_queue;

public:
    CRendererThread();
    ~CRendererThread();

    void terminate();
    void execute(job &&job);
    void wait();

private:
    std::thread m_Thread;
    job_queue m_JobQueue;

    bool m_Alive;

    std::mutex m_JobQueueMtx;
    std::condition_variable m_JobQueueCond;

    bool m_WaitCalled;
    std::mutex m_WaitMtx;
    std::condition_variable m_WaitCond;

    bool m_JobDone;
    std::mutex m_DoneMtx;
    std::condition_variable m_DoneCond;

    void _thread_function();
};


#endif //TOMESHSTEIN_RENDERER_THREAD_H
