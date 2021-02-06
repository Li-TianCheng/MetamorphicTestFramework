//
// Created by 76484 on 2021/1/31.
//

#ifndef AUTOPOINT_H
#define AUTOPOINT_H

#include <cstddef>

template<typename T>
class AutoPoint {
public:
    template<typename U>
    explicit AutoPoint(U* point = nullptr);
    AutoPoint(const AutoPoint<T>& autoPoint);
    AutoPoint(AutoPoint<T>&& autoPoint) noexcept ;
    AutoPoint<T>& operator=(const AutoPoint<T>& autoPoint);
    AutoPoint<T>& operator=(AutoPoint<T>&& autoPoint) noexcept ;
    T* operator ->() const;
    T& operator *() const;
    ~AutoPoint();
private:
    T* _point;
    size_t* _count;
};

template<typename T> template<typename U> inline
AutoPoint<T>::AutoPoint(U* point):_point(point), _count(new size_t(1)){}

template<typename T> inline
AutoPoint<T>::AutoPoint(const AutoPoint<T>& autoPoint):_point(autoPoint._point), _count(autoPoint._count) {
    (*_count)++;
}

template<typename T> inline
AutoPoint<T>::AutoPoint(AutoPoint<T> &&autoPoint) noexcept:_point(autoPoint._point), _count(autoPoint._count){
    autoPoint._point = nullptr;
    autoPoint._count = nullptr;
}

template<typename T> inline
AutoPoint<T>& AutoPoint<T>::operator=(const AutoPoint<T> &autoPoint) {
    if (autoPoint._point == _point){
        return *this;
    }
    this->~AutoPoint();
    _point = autoPoint._point;
    _count = autoPoint._count;
    (*_count)++;
    return *this;
}

template<typename T> inline
AutoPoint<T> &AutoPoint<T>::operator=(AutoPoint<T> &&autoPoint) noexcept {
    this->~AutoPoint();
    _point = autoPoint._point;
    _count = autoPoint._count;
    autoPoint._point = nullptr;
    autoPoint._count = nullptr;
    return *this;
}

template<typename T> inline
T* AutoPoint<T>::operator->() const {
    return _point;
}

template<typename T> inline
T &AutoPoint<T>::operator*() const {
    return *_point;
}

template<typename T> inline
AutoPoint<T>::~AutoPoint() {
    if (_count != nullptr){
        (*_count)--;
        if (*(_count) == 0){
            delete _point;
            delete _count;
        }
    }
}


#endif //AUTOPOINT_H
