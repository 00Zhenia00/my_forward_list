#pragma once

#include<iostream>
#include<initializer_list>
#include<memory>


template <class T>
struct Node
{
	T data;
	Node* next = nullptr;
};


template< class T >
class ForwardList {

public:

	ForwardList() : begin_{ nullptr }, end_{ nullptr }, count(0) {}
	 
	// Copy constructor
	ForwardList(const ForwardList& other) {
		try {
			copy(other);
		}
		catch (const std::bad_alloc&) {
			clear();
		}
	}

	// Move constructor
	ForwardList(ForwardList&& other) : begin_(other.begin_), end_(other.end_), count(other.count){

		other.begin_ = nullptr;
		other.end_ = nullptr;
		other.count = 0;

	}

	ForwardList(std::initializer_list<T> init) : count(init.size()) {

		try {
			// Initialize first node
			Node<T>* current = new Node<T>;
			current->data = *(init.begin());
			begin_ = current;

			// Initialize other nodes
			for (auto it = init.begin() + 1; it != init.end(); ++it) {
				current->next = new Node<T>;
				current->next->data = *it;
				current = current->next;
			}

			// Initialize last node
			end_ = current;
			current = nullptr;
		}
		catch (const std::bad_alloc&) {
			std::cout << "Failed to initialize ForwardList object!\n";
			clear();
		}

	}


	~ForwardList() {
		clear();
	}


	// Copy assignment
	// basic guarantee: if assignment failed return empty list
	ForwardList<T>& operator=(const ForwardList& other){
		try {
			copy(other);
			return *this;
		}
		catch (const std::bad_alloc&) {
			clear();
		}
	}

	// Move assignment
	ForwardList<T>& operator=(const ForwardList&& other) {
		
		clear();

		begin_ = other.begin_;
		end_ = other.end_;
		count = other.count;

		other.begin_ = nullptr;
		other.end_ = nullptr;
		other.count = 0;

		return *this;
	}

	const T& front() const {
		return begin_->data;
	}

	void push_front(const T& value) noexcept {
		insert(value, 0);
	}

	void push_front(T&& value) noexcept {

		try {
			Node<T>* n = new Node<T>;
			n->data = std::move(value);
			n->next = begin_;
			begin_ = n;
			count++;
		}
		catch (std::bad_alloc& e) {
			std::cout << "Failed to push_front element!\n" << std::endl;
		}

	}

	int size() const {
		return count;
	}

	bool empty() const{
		return count == 0;
	}

	void remove(int index) noexcept {

		if (empty()) return;

		if (index < 0 || index > count) return;

		if (index == 0) {
			// if remove first node
			// std::unique_ptr<Node<T>> tmp(begin);	* I think unique_ptr is not necessary here
			Node<T>* tmp = begin_;
			begin_ = begin_->next;
			delete tmp;

		}
		else {

			Node<T>* n_prev = find_node(index - 1);

			Node<T>* n = n_prev->next;

			n_prev->next = n->next;

			delete n;

		}

		count--;

	}

	void pop_front() noexcept {
		remove(0);
	}

	void clear() noexcept{

		while (begin_ != nullptr) {
			pop_front();
		}

	}

	void insert(T value, int index) noexcept {

		if ((count != 0) && ((index < 0) || (index >= count)))
			return;

		try {

			if (empty()) {
				push_back(value);
			}
			else {

				Node<T>* n = find_node(index);
				Node<T>* n_new = new Node<T>;
				n_new->data = value;

				if (index == 0) {
					// if node insert on first position
					n_new->next = begin_;
					begin_ = n_new;
				}
				else {
					// if node insert on after first position
					n = find_node(index - 1);
					n_new->next = n->next;
					n->next = n_new;

				}

				count++;

			}
		}
		catch (const std::bad_alloc& e) {
			std::cout << "Failed to insert element!\n" << std::endl;
		}

	}

	class Iterator {
	public:
		Iterator(Node<T>* cur = nullptr) : current(cur) {}

		T& operator*() const {
			if (current)
				return current->data;
			else
				throw std::exception("wrong iterator position");
		}

		Iterator& operator ++() {
			if (current)
				current = current->next;
			return *this;
		}

		bool operator ==(const Iterator& other) const {
			return current == other.current;
		}

		bool operator !=(const Iterator& other) const {
			return current != other.current;
		}

	private:
		Node<T>* current;
	};

	Iterator begin() {
		return Iterator(begin_);
	}

	Iterator end() {
		return Iterator();
	}


private:

	void copy(const ForwardList& other) {	// throw(std::bad_alloc)

		clear();

		Node<T>* n = other.begin_;

		for (int i = 0; i < other.count; i++)
		{
			push_back(n->data);
			n = n->next;
		}

		count = other.count;

	}


	Node<T>* find_node(int index) noexcept {

		if (count == 0 || index < 0 || index > count)
			return nullptr;

		Node<T>* n = begin_;

		for (int i = 0; i < index; i++) {
			n = n->next;
		}

		return n;

	}


	void push_back(const T& data) {	// throw(std::bad_alloc)

		// Create and fill new node
		Node<T>* node = new Node<T>;
		node->data = data;
		node->next = nullptr; // last node in list

		// push_back node
		if (begin_ == nullptr)
		{
			// if node is first
			begin_ = node;
			end_ = node;
		}
		else
		{
			// if node is not first add to the end of list
			end_->next = node;
			end_ = node;
		}

		count++;

	}

private:
	Node<T>* begin_;
	Node<T>* end_;
	int count; // amount of nodes in list

};
