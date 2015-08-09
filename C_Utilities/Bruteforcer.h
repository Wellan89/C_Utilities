#ifndef DEF_BRUTEFORCER
#define DEF_BRUTEFORCER

#include <iostream>
#include <vector>
#include <algorithm>
#include "AlgorithmUtility.h"

#ifndef BRUTEFORCER_DEBUG_LOG
#define BRUTEFORCER_DEBUG_LOG(msg)
#endif

// Classe permettant de tester un ensemble de paramètres d'un algorithme par Bruteforce.
// Algorithm doit avoir la fonction suivante :
//		ReturnType solve();				// Fonction de calcul de l'algorithme, renvoyant son score. Plus il est haut, meilleur il est.
template<class Algorithm, class Param>
class Bruteforcer
{
public:
	typedef decltype(std::declval<Algorithm>().solve()) ReturnType;

protected:
	Algorithm& algo;

	// Les paramètres réglables de l'algorithme à tester
	std::vector<ParamRange<Param> > params;

	// Les meilleurs paramètres trouvés jusqu'à présent
	std::vector<Param> bestParams;

	ReturnType bestScore;

	void copyParamsToBestParams()
	{
		for (size_t i = 0; i < params.size(); i++)
			bestParams[i] = *(params[i].value);
	}

	// Teste toutes les combinaisons possibles à partir d'un certain paramètre
	void runFromParam(size_t paramIdx)
	{
		if (paramIdx >= params.size())
		{
			// Il n'y a plus de paramètre à modifier : on évalue notre algorithme ici.
			ReturnType score = algo.solve();
			if (score > bestScore)
			{
				bestScore = score;
				copyParamsToBestParams();
				BRUTEFORCER_DEBUG_LOG("Found better score : " << bestScore << std::endl);
			}
		}
		else
		{
			// Essaie toutes les valeurs de ce paramètre, puis tous les paramètres suivants
			ParamRange<Param>& p = params[paramIdx];
			for ((*p.value) = p.minValue; (*p.value) <= p.maxValue; (*p.value) += p.step)
				runFromParam(paramIdx + 1);
		}
	}

public:
	Bruteforcer(Algorithm& _algo, const std::vector<ParamRange<Param> >& _params)
		: algo(_algo), params(_params)
	{ }

	void run()
	{
		// Initialise la recherche avec les paramètres fournis par défaut
		bestParams.resize(params.size());
		copyParamsToBestParams();
		bestScore = algo.solve();

		// Teste toutes les combinaisons possibles de tous les paramètres
		runFromParam(0);
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
