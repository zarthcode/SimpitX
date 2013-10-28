// file name:	capplication.h
// author:		Victor Saar, Ingmar Rötzler
// e-mail:		vsaar@web.de, ingmar.r@web.de
// web site:	http://www.two-kings.de

#ifndef capplication_h
#define capplication_h

#include"main.h"

class CApplication
{
public:
	CApplication(void);
	~CApplication(void);

	void InitWindow(void);

	/// Returns true if the program is ready to terminate.
	inline bool Exit()const { return m_bProgramExit; };

private:

	bool m_bProgramExit;
};

#endif