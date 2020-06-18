//
// Created by tomesh on 6/17/20.
//

#include "renderer_thread.h"


CRendererThread::CRendererThread()
        : m_Thread(&CRendererThread::_thread_function, this),
          m_Alive(true), m_WaitCalled(false), m_JobDone(false) {}

CRendererThread::~CRendererThread() {
    m_Thread.join();
}

void CRendererThread::terminate() {
    {
        lock queueLock(m_JobQueueMtx);
        m_Alive = false;
    }
    m_JobQueueCond.notify_one();
}

void CRendererThread::execute(job &&job) {
    {
        lock queueLock(m_JobQueueMtx);
        m_JobQueue.push(job);
    }
    m_JobQueueCond.notify_one();
}

void CRendererThread::wait() {
    {
        lock waitLock(m_WaitMtx);
        m_WaitCalled = true;
    }
    m_WaitCond.notify_one();

    u_lock doneLock(m_DoneMtx);
    m_DoneCond.wait(doneLock, [this] {
        bool jobDone = m_JobDone;
        m_JobDone = false;
        return jobDone;
    });
}

void CRendererThread::_thread_function() {
    u_lock jobQueueLock(m_JobQueueMtx);

    while (true) {
        job job = nullptr;
        m_JobQueueCond.wait(jobQueueLock, [this, &job] {
            if (m_JobQueue.empty()) {
                return !m_Alive;
            } else {
                job = std::move(m_JobQueue.front());
                m_JobQueue.pop();
                return true;
            }
        });

        if (!job) break;
        job();

        {
            lock doneLock(m_DoneMtx);
            m_JobDone = true;
        }
        m_DoneCond.notify_one();

        u_lock waitLock(m_WaitMtx);
        m_WaitCond.wait(waitLock, [this] {
            bool waitCalled = m_WaitCalled;
            m_WaitCalled = false;
            return waitCalled;
        });
    }
}
