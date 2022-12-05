//
// Created by tomas on 13.9.22.
//

#include <iostream>
#include <vector>
#include <tuple>

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

