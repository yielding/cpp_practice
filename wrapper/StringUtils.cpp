#include "HGUNet.h"

void OutputDebugString( const char* inString )
{
	printf("%s", inString);
}

string StringUtils::Sprintf(const char* inFormat, ...)
{
	//not thread safe...
	static char temp[4096];
	
	va_list args;
	va_start(args, inFormat);
	
	vsnprintf(temp, 4096, inFormat, args);

	return string(temp);
}

// void StringUtils::Log(const char* inFormat)
// {
// 	OutputDebugString(inFormat);
// 	OutputDebugString("\n");
// }

void StringUtils::Log(const char* inFormat, ...)
{
	//not thread safe...
	static char temp[4096];
	
	va_list args;
	va_start (args, inFormat);
	
	vsnprintf(temp, 4096, inFormat, args);

	OutputDebugString(temp);
	OutputDebugString("\n");
}

