#pragma once
#include <atomic>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <string>

struct Bulk;

template <typename T>
class queueLists
{
    public:
        queueLists() : m_EOF{false}{};
        void push(T line);
        bool pop(T& );
        void setEOF();
        size_t size () {
            std::unique_lock<std::mutex> lock(m_mutex);
            return m_deque.size();}
    private:
        std::deque<T> m_deque;
        std::mutex m_mutex; 
        std::condition_variable m_cv;
        bool m_EOF; 
};

template <typename T>
void queueLists<T>::setEOF()
{
    m_EOF = true;
    m_cv.notify_one(); // all ( one after one );
}

template <typename T>
void queueLists<T>::push(T line )
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_deque.push_back(line);
    m_cv.notify_one();
}

template <typename T>
bool queueLists<T>::pop(T& line)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    while (m_deque.size() == 0 && !m_EOF)
			m_cv.wait(lock);
            
    if (m_deque.size())
    {
        line = std::move(m_deque.front());
        m_deque.pop_front();
        return true;
    }
    m_cv.notify_one(); // all ( one after one );
    return false;
}