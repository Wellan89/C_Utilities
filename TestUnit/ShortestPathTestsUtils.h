#ifndef DEF_SHORTEST_PATH_TESTS_UTILS
#define DEF_SHORTEST_PATH_TESTS_UTILS

/*
TODO :
Bug : les tests ne supportent pas un type de coût non signé actuellement

Détecter les conditions d'application des algorithmes lorsqu'ils s'appliquent sur un graphe : ex : détection d'un lien négatif pour Dijkstra
Créer une fonction pour chaque algorithme de calcul du chemin le plus court permettant d'obtenir la liste des liens empruntés
Indiquer la complexité et les points forts/points faibles de chacun des algorithmes,
	et indiquer (et vérifier à l'exécution) les limites de ces algorithmes (graphes sans cycles, à coûts positifs...)
Ecrire une fonction de conversion d'un graphe dynamique vers un graphe statique
Dans les fonctions Graph::removeLinks, parcourir les liens de la fin vers le début, pour obtenir une meilleure complexité en moyenne

Tester la fonction de suppression de liens du graphe
Tester la détection de circuits absorbants, et les cas de graphes à un ou deux noeuds pour Bellman-Ford et Bellman-Ford-Yen
Tester les algorithmes sur des graphes non simples : avec des boucles (une arête d'un noeud sur lui-même) ou plusieurs liens entre chaque noeud
Ecrire un test avec un graphe sans cycles, afin de pouvoir comparer Bellman aux autres algorithmes

Déplacer toutes les fonctions des fichiers sources dépendant de templates dans les headers
*/



#ifdef _DEBUG
#define PATH_FINDERS_COMPUTE_LOOPS				800

#define BIG_MAZE_SIZE							201		// Doit être impair
#define EMPTY_MAP_SIZE							81

#define RANDOM_MAP_SIZE							151
#define RANDOM_MAP_MAX_NB_LINKS					15
#define RANDOM_MAP_MAX_COST						1000000
#else
#define PATH_FINDERS_COMPUTE_LOOPS				500000

#define BIG_MAZE_SIZE							3001	// Doit être impair
#define EMPTY_MAP_SIZE							2001

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
#include "AStarDynamic.cpp"
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
	template<class T>
	vector<T> reverse_vect(const deque<T>& d)
	{
		vector<T> v;
		v.reserve(d.size());
		for (auto it = d.rbegin(); it != d.rend(); ++it)
			v.push_back(*it);
		return v;
	}
	template<class T>
	vector<T> deque_to_vect(const deque<T>& d)
	{
		vector<T> v;
		v.reserve(d.size());
		for (auto it = d.begin(); it != d.end(); ++it)
			v.push_back(*it);
		return v;
	}

	template<class Cost = unsigned int, class NodeIndex = Graph<>::IndexNoeud>
	struct ShortestPathTest
	{
		typedef Cost Cout;
		typedef NodeIndex IndexNoeud;
		typedef Graph<Cout, IndexNoeud> Graphe;

		unsigned int nbComputeLoops;
		unsigned int testsRefsCount;

		Graphe g;
		IndexNoeud startNode;
		IndexNoeud closestFinalNode;

		vector<Cout> costs;
		vector<deque<IndexNoeud> > paths;

		vector<vector<IndexNoeud> > reversePaths;

		ShortestPathTest(IndexNoeud nodesNb, unsigned int refsCount, unsigned int nbLoops = PATH_FINDERS_COMPUTE_LOOPS)
			: g(nodesNb), testsRefsCount(refsCount), nbComputeLoops(nbLoops)
		{
			costs.resize(nodesNb, Graphe::INFINITE_COST());
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
	ShortestPathTest<> *simpleTest, *littleMaze,
		*negativeLinksGraph, *notSimpleGraph, *algo2Graph, *roGraph,
		*bigMaze, *emptyMap, *randomMap;

#define CHECK_TEST_DELETE(test)					\
	do {										\
		if (test && test->testsRefsCount > 0) {	\
			test->testsRefsCount--;				\
			if (test->testsRefsCount == 0) {	\
				delete test;					\
				test = NULL;					\
			}									\
		}										\
	} while (false)

#define RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(test, spf)										\
	do {																						\
		if (!test)																				\
			Assert::Fail(L"Invalid test references count !");									\
		for (unsigned int i = 0; i < test->nbComputeLoops; i++)									\
			spf.computeShortestPathsFrom(test->startNode);										\
		for (Graph<>::IndexNoeud i = 0; i < test->g.size(); i++) {								\
			if (test->costs[i] != Graph<>::INFINITE_COST()) {									\
				Assert::IsTrue(spf.canReachNode(i));											\
				Assert::AreEqual(test->costs[i], spf.getCostTo(i));								\
				if (test->paths[i].size() > 1) {												\
					Assert::AreEqual(test->paths[i], spf.getShortestPathTo(i));					\
					Assert::AreEqual(test->reversePaths[i], spf.getReverseShortestPathTo(i));	\
				}																				\
			}																					\
		}																						\
		CHECK_TEST_DELETE(test);																\
	} while (false)

#define RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(test, spf)								\
	do {																						\
		if (!test)																				\
			Assert::Fail(L"Invalid test references count !");									\
		for (unsigned int i = 0; i < test->nbComputeLoops; i++)									\
			spf.computeShortestPathFrom(test->startNode);										\
		Graph<>::IndexNoeud i = test->closestFinalNode;											\
		if (test->costs[i] != Graph<>::INFINITE_COST()) {										\
			Assert::AreEqual(test->costs[i], spf.getPathCost());								\
			if (test->paths[i].size() > 1) {													\
				Assert::AreEqual(test->paths[i], spf.getShortestPath());						\
				Assert::AreEqual(test->reversePaths[i], spf.getReverseShortestPath());			\
			}																					\
		}																						\
		CHECK_TEST_DELETE(test);																\
	} while (false)

#define RUN_SHORTEST_PATH_FINDER_TEST_ALL_PAIRS_OF_NODES(test, spf)								\
	do {																						\
		if (!test)																				\
			Assert::Fail(L"Invalid test references count !");									\
		for (unsigned int i = 0; i < test->nbComputeLoops; i++)									\
			spf.computeShortestPaths();															\
		Graph<>::IndexNoeud j = test->startNode;												\
		for (Graph<>::IndexNoeud i = 0; i < test->g.size(); i++) {								\
			if (test->costs[i] != Graph<>::INFINITE_COST()) {									\
				Assert::IsTrue(spf.pathExists(j, i));											\
				Assert::AreEqual(test->costs[i], spf.getPathCost(j, i));						\
				if (test->paths[i].size() > 1)													\
					Assert::AreEqual(deque_to_vect(test->paths[i]), spf.getShortestPath(j, i));	\
			}																					\
		}																						\
		CHECK_TEST_DELETE(test);																\
	} while (false)

#define SHORTEST_PATH_TEST_METHOD_SEP(test, SPClass, RUN_MACRO, sep)	\
	TEST_METHOD(##SPClass ##sep ##test) {							\
		SPClass<ShortestPathTest<>::Graphe> sp(test->g);	\
		RUN_MACRO(test, sp);								\
	}

#define SHORTEST_PATH_TEST_METHOD(test, SPClass, RUN_MACRO)	SHORTEST_PATH_TEST_METHOD_SEP(test, SPClass, RUN_MACRO, _)
}

#endif
