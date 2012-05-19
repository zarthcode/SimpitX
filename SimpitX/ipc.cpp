#include "StdAfx.h"
#include "helper.h"
#include "ipc.h"

IPC::Server::Server(void)
{
	// Set default params
	m_hMapFile = 0;
	m_hSignal = 0;
	m_hAvail = 0;
	m_pBuf = NULL;
	m_sAddr = NULL;

	// create the server
	create();
};

IPC::Server::~Server(void)
{
	// Free memory
	if (m_sAddr) {
		free(m_sAddr);
		m_sAddr = NULL;
	}

	// Close the server
	close();
};

void IPC::Server::create(void)
{
	// Determine the name of the memory
	DWORD ProcessID = GetCurrentProcessId();
	DWORD ThreadID = GetCurrentThreadId();
	DWORD ServerID = IPC::GetID();

	m_sAddr = (char*)malloc(IPC_MAX_ADDR);
	if (!m_sAddr) return;
	sprintf_s(m_sAddr, IPC_MAX_ADDR, "IPC_%04u_%04u_%04u", ProcessID, ThreadID, ServerID);

	char *m_sEvtAvail = (char*)malloc(IPC_MAX_ADDR);
	if (!m_sEvtAvail) { printf("server_create: failed: %01d\n", __LINE__); return; }
	sprintf_s(m_sEvtAvail, IPC_MAX_ADDR, "%s_evt_avail", m_sAddr);

	char *m_sEvtFilled = (char*)malloc(IPC_MAX_ADDR);
	if (!m_sEvtFilled) { free(m_sEvtAvail); printf("server_create: failed: %01d\n", __LINE__); return; }
	sprintf_s(m_sEvtFilled, IPC_MAX_ADDR, "%s_evt_filled", m_sAddr);

	char *m_sMemName = (char*)malloc(IPC_MAX_ADDR);
	if (!m_sMemName) { free(m_sEvtAvail); free(m_sEvtFilled); printf("server_create: failed: %01d\n", __LINE__); return; }
	sprintf_s(m_sMemName, IPC_MAX_ADDR, "%s_mem", m_sAddr);

	// Create the events
	m_hSignal = CreateEventA(NULL, FALSE, FALSE, (LPCSTR)m_sEvtFilled);
	if (m_hSignal == NULL || m_hSignal == INVALID_HANDLE_VALUE) { free(m_sEvtAvail); free(m_sEvtFilled); free(m_sMemName); printf("server_create: failed: %01d\n", __LINE__); return; }
	m_hAvail = CreateEventA(NULL, FALSE, FALSE, (LPCSTR)m_sEvtAvail);
	if (m_hAvail == NULL || m_hSignal == INVALID_HANDLE_VALUE) { free(m_sEvtAvail); free(m_sEvtFilled); free(m_sMemName); printf("server_create: failed: %01d\n", __LINE__); return; }

	// Create the file mapping
	m_hMapFile = CreateFileMapping(	INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(MemBuff),
		(LPCWSTR)m_sMemName);
	if (m_hMapFile == NULL || m_hMapFile == INVALID_HANDLE_VALUE)  { free(m_sEvtAvail); free(m_sEvtFilled); free(m_sMemName); printf("server_create: failed: %01d\n", __LINE__); return; }

	// Map to the file
	m_pBuf = (MemBuff*)MapViewOfFile(	m_hMapFile,				// handle to map object
		FILE_MAP_ALL_ACCESS,	// read/write permission
		0,
		0,
		sizeof(MemBuff)); 
	if (m_pBuf == NULL) { free(m_sEvtAvail); free(m_sEvtFilled); free(m_sMemName); printf("server_create: failed: %01d\n", __LINE__); return; }

	// Clear the buffer
	ZeroMemory(m_pBuf, sizeof(MemBuff));

	// Create a circular linked list
	int N = 1;
	m_pBuf->m_Blocks[0].Next = 1;
	m_pBuf->m_Blocks[0].Prev = (IPC_BLOCK_COUNT-1);
	for (;N < IPC_BLOCK_COUNT-1; N++)
	{
		// Add this block into the available list
		m_pBuf->m_Blocks[N].Next = (N+1);
		m_pBuf->m_Blocks[N].Prev = (N-1);
	}
	m_pBuf->m_Blocks[N].Next = 0;
	m_pBuf->m_Blocks[N].Prev = (IPC_BLOCK_COUNT-2);

	// Initialize the pointers
	m_pBuf->m_ReadEnd = 0;
	m_pBuf->m_ReadStart = 0;
	m_pBuf->m_WriteEnd = 1;
	m_pBuf->m_WriteStart = 1;

	// Release memory
	free(m_sEvtAvail);
	free(m_sEvtFilled);
	free(m_sMemName);
};

