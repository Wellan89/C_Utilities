#ifndef DEF_RANDOMIZED_ALGO_EVALUATOR
#define DEF_RANDOMIZED_ALGO_EVALUATOR

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

// Classe permettant d'évaluer des jeux de probabilité
// pour un algorithme basé sur une heuristique aléatoire.
// RandomizedAlgorithm doit avoir les fonctions suivantes :
//		void nextRandomProbs();		// Passe au jeu de probabilité suivant.
//		bool allProbsTested();		// Détermine si tous les jeux de probabilité ont été testés.
//		int getRandomProbsId();		// Renvoit un identificateur permettant de retrouver les meilleurs paramètres de l'algorithme.
//		float solve();				// Fonction de calcul de l'algorithme, renvoyant son score. Plus il est haut, meilleur il est.
template<class RandomizedAlgorithm>
class RandomizedAlgoEvaluator
{
protected:
	RandomizedAlgorithm& algo;

	// Les scores moyens de l'algorithme avec leur jeu de probabilité associé
	std::vector<std::pair<float, int> > scores;

	const int solveLoopsCount;

	float evaluate()
	{
		float sum = 0.0f;
#ifdef RANDOMIZED_ALGO_EVALUATOR_USE_OPEN_MP
#pragma omp parallel for schedule(static) reduction(+ : sum)
#endif
		for (int i = 0; i < solveLoopsCount; i++)
			sum += algo.solve();
		return sum;
	}

public:
	RandomizedAlgoEvaluator(RandomizedAlgorithm& _algo, int _solveLoopsCount = 1000)
		: algo(_algo), solveLoopsCount(std::max(_solveLoopsCount, 1))
	{ }

	void run()
	{
		for (; !algo.allProbsTested(); algo.nextRandomProbs())
		{
			float eval = evaluate();
			scores.push_back(std::pair<float, int>(
				eval / (float)solveLoopsCount, algo.getRandomProbsId()));
		}
		std::sort(scores.begin(), scores.end());
	}
	int getBestResultsProbsId() const
	{
		return scores.back().second;
	}
	void printSelf(std::ostream& os) const
	{
		os << "RandomizedAlgoEvaluator results :" << endl
			<< "Best result : Probs Id = " << getBestResultsProbsId() << endl
			<< endl << "All results :" << endl;
		for (auto it = scores.rbegin(); it != scores.rend(); ++it)
			os << "Mean score = " << it->first << " with Probs Id = " << it->second << endl;
	}
};
template<class RandomizedAlgorithm>
std::ostream& operator<<(std::ostream& os,
	const RandomizedAlgoEvaluator<RandomizedAlgorithm>& evaluator)
{
	evaluator.printSelf(os);
	return os;
}

#endif
