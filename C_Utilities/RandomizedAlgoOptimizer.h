#ifndef DEF_RANDOMIZED_ALGO_OPTIMIZER
#define DEF_RANDOMIZED_ALGO_OPTIMIZER

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#ifndef ALGORITHM_OPTIMIZATION_DEBUG_LOG
#define ALGORITHM_OPTIMIZATION_DEBUG_LOG(msg)
#endif

// Classe permettant d'optimiser automatiquement les jeux de probabilité
// d'un algorithme basé sur une heuristique aléatoire.
// RandomizedAlgorithm doit avoir la fonction suivante :
//		float solve();				// Fonction de calcul de l'algorithme, renvoyant son score. Plus il est haut, meilleur il est.
template<class Param = float>
struct ParamRange
{
	Param* value;
	Param minValue;
	Param maxValue;
	Param step;

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
template<class RandomizedAlgorithm, class Param = float>
class RandomizedAlgoOptimizer
{
protected:
	RandomizedAlgorithm& algo;

	// Les paramètres réglables de l'algorithme à optimiser
	std::vector<ParamRange<Param> > params;

	const int solveLoopsCount;

	float bestScore;

	float evaluate()
	{
		float sum = 0.0f;
#ifdef RANDOMIZED_ALGO_OPTIMIZER_USE_OPEN_MP
#pragma omp parallel for schedule(static) reduction(+ : sum)
#endif
		for (int i = 0; i < solveLoopsCount; i++)
			sum += algo.solve();
		return sum;
	}

public:
	RandomizedAlgoOptimizer(RandomizedAlgorithm& _algo, const std::vector<ParamRange<Param> >& _params,
			int _solveLoopsCount = 1000)
		: algo(_algo), params(_params), solveLoopsCount(std::max(_solveLoopsCount, 1)), bestScore(0.0f)
	{ }

