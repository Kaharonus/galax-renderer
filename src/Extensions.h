//
// Created by tomas on 13.9.22.
//

#pragma once

#include <iostream>
#include <vector>
#include <tuple>
#include <coroutine>
#include <iterator>
#include <memory>
#include <utility>

// ----------------- 1. -----------------
// Cute lil macro that will wrap any code in opengl's debug groups for that sweet renderdoc goodness
#ifdef DEBUG
#define GL_DEBUG(name, fn) \
    glPushDebugGroup((gl::GLenum) GL_DEBUG_SOURCE_APPLICATION, 0, -1, name); \
    fn \
    glPopDebugGroup();
#else
#define GL_DEBUG(fn, name) \
    fn
#endif

// ----------------- 2. -----------------
// A macro that will disable a gl feature for the scope of the macro
#define GL_DISABLE(feature, fn) \
	gl::glDisable(feature); \
	fn \
	gl::glEnable(feature);


// ----------------- 2. -----------------
// Used to enumarete over a list of types and get an index with the value

// Wrapper class
template<typename T>
class enumerate_impl {
public:
    // The return value of the operator* of the iterator, this
    // is what you will get inside the for loop
    struct item {
        size_t i;
        typename T::value_type &item;
    };
    typedef item value_type;

    // Custom iterator with minimal interface
    struct iterator {
        iterator(typename T::iterator _it, size_t counter = 0) :
                it(_it), counter(counter) {}

        iterator operator++() {
            return iterator(++it, ++counter);
        }

        bool operator!=(iterator other) {
            return it != other.it;
        }

        typename T::iterator::value_type item() {
            return *it;
        }

        value_type operator*() {
            return value_type{counter, *it};
        }

        size_t index() {
            return counter;
        }

    private:
        typename T::iterator it;
        size_t counter;
    };

    enumerate_impl(T &t) : container(t) {}

    iterator begin() {
        return iterator(container.begin());
    }

    iterator end() {
        return iterator(container.end());
    }

private:
    T &container;
};

// A templated free function allows you to create the wrapper class
// conveniently
template<typename T>
enumerate_impl<T> enumerate(T &t) {
    return enumerate_impl<T>(t);
}

// ----------------- 3. -----------------
// Used to check if a collection contains a value

template<typename T>
bool contains(const T &container, const typename T::value_type &value) {
    return std::find(container.begin(), container.end(), value) != container.end();
}


// ----------------- 4. -----------------
// A macro that will switch code path based on debug mode
#ifdef DEBUG
#define IS_DEBUG(dbg, rls) \
  dbg
#else
#define IS_DEBUG(dbg, rls) \
  rls
#endif

// ----------------- 5. -----------------
// A generator for C++20
// Taken from: https://raw.githubusercontent.com/Quuxplusone/coro/master/include/coro/unique_generator.h
/*
#ifndef INCLUDED_CORO_MANUAL_LIFETIME_H
#define INCLUDED_CORO_MANUAL_LIFETIME_H

template<class T>
struct manual_lifetime {
public:
	manual_lifetime() noexcept {}
	~manual_lifetime() noexcept {}

	template<class... Args>
	void construct(Args&&... args) {
		::new (static_cast<void*>(std::addressof(value))) T(static_cast<Args&&>(args)...);
	}

	void destruct() noexcept {
		value.~T();
	}

	T& get() & { return value; }
	const T& get() const & { return value; }
	T&& get() && { return (T&&)value; }
	const T&& get() const && { return (const T&&)value; }

private:
	union { T value; };
};

template<class T>
struct manual_lifetime<T&> {
	manual_lifetime() noexcept = default;

	void construct(T& value) noexcept {
		ptr = std::addressof(value);
	}
	void destruct() noexcept {}
	T& get() const noexcept { return *ptr; }

private:
	T *ptr = nullptr;
};


template<class T>
struct manual_lifetime<T&&> {
	manual_lifetime() noexcept = default;

	void construct(T&& value) noexcept {
		ptr = std::addressof(value);
	}
	void destruct() noexcept {}
	T&& get() const noexcept { return *ptr; }

private:
	T *ptr = nullptr;
};

template<>
struct manual_lifetime<void> {
	void construct() noexcept {}
	void destruct() noexcept {}
	void get() const noexcept {}
};
#endif // INCLUDED_CORO_MANUAL_LIFETIME_H


template<class Ref, class Value = std::decay_t<Ref>>
class unique_generator {
public:
	class promise_type {
	public:
		promise_type() noexcept = default;

		~promise_type() noexcept {
			clear_value();
		}

		void clear_value() {
			if (hasValue_) {
				hasValue_ = false;
				ref_.destruct();
			}
		}

		unique_generator get_return_object() noexcept {
			return unique_generator(
					std::coroutine_handle<promise_type>::from_promise(*this)
			);
		}

		auto initial_suspend() noexcept {
			return std::suspend_always{};
		}

		auto final_suspend() noexcept {
			return std::suspend_always{};
		}

		auto yield_value(Ref ref)
		noexcept(std::is_nothrow_move_constructible_v<Ref>)
		{
			ref_.construct(std::move(ref));
			hasValue_ = true;
			return std::suspend_always{};
		}

		void return_void() {}

		void unhandled_exception() {
			throw;
		}

		Ref get() {
			return ref_.get();
		}

	private:
		manual_lifetime<Ref> ref_;
		bool hasValue_ = false;
	};

	using handle_t = std::coroutine_handle<promise_type>;

	unique_generator(unique_generator&& g) noexcept :
			coro_(std::exchange(g.coro_, {}))
	{}

	~unique_generator() {
		if (coro_) {
			coro_.destroy();
		}
	}

	struct sentinel {};

	class iterator {
	public:
		using reference = Ref;
		using value_type = Value;
		using difference_type = std::ptrdiff_t;
		using pointer = std::add_pointer_t<Ref>;
		using iterator_category = std::input_iterator_tag;

		iterator() noexcept {}

		explicit iterator(handle_t coro) noexcept :
				coro_(coro)
		{}

		reference operator*() const {
			return coro_.promise().get();
		}

		iterator& operator++() {
			coro_.promise().clear_value();
			coro_.resume();
			return *this;
		}

		void operator++(int) {
			coro_.promise().clear_value();
			coro_.resume();
		}

		friend bool operator==(const iterator& it, sentinel) noexcept { return it.coro_.done(); }
		friend bool operator==(sentinel, const iterator& it) noexcept { return it.coro_.done(); }
		friend bool operator!=(const iterator& it, sentinel) noexcept { return !it.coro_.done(); }
		friend bool operator!=(sentinel, const iterator& it) noexcept { return !it.coro_.done(); }

	private:
		handle_t coro_;
	};

	iterator begin() {
		coro_.resume();
		return iterator{coro_};
	}

	sentinel end() {
		return {};
	}

private:

	explicit unique_generator(handle_t coro) noexcept :
			coro_(coro)
	{}

	handle_t coro_;
};
*/
