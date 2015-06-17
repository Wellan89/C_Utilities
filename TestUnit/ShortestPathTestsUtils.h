#ifndef DEF_SHORTEST_PATH_TESTS_UTILS
#define DEF_SHORTEST_PATH_TESTS_UTILS

/*
TODO :
R�ecrire Dijkstra avec le nouvel algo wikip�dia + chercher sur internet la solution la plus efficace (possible en O(n.log n) apparemment)
Ecrire un type de graphe "infini" : g�n�ration dynamique de la liste des noeuds et de la liste des liens entre eux
Permettre la gestion des co�ts n�gatifs, avec une gestion appropri�e des erreurs pour les algos A* et Dijkstra (entre autres)
Tester la fonction de suppression de liens du graphe
Tester le d�tection de circuits absorbants, et les cas de graphes � un ou deux noeuds pour Bellman-Ford et Bellman-Ford-Yen
Tester les algorithmes sur des graphes non simples : avec des boucles (une ar�te d'un noeud sur lui-m�me)
Cr�er une fonction pour chaque algorithme de calcul du chemin le plus court permettant d'obtenir la liste des liens emprunt�s
Indiquer la complexit� et les points forts/points faibles de chacun des algorithmes,
et indiquer (et v�rifier � l'ex�cution) les limites de ces algorithmes (graphes sans cycles, � co�ts positifs...)
Permettre l'utilisation de plus de m�moire au processus de test de Visual Studio (on semble ici limit� aux alentours des 1.6 gigabyte)
	+ Trouver un moyen de limiter l'utilisation m�moire des tests
	+ Essayer d'ex�cuter plusieurs tests en parall�le
Ecrire le test "CounterCulture" du google code jam round 1B 2015
*/



#ifdef _DEBUG
#define PATH_FINDERS_COMPUTE_LOOPS				800

#define BIG_MAZE_SIZE							201		// Doit �tre impair
#define EMPTY_MAP_SIZE							81

#define RANDOM_MAP_SIZE							151
#define RANDOM_MAP_MAX_NB_LINKS					15
#define RANDOM_MAP_MAX_COST						1000000
#else
#define PATH_FINDERS_COMPUTE_LOOPS				500000

#define BIG_MAZE_SIZE							3001	// Doit �tre impair
#define EMPTY_MAP_SIZE							1501

#define RANDOM_MAP_SIZE							20000
#define RANDOM_MAP_MAX_NB_LINKS					5000	// Graphe dense
#define RANDOM_MAP_MAX_COST						1000000
#endif



// ATTENTION : L'inclusion des .h ne marche pas ici : trouver pourquoi !
#include <cstdlib>
#include <ctime>
#include "CppUnitTest.h"
#include "ToStringSpecializations.cpp"
#include "Graph.h"
#include "DynamicGraph.h"
#include "Dijkstra.cpp"
#include "AStar.cpp"
#include "Bellman.cpp"
#include "DFS_ShortestPath.cpp"
#include "BFS_ShortestPath.cpp"
#include "BellmanFord.cpp"
#include "BellmanFordYen.cpp"
#include "FloydWarshall.cpp"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestUnit
{
	vector<unsigned int> reverse_vect(const deque<unsigned int>& d)
	{
		vector<unsigned int> v;
		v.reserve(d.size());
		for (auto it = d.rbegin(); it != d.rend(); ++it)
			v.push_back(*it);
		return v;
	}
	vector<unsigned int> deque_to_vect(const deque<unsigned int>& d)
	{
		vector<unsigned int> v;
		v.reserve(d.size());
		for (auto it = d.begin(); it != d.end(); ++it)
			v.push_back(*it);
		return v;
	}

	struct ShortestPathTest
	{
		unsigned int nbComputeLoops;

		Graph g;
		unsigned int startNode;
		unsigned int closestFinalNode;

		vector<unsigned int> costs;
		vector<deque<unsigned int> > paths;

		vector<vector<unsigned int> > reversePaths;

		ShortestPathTest(unsigned int nodesNb, unsigned int nbLoops = PATH_FINDERS_COMPUTE_LOOPS)
			: g(nodesNb), nbComputeLoops(nbLoops)
		{
			costs.resize(nodesNb, (unsigned int)(-1));
			paths.resize(nodesNb);
			reversePaths.resize(nodesNb);
		}
		void finishTest()
		{
			g.setNodeFinal(closestFinalNode);

			for (unsigned int i = 0; i < paths.size(); i++)
				reversePaths[i] = reverse_vect(paths[i]);
		}
	};
	ShortestPathTest *simpleTest, *littleMaze, *bigMaze, *emptyMap, *randomMap;

#define RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(test, spf)										\
	do {																						\
		for (unsigned int i = 0; i < test->nbComputeLoops; i++)									\
			spf.computeShortestPathsFrom(test->startNode);										\
		for (unsigned int i = 0; i < test->g.size(); i++) {										\
			if (test->costs[i] != (unsigned int)(-1)) {											\
				Assert::IsTrue(spf.canReachNode(i));											\
				Assert::AreEqual(test->costs[i], spf.getCostTo(i));								\
				if (test->paths[i].size() > 1) {												\
					Assert::AreEqual(test->paths[i], spf.getShortestPathTo(i));					\
					Assert::AreEqual(test->reversePaths[i], spf.getReverseShortestPathTo(i));	\
				}																				\
			}																					\
		}																						\
	} while (false)

#define RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(test, spf)								\
	do {																						\
		for (unsigned int i = 0; i < test->nbComputeLoops; i++)									\
			spf.computeShortestPathFrom(test->startNode);										\
		unsigned int i = test->closestFinalNode;												\
		if (test->costs[i] != (unsigned int)(-1)) {												\
			Assert::AreEqual(test->costs[i], spf.getPathCost());								\
			if (test->paths[i].size() > 1) {													\
				Assert::AreEqual(test->paths[i], spf.getShortestPath());						\
				Assert::AreEqual(test->reversePaths[i], spf.getReverseShortestPath());			\
			}																					\
		}																						\
	} while (false)

#define RUN_SHORTEST_PATH_FINDER_TEST_ALL_PAIRS_OF_NODES(test, spf)								\
	do {																						\
		for (unsigned int i = 0; i < test->nbComputeLoops; i++)									\
			spf.computeShortestPaths();															\
		unsigned int j = test->startNode;														\
		for (unsigned int i = 0; i < test->g.size(); i++) {										\
			if (test->costs[i] != (unsigned int)(-1)) {											\
				Assert::IsTrue(spf.pathExists(j, i));											\
				Assert::AreEqual(test->costs[i], spf.getPathCost(j, i));						\
				if (test->paths[i].size() > 1)													\
					Assert::AreEqual(deque_to_vect(test->paths[i]), spf.getShortestPath(j, i));	\
			}																					\
		}																						\
	} while (false)

}

#endif
