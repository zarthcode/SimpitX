#define WIN32_LEAN_AND_MEAN

#ifndef _HOOKMANAGER_H
#define _HOOKMANAGER_H

#include <string>
#include <map>
#include <Windows.h>
#include "misc.h"

class D3DHookMgr
{

public:

	/// Return an instance of the HookManager singleton.
	static D3DHookMgr* GetSingleton();

	/// Returns a pointer to requested function
	template<typename F>
	F GetDetour(std::string FunctionName)
	{
		std::map<std::string, void* const>::iterator itTargetFunction = m_mDetourFunctions.find(FunctionName);
		if (itTargetFunction == m_mDetourFunctions.end())
		{
			// Failed to find the function being enabled
			ostr << LOG_INFO() << ("Failed to locate function: ", FunctionName.c_str());

		}

		F functPtr = reinterpret_cast<F>(itTargetFunction->second);
		return functPtr;
	}

	/// Returns the original function
	template<typename F>
	F GetFunction(std::string FunctionName)
	{
		std::map<std::string, void*>::iterator itTargetFunction = m_mTrampolineFunctions.find(FunctionName);
		if (itTargetFunction == m_mTrampolineFunctions.end())
		{
			// Failed to find the function being enabled
			throw std::logic_error(throw_log("D3DHookMgr::GetFunction(\"%s\") Failed to locate function.", FunctionName.c_str()));

		}

		F functPtr = reinterpret_cast<F>(itTargetFunction->second);
		return functPtr;
	}

	// Initializes the component
	void Initialize();

	// Performs D3D9 Hooking
	void HookD3D9();

	//************************************
	// Method:    Create
	// FullName:  HookManager::Create
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: void * pTargetFunction - A pointer to the target function, which will be overridden by the detour function.
	// Parameter: void * const pDetourFunction - A pointer to the detour function, which will override the target function.
	// Parameter: void * * ppOriginalFunction - A pointer to the trampoline function, which will be used to call the original target function.
	//************************************
	void Create(const std::string pTargetFunction, void* const pDetourFunction, void* &pOriginalFunction);

	// convenience function.
	void Create(const std::string pTargetFunction, void* const pDetourFunction);

	// Enable/disable a specific hook.
	void Enable(std::string TargetFunctionName, bool bState = true);

	// Disables all hooks.
	void DisableAll();

private:

	// Private Constructor
	D3DHookMgr();

	// Private Destructor
	virtual ~D3DHookMgr();

	// Instance
	static D3DHookMgr* m_pHookManager;

	// Initialization flag
	bool m_bInitialized;

	// D3D9 module.
	HMODULE m_hModule;

	// Detour functions
	std::map<std::string, void* const> m_mDetourFunctions;

	// Original functions
	std::map<std::string, void*> m_mTrampolineFunctions;


};

#endif