void IPC::Server::close(void)
{
	// Close the event
	if (m_hSignal) {
		HANDLE handle = m_hSignal;
		m_hSignal = NULL;
		CloseHandle(handle);
	}

	// Close the event
	if (m_hAvail) {
		HANDLE handle = m_hAvail;
		m_hAvail = NULL;
		CloseHandle(handle);
	}

	// Unmap the memory
	if (m_pBuf) {
		MemBuff *pBuff = m_pBuf;
		m_pBuf = NULL;
		UnmapViewOfFile(pBuff);
	}

	// Close the file handle
	if (m_hMapFile) {
		HANDLE handle = m_hMapFile;
		m_hMapFile = NULL;
		CloseHandle(handle);
	}
};

IPC::Block* IPC::Server::getBlock(DWORD dwTimeout)
{
	// Grab another block to read from
	// Enter a continous loop (this is to make sure the operation is atomic)
	for (;;)
	{
		// Check if there is room to expand the read start cursor
		LONG blockIndex = m_pBuf->m_ReadStart;
		Block *pBlock = m_pBuf->m_Blocks + blockIndex;
		if (pBlock->Next == m_pBuf->m_WriteEnd)
		{
			// No room is available, wait for room to become available
			if (WaitForSingleObject(m_hSignal, dwTimeout) == WAIT_OBJECT_0)
				continue;

			// Timeout
			return NULL;
		}

		// Make sure the operation is atomic
		if (InterlockedCompareExchange(&m_pBuf->m_ReadStart, pBlock->Next, blockIndex) == blockIndex)
			return pBlock;

		// The operation was interrupted by another thread.
		// The other thread has already stolen this block, try again
		continue;
	}
};

void IPC::Server::retBlock(IPC::Block* pBlock)
{
	// Set the done flag for this block
	pBlock->doneRead = 1;

	// Move the read pointer as far forward as we can
	for (;;)
	{
		// Try and get the right to move the poitner
		DWORD blockIndex = m_pBuf->m_ReadEnd;
		pBlock = m_pBuf->m_Blocks + blockIndex;
		if (InterlockedCompareExchange(&pBlock->doneRead, 0, 1) != 1)
		{
			// If we get here then another thread has already moved the pointer
			// for us or we have reached as far as we can possible move the pointer
			return;
		}

		// Move the pointer one forward (interlock protected)
		InterlockedCompareExchange(&m_pBuf->m_ReadEnd, pBlock->Next, blockIndex);

		// Signal availability of more data but only if a thread is waiting
		if (pBlock->Prev == m_pBuf->m_WriteStart)
			SetEvent(m_hAvail);
	}
};

DWORD IPC::Server::read(void *pBuff, DWORD buffSize, DWORD dwTimeout)
{
	// Grab a block
	Block *pBlock = getBlock(dwTimeout);
	if (!pBlock) return 0;

	// Copy the data
	DWORD dwAmount = min(pBlock->Amount, buffSize);
	memcpy(pBuff, pBlock->Data, dwAmount);

	// Return the block
	retBlock(pBlock);

	// Success
	return dwAmount;
};

IPC::Client::Client(void)
{
	// Set default params
	m_hMapFile = 0;
	m_hSignal = 0;
	m_hAvail = 0;
	m_pBuf = NULL;
};

