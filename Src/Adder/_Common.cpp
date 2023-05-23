#include <Adder/_Common.hpp>


namespace Adder
{


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#if 0
std::ios_base &QHexBase (std::ios_base &base)
{
	base.setf
	(
		std::ios_base::right       | std::ios_base::hex       | std::ios_base::uppercase,
		std::ios_base::adjustfield | std::ios_base::basefield | std::ios_base::uppercase
	);
	
	return base;
}

std::ios_base &QDecBase (std::ios_base &base)
{
	base.setf
	(
		std::ios_base::right       | std::ios_base::dec,
		std::ios_base::adjustfield | std::ios_base::basefield
	);
	
	return base;
}

std::ios_base &QTxtBase (std::ios_base &base)
{
	base.setf
	(
		std::ios_base::left,
		std::ios_base::adjustfield
	);
	
	return base;
}
#endif


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


}
