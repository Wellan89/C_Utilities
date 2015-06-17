#include "ShortestPathTestsUtils.h"

// Test basé sur l'exercice "Counter Culture" du Google Code Jam 2015 - Round 1B.
// On exploite ici les graphes dynamiques, mais on compare aussi les résultats
// avec les méthodes utilisant les graphes classiques. De plus, on prend en compte
// le temps de création du graphe en lui-même dans le temps d'exécution du test.

// Réponses obtenues grâce au programme CounterCulture créé pour ce concours.
// En moyenne, l'algorithme basé sur le graphe dynamique est deux fois plus lent
// mais utilise deux fois moins de mémoire que l'algorithme basique.

#ifdef _DEBUG
#define COUNTER_CULTURE_FINAL_NODE	1234
#define COUNTER_CULTURE_PATH_COST	193
#else
#define COUNTER_CULTURE_FINAL_NODE	600000
#define COUNTER_CULTURE_PATH_COST	3431

//#define COUNTER_CULTURE_FINAL_NODE	1000000
//#define COUNTER_CULTURE_PATH_COST	3435

//#define COUNTER_CULTURE_FINAL_NODE	2000000
//#define COUNTER_CULTURE_PATH_COST	14426

// A partir d'ici, seul l'algorithme dynamique permet de donner une solution au problème !
// Les autres algorithmes utilisent trop de mémoire
// pour pouvoir être exécutés dans l'environnement de test de Visual Studio.

//#define COUNTER_CULTURE_FINAL_NODE	10000000
//#define COUNTER_CULTURE_PATH_COST	14434

//#define COUNTER_CULTURE_FINAL_NODE	12345678
//#define COUNTER_CULTURE_PATH_COST	24433

//#define COUNTER_CULTURE_FINAL_NODE	18273645
//#define COUNTER_CULTURE_PATH_COST	25360

//#define COUNTER_CULTURE_FINAL_NODE	20000000
//#define COUNTER_CULTURE_PATH_COST	34425
#endif

class CounterCultureNodeLinksGenerator
{
public:
	std::vector<DynamicLink> operator()(unsigned int index) const
	{
		std::vector<DynamicLink> links;
		links.reserve(2);

		unsigned int target = index + 1;
		if (target <= COUNTER_CULTURE_FINAL_NODE)
			links.push_back(DynamicLink(target, 1));

		target = TestUnit::swap(index);
		if (target <= COUNTER_CULTURE_FINAL_NODE)
			links.push_back(DynamicLink(target, 1));

		return links;
	}
};
class CounterCultureNodeFinalGenerator
{
public:
	bool operator()(unsigned int index) const
	{
		return (index == COUNTER_CULTURE_FINAL_NODE);
	}
};
class CounterCultureNodeHeuristicGenerator
{
public:
	unsigned int operator()(unsigned int index) const
	{
		// Attention : on doit toujours vérifier : h(x) <= d(x, finalNode),
		// ce qui est difficile à garantir ici !
		return 0;
	}
};

namespace TestUnit
{
	typedef DynamicGraph<CounterCultureNodeLinksGenerator,
		CounterCultureNodeFinalGenerator,
		CounterCultureNodeHeuristicGenerator> CounterCultureGraph;

	TEST_CLASS(AStarDynamicTests)
	{
		TEST_METHOD(AStarDynamicCounterCulture)
		{
			CounterCultureNodeLinksGenerator linksGen;
			CounterCultureNodeFinalGenerator finalGen;
			CounterCultureNodeHeuristicGenerator heuristicGen;
			CounterCultureGraph ccg(linksGen, finalGen, heuristicGen);

			AStarDynamic<CounterCultureGraph> asd(ccg);
			asd.computeShortestPathFrom(0);
			Assert::IsTrue(asd.hasFoundPath());
			Assert::AreEqual((unsigned int)COUNTER_CULTURE_PATH_COST, asd.getPathCost());
		}
	};

	TEST_CLASS(AStarTests)
	{
		TEST_METHOD(AStarCounterCulture)
		{
			Graph g(COUNTER_CULTURE_FINAL_NODE + 1);

			g.setNodeFinal(COUNTER_CULTURE_FINAL_NODE);
			CounterCultureNodeHeuristicGenerator heuristicGen;
			for (unsigned int i = 0; i < COUNTER_CULTURE_FINAL_NODE + 1; i++)
			{
				if (i < COUNTER_CULTURE_FINAL_NODE)
					g.addLink(i, i + 1, 1, true);

				unsigned int sw = swap(i);
				if (sw <= COUNTER_CULTURE_FINAL_NODE)
					g.addLink(i, sw, 1, true);

				g.setNodeHeuristic(i, heuristicGen(i));
			}

			AStar<> as(g);
			as.computeShortestPathFrom(0);
			Assert::IsTrue(as.hasFoundPath());
			Assert::AreEqual((unsigned int)COUNTER_CULTURE_PATH_COST, as.getPathCost());
		}
	};

	TEST_CLASS(DijkstraTests)
	{
		TEST_METHOD(DijkstraCounterCulture)
		{
			Graph g(COUNTER_CULTURE_FINAL_NODE + 1);
			for (unsigned int i = 0; i < COUNTER_CULTURE_FINAL_NODE + 1; i++)
			{
				if (i < COUNTER_CULTURE_FINAL_NODE)
					g.addLink(i, i + 1, 1, true);

				unsigned int sw = swap(i);
				if (sw <= COUNTER_CULTURE_FINAL_NODE)
					g.addLink(i, sw, 1, true);
			}

			Dijkstra<> dj(g);
			dj.computeShortestPathsFrom(0);
			Assert::IsTrue(dj.canReachNode(COUNTER_CULTURE_FINAL_NODE));
			Assert::AreEqual((unsigned int)COUNTER_CULTURE_PATH_COST, dj.getCostTo(COUNTER_CULTURE_FINAL_NODE));
		}
	};
}
