#ifndef DEF_MEMOIZATOR
#define DEF_MEMOIZATOR

#include <unordered_map>
#include <utility>

// Classe effectuant la mémoïsation d'une fonction prenant la classe Argument comme argument.
// La fonction doit être référentiellement transparente : son résultat dépend uniquement de son argument.
// De plus la mémoïsation ne sera pas efficace si la fonction est récursive et s'appelle sans passer par cette classe.
// La classe Argument doit disposer d'une fonction de hachage déjà définie (std::hash<Argument> est défini).
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
