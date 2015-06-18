#ifndef DEF_TO_STRING_SPECIALIZATIONS
#define DEF_TO_STRING_SPECIALIZATIONS

#include "CppUnitTest.h"
#include "List.h"
#include <list>
#include <deque>
#include <vector>

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			static std::wstring ToString(List* l);
			template<class T> static std::wstring ToString(const std::list<T>& l);
			template<class T> static std::wstring ToString(const std::deque<T>& l);
			template<class T> static std::wstring ToString(const std::vector<T>& l);
			static std::wstring ToString(const long long int& t);
		}
	}
}

#endif
