#ifndef DEF_ALGORITHM_UTILITY
#define DEF_ALGORITHM_UTILITY

#include <tuple>

// Classe permettant de modifier la valeur d'un param�tre dans un intervalle
template<class Param>
class ParamRange
{
public:
	Param* value;
	Param minValue;
	Param maxValue;
	Param step;

	// Variable utilis�e dans RandomizedAlgorithm uniquement :
	// Le choix de changement d'un param�tre ne sera effectu� que si le rapport (flottant)
	// entre le score avec l'ancien param�tre et celui avec le nouveau param�tre
	// est hors de l'intervalle [1 - baseRatioThreshold ; 1 + baseRatioThreshold].
	float baseRatioThreshold;

	ParamRange(Param* val, Param minVal, Param maxVal, Param stepVal,
		float baseRatioThresholdVal = 0.1f)
		: value(val), minValue(minVal), maxValue(maxVal), step(stepVal),
		baseRatioThreshold(baseRatioThresholdVal)
	{ }
};

// Classe permettant l'utilisation d'une fonction en tant qu'algorithme
template<class Function, class... Args>
class FunctionAlgorithm
{
protected:
	Function& func;
	std::tuple<Args...> args;

	// Invoque une fonction prenant plusieurs arguments d'apr�s un tuple.
	template<std::size_t... index>
	decltype(auto) invokeHelper(std::index_sequence<index...>)
	{
		return func(std::get<index>(args)...);
	}
	decltype(auto) invoke()
	{
		constexpr auto size = std::tuple_size<decltype(args)>::value;
		return invokeHelper(std::make_index_sequence<size>{});
	}

public:
	FunctionAlgorithm(Function& function)
		: func(function)
	{ }

	// Renvoit les arguments avec lesquels la fonction sera appel�e
	std::tuple<Args...>& getArgs()
	{
		return args;
	}

	decltype(auto) solve()
	{
		return invoke();
	}
};

#endif
