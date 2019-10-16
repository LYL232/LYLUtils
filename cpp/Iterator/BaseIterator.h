#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifndef LYL_UTILS_BASE_ITERATOR_H

#include<exception>
#include <cstddef>

class IteratorException : std::exception {
public:
    enum TypeCode {
        OutOfBound = 0,
        InvalidIndex,
    };

    explicit IteratorException(TypeCode _code) {
        code = _code;
    }

private:
    size_t code;
};

template<class T>
class BaseIterator {
    //C++循环语句三件套: for(auto i: object) {} 需要在主类中实现
   // Iterator begin() const 和 Iterator end()const 方法

    inline T operator*() const {
        return get();
    }

    virtual inline bool operator!=(const BaseIterator &other) const =0;

    virtual inline const BaseIterator &operator++() =0;

    // 理论上更快的循环组合 for(auto i = object.begin(); i.has_next(); i.t0_next()) {}
    virtual inline void to_next()=0;

    virtual inline void to_previous()=0;

    virtual inline T get() const =0;

    virtual inline bool has_next() const =0;

    virtual inline bool has_previous() const =0;
};

#define LYL_UTILS_BASE_ITERATOR_H

#endif //LYL_UTILS_BASE_ITERATOR_H

#pragma clang diagnostic pop