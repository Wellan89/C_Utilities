#ifndef DEF_ALGORITHM_UTILITY
#define DEF_ALGORITHM_UTILITY

#include <tuple>
#include <functional>

// Classe permettant de modifier la valeur d'un paramètre dans un intervalle
template<class Param>
class ParamRange
{
public:
	Param* value;
	Param minValue;
	Param maxValue;
	Param step;

	// Variable utilisée dans RandomizedAlgorithm uniquement :
	// Le choix de changement d'un paramètre ne sera effectué que si le rapport (flottant)
	// entre le score avec l'ancien paramètre et celui avec le nouveau paramètre
	// est hors de l'intervalle [1 - baseRatioThreshold ; 1 + baseRatioThreshold].
	float baseRatioThreshold;

	ParamRange(Param* val, Param minVal, Param maxVal, Param stepVal,
		float baseRatioThresholdVal = 0.1f)
		: value(val), minValue(minVal), maxValue(maxVal), step(stepVal),
		baseRatioThreshold(baseRatioThresholdVal)
	{ }
};

// Classe permettant l'utilisation d'une fonction en tant qu'algorithme
template<class ReturnType, class... Args>
class FunctionAlgorithm
{
protected:
	typedef FunctionAlgorithm<ReturnType, Args...> MyType;
	typedef std::function<ReturnType(Args...)> Func;

	Func func;
	std::tuple<Args...> args;

	// Invoque une fonction prenant plusieurs arguments d'après un tuple.
	template<std::size_t... index>
	ReturnType invokeHelper(std::index_sequence<index...>)
	{
		return func(std::get<index>(args)...);
	}
	ReturnType invoke()
	{
		constexpr auto size = std::tuple_size<decltype(args)>::value;
		return invokeHelper(std::make_index_sequence<size>{});
	}

public:
	template<class AutoFunc>
	FunctionAlgorithm(const AutoFunc& function) : func(function)
	{ }

	// Renvoit les arguments avec lesquels la fonction sera appelée
	std::tuple<Args...>& getArgs()
	{
		return args;
	}

	// Renvoit l'argument d'indice spécifié
	template<size_t index>
	decltype(std::get<index>(args))& getArg()
	{
		return std::get<index>(args);
	}

	ReturnType solve()
	{
		return invoke();
	}
};

#endif
