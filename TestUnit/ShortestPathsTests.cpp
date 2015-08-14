#include "ShortestPathTestsUtils.h"
#include <cstdlib>
#include <ctime>

namespace TestUnit
{
	void SimpleTestInit(unsigned int testRefsCount)
	{
#define CTI(c)	(Graphe::IndexNoeud)(c - 'a')
		// Wikipedia example test : http://fr.wikipedia.org/wiki/Algorithme_de_Dijkstra
		simpleTest = new ShortestPathTest<>(CTI('j') + 1, testRefsCount);
		simpleTest->g.addLink(CTI('a'), CTI('b'), 85);
		simpleTest->g.addLink(CTI('a'), CTI('c'), 217);
		simpleTest->g.addLink(CTI('a'), CTI('e'), 173);
		simpleTest->g.addLink(CTI('b'), CTI('f'), 80);
		simpleTest->g.addLink(CTI('c'), CTI('g'), 186);
		simpleTest->g.addLink(CTI('c'), CTI('h'), 103);
		simpleTest->g.addLink(CTI('d'), CTI('h'), 183);
		simpleTest->g.addLink(CTI('e'), CTI('j'), 502);
		simpleTest->g.addLink(CTI('f'), CTI('i'), 250);
		simpleTest->g.addLink(CTI('i'), CTI('j'), 84);

		simpleTest->startNode = CTI('a');
		simpleTest->closestFinalNode = CTI('j');

		const Graphe::Cout heuristics[] = { 450, 400, 250, 300, 500, 300, 400, 150, 50, 0 };
		for (Graphe::IndexNoeud i = 0; i <= CTI('j'); i++)
			simpleTest->g.setNodeHeuristic(i, heuristics[i]);

		simpleTest->infiniteCostCheck = true;
		simpleTest->costs = { 0, 85, 217, 503, 173, 165, 403, 320, 415, 499 };
		simpleTest->paths = {
			deque < Graphe::IndexNoeud > { CTI('a') },
			deque < Graphe::IndexNoeud > { CTI('a'), CTI('b') },
			deque < Graphe::IndexNoeud > { CTI('a'), CTI('c') },
			deque < Graphe::IndexNoeud > { CTI('a'), CTI('c'), CTI('h'), CTI('d') },
			deque < Graphe::IndexNoeud > { CTI('a'), CTI('e') },
			deque < Graphe::IndexNoeud > { CTI('a'), CTI('b'), CTI('f') },
			deque < Graphe::IndexNoeud > { CTI('a'), CTI('c'), CTI('g') },
			deque < Graphe::IndexNoeud > { CTI('a'), CTI('c'), CTI('h') },
			deque < Graphe::IndexNoeud > { CTI('a'), CTI('b'), CTI('f'), CTI('i') },
			deque < Graphe::IndexNoeud > { CTI('a'), CTI('b'), CTI('f'), CTI('i'), CTI('j') } };
		simpleTest->finishTest();
#undef CTI
	}
	void LittleMazeInit(unsigned int testRefsCount)
	{
#define ID(i, j)	(Graphe::IndexNoeud)((i) * width + (j))
		vector<string> map = {
			"###########",
			"#         #",
			"# #########",
			"#         #",
			"# #########",
			"#S        #",
			"# #########",
			"#         #",
			"# #########",
			"#        E#",
			"###########" };
		const Graphe::IndexNoeud width = map[0].size();
		const Graphe::IndexNoeud height = map.size();

		const Graphe::Cout cost = 12;
		const deque<Graphe::IndexNoeud> path = { ID(5, 1), ID(6, 1), ID(7, 1), ID(8, 1), ID(9, 1),
			ID(9, 2), ID(9, 3), ID(9, 4), ID(9, 5), ID(9, 6), ID(9, 7), ID(9, 8), ID(9, 9) };

		littleMaze = new ShortestPathTest<>(width * height, testRefsCount);
		for (Graphe::IndexNoeud i = 0; i < height; i++)
		{
			for (Graphe::IndexNoeud j = 0; j < width; j++)
			{
				if (map[i][j] != '#')
				{
					Graphe::IndexNoeud id = ID(i, j);

					if (i < height - 1 && map[i + 1][j] != '#')
						littleMaze->g.addLink(id, ID(i + 1, j), 1);
					if (j < width - 1 && map[i][j + 1] != '#')
						littleMaze->g.addLink(id, ID(i, j + 1), 1);

					if (map[i][j] == 'S')
						littleMaze->startNode = id;
					else if (map[i][j] == 'E')
						littleMaze->closestFinalNode = id;
				}
			}
		}
		for (Graphe::IndexNoeud i = 0; i < height; i++)
			for (Graphe::IndexNoeud j = 0; j < width; j++)
				littleMaze->g.setNodeHeuristic(ID(i, j),
					(Graphe::Cout)(abs((long)(littleMaze->closestFinalNode / width) - (long)i)
						+ abs((long)(littleMaze->closestFinalNode % width) - (long)j)));

		littleMaze->costs[littleMaze->closestFinalNode] = cost;
		littleMaze->paths[littleMaze->closestFinalNode] = path;
		littleMaze->finishTest();
#undef ID
	}
	void NegativeLinksGraphInit(unsigned int testRefsCount)
	{
		negativeLinksGraph = new ShortestPathTest<>(11, testRefsCount);
		negativeLinksGraph->g.addLink(0, 1, 1);
		negativeLinksGraph->g.addLink(0, 2, 2);
		negativeLinksGraph->g.addLink(0, 3, 3);
		negativeLinksGraph->g.addLink(0, 8, 0);
		negativeLinksGraph->g.addLink(1, 3, 5);
		negativeLinksGraph->g.addLink(1, 5, 5);
		negativeLinksGraph->g.addLink(1, 8, 2);
		negativeLinksGraph->g.addLink(1, 9, -6, true);
		negativeLinksGraph->g.addLink(2, 4, 3);
		negativeLinksGraph->g.addLink(2, 5, 5);
		negativeLinksGraph->g.addLink(3, 4, 1);
		negativeLinksGraph->g.addLink(3, 6, 2);
		negativeLinksGraph->g.addLink(3, 9, 7);
		negativeLinksGraph->g.addLink(4, 7, 2);
		negativeLinksGraph->g.addLink(6, 5, -2, true);
		negativeLinksGraph->g.addLink(6, 7, 2);
		negativeLinksGraph->g.addLink(7, 8, 8);
		negativeLinksGraph->g.addLink(10, 10, -1);

		negativeLinksGraph->g.setNodeFinal(5);
		negativeLinksGraph->g.setNodeFinal(10);

		negativeLinksGraph->startNode = 0;
		negativeLinksGraph->closestFinalNode = 5;

		negativeLinksGraph->infiniteCostCheck = true;
		negativeLinksGraph->costs = { 0, 1, 2, 2, 3, 2, 4, 5, 0, -5, Graphe::INFINITE_COST };
		negativeLinksGraph->paths = {
			deque < Graphe::IndexNoeud > { 0 },
			deque < Graphe::IndexNoeud > { 0, 1 },
			deque < Graphe::IndexNoeud > { 0, 2 },
			deque < Graphe::IndexNoeud > { 0, 1, 9, 3 },
			deque < Graphe::IndexNoeud > { 0, 1, 9, 3, 4 },
			deque < Graphe::IndexNoeud > { 0, 1, 9, 3, 6, 5 },
			deque < Graphe::IndexNoeud > { 0, 1, 9, 3, 6 },
			deque < Graphe::IndexNoeud > { 0, 1, 9, 3, 4, 7 },
			deque < Graphe::IndexNoeud > { 0, 8 },
			deque < Graphe::IndexNoeud > { 0, 1, 9 },
			deque < Graphe::IndexNoeud > { 10 } };
		negativeLinksGraph->finishTest();
	}
	void NegativeCycleGraphInit(unsigned int testRefsCount)
	{
		negativeCycleGraph = new ShortestPathTest<>(8, testRefsCount);
		negativeCycleGraph->g.addLink(0, 1, 1);
		negativeCycleGraph->g.addLink(0, 5, -1);
		negativeCycleGraph->g.addLink(0, 6, 3);
		negativeCycleGraph->g.addLink(1, 2, -1);
		negativeCycleGraph->g.addLink(1, 2, -1);
		negativeCycleGraph->g.addLink(2, 3, 0);
		negativeCycleGraph->g.addLink(2, 6, 1);
		negativeCycleGraph->g.addLink(3, 4, -1);
		negativeCycleGraph->g.addLink(4, 5, 1);
		negativeCycleGraph->g.addLink(6, 7, 5);

		negativeCycleGraph->startNode = 0;
		negativeCycleGraph->closestFinalNode = 7;

		negativeCycleGraph->infiniteCostCheck = true;
		negativeCycleGraph->costs = { Graphe::INFINITE_COST, Graphe::INFINITE_COST,
			Graphe::INFINITE_COST, Graphe::INFINITE_COST,
			Graphe::INFINITE_COST, Graphe::INFINITE_COST,
			Graphe::INFINITE_COST, Graphe::INFINITE_COST };
		negativeCycleGraph->finishTest();
	}
	void NegativeLoopGraphInit(unsigned int testRefsCount)
	{
		negativeLoopGraph = new ShortestPathTest<>(4, testRefsCount);
		negativeLoopGraph->g.addLink(0, 1, 3, true);
		negativeLoopGraph->g.addLink(0, 2, -5, true);
		negativeLoopGraph->g.addLink(1, 1, -1, true);
		negativeLoopGraph->g.addLink(1, 2, 1, true);
		negativeLoopGraph->g.addLink(2, 3, 1, true);

		negativeLoopGraph->startNode = 0;
		negativeLoopGraph->closestFinalNode = 3;

		negativeLoopGraph->infiniteCostCheck = true;
		negativeLoopGraph->costs = { Graphe::INFINITE_COST, Graphe::INFINITE_COST,
			Graphe::INFINITE_COST, Graphe::INFINITE_COST };
		negativeLoopGraph->finishTest();
	}
	void NotSimpleGraphInit(unsigned int testRefsCount)
	{
		notSimpleGraph = new ShortestPathTest<>(13, testRefsCount);
		notSimpleGraph->g.addLink(0, 0, 0);
		notSimpleGraph->g.addLink(0, 1, 1);
		notSimpleGraph->g.addLink(0, 1, 2, true);
		notSimpleGraph->g.addLink(0, 1, 4);
		notSimpleGraph->g.addLink(0, 4, 3, true);
		notSimpleGraph->g.addLink(0, 10, 30);
		notSimpleGraph->g.addLink(0, 11, 27);
		notSimpleGraph->g.addLink(1, 0, 3, true);
		notSimpleGraph->g.addLink(1, 1, 5);
		notSimpleGraph->g.addLink(1, 7, 15);
		notSimpleGraph->g.addLink(1, 9, 8);
		notSimpleGraph->g.addLink(2, 0, 2, true);
		notSimpleGraph->g.addLink(2, 3, 1);
		notSimpleGraph->g.addLink(2, 2, 6);
		notSimpleGraph->g.addLink(3, 3, 1);
		notSimpleGraph->g.addLink(3, 6, 2, true);
		notSimpleGraph->g.addLink(3, 7, 7);
		notSimpleGraph->g.addLink(3, 8, 4);
		notSimpleGraph->g.addLink(4, 3, 5, true);
		notSimpleGraph->g.addLink(5, 5, 0, true);
		notSimpleGraph->g.addLink(5, 6, 1, true);
		notSimpleGraph->g.addLink(5, 7, 6);
		notSimpleGraph->g.addLink(5, 10, 5, true);
		notSimpleGraph->g.addLink(6, 7, 4, true);
		notSimpleGraph->g.addLink(7, 7, 0);
		notSimpleGraph->g.addLink(7, 7, 2, true);
		notSimpleGraph->g.addLink(7, 5, 4, true);
		notSimpleGraph->g.addLink(7, 8, 5);
		notSimpleGraph->g.addLink(7, 8, 3);
		notSimpleGraph->g.addLink(8, 9, 10);
		notSimpleGraph->g.addLink(9, 8, 7, true);
		notSimpleGraph->g.addLink(10, 0, 2, true);
		notSimpleGraph->g.addLink(12, 4, 3, true);

		notSimpleGraph->g.setNodeFinal(10);
		notSimpleGraph->g.setNodeFinal(11);
		notSimpleGraph->g.setNodeFinal(12);

		notSimpleGraph->startNode = 0;
		notSimpleGraph->closestFinalNode = 10;

		notSimpleGraph->infiniteCostCheck = true;
		notSimpleGraph->costs = { 0, 1, 9, 8, 3, 18, 10, 14, 12, 9, 23, 27, Graphe::INFINITE_COST };
		notSimpleGraph->paths = {
			deque < Graphe::IndexNoeud > { 0 },
			deque < Graphe::IndexNoeud > { 0, 1 },
			deque < Graphe::IndexNoeud > { 0, 4, 3, 2 },
			deque < Graphe::IndexNoeud > { 0, 4, 3 },
			deque < Graphe::IndexNoeud > { 0, 4 },
			deque < Graphe::IndexNoeud > { 0, 4, 3, 6, 7, 5 },
			deque < Graphe::IndexNoeud > { 0, 4, 3, 6 },
			deque < Graphe::IndexNoeud > { 0, 4, 3, 6, 7 },
			deque < Graphe::IndexNoeud > { 0, 4, 3, 8 },
			deque < Graphe::IndexNoeud > { 0, 1, 9 },
			deque < Graphe::IndexNoeud > { 0, 4, 3, 6, 7, 5, 10 },
			deque < Graphe::IndexNoeud > { 0, 11 },
			deque < Graphe::IndexNoeud > { 12 } };
		notSimpleGraph->finishTest();
	}
	void LittleGraphsInit(unsigned int testRefsCount)
	{
		emptyGraph = new ShortestPathTest<>(0, testRefsCount);
		emptyGraph->startNode = 0;
		emptyGraph->closestFinalNode = 0;
		emptyGraph->costs = { Graphe::INFINITE_COST };
		emptyGraph->finishTest();

		oneNodeGraph = new ShortestPathTest<>(*emptyGraph);
		oneNodeGraph->g.addNodes();
		oneNodeGraph->costs = { 0 };
		oneNodeGraph->paths.push_back(deque < Graphe::IndexNoeud > { 0 });
		oneNodeGraph->finishTest();

		twoNodesGraph = new ShortestPathTest<>(*oneNodeGraph);
		twoNodesGraph->g.addNodes();
		twoNodesGraph->g.addLink(0, 1, 3, true);
		twoNodesGraph->g.setNodeFinal(0, false);
		twoNodesGraph->startNode = 0;
		twoNodesGraph->closestFinalNode = 1;
		twoNodesGraph->costs.push_back(3);
		twoNodesGraph->paths.push_back(deque < Graphe::IndexNoeud > { 0, 1 });
		twoNodesGraph->finishTest();

		noFinalNodeGraph = new ShortestPathTest<>(*emptyGraph);
		noFinalNodeGraph->g.addNodes(3);
		noFinalNodeGraph->g.addLink(0, 1, 1, true);
		noFinalNodeGraph->g.addLink(0, 2, 3, true);
		noFinalNodeGraph->g.addLink(1, 2, 1, true);
		noFinalNodeGraph->startNode = 0;
		noFinalNodeGraph->closestFinalNode = 3;
		noFinalNodeGraph->costs = { 0, 1, 2, Graphe::INFINITE_COST };
		noFinalNodeGraph->paths = {
			deque < Graphe::IndexNoeud > { 0 },
			deque < Graphe::IndexNoeud > { 0, 1 },
			deque < Graphe::IndexNoeud > { 0, 1, 2 },
			deque < Graphe::IndexNoeud > { } };
		noFinalNodeGraph->finishTest();
	}
	void Algo2GraphInit(unsigned int testRefsCount)
	{
		// Exemple tiré du cours d'Algorithmique 2 (Ensimag 1A) :
		algo2Graph = new ShortestPathTest<>(8, testRefsCount);
		algo2Graph->g.addLink(7, 6, 1, true);
		algo2Graph->g.addLink(7, 5, 1, true);
		algo2Graph->g.addLink(6, 3, 2, true);
		algo2Graph->g.addLink(5, 3, 1, true);
		algo2Graph->g.addLink(4, 2, 1, true);
		algo2Graph->g.addLink(3, 1, 1, true);
		algo2Graph->g.addLink(2, 1, 1, true);
		algo2Graph->g.addLink(1, 0, 1, true);

		algo2Graph->startNode = 7;
		algo2Graph->closestFinalNode = 0;

		algo2Graph->infiniteCostCheck = true;
		algo2Graph->costs = { 4, 3, Graphe::INFINITE_COST, 2, Graphe::INFINITE_COST, 1, 1, 0 };
		algo2Graph->paths = {
			deque < Graphe::IndexNoeud > { 7, 5, 3, 1, 0 },
			deque < Graphe::IndexNoeud > { 7, 5, 3, 1 },
			deque < Graphe::IndexNoeud > { 2 },
			deque < Graphe::IndexNoeud > { 7, 5, 3 },
			deque < Graphe::IndexNoeud > { 4 },
			deque < Graphe::IndexNoeud > { 7, 5 },
			deque < Graphe::IndexNoeud > { 7, 6 },
			deque < Graphe::IndexNoeud > { 7 } };
		algo2Graph->finishTest();
	}
	void ROGraphInit(unsigned int testRefsCount)
	{
		// Exemple tiré du cours de Rercherche Opérationnelle (Ensimag 1A) :
		roGraph = new ShortestPathTest<>(10, testRefsCount);
		roGraph->g.addLink(0, 2, 1, true);
		roGraph->g.addLink(0, 3, 1, true);
		roGraph->g.addLink(1, 3, 1, true);
		roGraph->g.addLink(2, 5, 1, true);
		roGraph->g.addLink(2, 6, 1, true);
		roGraph->g.addLink(3, 4, 1, true);
		roGraph->g.addLink(3, 5, 2, true);
		roGraph->g.addLink(3, 7, 1, true);
		roGraph->g.addLink(5, 6, 1, true);
		roGraph->g.addLink(5, 7, 1, true);

		// Ajoute deux noeuds supplémentaires non connectés au reste du graphe
		roGraph->g.addLink(8, 9, -1, true);

		roGraph->startNode = 0;
		roGraph->closestFinalNode = 7;

		roGraph->infiniteCostCheck = true;
		roGraph->costs = { 0, Graphe::INFINITE_COST, 1, 1, 2, 2, 2, 2, Graphe::INFINITE_COST, Graphe::INFINITE_COST };
		roGraph->paths = {
			deque < Graphe::IndexNoeud > { 0 },
			deque < Graphe::IndexNoeud > { 1 },
			deque < Graphe::IndexNoeud > { 0, 2 },
			deque < Graphe::IndexNoeud > { 0, 3 },
			deque < Graphe::IndexNoeud > { 0, 3, 4 },
			deque < Graphe::IndexNoeud > { 0, 2, 5 },
			deque < Graphe::IndexNoeud > { 0, 2, 6 },
			deque < Graphe::IndexNoeud > { 0, 3, 7 },
			deque < Graphe::IndexNoeud > { 8 },
			deque < Graphe::IndexNoeud > { 9 } };
		roGraph->finishTest();
	}
	void BigCostPathsInit(unsigned int testRefsCount)
	{
		// Dans ce graphe, le coût du chemin le plus long depuis le noeud de départ
		// jusqu'au noeud final dépasse la valeur maximale pouvant être contenue
		// dans le type Cout, mais ce n'est pas le cas pour le coût du chemin le plus court.
		bigCostPaths = new ShortestPathTest<signed char, signed char>(127, testRefsCount);
		bigCostPaths->g.addLink(16, 68, 6, true);
		bigCostPaths->g.addLink(68, 52, 3, true);
		bigCostPaths->g.addLink(52, 110, 10, true);
		for (int i = 0; i < 127 - 1; i++)
			bigCostPaths->g.addLink((signed char)i, (signed char)(i + 1), 2, false);

		bigCostPaths->startNode = 0;
		bigCostPaths->closestFinalNode = 127;

		for (int i = 0; i <= 16; i++)
			bigCostPaths->costs[i] = (signed char)(2 * i);
		bigCostPaths->costs[68] = bigCostPaths->costs[16] + 6;
		bigCostPaths->costs[52] = bigCostPaths->costs[68] + 3;
		bigCostPaths->costs[110] = bigCostPaths->costs[52] + 10;
		for (int i = 111; i < 127; i++)
			bigCostPaths->costs[i] = bigCostPaths->costs[110] + (signed char)(2 * (i - 110));

		for (int i = 1; i <= 16; i++)
		{
			bigCostPaths->paths[i] = bigCostPaths->paths[i - 1];
			bigCostPaths->paths[i].push_back((signed char)i);
		}
		bigCostPaths->paths[68] = bigCostPaths->paths[16];
		bigCostPaths->paths[68].push_back(68);
		bigCostPaths->paths[52] = bigCostPaths->paths[68];
		bigCostPaths->paths[52].push_back(52);
		bigCostPaths->paths[110] = bigCostPaths->paths[52];
		bigCostPaths->paths[110].push_back(110);
		bigCostPaths->paths[110] = bigCostPaths->paths[52];
		bigCostPaths->paths[110].push_back(110);
		for (int i = 111; i < 127; i++)
		{
			bigCostPaths->paths[i] = bigCostPaths->paths[i - 1];
			bigCostPaths->paths[i].push_back((signed char)i);
		}

		bigCostPaths->finishTest();
	}
	void ManyNullCostLinksInit(unsigned int testRefsCount)
	{
		// Constante permettant de modifier rapidement le coût des liens nuls
		// à une valeur non nulle.
		constexpr int nullCost = 0;
		constexpr size_t nodesCount = 40;
		manyNullCostLinks = new ShortestPathTest<>(nodesCount, testRefsCount);

		size_t middleNode = nodesCount * 3 / 4;
		manyNullCostLinks->g.addLink(0, 1, 100);
		manyNullCostLinks->g.addLink(middleNode, nodesCount - 1, 100);
		for (int i = 1; i < nodesCount - 1; i++)
			for (int j = 1; j < nodesCount - 1; j++)
				manyNullCostLinks->g.addLink(i, j, nullCost);

		manyNullCostLinks->startNode = 0;
		manyNullCostLinks->closestFinalNode = nodesCount - 1;

		manyNullCostLinks->costs[0] = 0;
		for (int i = 1; i < nodesCount - 1; i++)
			manyNullCostLinks->costs[i] = 100 + nullCost;
		manyNullCostLinks->costs[nodesCount - 1] = 200;

		manyNullCostLinks->paths[0] = { 0 };
		for (int i = 1; i < nodesCount - 1; i++)
			manyNullCostLinks->paths[i] = { 0, (size_t)i };
		manyNullCostLinks->paths[nodesCount - 1] = { 0, middleNode, nodesCount - 1 };

		manyNullCostLinks->finishTest();
	}

