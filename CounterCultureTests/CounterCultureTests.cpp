#include "ShortestPathTestsUtils.h"
#include "CounterCulture.h"

// Test basé sur l'exercice "Counter Culture" du Google Code Jam 2015 - Round 1B.
// On exploite ici les graphes dynamiques, mais on compare aussi les résultats
// avec les méthodes utilisant les graphes classiques. De plus, on prend en compte
// le temps de création du graphe en lui-même dans le temps d'exécution du test.

// En moyenne, l'algorithme basé sur le graphe dynamique est deux fois plus lent
// mais utilise deux fois moins de mémoire que l'algorithme basique.

#ifdef _DEBUG
#define COUNTER_CULTURE_FINAL_NODE	1234
#else
#define COUNTER_CULTURE_FINAL_NODE	600000

//#define COUNTER_CULTURE_FINAL_NODE	1000000

//#define COUNTER_CULTURE_FINAL_NODE	2000000

//#define COUNTER_CULTURE_FINAL_NODE	10000000

// A partir d'ici, seul l'algorithme dynamique permet de donner une solution au problème !
// Les autres algorithmes utilisent trop de mémoire
// pour pouvoir être exécutés dans l'environnement de test de Visual Studio.

//#define COUNTER_CULTURE_FINAL_NODE	12345678

//#define COUNTER_CULTURE_FINAL_NODE	18273645

//#define COUNTER_CULTURE_FINAL_NODE	20000000
#endif

namespace TestUnit
{
	unsigned long long COUNTER_CULTURE_PATH_COST;

	TEST_MODULE_INITIALIZE(CounterCultureTestsInit)
	{
		COUNTER_CULTURE_PATH_COST = cc_solve(COUNTER_CULTURE_FINAL_NODE);
	}

	TEST_CLASS(AStarDynamicTests)
	{
		TEST_METHOD(AStarDynamicCounterCulture)
		{
			CCNodeLinksGenerator linksGen(COUNTER_CULTURE_FINAL_NODE);
			CCNodeFinalGenerator finalGen(COUNTER_CULTURE_FINAL_NODE);
			CCNodeHeuristicGenerator heuristicGen(COUNTER_CULTURE_FINAL_NODE);
			CCDynGraph ccdg(linksGen, finalGen, heuristicGen);

			AStarDynamic<CCDynGraph> asd(ccdg);
			asd.computeShortestPathFrom(0);
			Assert::IsTrue(asd.hasFoundPath());
			Assert::AreEqual((CCDynGraph::Cout)COUNTER_CULTURE_PATH_COST, asd.getPathCost());
		}
	};

	TEST_CLASS(AStarTests)
	{
		TEST_METHOD(AStarCounterCulture)
		{
			CCGraph ccg = generateCCGraph(COUNTER_CULTURE_FINAL_NODE);

			AStar<CCGraph> as(ccg);
			as.computeShortestPathFrom(0);
			Assert::IsTrue(as.hasFoundPath());
			Assert::AreEqual((CCGraph::Cout)COUNTER_CULTURE_PATH_COST, as.getPathCost());
		}
	};

	TEST_CLASS(DijkstraTests)
	{
		TEST_METHOD(DijkstraCounterCulture)
		{
			CCGraph ccg = generateCCGraph(COUNTER_CULTURE_FINAL_NODE);

			Dijkstra<CCGraph> dj(ccg);
			dj.computeShortestPathsFrom(0);
			Assert::IsTrue(dj.canReachNode(COUNTER_CULTURE_FINAL_NODE));
			Assert::AreEqual((CCGraph::Cout)COUNTER_CULTURE_PATH_COST, dj.getCostTo(COUNTER_CULTURE_FINAL_NODE));
		}
	};
}
