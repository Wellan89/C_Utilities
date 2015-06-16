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
				Cell* it = list_getFrontIterator(l);
				for (Cell* it = list_getFrontIterator(l); list_isValidIt(it); it = list_incrIt(it))
				{
					if (!list_isFirst(it))
						str << L" -> ";
					str << list_getElt(it);
				}

				wstring outStr;
				str >> outStr;
				return outStr;
			}
			static wstring ToString(const list<unsigned int>& l)
			{
				wstringstream str;
				for (auto it = l.cbegin(); it != l.cend(); ++it)
				{
					if (it != l.cbegin())
						str << L" -> ";
					str << (*it);
				}

				wstring outStr;
				str >> outStr;
				return outStr;
			}
			static wstring ToString(const deque<unsigned int>& l)
			{
				wstringstream str;
				for (auto it = l.cbegin(); it != l.cend(); ++it)
				{
					if (it != l.cbegin())
						str << L" -> ";
					str << (*it);
				}

				wstring outStr;
				str >> outStr;
				return outStr;
			}
			static wstring ToString(const vector<unsigned int>& l)
			{
				wstringstream str;
				for (auto it = l.cbegin(); it != l.cend(); ++it)
				{
					if (it != l.cbegin())
						str << L" -> ";
					str << (*it);
				}

				wstring outStr;
				str >> outStr;
				return outStr;
			}
		}
	}
}
