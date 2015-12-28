/************************************************************************/
/* Defines
/*
/* ByeDream
/* Dec-20-2015
/************************************************************************/
#ifndef __DEFINES_H__
#define __DEFINES_H__

namespace HV
{
#	if defined(WIN32)
#		define BIT_32
#	else
#		define BIT_64
#	endif

#	define INT8					char
#	define UINT8				unsigned char
#	define INT16				short
#	define UINT16				unsigned short
#	define INT32				int
#	define UINT32				unsigned int
#	define INT64				long long
#	define UINT64				unsigned long long

#	ifdef BIT_32
#		define INTPTR			UINT32
#	else
#		define INTPTR			UINT64
#	endif

#	define USE_DOUBLE_DECIMAL
#	ifdef USE_DOUBLE_DECIMAL
#		define DECIMAL			double
#	else
#		define DECIMAL			float
#	endif

#	define BYTE					UINT8
#	define WORD					UINT16
#	define DWORD				UINT32
#	define SIZE_T				UINT32
#	define INDEX_T				UINT32

	typedef struct _RECT2D_
	{
		UINT32 left;
		UINT32 top;
		UINT32 right;
		UINT32 bottom;
	} RECT2D, *PRECT2D;
	typedef const PRECT2D CPRECT2D;

	typedef struct _POINT2D_
	{
		UINT32 x;
		UINT32 y;
	} POINT2D, *PPOINT2D;
	typedef const PPOINT2D CPPOINT2D;

#	define SAFE_DELETE(ptr) if(ptr) { delete ptr; ptr = NULL; }
#	define SAFE_DELETE_ARRAY(ptr) if(ptr) { delete[] ptr; ptr = NULL; }
#	define SAFE_RELEASE(ptr) if(ptr) { ptr->Release(); ptr = NULL; }

#	define SIZE_OF_ARRAY(a) (sizeof(a)/sizeof((a)[0]))

	template<typename T>
	inline T					_max(T a, T b)
	{ 
		return a > b ? a : b; 
	}

	template<typename T>
	inline T					_min(T a, T b) 
	{ 
		return a < b ? a : b; 
	}

	inline const char *			_analyzeFileName(const char *filePath)
	{
		const char *fileName = strrchr(filePath, '/');
		if (fileName != NULL)
		{
			fileName = fileName + 1;
		}
		else
		{
			fileName = strrchr(filePath, '\\');
			if (fileName != NULL)
			{
				fileName = fileName + 1;
			}
			else
			{
				fileName = filePath;
			}
		}
		return fileName;
	}
}

#endif // !__DEFINES_H__

// EOF