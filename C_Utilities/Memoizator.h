#ifndef DEF_MEMOIZATOR
#define DEF_MEMOIZATOR

#include <unordered_map>
#include <utility>
#include <functional>
#include <tuple>
#include "TupleHash.h"

// Classe effectuant la m�mo�sation d'une fonction prenant la classe Argument comme argument.
// La fonction doit �tre r�f�rentiellement transparente : son r�sultat d�pend uniquement de son argument.
// De plus la m�mo�sation ne sera pas efficace si la fonction est r�cursive.
// La classe Argument doit disposer d'une fonction de hachage d�j� d�finie (std::hash<Argument> est d�fini).
template<class ReturnType, class Argument, class Hasher = std::hash<Argument> >
class Memoizator
{
protected:
	std::function<ReturnType(Argument)> func;
	std::unordered_map<Argument, ReturnType, Hasher> table;

public:
	template<class AutoFunc>
	Memoizator(AutoFunc& function) : func(function)
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

// Classe effectuant la m�mo�sation d'une fonction prenant la classe Argument comme argument.
// La fonction doit �tre r�f�rentiellement transparente : son r�sultat d�pend uniquement de son argument.
// Cette version est d�di�e aux fonctions r�cursives, qui recevront alors la fonction m�mo�s�e en premier argument.
// La classe Argument doit disposer d'une fonction de hachage d�j� d�finie (std::hash<Argument> est d�fini).
template<class ReturnType, class Argument, class Hasher = std::hash<Argument> >
class RecursiveMemoizator : public Memoizator<ReturnType, Argument, Hasher>
{
protected:
	typedef Memoizator<ReturnType, Argument, Hasher> MyBase;
	typedef RecursiveMemoizator<ReturnType, Argument, Hasher> MyType;
	typedef std::function<ReturnType(MyType&, Argument)> RecFunc;

	RecFunc recFunc;

public:
	template<class AutoRecFunc>
	RecursiveMemoizator(AutoRecFunc& function)
		: recFunc(function),
		MyBase(
			[this](const Argument& arg)
			{
				return recFunc(*this, arg);
			})
	{ }
};

// Classe effectuant la m�mo�sation d'une fonction prenant plusieurs param�tres.
// La fonction doit �tre r�f�rentiellement transparente : son r�sultat d�pend uniquement de ses arguments.
// De plus la m�mo�sation ne sera pas efficace si la fonction est r�cursive.
// Chaque argument doit disposer d'une fonction de hachage d�j� d�finie (std::hash<Argument> est d�fini).
template<class ReturnType, class... Args>
class MultipleArgsMemoizator : public Memoizator<ReturnType, std::tuple<Args...> >
{
protected:
	typedef Memoizator<ReturnType, std::tuple<Args...> > MyBase;
	typedef std::function<ReturnType(Args...)> MultArgsFunc;

	MultArgsFunc multArgsFunc;

	// Invoque une fonction prenant plusieurs arguments d'apr�s un tuple.
	// Code trouv� ici : http://en.cppreference.com/w/cpp/utility/integer_sequence
	template<typename Tuple, std::size_t... index>
	ReturnType invokeHelper(const Tuple& args, std::index_sequence<index...>)
	{
		return multArgsFunc(std::get<index>(args)...);
	}
	template<typename Tuple>
	ReturnType invoke(const Tuple& args)
	{
		constexpr auto size = std::tuple_size<Tuple>::value;
		return invokeHelper(args, std::make_index_sequence<size>{});
	}

public:
	template<class AutoFunc>
	MultipleArgsMemoizator(AutoFunc& function)
		: multArgsFunc(function),
		MyBase(
			[this](const std::tuple<Args...>& args)
			{
				return invoke(args);
			})
	{ }

	ReturnType operator()(const Args&... args)
	{
		return MyBase::operator()(make_tuple(args...));
	}
};

// Classe effectuant la m�mo�sation d'une fonction prenant plusieurs param�tres.
// La fonction doit �tre r�f�rentiellement transparente : son r�sultat d�pend uniquement de ses arguments.
// Cette version est d�di�e aux fonctions r�cursives, qui recevront alors la fonction m�mo�s�e en premier argument.
// Chaque argument doit disposer d'une fonction de hachage d�j� d�finie (std::hash<Argument> est d�fini).
template<class ReturnType, class... Args>
class RecursiveMultipleArgsMemoizator : public MultipleArgsMemoizator<ReturnType, Args...>
{
protected:
	typedef MultipleArgsMemoizator<ReturnType, Args...> MyBase;
	typedef RecursiveMultipleArgsMemoizator<ReturnType, Args...> MyType;
	typedef std::function<ReturnType(MyType&, Args...)> RecMultArgsFunc;

	RecMultArgsFunc recMultArgsFunc;

public:
	template<class AutoRecFunc>
	RecursiveMultipleArgsMemoizator(AutoRecFunc& function)
		: recMultArgsFunc(function),
		MyBase(
			[this](const Args&... args)
			{
				return recMultArgsFunc(*this, args...);
			})
	{ }
};

#endif
