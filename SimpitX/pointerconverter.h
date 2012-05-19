// -----------------------
// -- Pointer Converter --
// -----------------------
union PointerConverter
{
	// Construct / Destruct
	PointerConverter() { ptrInt64 = 0; };
	PointerConverter(UINT64 val) { ptrInt64 = val; };
	PointerConverter(void * val) { ptrInt64 = 0; ptr = val; };

	// Variables
	void	*ptr;
	int		ptrInt;
	UINT64	ptrInt64;
	DWORD	ptrDword;
	SHORT	ptrShort;
	LONG	ptrLong;

	// Functions
	int		ToInt() { return ptrInt; };
	UINT64	ToInt64() { return ptrInt64; };
	void *	ToPtr() { return ptr; };

	// Operators
	void operator = (UINT64 val) { ptrInt64 = val; };
	void operator = (void * val) { ptrInt64 = 0; ptr = val; };
	PointerConverter operator - (const PointerConverter &val) { return ptrInt64 - val.ptrInt64; };
	PointerConverter operator + (const PointerConverter &val) { return ptrInt64 + val.ptrInt64; };
	PointerConverter operator -= (const PointerConverter &val) { ptrInt64 -= val.ptrInt64; return *this; };
	PointerConverter operator += (const PointerConverter &val) { ptrInt64 += val.ptrInt64; return *this; };
};