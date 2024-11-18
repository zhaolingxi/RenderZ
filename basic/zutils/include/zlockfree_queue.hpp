#pragma once 

#include <atomic>
#include <queue>
#include <memory>
#include <iostream>
#include <list>
#include"zutils_sdk.h"

ZUTILS_NS_BEGIN


template<typename T>
struct Node {
    std::shared_ptr<T> val_{nullptr};
    std::shared_ptr<Node> next_{ nullptr };
};

template<typename T>
class ZUTILS_API ZLockFreeQueue
{
public:
    ZLockFreeQueue() {
        front_ = std::make_shared<Node<T>>();
        tail_ = front_;
    }
    virtual ~ZLockFreeQueue() {
        auto p = std::move(front_);
        while (p->next_ != nullptr && p->next_.use_count() == 1) {
            // Some other thread may start pointing to `p->tail`
            // and increases its use count before the next line
            p = std::move(p->next_);
        }
    }

    bool push(const T& val) {
        // if (stop_push_) { return false; }
        // auto last = nullptr;
        // auto new_node = std::make_shared<Node<T>>();
        // new_node->val_ = val;
        // last = atomic_load(&tail_);
        // //CAS compare and swap
        // while (!std::atomic_compare_exchange_strong(&tail_, &last, new_node));
        // //3.�����ɹ� tail_ = new_node
        // //4.last->next = new_node;
        // std::atomic_store(&(last->next_), new_node); //
        // ++size_;
        return true;
    }


    bool push(T&& val) {
        // if (stop_push_) return false;
        // auto last = nullptr;
        // auto new_node = std::make_shared<Node<T>>();
        // new_node->val_ = std::move(val);
        // last = atomic_load(&tail_);
        // //CAS compare and swap
        // while (!std::atomic_compare_exchange_strong(&tail_, &last, new_node));
        // std::atomic_load(&(last->next_), new_node);
        // ++size_;
        return true;
    }

    bool pop(T& val) {
        // if (stop_pop_) return false;
        // auto first = nullptr;
        // auto first_next = nullptr;

        // do {
        //     first = std::atomic_load(&front_);
        //     first_next = std::atomic_load(&(front_->next_));
        //     if (!first_next) return false;
        //     //CAS compare and swap
        // } while (!std::atomic_compare_exchange_strong(&front_, &first, first_next));
        // --size_;
        // val = std::move(first_next->val_);
        return true;
    }

    inline int size() {
        return size_;
    }

    inline bool empty() {
        return size_ == 0;
    }

private:
    mutable std::atomic_int32_t size_ = 0;
    mutable std::atomic_bool stop_push_ = false;
    mutable std::atomic_bool stop_pop_ = false;
    std::shared_ptr<Node<T>> front_;
    std::shared_ptr<Node<T>> tail_;
};
ZUTILS_NS_END


