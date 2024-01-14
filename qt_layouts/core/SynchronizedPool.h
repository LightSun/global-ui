#pragma once

#include <memory>
#include "SafeLinkedList.h"

namespace h7 {
    template <class T>
    class SynchronizedPool{
    public:
        using SPT = std::shared_ptr<T>;

        SynchronizedPool(int size):m_maxSize(size){}
        SynchronizedPool():m_maxSize(0){}

        template <typename ...Args>
        T* acquire(Args&& ... args){
            T* spt;
            if(m_list.pollFirst(spt)){
                return spt;
            }
            return new T(std::forward<Args>(args)...);
        }
        void release(T* spt){
            if(m_maxSize > 0 && m_list.size() == m_maxSize){
                return;
            }
            m_list.addLast(spt);
        }
    private:
        SafeLinkedList<T*> m_list;
        int m_maxSize{0};
    };

    template <class T>
    class SynchronizedPool2{
    public:
        using SPT = std::shared_ptr<T>;

        template <typename ...Args>
        SPT acquire(Args&& ... args){
            SPT spt;
            if(m_list.pollFirst(spt)){
                return spt;
            }
            return std::make_shared<T>(std::forward<Args>(args)...);
        }
        void release(SPT spt){
            m_list.addLast(spt);
        }
    private:
        SafeLinkedList<SPT> m_list;
    };
}
