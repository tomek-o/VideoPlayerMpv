/** \file
	\brief Utility writing binary data to file for debugging purposes
*/

//---------------------------------------------------------------------------

#ifndef DebugFileBinaryDumpH
#define DebugFileBinaryDumpH
//---------------------------------------------------------------------------

#include <stdio.h>

class DebugFileBinaryDump
{
public:
	/** \param filename Name of the file to write to; if empty - timestamp is used as name and file is created next to executable
	*/
	DebugFileBinaryDump(AnsiString filename = "");
	~DebugFileBinaryDump(void);
	int Write(void *buffer, unsigned int size);
private:
	FILE *fp;
};

#endif
