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
			static std::wstring ToString(List* l)
			{
				if (!l)
					return std::wstring(L"NULL");

				std::wstringstream str;
				for (Cell* it = list_getFrontIterator(l); list_isValidIt(it); it = list_incrIt(it))
				{
#ifdef _DEBUG
					// Désactivé en mode Release pour accélérer les assertions
					if (!list_isFirst(it))
						str << L" -> ";
#endif
					str << list_getElt(it);
				}

				return str.str();
			}
			template<class T> static std::wstring ToString(const std::list<T>& l)
			{
				std::wstringstream str;
				for (auto it = l.cbegin(); it != l.cend(); ++it)
				{
#ifdef _DEBUG
					// Désactivé en mode Release pour accélérer les assertions
					if (it != l.cbegin())
						str << L" -> ";
#endif
					str << (*it);
				}

				return str.str();
			}
			template<class T> static std::wstring ToString(const std::deque<T>& l)
			{
				std::wstringstream str;
				for (auto it = l.cbegin(); it != l.cend(); ++it)
				{
#ifdef _DEBUG
					// Désactivé en mode Release pour accélérer les assertions
					if (it != l.cbegin())
						str << L" -> ";
#endif
					str << (*it);
				}

				return str.str();
			}
			template<class T> static std::wstring ToString(const std::vector<T>& l)
			{
				std::wstringstream str;
				for (auto it = l.cbegin(); it != l.cend(); ++it)
				{
#ifdef _DEBUG
					// Désactivé en mode Release pour accélérer les assertions
					if (it != l.cbegin())
						str << L" -> ";
#endif
					str << (*it);
				}

				return str.str();
			}
			static std::wstring ToString(const unsigned short& t) { RETURN_WIDE_STRING(t); }
			static std::wstring ToString(const long long int& t) { RETURN_WIDE_STRING(t); }
		}
	}
}

#endif
