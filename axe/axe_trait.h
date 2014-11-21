//-----------------------------------------------------------------------------
//  Copyright (C) 2011-2012, GB Research, LLC (www.gbresearch.com)
//  
//  Boost Software License - Version 1.0 - August 17th, 2003
//
//  Permission is hereby granted, free of charge, to any person or organization
//  obtaining a copy of the software and accompanying documentation covered by
//  this license (the "Software") to use, reproduce, display, distribute,
//  execute, and transmit the Software, and to prepare derivative works of the
//  Software, and to permit third-parties to whom the Software is furnished to
//  do so, all subject to the following:
//
//  The copyright notices in the Software and this entire statement, including
//  the above license grant, this restriction and the following disclaimer,
//  must be included in all copies of the Software, in whole or in part, and
//  all derivative works of the Software, unless such copies or derivative
//  works are solely in the form of machine-executable object code generated by
//  a source language processor.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
//  FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef AXE_TRAIT_H
#define AXE_TRAIT_H

#include <type_traits>
#include <vector>
#include "axe_result.h"

namespace axe {

#ifdef _MSC_VER
#if _MSC_VER >= 1600
    #define AXE_USE_CUSTOM_TRAITS
#else
    #error Visual C++ 2010 or later required
#endif
#else
    #define AXE_USE_LIT_TRAITS
#endif

#ifdef AXE_USE_CUSTOM_TRAITS

    #define AXE_IS_RULE(...)\
        axe::is_rule<typename std::decay<__VA_ARGS__>::type>::value

    #define AXE_IS_EXTRACTOR(...)\
        axe::is_extractor<typename std::decay<__VA_ARGS__>::type>::value

    #define AXE_IS_PREDICATE(...)\
        axe::is_predicate<typename std::decay<__VA_ARGS__>::type>::value

    #define AXE_RULE
    #define AXE_EXTRACTOR
    #define AXE_PREDICATE

    //-----------------------------
    template<class R>
    class is_rule
    {
        struct yes { char c; };
        struct no { yes y[2]; };

        template<class T, class I, result<I> (T::*)(I,I)>       struct sfinae {};
        template<class T, class I, result<I> (T::*)(I,I) const> struct const_sfinae {};

        template<class T>
        static yes test(T*, sfinae<T, const char*, &T::operator()>* = 0);

        template<class T>
        static yes test(T*, const_sfinae<T, const char*, &T::operator()>* = 0);

        template<class T>
        static no test(...);
    public:
        static const bool value = sizeof(yes) == sizeof(test<R>(0));
    };

    //-----------------------------
    template<class E>
    class is_extractor
    {
        struct yes { char c; };
        struct no { yes y[2]; };

        template<class T, class I, void (T::*)(I,I)>          struct sfinae {};
        template<class T, class I, void (T::*)(I,I) const>    struct const_sfinae {};

        template<class T>
        static yes test(T*, sfinae<T, const char*, &T::operator()>* = 0);

        template<class T>
        static yes test(T*, const_sfinae<T, const char*, &T::operator()>* = 0);

        template<class T>
        static no test(...);
    public:
        static const bool value = sizeof(yes) == sizeof(test<E>(0));
    };

    //-----------------------------
    template<class E>
    class is_predicate
    {
        struct yes { char c; };
        struct no { yes y[2]; };

        template<class T, class CharT, bool (T::*)(CharT)>          struct sfinae {};
        template<class T, class CharT, bool (T::*)(CharT) const>    struct const_sfinae {};

        template<class T>
        static yes test(T*, const_sfinae<T, char, &T::operator()>* = 0);

        template<class T>
        static no test(...);
    public:
        static const bool value = sizeof(yes) == sizeof(test<E>(0));
    };
#else
    //-----------------------------
    // base classes for rules, extractors, predicates
    //-----------------------------
    class r_base {};
    class e_base {};
    class p_base {};
    //-----------------------------
    #define AXE_IS_RULE(...)\
        std::is_base_of<axe::r_base, typename std::remove_reference<__VA_ARGS__>::type>::value

    #define AXE_IS_EXTRACTOR(...)\
        std::is_base_of<axe::e_base, typename std::remove_reference<__VA_ARGS__>::type>::value

    #define AXE_IS_PREDICATE(...)\
        std::is_base_of<axe::p_base, typename std::remove_reference<__VA_ARGS__>::type>::value

    #define AXE_RULE : axe::r_base
    #define AXE_EXTRACTOR : axe::e_base
    #define AXE_PREDICATE : axe::p_base

#endif

#define AXE_ASSERT_RULE(...)\
    static_assert(AXE_IS_RULE(__VA_ARGS__), "type '" #__VA_ARGS__ "' is not a rule");
#define AXE_ASSERT_NOT_RULE(...)\
    static_assert(!AXE_IS_RULE(__VA_ARGS__), "type '" #__VA_ARGS__ "' is a rule");

    //-----------------------------
    template<class R>
    class has_name
    {
        struct yes { char c; };
        struct no { yes y[2]; };

        template<class T, class Ret, Ret (T::*)() const> struct const_sfinae {};

        template<class T>
        static yes test(T* t, const_sfinae<T, std::string, &T::name>* = 0);
        template<class T>
        static yes test(T* t, const_sfinae<T, const std::string&, &T::name>* = 0);
        template<class T>
        static yes test(T* t, const_sfinae<T, const char*, &T::name>* = 0);

        template<class T>
        static no test(...);
    public:
        static const bool value = sizeof(yes) == sizeof(test<R>(0));
    };

}

#endif