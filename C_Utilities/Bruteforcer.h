#ifndef DEF_BRUTEFORCER
#define DEF_BRUTEFORCER

#include <iostream>
#include <vector>
#include <algorithm>
#include "AlgorithmUtility.h"

#ifndef BRUTEFORCER_DEBUG_LOG
#define BRUTEFORCER_DEBUG_LOG(msg)
#endif

// Classe permettant de tester un ensemble de param�tres d'un algorithme par Bruteforce.
// Algorithm doit avoir la fonction suivante :
//		ReturnType solve();				// Fonction de calcul de l'algorithme, renvoyant son score. Plus il est haut, meilleur il est.
template<class Algorithm, class Param>
class Bruteforcer
{
public:
	typedef decltype(std::declval<Algorithm>().solve()) ReturnType;

protected:
	Algorithm& algo;

	// Les param�tres r�glables de l'algorithme � tester
	std::vector<ParamRange<Param> > params;

	// Les meilleurs param�tres trouv�s jusqu'� pr�sent
	std::vector<Param> bestParams;

	ReturnType bestScore;

	void copyParamsToBestParams()
	{
		for (size_t i = 0; i < params.size(); i++)
			bestParams[i] = *(params[i].value);
	}

public:
	Bruteforcer(Algorithm& _algo, const std::vector<ParamRange<Param> >& _params)
		: algo(_algo), params(_params)
	{ }

	void run()
	{
		// Initialise la recherche avec les param�tres minimaux
		for (ParamRange<Param>& param : params)
			(*param.value) = param.minValue;

		bestParams.resize(params.size());
		copyParamsToBestParams();
		bestScore = algo.solve();

		// Teste toutes les combinaisons possibles de tous les param�tres
		while (true)
		{
			// Incr�mente l'ensemble des param�tres
			auto it = params.rbegin();
			for (; it != params.rend(); ++it)
			{
				if ((*(it->value)) < it->maxValue)
				{
					// Si ce param�tre n'est pas � son maximum,
					// on l'incr�mente et on sort de cette boucle.
					(*(it->value)) += it->step;
					break;
				}
				else
				{
					// Sinon, on le r�initialise et on passe au param�tre pr�c�dent
					(*(it->value)) = it->minValue;
				}
			}

			// Si on a termin� d'�valuer tous les param�tres,
			// ie : on n'a trouv� aucun param�tre � incr�menter, on quitte.
			if (it == params.rend())
				break;

			// Effectue l'�valuation de l'algorithme avec ces param�tres
			ReturnType score = algo.solve();
			if (score > bestScore)
			{
				bestScore = score;
				copyParamsToBestParams();
				BRUTEFORCER_DEBUG_LOG("Found better score : " << bestScore << std::endl);
			}
		}
	}

	ReturnType getBestScore() const
	{
		return bestScore;
	}
	const std::vector<Param>& getBestParams() const
	{
		return bestParams;
	}

	void printSelf(std::ostream& os) const
	{
		os << "Bruteforcer results :" << endl
			<< "Best score = " << bestScore << endl
			<< endl << "Best parameters :" << endl;
		for (size_t i = 0; i < params.size(); i++)
			os << "Parameter " << i << " = " << *(params[i].value) << endl;
	}
};
template<class Algorithm, class Param>
std::ostream& operator<<(std::ostream& os,
	const Bruteforcer<Algorithm, Param>& optimizer)
{
	optimizer.printSelf(os);
	return os;
}

#endif
