#pragma once
#include <type_traits>
#include <utility>

template<class T>
struct vetor {
    T* array;
    int asize;
    int capacity;

    vetor(int n = 1, T* a = nullptr) {
        asize = 0;
        capacity = n;
        array = new T[capacity];
        if (a != nullptr) {
            asize = n;
            for (int i = 0; i < capacity; i++) {
                array[i] = a[i];
            }
        }
    }

    vetor(const vetor < T >& other) {
        asize = other.asize;
        capacity = other.capacity;
        array = new T[capacity];
        for (int i = 0; i < asize; i++) array[i] = other.array[i];
    }

    vetor<T>& operator = (const vetor <T>& other) {
        if (this == &other) return *this;
        delete[] array;
        asize = other.asize;
        capacity = other.capacity;
        array = new T[capacity];
        for (int i = 0; i < asize; i++) array[i] = other.array[i];
        return *this;
    }

    ~vetor() {
        delete[] array;
    }

    template<typename Predicate>
    void foreach(Predicate fn) {
        for (int i = 0; i < asize; i++)
            fn(i, array[i]);
    }

    int size() const { return asize; }

    void push_back(const T& o) {
        if (asize >= capacity) {
            capacity++;
            T* newArray = new T[capacity];
            for (int i = 0; i < asize; i++)
                newArray[i] = array[i];
            delete[] array;
            array = newArray;
        }
        array[asize++] = o;
    }

    T pop_back() {
        if (asize == 0) NULL;
        array[--asize];
        return array[asize];
    }

    vetor<T> clone() {
        T* newArray = new T[capacity];
        for (int i = 0; i < asize; i++)
            newArray[i] = array[i];
        return vetor<T>(asize, newArray);
    }

    void clear() {
        delete[] array;
        capacity = 1;
        array = new T[capacity];
        asize = 0;
    }

    vetor<T> copy(int from, int to) {
        if (from < 0 || to > asize || from > to)
            return vetor<T>();
        int len = to - from;
        T* newArray = new T[len];
        for (int i = from; i < to; i++)
            newArray[i - from] = array[i];
        return vetor<T>(len, newArray);
    }

    template<typename Predicate>
    T* where(Predicate fn) {
        for (int i = 0; i < asize; i++) {
            if (fn(array[i]))
                return &array[i];
        }
        return nullptr;
    }

    template<typename Predicate>
    vetor<T> whereRange(Predicate fn) {
        vetor<T> result;
        for (int i = 0; i < asize; i++) {
            if (fn(array[i]))
                result.push_back(array[i]);
        }
        return result;
    }

    T& at(int index) { return array[index]; }

    int max() {
        if constexpr (!std::is_arithmetic_v<T>) return NULL;

        if (!(size() > 1)) return NULL;

        int max = array[0];

        for (int i = 0; i < asize; i++) {
            if (array[i] > max) {
                max = array[i];
            }
        }

        return max;
    }

    int min() {
        if constexpr (!std::is_arithmetic_v<T>) return NULL;

        if (!(size() > 1)) return NULL;

        int min = array[0];

        for (int i = 0; i < asize; i++) {
            if (array[i] < min) {
                min = array[i];
            }
        }

        return min;
    }

    void swap(vetor<T>& other) {
        std::swap(array, other.array);
        std::swap(asize, other.asize);
        std::swap(capacity, other.capacity);
    }

    T* begin() {
        return array;
    }

    T* end() {
        return array + asize;
    }

    template<typename Predicate>
    T* removeWhere(Predicate fn) {
        for (int i = 0; i < asize; i++) {
            if (fn(i, &array[i])) {
                T* obj = &array[i];

                for (int j = i; j < asize - 1; j++) {
                    array[j] = array[j + 1];
                }

                asize--;
                return obj;
            }
        }
        return nullptr;
    }
    template<typename Predicate>
    void sort(Predicate fn) {
        for (int i = 0; i < asize - 1; i++) {
            for (int j = 0; j < asize - i - 1; j++) {
                if (fn(array[j + 1], array[j])) {
                    T temp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = temp;
                }
            }
        }
    }

};