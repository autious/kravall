#ifndef SRC_CORE_COMPONENTFRAMEWORK_CHECKTEMPLATEPRESENCE_H
#define SRC_CORE_COMPONENTFRAMEWORK_CHECKTEMPLATEPRESENCE_H

namespace Core
{
    //**********************************************************************
    
    /*!
        This template hack is for checking if a component
        given in an function exists for the handler.
    */
    template <class... R>
    struct Match;

    template <class T, class U>
    struct Match<T,U>
    {
        static const bool exists = false;
    };

    template <class T>
    struct Match<T,T>
    {
        static const bool exists = true;
    };

    template <class T, class... R> 
    struct Match<T,T,R...>
    {
        static const bool exists = true;
    };

    template <class T, class U, class... R> 
    struct Match<T,U,R...>
    {
        static const bool exists = Match<T,R...>::exists;
    };

    //**********************************************************************
}

#endif