	TEST_MODULE_INITIALIZE(ShortestPathsTestsInit)
	{
		SimpleTestInit(7);
		LittleMazeInit(6);
		NegativeLinksGraphInit(4);
		NegativeCycleGraphInit(6);
		NegativeLoopGraphInit(7);
		NotSimpleGraphInit(7);
		LittleGraphsInit(8);
		Algo2GraphInit(8);
		ROGraphInit(8);
		BigCostPathsInit(8);
		ManyNullCostLinksInit(8);

		// Pour les tests de RandomizedAlgoHelpersTests
		srand((unsigned int)time(NULL));
	}
	TEST_MODULE_CLEANUP(ShortestPathsTestsCleanup)
	{
		safeTestDelete(simpleTest);
		safeTestDelete(littleMaze);
		safeTestDelete(negativeLinksGraph);
		safeTestDelete(negativeCycleGraph);
		safeTestDelete(negativeLoopGraph);
		safeTestDelete(notSimpleGraph);
		safeTestDelete(emptyGraph);
		safeTestDelete(oneNodeGraph);
		safeTestDelete(twoNodesGraph);
		safeTestDelete(noFinalNodeGraph);
		safeTestDelete(algo2Graph);
		safeTestDelete(roGraph);
		safeTestDelete(bigCostPaths);
		safeTestDelete(manyNullCostLinks);
	}

