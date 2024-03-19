
#pragma once

#include <atomic>
#include <cstddef>

template <typename T> class LockFreeQueueWithChain
{
public:
	explicit LockFreeQueueWithChain(size_t capacity)
	{
		_capacity = capacity;
		_head.store(nullptr);
		_tail.store(nullptr);
	}

	~LockFreeQueueWithChain()
	{
		while (size()) {
			std::shared_ptr<T> result;
			pop_front(result);
			result->reset();
		}
	}

	size_t capacity() const { return _capacity; }

	size_t size() const
	{
		return _size;
	}

	bool push_back(const std::shared_ptr<T>& data)
	{
		auto tail = _tail.load();
		Node *node=new Node{ data,tail,nullptr };
		for (;;)
		{
			if (!_size.load()) {
				_tail.compare_exchange_weak(tail, node);
				_head.compare_exchange_weak(tail, node);
			}
			else {
				_tail.compare_exchange_weak(tail, node);
				tail->back.store(node);
			}
			++_size;
			break;
		}
		return true;
	}

	bool push_front(const std::shared_ptr<T>& data)
	{
		auto head = _head.load();
		Node* node = new Node{ data,nullptr,head };
		for (;;)
		{
			if (!_size.load()) {
				_tail.compare_exchange_weak(head, node);
				_head.compare_exchange_weak(head, node);
			}
			else {
				_head.compare_exchange_weak(head, node);
				head->front.store(node);
			}
			++_size;
			break;
		}
		return true;
	}

	bool pop_front(std::shared_ptr<T>& result)
	{
		auto head = _head.load();
		for (;;)
		{
			if (!_size.load() || !head) {
				return false;
			}
			else {
				_head.compare_exchange_weak(head, head->back);
				--_size;
				if (!_size) {
					_tail.store(nullptr);
				}
				break;
			}
		}
		result = head->data;
		head->data.reset(); head->data = nullptr;
		return true;
	}

	bool pop_back(std::shared_ptr<T>& result)
	{
		auto tail = _tail.load();
		for (;;)
		{
			if (!_size.load() || !tail) {
				return false;
			}
			else {
				_tail.compare_exchange_weak(tail, tail->front);
				--_size;
				if (!_size) {
					_head.store(nullptr);
				}
				break;
			}
		}
		result = tail->data;
		tail->data.reset(); tail->data = nullptr;
		return true;
	}

private:
	struct Node
	{
		std::shared_ptr<T> data{ nullptr };
		std::atomic<Node*> front{ nullptr };
		std::atomic<Node*> back{ nullptr };
	};

private:
	size_t _capacity{0};
	std::atomic<size_t> _size{0};
	std::atomic<Node*> _tail{ nullptr };
	std::atomic<Node*> _head{ nullptr };
};