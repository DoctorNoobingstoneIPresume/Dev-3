#include <Adder/_Fwd.hpp>

#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
 #include <windows.h>
#else
 #include <syslog.h>
#endif


namespace Adder
{


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void
AzzertionHasFailed_Msg
(const char *pszPathName, unsigned iLine, const char *pszFunc, const char *pszPrettyFunction, const char *pszCondition, const char *pszMsg)
{
	const char *pszFileName = pszPathName;
	{
		for (const char *pc = pszPathName; ; ++pc)
			if (const char c = *pc)
			{
				if (c == '/' || c == '\\')
					pszFileName = pc + 1;
			}
			else
				break;
	}
	
	char acBuffer [8192];
	{
		std::snprintf
		(
			acBuffer,
			nelems (acBuffer),
			"%s:%u in function \'%s\': AzzertionHasFailed_Msg: condition \"%s\", message \"%s\" ! Longer version: %s:%u in function \'%s\' !",
			pszFileName, iLine, pszFunc,
			pszCondition, pszMsg,
			pszPathName, iLine, pszPrettyFunction
		);
	}
	
	#ifdef _WIN32
		OutputDebugStringA (acBuffer);
	#else
		syslog (LOG_ERR, "%s", acBuffer);
	#endif
	
	// [2020-08-29] TODO: Maybe for Borland_560 (stdout): "using namespace std;".
	std::printf ("%s\n", acBuffer);
	std::fflush (stdout);
	
	std::abort ();
}

void
AzzertionHasFailed
(const char *pszPathName, unsigned iLine, const char *pszFunc, const char *pszPrettyFunction, const char *pszCondition)
{
	return AzzertionHasFailed_Msg (pszPathName, iLine, pszFunc, pszPrettyFunction, pszCondition, "???");
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void _Fwd_f ()
{
	ADDER_AZZERT (0);
}



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=




//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


}
