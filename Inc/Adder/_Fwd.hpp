#ifndef __Dev_3__Fwd_hpp
#define __Dev_3__Fwd_hpp 1


#include "_ForceInclude/_Config.hpp"

#include <stdint.h>


namespace Adder
{


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

namespace StdInt
{
	// [2020-08-29] TODO: CStdInt.hpp from Dev-2.2 !
	using ::uint8_t; using ::uint16_t; using ::uint32_t; using ::uint64_t; using ::uintptr_t;
	using :: int8_t; using :: int16_t; using :: int32_t; using :: int64_t; using :: intptr_t;
}

using namespace StdInt;


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// nelems:
// 
// Given an array, this macro returns the number of elements in the array.
// 
// [2012-10-21]
//   According to good advice from Matthew Wilson's book ("Imperfect C++"),
//   we are modifying the implementation of this macro.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//#define nelems(x) (sizeof (x) / sizeof ((x) [0]))
#define nelems(x) (sizeof (x) / sizeof (0 [x]))


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void
AzzertionHasFailed_Msg
(const char *pszPathName, unsigned iLine, const char *pszFunc, const char *pszPrettyFunction, const char *pszCondition, const char *pszMsg);

void
AzzertionHasFailed
(const char *pszPathName, unsigned iLine, const char *pszFunc, const char *pszPrettyFunction, const char *pszCondition);

#define ADDER_AZZERT_MSG(bCondition, pszMsg) \
    do \
    { \
        if (! (bCondition)) \
            Adder::AzzertionHasFailed_Msg (__FILE__, __LINE__, __func__, __PRETTY_FUNCTION__, #bCondition, pszMsg); \
    } while (0)

#define ADDER_AZZERT(bCondition) \
    do \
    { \
        if (! (bCondition)) \
            Adder::AzzertionHasFailed     (__FILE__, __LINE__, __func__, __PRETTY_FUNCTION__, #bCondition        ); \
    } while (0)


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


}


#endif
