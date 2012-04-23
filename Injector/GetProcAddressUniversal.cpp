// Custom GetProcAddress implementation. Needed because of Vista 
// doing potential IAT hooks and generally messing things up for the 
// rest of us. (AcLayers/ShimEngine)
FARPROC Injector::CustomGetProcAddress(HMODULE Module, const std::string& FunctionName)
{
	try
	{
		// Guard from structured exceptions
		SehGuard Guard;

		// Get pointer to DOS header
		PIMAGE_DOS_HEADER pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(Module);
		if (!pDosHeader || pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
			throw InjectorException("Injector: DOS PE header is invalid.");

		// Get pointer to NT header
		PIMAGE_NT_HEADERS pNtHeader = reinterpret_cast<PIMAGE_NT_HEADERS>
			(reinterpret_cast<PCHAR>(Module) + pDosHeader->e_lfanew);
		if (pNtHeader->Signature != IMAGE_NT_SIGNATURE)
			throw InjectorException("Injector: NT PE header is invalid.");

		// Get pointer to image export directory
		PVOID pExportDirTemp = reinterpret_cast<PBYTE>(Module) + 
			pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
		PIMAGE_EXPORT_DIRECTORY pExportDir = 
			reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(pExportDirTemp);

		// Get pointer to export names table
		PDWORD pNamesRvas = reinterpret_cast<PDWORD>(reinterpret_cast<PBYTE>(Module) 
			+ pExportDir->AddressOfNames);
		// Get pointer to export ordinal table
		PWORD pNameOrdinals = reinterpret_cast<PWORD>(reinterpret_cast<PBYTE>(Module) 
			+ pExportDir->AddressOfNameOrdinals);
		// Get pointer to export address table
		PDWORD pFunctionAddresses = reinterpret_cast<PDWORD>(reinterpret_cast<PBYTE>(Module) 
			+ pExportDir->AddressOfFunctions);

		// Walk the array of this module's function names 
		for (DWORD n = 0; n < pExportDir->NumberOfNames; n++) 
		{
			// Get the function name
			PSTR CurrentNameTemp = reinterpret_cast<PSTR>(reinterpret_cast<PBYTE>(Module) 
				+ pNamesRvas[n]);

			// Create string from c-style string
			// Note: Throws an access violation if not a valid string
			std::string CurrentName(CurrentNameTemp);

			// If not the specified function, try the next function	  
			if (CurrentName != FunctionName) continue;

			// We found the specified function
			// --> Get this function's Ordinal value
			WORD Ordinal = pNameOrdinals[n];

			// Get the address of this function's address
			return reinterpret_cast<FARPROC>(reinterpret_cast<PBYTE>(Module) 
				+ pFunctionAddresses[Ordinal]);
		}
	}
	// Catch custom SEH-proxy exceptions.
	catch (const SehException& e)
	{
		TDBGOUT(e);
	}
	// Catch Injector exceptions
	catch (const InjectorException& e)
	{
		CDBGOUT("InjectorException: " << e.what() << ". File: " << __FILE__ << 
			"Line: " << __LINE__ << ".");
	}

	// Nothing found, return zero
	return NULL;
}