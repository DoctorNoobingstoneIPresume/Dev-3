#ifndef __Dev_3_Windows_HandleTraits_HANDLE_CloseHandle_hpp
#define __Dev_3_Windows_HandleTraits_HANDLE_CloseHandle_hpp 1


#include <Adder/_Fwd.hpp>

#include <cstdlib>

#include <windows.h>


namespace Adder
{


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

class HANDLE_CloseHandle
{
 protected:
	~HANDLE_CloseHandle ()
	{}

 public:
	typedef HANDLE HandleType;
	
	BOOST_CONSTEXPR static bool IsValid (HandleType h) BOOST_NOEXCEPT
	{
		return h != INVALID_HANDLE_VALUE;
	}
	
	BOOST_CONSTEXPR static HandleType GetAnInvalidValue () BOOST_NOEXCEPT
	{
		return INVALID_HANDLE_VALUE;
	}
	
	static void Close (HandleType h) BOOST_NOEXCEPT
	{
		const BOOL bResult = CloseHandle (h);
		ADDER_AZZERT (bResult);
	}
};


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


}


#endif
