#ifndef DEF_MEMOIZATOR
#define DEF_MEMOIZATOR

#include <unordered_map>
#include <utility>

// Classe effectuant la m�mo�sation d'une fonction prenant la classe Argument comme argument.
// La fonction doit �tre r�f�rentiellement transparente : son r�sultat d�pend uniquement de son argument.
// De plus la m�mo�sation ne sera pas efficace si la fonction est r�cursive et s'appelle sans passer par cette classe.
// La classe Argument doit disposer d'une fonction de hachage d�j� d�finie (std::hash<Argument> est d�fini).
template<class Function, class Argument>
class Memoizator
{
public:
	typedef decltype(std::declval<Function>()(std::declval<Argument>())) ReturnType;

protected:
	Function& func;
	std::unordered_map<Argument, ReturnType> table;

public:
	Memoizator(Function& function)
		: func(function)
	{ }

	void clear()
	{
		table.clear();
	}

	ReturnType operator()(const Argument& arg)
	{
		auto computedResult = table.find(arg);
		if (computedResult == table.end())
		{
			ReturnType result = func(arg);
			computedResult = table.insert(make_pair(arg, result)).first;
		}
		return computedResult->second;
	}
};

#endif