	void run()
	{
		// Optimise chaque probabilité en les traitant dans l'ordre fourni
		for (unsigned int i = 0; i < params.size(); i++)
		{
			ALGORITHM_OPTIMIZATION_DEBUG_LOG(
				"Optimizing parameter " << i << "..." << std::endl);

			Param& param = *(params[i].value);

			// Réinitialise la probabilité
			param = params[i].minValue;

			// Augmente la probabilité jusqu'à ce que son score diminue
			float lastScore;
			float currentScore = evaluate();
			float lastScoreAdd = 0.0f, currentScoreAdd = 0.0f;
			bool shouldContinue = true;
			int nbWaitedTurns = 0;
			do
			{
				// Si on n'a pas encore effectué de tour d'attente,
				// c'est qu'on doit modifier la valeur du paramètre.
				float ratioThreshold;
				if (nbWaitedTurns == 0)
				{
					param += params[i].step;

					// Si on a atteint la valeur maximale du paramètre,
					// on quitte ici.
					if (param > params[i].maxValue)
						break;

					ALGORITHM_OPTIMIZATION_DEBUG_LOG("New step : parameter " << i << " = " << param << endl);

					lastScore = currentScore;
					currentScore = evaluate();

					ratioThreshold = params[i].baseRatioThreshold;
				}
				else
				{
					// Sinon on recalcule les valeurs de l'ancien score et du nouveau
					// en espérant avoir un écart plus important.
					param -= params[i].step;
					lastScore = evaluate();

					param += params[i].step;
					currentScore = evaluate();

					// Fait décroître le ratio exponentiellement pour accélérer les choix effectués,
					// puisqu'ils prennent alors en compte plus de valeurs.
					ratioThreshold *= 0.8f;	// Divise par 10 tous les 10 tours environ
				}

				float lastScoreTotal = lastScore + lastScoreAdd;
				float currentScoreTotal = currentScore + currentScoreAdd;
				if (std::abs(lastScoreTotal) > 0.01f && std::abs(currentScoreTotal) > 0.01f)
				{
					float ratio = currentScoreTotal / lastScoreTotal;
					if (std::abs(ratio - 1.0f) > ratioThreshold)
					{
						shouldContinue = (currentScoreTotal >= lastScoreTotal);

						ALGORITHM_OPTIMIZATION_DEBUG_LOG("Turn " << nbWaitedTurns + 1);
						ALGORITHM_OPTIMIZATION_DEBUG_LOG(
							(const char*)(shouldContinue ? " : Next step : current mean = " : " : Stopping here : current mean = "));
						ALGORITHM_OPTIMIZATION_DEBUG_LOG(
							currentScoreTotal / (float)((nbWaitedTurns + 1) * solveLoopsCount)
							<< " ; last mean = "
							<< lastScoreTotal / (float)((nbWaitedTurns + 1) * solveLoopsCount) << std::endl);

						// Le ratio entre les deux valeurs est suffisamment éloigné de 1
						// pour qu'on puisse conclure d'une amélioration
						// ou dégradation réelle du score moyen.
						nbWaitedTurns = 0;
						lastScoreAdd = 0.0f;
						currentScoreAdd = 0.0f;
					}
					else
					{
						ALGORITHM_OPTIMIZATION_DEBUG_LOG(
							"Turn " << nbWaitedTurns + 1 << " : Keep waiting : ratio = "
							<< ratio << " ; ratioThreshold = " << ratioThreshold << std::endl);

						// Le ratio entre les deux valeurs est trop proche de 1
						// pour qu'on puisse conclure d'une amélioration
						// ou dégradation réelle du score moyen.
						// On note qu'on a attendu un tour supplémentaire pour nous décider,
						// et on diminue l'écart nécessaire pour conclure sur l'effet de ce paramètre.
						// On note aussi les valeurs obtenues du score actuel et de l'ancien,
						// afin de pouvoir effectuer le ratio plus tard sur des scores prenant en compte
						// tous les scores obtenus précédemment ici.
						nbWaitedTurns++;
						shouldContinue = true;
						lastScoreAdd += lastScore;
						currentScoreAdd += currentScore;
					}
				}
				else
				{
					ALGORITHM_OPTIMIZATION_DEBUG_LOG(
						"Turn " << nbWaitedTurns + 1 <<
						" : Keep waiting : scores still too close from zero." << std::endl);

					// On ne calcule pas le ratio si lastScore ou currentScore est nul ou presque,
					// à la place on relance un calcul de ces valeurs.
					nbWaitedTurns++;
					shouldContinue = true;
					lastScoreAdd += lastScore;
					currentScoreAdd += currentScore;
				}

				// On évite une boucle infinie au cas où le score actuel
				// serait toujours précisément égal au score précédent :
				// dans ce cas on décide s'il faut continuer en se basant sur tous les scores obtenus précédemment.
				if (nbWaitedTurns >= 100)
				{
					ALGORITHM_OPTIMIZATION_DEBUG_LOG(
						"Turn " << nbWaitedTurns
						<< " : Stopped waiting ! current mean = " << currentScoreTotal / (float)(nbWaitedTurns * solveLoopsCount)
						<< " ; last mean = " << lastScoreTotal / (float)(nbWaitedTurns * solveLoopsCount) << std::endl);

					nbWaitedTurns = 0;
					shouldContinue = (currentScoreTotal >= lastScoreTotal);
					lastScoreAdd = 0.0f;
					currentScoreAdd = 0.0f;
				}
			} while (shouldContinue);

			// Rétablit le paramètre à sa meilleure valeur et retient le meilleur score obtenu
			param -= params[i].step;
			bestScore = lastScore;

			ALGORITHM_OPTIMIZATION_DEBUG_LOG(
				"Found best value for parameter " << i << " : " << param << std::endl << std::endl);
		}

		if (solveLoopsCount != 0)
			bestScore /= (float)solveLoopsCount;
	}

	float getBestScore() const
	{
		return bestScore;
	}
	const std::vector<ParamRange<Param> >& getBestParams() const
	{
		return params;
	}

	void printSelf(std::ostream& os) const
	{
		os << "RandomizedAlgoOptimizer results :" << endl
			<< "Best score = " << bestScore << endl
			<< endl << "Best parameters :" << endl;
		for (unsigned int i = 0; i < params.size(); i++)
			os << "Parameter " << i << " = " << *(params[i].value) << endl;
	}
};
template<class RandomizedAlgorithm, class Param>
std::ostream& operator<<(std::ostream& os,
	const RandomizedAlgoOptimizer<RandomizedAlgorithm, Param>& optimizer)
{
	optimizer.printSelf(os);
	return os;
}

#endif
