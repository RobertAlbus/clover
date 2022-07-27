#pragma once
#include <functional>
#include <stdio.h>


// https://stackoverflow.com/questions/2795023/c-template-typedef
template <class __T>
using List_ForEachFn = std::function<__T>; 
// want to return an iterator or something

template <class __T, int __count>
class Container
{
public:
    Container();

    void operator<< (__T* objectPointer);
    void pushBack(__T* objectPointer);

    __T* operator[] (int index);
    __T* getAt(int index);

    void forEach(List_ForEachFn<__T> fn);

    int cap();
    int size();

private:
    __T* _array[__count];
    int _nextAvailable;
    int _capacity;

};

template <class __T, int __count>
Container<__T, __count>::Container() : 
    _capacity(__count),
    _nextAvailable(0),
    _array {}
{

}

template <class __T, int __count>
void Container<__T, __count>::operator<<(__T* objectPointer)
{
    pushBack(objectPointer);
}

template <class __T, int __count>
void Container<__T, __count>::pushBack(__T* objectPointer)
{
    bool hasCapacity = size() < cap();
    if (!hasCapacity) throw;
    _array[_nextAvailable++] = objectPointer;
}

template <class __T, int __count>
__T* Container<__T, __count>::operator[](int index)
{
    return getAt(index);
}

template <class __T, int __count>
__T* Container<__T, __count>::getAt(int index)
{
    if (index >= size()) throw;
    return _array[index];
}

template <class __T, int __count>
int Container<__T, __count>::cap()
{
    return _capacity;
}

template <class __T, int __count>
int Container<__T, __count>::size()
{
    return _nextAvailable;
}

template <class __T, int __count>
void Container<__T, __count>::forEach(List_ForEachFn<__T> fn)
{
    for (int i = 0, end = size(); i < end; i++)
    {
        fn(_array[i]);
    }
}
