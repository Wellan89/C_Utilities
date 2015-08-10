#ifndef DEF_MEMOIZATOR
#define DEF_MEMOIZATOR

#include <unordered_map>
#include <utility>
#include <functional>
#include <tuple>
#include "TupleHash.h"

// Classe effectuant la mémoïsation d'une fonction prenant la classe Argument comme argument.
// La fonction doit être référentiellement transparente : son résultat dépend uniquement de son argument.
// De plus la mémoïsation ne sera pas efficace si la fonction est récursive.
// La classe Argument doit disposer d'une fonction de hachage déjà définie (std::hash<Argument> est défini).
template<class ReturnType, class Argument, class Hasher = std::hash<Argument> >
class Memoizator
{
protected:
	typedef Memoizator<ReturnType, Argument, Hasher> MyType;
	typedef std::function<ReturnType(Argument)> Func;

	Func func;
	std::unordered_map<Argument, ReturnType, Hasher> table;

public:
	template<class AutoFunc>
	Memoizator(const AutoFunc& function) : func(function)
	{ }

	void clear()
	{
		table.clear();
	}
	size_t getTableSize() const
	{
		return table.size();
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

// Classe effectuant la mémoïsation d'une fonction prenant la classe Argument comme argument.
// La fonction doit être référentiellement transparente : son résultat dépend uniquement de son argument.
// Cette version est dédiée aux fonctions récursives, qui recevront alors la fonction mémoïsée en premier argument.
// La classe Argument doit disposer d'une fonction de hachage déjà définie (std::hash<Argument> est défini).
template<class ReturnType, class Argument, class Hasher = std::hash<Argument> >
class RecursiveMemoizator
{
protected:
	typedef RecursiveMemoizator<ReturnType, Argument, Hasher> MyType;
	typedef std::function<ReturnType(MyType&, Argument)> RecFunc;

	RecFunc recFunc;
	std::unordered_map<Argument, ReturnType, Hasher> table;

public:
	template<class AutoRecFunc>
	RecursiveMemoizator(const AutoRecFunc& recFunction) : recFunc(recFunction)
	{ }

	void clear()
	{
		table.clear();
	}
	size_t getTableSize() const
	{
		return table.size();
	}

	ReturnType operator()(const Argument& arg)
	{
		auto computedResult = table.find(arg);
		if (computedResult == table.end())
		{
			ReturnType result = recFunc(*this, arg);
			computedResult = table.insert(make_pair(arg, result)).first;
		}
		return computedResult->second;
	}
};

// Classe effectuant la mémoïsation d'une fonction prenant plusieurs paramètres.
// La fonction doit être référentiellement transparente : son résultat dépend uniquement de ses arguments.
// De plus la mémoïsation ne sera pas efficace si la fonction est récursive.
// Chaque argument doit disposer d'une fonction de hachage déjà définie (std::hash<Argument> est défini).
template<class ReturnType, class... Args>
class MultipleArgsMemoizator : public Memoizator<ReturnType, std::tuple<Args...> >
{
protected:
	typedef Memoizator<ReturnType, std::tuple<Args...> > MyBase;
	typedef MultipleArgsMemoizator<ReturnType, Args...> MyType;
	typedef std::function<ReturnType(Args...)> MultArgsFunc;

	// Invoque une fonction prenant plusieurs arguments d'après un tuple.
	// Code trouvé ici : http://en.cppreference.com/w/cpp/utility/integer_sequence
	template<typename Tuple, std::size_t... index>
	static ReturnType invokeHelper(const MultArgsFunc& multArgsFunc, const Tuple& args, std::index_sequence<index...>)
	{
		return multArgsFunc(std::get<index>(args)...);
	}
	template<typename Tuple>
	static ReturnType invoke(const MultArgsFunc& multArgsFunc, const Tuple& args)
	{
		constexpr auto size = std::tuple_size<Tuple>::value;
		return invokeHelper(multArgsFunc, args, std::make_index_sequence<size>{});
	}

public:
	MultipleArgsMemoizator(const MultArgsFunc& multArgsFunc)
		: MyBase([multArgsFunc](const std::tuple<Args...>& args)
				{
					return invoke(multArgsFunc, args);
				})
	{ }

	template<class AutoMultArgsFunc>
	MultipleArgsMemoizator(const AutoMultArgsFunc& multArgsFunc)
		: MyType(MultArgsFunc(multArgsFunc))
	{ }

	ReturnType operator()(const Args&... args)
	{
		return MyBase::operator()(make_tuple(args...));
	}
};

// Classe effectuant la mémoïsation d'une fonction prenant plusieurs paramètres.
// La fonction doit être référentiellement transparente : son résultat dépend uniquement de ses arguments.
// Cette version est dédiée aux fonctions récursives, qui recevront alors la fonction mémoïsée en premier argument.
// Chaque argument doit disposer d'une fonction de hachage déjà définie (std::hash<Argument> est défini).
template<class ReturnType, class... Args>
class RecursiveMultipleArgsMemoizator : public RecursiveMemoizator<ReturnType, std::tuple<Args...> >
{
protected:
	typedef RecursiveMemoizator<ReturnType, std::tuple<Args...> > MyBase;
	typedef RecursiveMultipleArgsMemoizator<ReturnType, Args...> MyType;
	typedef std::function<ReturnType(MyType&, Args...)> RecMultArgsFunc;

	// Invoque une fonction prenant plusieurs arguments d'après un tuple.
	// Code trouvé ici : http://en.cppreference.com/w/cpp/utility/integer_sequence
	template<typename Tuple, std::size_t... index>
	static ReturnType invokeHelper(MyType& memRec, const RecMultArgsFunc& recMultArgsFunc,
		const Tuple& args, std::index_sequence<index...>)
	{
		return recMultArgsFunc(memRec, std::get<index>(args)...);
	}
	template<typename Tuple>
	static ReturnType invoke(MyType& memRec, const RecMultArgsFunc& recMultArgsFunc, const Tuple& args)
	{
		constexpr auto size = std::tuple_size<Tuple>::value;
		return invokeHelper(memRec, recMultArgsFunc, args, std::make_index_sequence<size>{});
	}

public:
	RecursiveMultipleArgsMemoizator(const RecMultArgsFunc& recMultArgsFunc)
		: MyBase([recMultArgsFunc](MyBase& memRec, const std::tuple<Args...>& args)
				{
					return invoke(reinterpret_cast<MyType&>(memRec), recMultArgsFunc, args);
				})
	{ }

	template<class AutoRecMultArgsFunc>
	RecursiveMultipleArgsMemoizator(const AutoRecMultArgsFunc& recMultArgsFunc)
		: MyType(RecMultArgsFunc(recMultArgsFunc))
	{ }

	ReturnType operator()(const Args&... args)
	{
		return MyBase::operator()(make_tuple(args...));
	}
};

#endif
