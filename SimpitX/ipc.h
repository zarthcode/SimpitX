// IPC structures/objects
#pragma once

#ifndef IPC_H
#define IPC_H

// Defines the data size in each block, in bytes (256x256, 512x512)
#define IPC_BLOCK_SIZE 4096

// Number of blocks available for use
#define IPC_BLOCK_COUNT 256

// Maximum size for the address string
#define IPC_MAX_ADDR 256

#define IPC_EVENT_FILLED "SimpitXFillEvent"


/// Used to transfer data from the in-game simpitx module
class IPC
{
public:

	struct Block
	{
		
		long Next;					// Next block in the circular linked list
		long Prev;					// Previous block in the circular linked list
	
		volatile long doneRead;		// Flag used to signal that this block has been read
		volatile long doneWrite;	// Flag used to signal that this block has been written
		
		DWORD DataSize;				// Amount of data held in this block
		DWORD _Padding;				// Padding used to ensure 64bit boundary alignment

		BYTE Data[IPC_BLOCK_SIZE];
		
	};


private:
	 // Shared memory buffer
	struct MemBuff 
	{
		Block m_Blocks[IPC_BLOCK_COUNT];

		// Cursors
		volatile long m_ReadEnd;		// End of the read cursor
		volatile long m_ReadStart;		// Start of the read cursor
		
		volatile long m_WriteEnd;		// End of the write cursor
		volatile long m_WriteStart;		// Start of the write cursor
	};

	
public:

	// ID Generator
	static DWORD GetID(void)
	{
		// Generate an ID and return id
		static volatile LONG id = 1;
		return (DWORD)InterlockedIncrement((LONG*)&id);
	};

	// Server class
	class Server
	{
	public:
		
		// Constructor
		Server();

		// Destructor
		virtual ~Server();

	// Exposed functions

		DWORD					read(void *pBuff, DWORD buffSize, DWORD timeout = INFINITE);
		const char*				getAddress(void) { return m_sAddr.c_str(); };

		// Block functions
		Block*					getBlock(DWORD dwTimeout = INFINITE);
		void					retBlock(Block* pBlock);

		// Create and destroy functions
		void create(const std::string connectionName);
		void					close(void);

	protected:


	private:

		// Internal variables
		std::string				m_sAddr;		// Address of this server
		HANDLE					m_hMapFile;		// Handle to the mapped memory file
		HANDLE					m_hSignal;		// Event used to signal when data exists
		HANDLE					m_hAvail;		// Event used to signal when some blocks become available
		MemBuff					*m_pBuf;		// Buffer that points to the shared memory
	};


	// Client class
	class Client
	{

	public:

		// Construct / Destruct
		Client(std::string connectionName);
		virtual ~Client();

		// Exposed functions
		DWORD					write(void *pBuff, DWORD amount, DWORD dwTimeout = INFINITE);	// Writes to the buffer
		bool					waitAvailable(DWORD dwTimeout = INFINITE);						// Waits until some blocks become available

		Block*					getBlock(DWORD dwTimeout = INFINITE);							// Gets a block
		void					postBlock(Block *pBlock);										// Posts a block to be processed				

		// Functions
		BOOL					IsOk(void) { if (m_pBuf) return true; else return false; };

	private:

		// Internal variables
		std::string				m_sAddr;		// Address of this server
		HANDLE					m_hMapFile;		// Handle to the mapped memory file
		HANDLE					m_hSignal;		// Event used to signal when data exists
		HANDLE					m_hAvail;		// Event used to signal when some blocks become available
		MemBuff					*m_pBuf;		// Buffer that points to the shared memory


	};



};

#endif