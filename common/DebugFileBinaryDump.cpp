//---------------------------------------------------------------------------


#pragma hdrstop
#include <vcl.h>
#include "DebugFileBinaryDump.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

DebugFileBinaryDump::DebugFileBinaryDump(AnsiString filename)
{
	if (filename == "")
	{
		filename = ExtractFileDir(Application->ExeName) + FormatDateTime("yyyymmdd_hhnnss_zzz", Now());
	}
	fp = fopen(filename.c_str(), "wb");
}

DebugFileBinaryDump::~DebugFileBinaryDump(void)
{
	if (fp)
		fclose(fp);
}

int DebugFileBinaryDump::Write(void *buffer, unsigned int size)
{
	if (fp == NULL)
		return -1;
	return fwrite(buffer, size, 1, fp);
}
