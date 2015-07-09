#ifndef DEF_RANDOMIZED_ALGO_EVALUATOR
#define DEF_RANDOMIZED_ALGO_EVALUATOR

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

// Classe permettant de déterminer les meilleurs jeux de probabilité
// pour un algorithme basé sur une heuristique aléatoire.
// RandomizedAlgorithm doit avoir les fonctions suivantes :
//		void nextRandomProbs();				// Passe au jeu de probabilité suivant.
//		bool allProbsTested();				// Détermine si tous les jeux de probabilité ont été testés.
//		unsigned int getRandomProbsId();	// Renvoit un identificateur permettant de retrouver les mailleurs paramètres de l'algorithme.
//		float solve();						// Fonction de calcul de l'algorithme, renvoyant son score. Plus il est haut, meilleur il est.
template<class RandomizedAlgorithm>
class RandomizedAlgoEvaluator
{
protected:
	RandomizedAlgorithm& algo;

	// Les scores moyens de l'algorithme avec leur jeu de probabilité associé
	std::vector<std::pair<float, int> > scores;

	unsigned int solveLoopsCount;

public:
	RandomizedAlgoEvaluator(RandomizedAlgorithm& _algo, unsigned int _solveLoopsCount = 1000)
		: algo(_algo), solveLoopsCount(_solveLoopsCount)
	{ }

	void run()
	{
		for (; !algo.allProbsTested(); algo.nextRandomProbs())
		{
			float sum = 0.0f;
			for (unsigned int i = 0; i < solveLoopsCount; i++)
				sum += algo.solve();
			scores.push_back(std::pair<float, int>(sum / (float)solveLoopsCount, algo.getRandomProbsId()));
		}
		sort(scores.begin(), scores.end());
	}
	int getBestResultsProbsId() const
	{
		return std::max_element(scores.begin(), scores.end())->second;
	}
};
template<class RandomizedAlgorithm>
std::ostream& operator<<(std::ostream& os, const RandomizedAlgoEvaluator<class RandomizedAlgorithm>& evaluator)
{
	os << "RandomizedAlgoEvaluator results :" << endl
		<< "Best result : Probs Id = " << getBestResultsProbsId() << endl
		<< endl << "All results :" << endl;
	for (auto it = scores.rbegin(); it != scores.rend(); ++it)
		os << "Mean score = " << it->first << " with Probs Id = " << it->second << endl;
	return os;
}

#endif
