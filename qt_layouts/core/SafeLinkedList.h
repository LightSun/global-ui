#pragma once

#include <list>
#include "locks.h"

namespace h7 {
template<typename T>
    class SafeLinkedList{
    public:
        using CT = const T&;
        void addLast(CT t){
            MutexLockHolder lck(m_lock);
            m_list.push_back(t);
        }
        void addFirst(CT t){
            MutexLockHolder lck(m_lock);
            m_list.push_front(t);
        }
        bool pollFirst(T& out){
            MutexLockHolder lck(m_lock);
            if(m_list.size() == 0){
                return false;
            }
            out = m_list.front();
            m_list.pop_front();
            return true;
        }
        bool pollLast(T& out){
            MutexLockHolder lck(m_lock);
            if(m_list.size() == 0){
                return false;
            }
            out = m_list.back();
            m_list.pop_back();
            return true;
        }
        void trimHeadToSize(int size){
            MutexLockHolder lck(m_lock);
            if(m_list.size() > size){
                int delta = m_list.size() - size;
                for(int i = 0 ; i < delta ; ++i){
                    m_list.pop_front();
                }
            }
        }
        void trimTailToSize(int size){
            MutexLockHolder lck(m_lock);
            if(m_list.size() > size){
                int delta = m_list.size() - size;
                for(int i = 0 ; i < delta ; ++i){
                    m_list.pop_back();
                }
            }
        }
        int size(){
            MutexLockHolder lck(m_lock);
            return m_list.size();
        }
    private:
        h7::MutexLock m_lock;
        std::list<T> m_list;
    };
}