IPC::Client::Client(char *connectAddr)
{
	// Set default params
	m_hMapFile = 0;
	m_hSignal = 0;
	m_hAvail = 0;
	m_pBuf = NULL;

	// Determine the name of the memory
	m_sAddr = (char*)malloc(IPC_MAX_ADDR);
	if (!m_sAddr) return;
	strcpy_s(m_sAddr, IPC_MAX_ADDR, connectAddr);

	char *m_sEvtAvail = (char*)malloc(IPC_MAX_ADDR);
	if (!m_sEvtAvail) return;
	sprintf_s(m_sEvtAvail, IPC_MAX_ADDR, "%s_evt_avail", m_sAddr);

	char *m_sEvtFilled = (char*)malloc(IPC_MAX_ADDR);
	if (!m_sEvtFilled) { free(m_sEvtAvail); return; }
	sprintf_s(m_sEvtFilled, IPC_MAX_ADDR, "%s_evt_filled", m_sAddr);

	char *m_sMemName = (char*)malloc(IPC_MAX_ADDR);
	if (!m_sMemName) { free(m_sEvtAvail); free(m_sEvtFilled); return; }
	sprintf_s(m_sMemName, IPC_MAX_ADDR, "%s_mem", m_sAddr);

	// Create the events
	m_hSignal = CreateEventA(NULL, FALSE, FALSE, (LPCSTR)m_sEvtFilled);
	if (m_hSignal == NULL || m_hSignal == INVALID_HANDLE_VALUE) { free(m_sEvtAvail); free(m_sEvtFilled); free(m_sMemName); return; }
	m_hAvail = CreateEventA(NULL, FALSE, FALSE, (LPCSTR)m_sEvtAvail);
	if (m_hAvail == NULL || m_hSignal == INVALID_HANDLE_VALUE) { free(m_sEvtAvail); free(m_sEvtFilled); free(m_sMemName); return; }

	// Open the shared file
	m_hMapFile = OpenFileMapping(	FILE_MAP_ALL_ACCESS,	// read/write access
		FALSE,					// do not inherit the name
		(LPCWSTR)m_sMemName);	// name of mapping object
	if (m_hMapFile == NULL || m_hMapFile == INVALID_HANDLE_VALUE)  { free(m_sEvtAvail); free(m_sEvtFilled); free(m_sMemName); return; }


	// Map to the file
	m_pBuf = (MemBuff*)MapViewOfFile(	m_hMapFile,				// handle to map object
		FILE_MAP_ALL_ACCESS,	// read/write permission
		0,
		0,
		sizeof(MemBuff)); 
	if (m_pBuf == NULL) { free(m_sEvtAvail); free(m_sEvtFilled); free(m_sMemName); return; }

	// Release memory
	free(m_sEvtAvail);
	free(m_sEvtFilled);
	free(m_sMemName);
};

IPC::Client::~Client(void)
{
	// Close the event
	CloseHandle(m_hSignal);

	// Close the event
	CloseHandle(m_hAvail);

	// Unmap the memory
	UnmapViewOfFile(m_pBuf);

	// Close the file handle
	CloseHandle(m_hMapFile);
};

IPC::Block* IPC::Client::getBlock(DWORD dwTimeout)
{
	// Grab another block to write too
	// Enter a continous loop (this is to make sure the operation is atomic)
	for (;;)
	{
		// Check if there is room to expand the write start cursor
		LONG blockIndex = m_pBuf->m_WriteStart;
		Block *pBlock = m_pBuf->m_Blocks + blockIndex;
		if (pBlock->Next == m_pBuf->m_ReadEnd)
		{
			// No room is available, wait for room to become available
			if (WaitForSingleObject(m_hAvail, dwTimeout) == WAIT_OBJECT_0)
				continue;

			// Timeout
			return NULL;
		}

		// Make sure the operation is atomic
		if (InterlockedCompareExchange(&m_pBuf->m_WriteStart, pBlock->Next, blockIndex) == blockIndex)
			return pBlock;

		// The operation was interrupted by another thread.
		// The other thread has already stolen this block, try again
		continue;
	}
};

void IPC::Client::postBlock(Block *pBlock)
{
	// Set the done flag for this block
	pBlock->doneWrite = 1;

	// Move the write pointer as far forward as we can
	for (;;)
	{
		// Try and get the right to move the poitner
		DWORD blockIndex = m_pBuf->m_WriteEnd;
		pBlock = m_pBuf->m_Blocks + blockIndex;
		if (InterlockedCompareExchange(&pBlock->doneWrite, 0, 1) != 1)
		{
			// If we get here then another thread has already moved the pointer
			// for us or we have reached as far as we can possible move the pointer
			return;
		}

		// Move the pointer one forward (interlock protected)
		InterlockedCompareExchange(&m_pBuf->m_WriteEnd, pBlock->Next, blockIndex);

		// Signal availability of more data but only if threads are waiting
		if (pBlock->Prev == m_pBuf->m_ReadStart)
			SetEvent(m_hSignal);
	}
};

DWORD IPC::Client::write(void *pBuff, DWORD amount, DWORD dwTimeout)
{
	// Grab a block
	Block *pBlock = getBlock(dwTimeout);
	if (!pBlock) return 0;

	// Copy the data
	DWORD dwAmount = min(amount, IPC_BLOCK_SIZE);
	memcpy(pBlock->Data, pBuff, dwAmount);
	pBlock->Amount = dwAmount;

	// Post the block
	postBlock(pBlock);

	// Fail
	return 0;
};

bool IPC::Client::waitAvailable(DWORD dwTimeout)
{
	// Wait on the available event
	if (WaitForSingleObject(m_hAvail, dwTimeout) != WAIT_OBJECT_0)
		return false;

	// Success
	return true;
};