#include "ToStringSpecializations.h"

using namespace std;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			static wstring ToString(List* l)
			{
				if (!l)
					return wstring(L"NULL");

				wstringstream str;
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
			template<class T> static wstring ToString(const list<T>& l)
			{
				wstringstream str;
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
			template<class T> static wstring ToString(const deque<T>& l)
			{
				wstringstream str;
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
			template<class T> static wstring ToString(const vector<T>& l)
			{
				wstringstream str;
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
			static wstring ToString(const unsigned short& t)	{ RETURN_WIDE_STRING(t); }
			static wstring ToString(const long long int& t)		{ RETURN_WIDE_STRING(t); }
		}
	}
}
