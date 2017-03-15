//
// Created by ukhegg on 15.03.2017.
//

#ifndef DATAGEN_LIB_DATAGEN_HPP
#define DATAGEN_LIB_DATAGEN_HPP

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
#define DATAGEN_HELPER_DLL_IMPORT __declspec(dllimport)
#define DATAGEN_HELPER_DLL_EXPORT __declspec(dllexport)
#define DATAGEN_HELPER_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define DATAGEN_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#define DATAGEN_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#define DATAGEN_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define DATAGEN_HELPER_DLL_IMPORT
#define DATAGEN_HELPER_DLL_EXPORT
#define DATAGEN_HELPER_DLL_LOCAL
#endif
#endif

// Now we use the generic helper definitions above to define DATAGEN_API and DATAGEN_LOCAL.
// DATAGEN_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// DATAGEN_LOCAL is used for non-api symbols.

#ifdef DATAGEN_DLL // defined if DATAGEN is compiled as a DLL
#ifdef DATAGEN_DLL_EXPORTS // defined if we are building the DATAGEN DLL (instead of using it)
    #define DATAGEN_API DATAGEN_HELPER_DLL_EXPORT
  #else
    #define DATAGEN_API DATAGEN_HELPER_DLL_IMPORT
  #endif // DATAGEN_DLL_EXPORTS
  #define DATAGEN_LOCAL DATAGEN_HELPER_DLL_LOCAL
#else // DATAGEN_DLL is not defined: this means DATAGEN is a static lib.
#define DATAGEN_API
#define DATAGEN_LOCAL
#endif // DATAGEN_DLL

#endif //DATAGEN_LIB_DATAGEN_HPP
