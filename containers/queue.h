#ifndef OOP_EXERCISE_05_QUEUE_H
#define OOP_EXERCISE_05_QUEUE_H

#include <iterator>
#include <memory>

namespace containers {
    //namespace
    //
    template<class T, class Allocator = std::allocator<T>>
    class queue {
    private:
        struct element;
        size_t size = 0;
    public:
        queue() = default;

        class forward_iterator {
        public:
            using value_type = T;
            using reference = T&;
            using pointer = T*;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;
            explicit forward_iterator(element* ptr);
            T& operator*();
            forward_iterator& operator++();
            forward_iterator operator++(int);
            bool operator== (const forward_iterator& other) const;
            bool operator!= (const forward_iterator& other) const;
        private:
            element* it_ptr;
            friend queue;
        };

        forward_iterator begin();
        forward_iterator end();
        void push(const T& value);
        T& top();
        void pop();
        size_t length();
        void delete_by_it(forward_iterator d_it);
        void delete_by_number(size_t N);
        void insert_by_it(forward_iterator ins_it, T& value);
        void insert_by_number(size_t N, T& value);
    private:
        using allocator_type = typename Allocator::template rebind<element>::other;

        struct deleter {
            deleter(allocator_type* allocator): allocator_(allocator) {}

            void operator() (element* ptr) {
                if (ptr != nullptr) {
                    std::allocator_traits<allocator_type>::destroy(*allocator_, ptr);
                    allocator_->deallocate(ptr, 1);
                }
            }

        private:
            allocator_type* allocator_;
        };

        using unique_ptr = std::unique_ptr<element, deleter>;

        struct element {
            T value;
            unique_ptr next_element{nullptr, deleter{nullptr}};
            element(const T& value_): value(value_) {}
            forward_iterator next();
        };

        allocator_type allocator_{};
        unique_ptr first{nullptr, deleter{nullptr}};
        element* tail = nullptr;
    };//===============================end-of-class-queue======================================//

    template<class T, class Allocator>
    typename queue<T, Allocator>::forward_iterator queue<T, Allocator>::begin() {
        return forward_iterator(first.get());
    }

    template<class T, class Allocator>
    typename queue<T, Allocator>::forward_iterator queue<T, Allocator>::end() {
        return forward_iterator(nullptr);
    }
//=========================base-methods-of-queue==========================================//
    template<class T, class Allocator>
    size_t queue<T, Allocator>::length() {
       return size;
    }

    template<class T, class Allocator>
    void queue<T, Allocator>::push(const T &value) {
        element* result = this->allocator_.allocate(1);
        std::allocator_traits<allocator_type>::construct(this->allocator_, result, value);
        if (!size) {
            first = unique_ptr(result, deleter{&this->allocator_});
            tail = first.get();
            size++;
            return;
        }
        tail->next_element = unique_ptr(result, deleter{&this->allocator_});
        tail = tail->next_element.get();
        size++;
    }

    template<class T, class Allocator>
    void queue<T, Allocator>::pop() {
        if (size == 0) {
            throw std::logic_error ("can`t pop from empty queue");
        }
        first = std::move(first->next_element);
        size--;
    }

    template<class T, class Allocator>
    T& queue<T, Allocator>::top() {
        if (size == 0) {
            throw std::logic_error ("queue is empty, lol, it has no top");
        }
        return first->value;
    }
//=================================advanced-methods========================================//

    template<class T, class Allocator>
    void queue<T, Allocator>::delete_by_it(containers::queue<T, Allocator>::forward_iterator d_it) {
        forward_iterator i = this->begin(), end = this->end();
        if (d_it == end) throw std::logic_error ("out of borders");
        if (d_it == this->begin()) {
            this->pop();
            return;
        }
        while((i.it_ptr != nullptr) && (i.it_ptr->next() != d_it)) {
           ++i;
        }
        if (i.it_ptr == nullptr) throw std::logic_error ("out of borders");
        i.it_ptr->next_element = std::move(d_it.it_ptr->next_element);
        size--;
    }

    template<class T, class Allocator>
    void queue<T, Allocator>::delete_by_number(size_t N) {
        N++;
        forward_iterator it = this->begin();
        for (size_t i = 1; i <= N; ++i) {
            if (i == N) break;
            ++it;
        }
        this->delete_by_it(it);
    }

    template<class T, class Allocator>
    void queue<T, Allocator>::insert_by_it(containers::queue<T, Allocator>::forward_iterator ins_it, T& value) {
        auto tmp = std::unique_ptr<element>(new element{value});
        forward_iterator i = this->begin();
        if (ins_it == this->begin()) {
            tmp->next_element = std::move(first);
            first = std::move(tmp);
            size++;
            return;
        }
        while((i.it_ptr != nullptr) && (i.it_ptr->next() != ins_it)) {
            ++i;
        }
        if (i.it_ptr == nullptr) throw std::logic_error ("out of borders");
        tmp->next_element = std::move(i.it_ptr->next_element);
        i.it_ptr->next_element = std::move(tmp);
        size++;
    }

    template<class T, class Allocator>
    void queue<T, Allocator>::insert_by_number(size_t N, T& value) {
        forward_iterator it = this->begin();
        for (size_t i = 1; i <= N; ++i) {
            if (i == N) break;
            ++it;
        }
        this->insert_by_it(it, value);
    }
//==============================iterator`s-stuff=======================================//
    template<class T, class Allocator>
    typename queue<T, Allocator>::forward_iterator queue<T, Allocator>::element::next() {
        return forward_iterator(this->next_element.get());
    }

    template<class T, class Allocator>
    queue<T, Allocator>::forward_iterator::forward_iterator(containers::queue<T, Allocator>::element *ptr) {
        it_ptr = ptr;
    }

    template<class T, class Allocator>
    T& queue<T, Allocator>::forward_iterator::operator*() {
        return this->it_ptr->value;
    }

    template<class T, class Allocator>
    typename queue<T, Allocator>::forward_iterator& queue<T, Allocator>::forward_iterator::operator++() {
        if (it_ptr == nullptr) throw std::logic_error ("out of queue borders");
        *this = it_ptr->next();
        return *this;
    }

    template<class T, class Allocator>
    typename queue<T, Allocator>::forward_iterator queue<T, Allocator>::forward_iterator::operator++(int) {
        forward_iterator old = *this;
        ++*this;
        return old;
    }

    template<class T, class Allocator>
    bool queue<T, Allocator>::forward_iterator::operator==(const forward_iterator& other) const {
        return it_ptr == other.it_ptr;
    }

    template<class T, class Allocator>
    bool queue<T, Allocator>::forward_iterator::operator!=(const forward_iterator& other) const {
        return it_ptr != other.it_ptr;
    }

    //
    //namespace
}



#endif //OOP_EXERCISE_05_QUEUE_H
