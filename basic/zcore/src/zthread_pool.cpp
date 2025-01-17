//#include <assert.h>
//
//#include "zthread_pool.h"
//
//ThreadPool::ThreadPool()
//    :m_mutex(),
//    m_isStarted(false)
//{
//
//}
//
//ThreadPool::~ThreadPool()
//{
//    if (m_isStarted)
//    {
//        stop();
//    }
//}
//
//void ThreadPool::start()
//{
//    assert(m_threads.empty());
//    m_isStarted = true;
//    m_threads.reserve(kInitThreadsSize);
//    for (int i = 0; i < kInitThreadsSize; ++i)
//    {
//        m_threads.push_back(new std::thread(std::bind(&ThreadPool::threadLoop, this)));
//    }
//
//}
//
//void ThreadPool::stop()
//{
//    {
//        std::unique_lock<std::mutex> lock(m_mutex);
//        m_isStarted = false;
//        m_cond.notify_all();
//    }
//
//    for (Threads::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
//    {
//        (*it)->join();
//        delete* it;
//    }
//    m_threads.clear();
//}
//
//
//void ThreadPool::threadLoop()
//{
//    while (m_isStarted)
//    {
//        Task task = take();
//        if (task)
//        {
//            task();
//        }
//    }
//}
//
//void ThreadPool::addTask(const Task& task)
//{
//    std::unique_lock<std::mutex> lock(m_mutex);
//    TaskPair taskPair(level2, task);
//    m_tasks.push(taskPair);
//    m_cond.notify_one();
//}
//
//void ThreadPool::addTask(const TaskPair& taskPair)
//{
//    std::unique_lock<std::mutex> lock(m_mutex);
//    m_tasks.push(taskPair);
//    m_cond.notify_one();
//}
//
//ThreadPool::Task ThreadPool::take()
//{
//    std::unique_lock<std::mutex> lock(m_mutex);
//    while (m_tasks.empty() && m_isStarted)
//    {
//        m_cond.wait(lock);
//    }
//
//
//    Task task;
//    Tasks::size_type size = m_tasks.size();
//    if (!m_tasks.empty() && m_isStarted)
//    {
//        task = m_tasks.top().second;
//        m_tasks.pop();
//        assert(size - 1 == m_tasks.size());
//
//    }
//
//    return task;
//
//}