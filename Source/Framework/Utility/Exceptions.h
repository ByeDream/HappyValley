/************************************************************************/
/* Exceptions
/*
/* ByeDream
/* Dec-20-2015
/************************************************************************/
#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include "Defines.h"

namespace HV
{
	typedef class _SimpleException_
	{
	public:
		_SimpleException_(const std::string &errorMsg)
			: mErrorMsg(errorMsg)
			, mTypeString("Exception")
		{}

		virtual ~_SimpleException_() {}

		const char *				getTypeString() const { return mTypeString.c_str(); }
		const char *				getErrorMsg() const { return mErrorMsg.c_str(); }

	protected:
		std::string					mTypeString;
		std::string					mErrorMsg;
	} SimpleException, *PSimpleException;

	typedef class _MemoryException_ : public _SimpleException_
	{
	public:
		_MemoryException_(const std::string &errorMsg) : _SimpleException_(errorMsg) 
		{
			mTypeString = "MemoryException";
		}
	} MemoryException, *PMemoryException;

	typedef class _FileException_ : public _SimpleException_
	{
	public:
		_FileException_(const std::string &errorMsg) : _SimpleException_(errorMsg) 
		{
			mTypeString = "FileException";
		}
	} FileException, *PFileException;

	template<class T>
	void _throwSimpleException(const char *filePath, INT32 lineNumber, char *errorMsg, ...)
	{
		const SIZE_T BUFFER_SIZE = 1024;
		char buffer[BUFFER_SIZE] = { 0 };
		sprintf_s(	buffer, 
					BUFFER_SIZE - 1, 
					"[%s :: line%d]", 
					_analyzeFileName(filePath), 
					lineNumber);
		std::string fileLineStr(buffer);
		
 		memset(buffer, 0, BUFFER_SIZE);
		va_list st;
 		va_start(st, errorMsg);
 		vsprintf_s(buffer, BUFFER_SIZE - 1, errorMsg, st);
 		va_end(st);
 		std::string msgStr(buffer);
		
		throw new T(fileLineStr + msgStr);
	}

#	define THROW_EXCEPTION(T, errorMsg, ...) \
 	_throwSimpleException<T>(__FILE__, __LINE__, errorMsg, ##__VA_ARGS__)
}

#endif // !__EXCEPTIONS_H__

// EOF