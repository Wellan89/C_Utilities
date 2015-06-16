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
			static std::wstring ToString(const std::list<unsigned int>& l);
			static std::wstring ToString(const std::deque<unsigned int>& l);
			static std::wstring ToString(const std::vector<unsigned int>& l);
		}
	}
}

#endif
