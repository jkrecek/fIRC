#ifndef SINGLETON_H
#define SINGLETON_H

#include "firc_lib_global.h"

template <class T>
class Singleton
{
    public:
        static T& Instance()
        {
            static T instance_m;                    // create static instance of our class
            return instance_m;                      // return it
        }

    private:
        Singleton();                                // hide constructor
        ~Singleton();                               // hide destructor
        Singleton(const Singleton &);               // hide copy constructor
        Singleton& operator=(const Singleton &);    // hide assign op
};

#endif // SINGLETON_H
