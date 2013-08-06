#ifndef _RISKEMULIBRARY_SINGLETON_H_
#define _RISKEMULIBRARY_SINGLETON_H_

#include "stdtypes.h"

#define MAKE_SINGLETON(CLASS) friend class Singleton<CLASS>

template<class T> class Singleton {
public: 
    inline static T& GetInstance();
    inline static T* GetInstance_ptr();
    inline static void DestroyInstance();
protected:
    inline Singleton() {
        Singleton::ms_Instance = static_cast<T*>(this); 
    }

    inline ~Singleton() {
        Singleton::ms_Instance = NULL; 
    }
private:
    static T* ms_Instance; 
    inline Singleton(Singleton const&) {}
    inline Singleton& operator=(Singleton const&) { return *this; }
};

template<class T> T& Singleton<T>::GetInstance() {
    if ( Singleton::ms_Instance == NULL ) {
        Singleton::ms_Instance = new T;
    }
    return *(Singleton::ms_Instance);
}

template<class T> T* Singleton<T>::GetInstance_ptr() {
    if ( Singleton::ms_Instance == NULL ) {
        Singleton::ms_Instance = new T;
    }
    return Singleton::ms_Instance;
}

template<class T> void Singleton<T>::DestroyInstance() {
    if ( Singleton::ms_Instance != NULL ) {
        delete Singleton::ms_Instance;
        Singleton::ms_Instance = NULL;
    }
}

template<class T> T* Singleton<T>::ms_Instance = NULL;

#endif //_RISKEMULIBRARY_SINGLETON_H_