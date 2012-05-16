#include "HookManager.h"

#include "MinHook.h"
#include "misc.h"
#include "d3d9.h"

void D3DHookMgr::Initialize()
{

	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
	{
		
		throw std::exception(throw_log("D3DHookMgr::Initialize - Failed.").c_str());
	}

	m_bInitialized = true;
	
}

void D3DHookMgr::HookD3D9()
{

	if(!m_bInitialized)
	{
		// Logic Error
		throw std::logic_error(throw_log("D3DHookMgr::HookD3D9 - Initialize not called."));
	}

	char sysd3d[320];
	GetSystemDirectory(sysd3d, 320);
	strcat_s(sysd3d, "\\d3d9.dll");

	add_log("LoadLibrary(\"%s\")", sysd3d);
	m_hModule = LoadLibrary(sysd3d);	
	add_log("\t...result 0x%x", m_hModule);


	// Create a hook for Direct3DCreate9, in disabled state.
	Create("Direct3DCreate9", hkDirect3DCreate9);
	Enable("Direct3DCreate9");

}

void D3DHookMgr::Create( const std::string TargetFunctionName, void* const pDetourFunction, void* &pOriginalFunction )
{
	if (!m_bInitialized)
	{
		throw std::logic_error(throw_log("D3DHookMgr::Create(", TargetFunctionName, ") - Initialize not called."));
	}


	add_log("Creating hook for function: %s", TargetFunctionName.c_str());



	void* outOriginalFunction = NULL;
	if (MH_CreateHook(GetProcAddress(m_hModule, TargetFunctionName.c_str()), pDetourFunction, &outOriginalFunction) != MH_OK)
	{
		throw std::logic_error(throw_log("\tMinHook Create failed."));
	}
	else
	{
		add_log("Hook created successfully.");
	}

	pOriginalFunction = outOriginalFunction;

	// Store Function Info for later retrieval
	if(m_mDetourFunctions.find(TargetFunctionName) != m_mDetourFunctions.end())
	{
		// Remove
		m_mDetourFunctions.erase(TargetFunctionName);
		m_mTrampolineFunctions.erase(TargetFunctionName);
	}

	m_mDetourFunctions.insert(std::make_pair<std::string, void* const>(TargetFunctionName, pDetourFunction));
	m_mTrampolineFunctions.insert(std::make_pair<std::string, void*>(TargetFunctionName, outOriginalFunction));
	add_log("Creating target '%s', Detour: 0x%x, Trampoline: 0x%x", TargetFunctionName.c_str(), pDetourFunction, outOriginalFunction);
	add_log("\tHook registered.");
}

void D3DHookMgr::Create( const std::string TargetFunctionName, void* const pDetourFunction)
{
	void* pOriginalFunction = NULL;
	Create(TargetFunctionName, pDetourFunction, pOriginalFunction);
	add_log("Discarded original function pointer for %s, 0x%x", TargetFunctionName.c_str(), pOriginalFunction);
}

void D3DHookMgr::Enable( std::string TargetFunctionName, bool bState /*= true*/ )
{
	if (!m_bInitialized)
	{
		throw std::logic_error(throw_log("Initialize not called."));
	
	}

	add_log("Initializing %s", TargetFunctionName.c_str());

	std::map<std::string, void* const>::iterator itTargetFunction = m_mDetourFunctions.find(TargetFunctionName);
	if (itTargetFunction == m_mDetourFunctions.end())
	{
		// Failed to find the function being enabled
		throw std::logic_error(throw_log("Failed to locate function: ", TargetFunctionName.c_str()));
		
	}
	
	if(MH_EnableHook(GetProcAddress(m_hModule, TargetFunctionName.c_str())) != MH_OK)
	{
		throw std::logic_error(throw_log("MinHook enable (0x%x) failed.", itTargetFunction->second));
		
	}

	add_log("Hook for ", TargetFunctionName, "Enabled");

}

void D3DHookMgr::DisableAll()
{
	std::map<std::string, void* const>::iterator itTargetFunction;
	for (itTargetFunction = m_mDetourFunctions.begin(); itTargetFunction != m_mDetourFunctions.end(); itTargetFunction++)
	{
		MH_DisableHook(itTargetFunction->second);
	}

}

D3DHookMgr* D3DHookMgr::m_pHookManager;

D3DHookMgr* D3DHookMgr::GetSingleton()
{
	if (!m_pHookManager)
	{
		m_pHookManager = new D3DHookMgr();
		m_pHookManager->Initialize();
	}

	return m_pHookManager;
}

D3DHookMgr::D3DHookMgr():m_hModule(NULL), m_bInitialized(false)
{

}

D3DHookMgr::~D3DHookMgr()
{

}



