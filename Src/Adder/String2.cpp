#include <Adder/String2.hpp>


namespace Adder
{


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

unsigned
Base36Digit_To_Value
(unsigned c)
{
	const byte ff = 0xFF;
	
	static
	const byte a [0x7B - 0x30] =
	{
	//	0   1   2   3   4   5   6   7   8   9
		 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
		
	//	:   ;   <   =   >   ?
		ff, ff, ff, ff, ff, ff,
		
	//      @
		ff,
		
	//	A   B   C   D   E   F   G   H   I   J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
	
	//	[   \   ]   ^   _
		ff, ff, ff, ff, ff,
	
	
	//	`
		ff,
	
	//	a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p   q   r   s   t   u   v   w   x   y   z
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
	
	//	{   |   }   ~   DEL
	//	ff, ff, ff, ff, ff
	};
	
	c -= 0x30;
	if (c >= nelems (a))
		return byte (-1);
	else
		return a [c];
}


unsigned
HexDigit_To_Value
(unsigned c)
{
	//if      (c >= BTEXT ('0') && c <= BTEXT ('9'))  return byte (c - BTEXT ('0')     );
	//else if (c >= BTEXT ('A') && c <= BTEXT ('F'))  return byte (c - BTEXT ('A') + 10);
	//else if (c >= BTEXT ('a') && c <= BTEXT ('f'))  return byte (c - BTEXT ('a') + 10);
	//else                                            return -1;
	
	const unsigned bValue = Base36Digit_To_Value (c);
	return bValue < 16 ? bValue : byte (-1);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


} // end namespace