	TEST_CLASS(DijkstraTests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(simpleTest, Dijkstra, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(littleMaze, Dijkstra, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(negativeCycleGraph, Dijkstra, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(negativeLoopGraph, Dijkstra, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(notSimpleGraph, Dijkstra, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(emptyGraph, Dijkstra, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(oneNodeGraph, Dijkstra, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(twoNodesGraph, Dijkstra, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(noFinalNodeGraph, Dijkstra, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(algo2Graph, Dijkstra, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(roGraph, Dijkstra, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(bigCostPaths, Dijkstra, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(manyNullCostLinks, Dijkstra, runSpfTest_AllNodes);
	};

	TEST_CLASS(AStarTests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(simpleTest, AStar, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(littleMaze, AStar, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(negativeCycleGraph, AStar, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(negativeLoopGraph, AStar, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(notSimpleGraph, AStar, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(emptyGraph, AStar, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(oneNodeGraph, AStar, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(twoNodesGraph, AStar, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(noFinalNodeGraph, AStar, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(algo2Graph, AStar, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(roGraph, AStar, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(bigCostPaths, AStar, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(manyNullCostLinks, AStar, runSpfTest_ClosestFinalNode);
	};

	TEST_CLASS(BellmanTests)
	{
		TEST_METHOD(Bellman_ImpossibleTopologicalOrder)
		{
			// Exemple tiré du cours de Rercherche Opérationnelle (Ensimag 1A) :
			Graphe g(6);
			g.addLink(0, 1, 2, true);
			g.addLink(0, 2, 8, true);
			g.addLink(1, 2, 5, true);
			g.addLink(1, 3, 1, true);
			g.addLink(2, 4, 1, true);
			g.addLink(3, 2, 2, true);
			g.addLink(3, 4, 4, true);
			g.addLink(3, 5, 1, true);
			g.addLink(4, 1, 3, true);
			g.addLink(4, 5, 1, true);

			Bellman<Graphe> bm(g);
			bm.computeShortestPathsFrom(0);
			for (Graphe::IndexNoeud i = 0; i < g.size(); i++)
				Assert::IsFalse(bm.canReachNode(i));
		}

		SHORTEST_PATH_TEST_METHOD(negativeLoopGraph, Bellman, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(emptyGraph, Bellman, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(oneNodeGraph, Bellman, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(twoNodesGraph, Bellman, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(noFinalNodeGraph, Bellman, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(algo2Graph, Bellman, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(roGraph, Bellman, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(bigCostPaths, Bellman, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(manyNullCostLinks, Bellman, runSpfTest_AllNodes);
	};

	TEST_CLASS(DFS_Tests)
	{
	public:
#ifndef DFS_NO_CYCLE_DETECTION
		SHORTEST_PATH_TEST_METHOD(simpleTest, DFS_ShortestPath, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(littleMaze, DFS_ShortestPath, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(negativeLinksGraph, DFS_ShortestPath, runSpfTest_ClosestFinalNode);

#ifdef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
		SHORTEST_PATH_TEST_METHOD(negativeCycleGraph, DFS_ShortestPath, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(negativeLoopGraph, DFS_ShortestPath, runSpfTest_ClosestFinalNode);
#endif

		SHORTEST_PATH_TEST_METHOD(notSimpleGraph, DFS_ShortestPath, runSpfTest_ClosestFinalNode);
#endif

		SHORTEST_PATH_TEST_METHOD(emptyGraph, DFS_ShortestPath, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(oneNodeGraph, DFS_ShortestPath, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(twoNodesGraph, DFS_ShortestPath, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(noFinalNodeGraph, DFS_ShortestPath, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(algo2Graph, DFS_ShortestPath, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(roGraph, DFS_ShortestPath, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(bigCostPaths, DFS_ShortestPath, runSpfTest_ClosestFinalNode);
		SHORTEST_PATH_TEST_METHOD(manyNullCostLinks, DFS_ShortestPath, runSpfTest_ClosestFinalNode);
	};

	TEST_CLASS(BFS_Tests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(simpleTest, BFS_ShortestPath, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(littleMaze, BFS_ShortestPath, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(negativeCycleGraph, BFS_ShortestPath, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(negativeLoopGraph, BFS_ShortestPath, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(notSimpleGraph, BFS_ShortestPath, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(emptyGraph, BFS_ShortestPath, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(oneNodeGraph, BFS_ShortestPath, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(twoNodesGraph, BFS_ShortestPath, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(noFinalNodeGraph, BFS_ShortestPath, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(algo2Graph, BFS_ShortestPath, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(roGraph, BFS_ShortestPath, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(bigCostPaths, BFS_ShortestPath, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(manyNullCostLinks, BFS_ShortestPath, runSpfTest_AllNodes);
	};

	TEST_CLASS(BellmanFordTests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(simpleTest, BellmanFord, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(littleMaze, BellmanFord, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(negativeLinksGraph, BellmanFord, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(negativeCycleGraph, BellmanFord, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(negativeLoopGraph, BellmanFord, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(notSimpleGraph, BellmanFord, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(emptyGraph, BellmanFord, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(oneNodeGraph, BellmanFord, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(twoNodesGraph, BellmanFord, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(noFinalNodeGraph, BellmanFord, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(algo2Graph, BellmanFord, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(roGraph, BellmanFord, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(bigCostPaths, BellmanFord, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(manyNullCostLinks, BellmanFord, runSpfTest_AllNodes);
	};

	TEST_CLASS(BellmanFordYenTests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(simpleTest, BellmanFordYen, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(littleMaze, BellmanFordYen, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(negativeLinksGraph, BellmanFordYen, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(negativeCycleGraph, BellmanFordYen, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(negativeLoopGraph, BellmanFordYen, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(notSimpleGraph, BellmanFordYen, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(emptyGraph, BellmanFordYen, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(oneNodeGraph, BellmanFordYen, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(twoNodesGraph, BellmanFordYen, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(noFinalNodeGraph, BellmanFordYen, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(algo2Graph, BellmanFordYen, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(roGraph, BellmanFordYen, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(bigCostPaths, BellmanFordYen, runSpfTest_AllNodes);
		SHORTEST_PATH_TEST_METHOD(manyNullCostLinks, BellmanFordYen, runSpfTest_AllNodes);
	};

	TEST_CLASS(FloydWarshallTests)
	{
		SHORTEST_PATH_TEST_METHOD(simpleTest, FloydWarshall, runSpfTest_AllPairsOfNodes);

		// Floyd-Warshall est extrêmement lent pour ce test !
		//SHORTEST_PATH_TEST_METHOD(littleMaze, FloydWarshall, runSpfTest_AllPairsOfNodes);

		SHORTEST_PATH_TEST_METHOD(negativeLinksGraph, FloydWarshall, runSpfTest_AllPairsOfNodes);
		//SHORTEST_PATH_TEST_METHOD(negativeCycleGraph, FloydWarshall, runSpfTest_AllPairsOfNodes);
		//SHORTEST_PATH_TEST_METHOD(negativeLoopGraph, FloydWarshall, runSpfTest_AllPairsOfNodes);
		SHORTEST_PATH_TEST_METHOD(notSimpleGraph, FloydWarshall, runSpfTest_AllPairsOfNodes);
		SHORTEST_PATH_TEST_METHOD(emptyGraph, FloydWarshall, runSpfTest_AllPairsOfNodes);
		SHORTEST_PATH_TEST_METHOD(oneNodeGraph, FloydWarshall, runSpfTest_AllPairsOfNodes);
		SHORTEST_PATH_TEST_METHOD(twoNodesGraph, FloydWarshall, runSpfTest_AllPairsOfNodes);
		SHORTEST_PATH_TEST_METHOD(noFinalNodeGraph, FloydWarshall, runSpfTest_AllPairsOfNodes);
		SHORTEST_PATH_TEST_METHOD(algo2Graph, FloydWarshall, runSpfTest_AllPairsOfNodes);
		SHORTEST_PATH_TEST_METHOD(roGraph, FloydWarshall, runSpfTest_AllPairsOfNodes);
		SHORTEST_PATH_TEST_METHOD(bigCostPaths, FloydWarshall, runSpfTest_AllPairsOfNodes);
		SHORTEST_PATH_TEST_METHOD(manyNullCostLinks, FloydWarshall, runSpfTest_AllPairsOfNodes);
	};
}
