#include "ShortestPathTestsUtils.h"

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
		negativeLinksGraph->g.addLink(6, 5 , -2, true);
		negativeLinksGraph->g.addLink(6, 7, 2);
		negativeLinksGraph->g.addLink(7, 8, 8);
		negativeLinksGraph->g.addLink(10, 10, -1);

		negativeLinksGraph->g.setNodeFinal(5);
		negativeLinksGraph->g.setNodeFinal(10);

		negativeLinksGraph->startNode = 0;
		negativeLinksGraph->closestFinalNode = 5;

		negativeLinksGraph->infiniteCostCheck = true;
		negativeLinksGraph->costs = { 0, 1, 2, 2, 3, 2, 4, 5, 0, -5, Graphe::INFINITE_COST() };
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
		negativeCycleGraph->costs = { Graphe::INFINITE_COST(), Graphe::INFINITE_COST(),
										Graphe::INFINITE_COST(), Graphe::INFINITE_COST(),
										Graphe::INFINITE_COST(), Graphe::INFINITE_COST(),
										Graphe::INFINITE_COST(), Graphe::INFINITE_COST() };
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
		negativeLoopGraph->costs = { Graphe::INFINITE_COST(), Graphe::INFINITE_COST(),
										Graphe::INFINITE_COST(), Graphe::INFINITE_COST() };
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
		notSimpleGraph->costs = { 0, 1, 9, 8, 3, 18, 10, 14, 12, 9, 23, 27, Graphe::INFINITE_COST() };
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
		algo2Graph->costs = { 4, 3, Graphe::INFINITE_COST(), 2, Graphe::INFINITE_COST(), 1, 1, 0 };
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
		roGraph->costs = { 0, Graphe::INFINITE_COST(), 1, 1, 2, 2, 2, 2, Graphe::INFINITE_COST(), Graphe::INFINITE_COST() };
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

	TEST_MODULE_INITIALIZE(ShortestPathsTestsInit)
	{
		SimpleTestInit(7);
		LittleMazeInit(6);
		NegativeLinksGraphInit(4);
		NegativeCycleGraphInit(6);
		NegativeLoopGraphInit(7);
		NotSimpleGraphInit(7);
		Algo2GraphInit(8);
		ROGraphInit(8);
	}
	TEST_MODULE_CLEANUP(ShortestPathsTestsCleanup)
	{
		SAFE_TEST_DELETE(simpleTest);
		SAFE_TEST_DELETE(littleMaze);
		SAFE_TEST_DELETE(negativeLinksGraph);
		SAFE_TEST_DELETE(negativeCycleGraph);
		SAFE_TEST_DELETE(negativeLoopGraph);
		SAFE_TEST_DELETE(notSimpleGraph);
		SAFE_TEST_DELETE(algo2Graph);
		SAFE_TEST_DELETE(roGraph);
	}

	TEST_CLASS(DijkstraTests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(simpleTest, Dijkstra, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(littleMaze, Dijkstra, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(negativeCycleGraph, Dijkstra, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(negativeLoopGraph, Dijkstra, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(notSimpleGraph, Dijkstra, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(algo2Graph, Dijkstra, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(roGraph, Dijkstra, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
	};

	TEST_CLASS(AStarTests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(simpleTest, AStar, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
		SHORTEST_PATH_TEST_METHOD(littleMaze, AStar, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
		SHORTEST_PATH_TEST_METHOD(negativeCycleGraph, AStar, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
		SHORTEST_PATH_TEST_METHOD(negativeLoopGraph, AStar, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
		SHORTEST_PATH_TEST_METHOD(notSimpleGraph, AStar, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
		SHORTEST_PATH_TEST_METHOD(algo2Graph, AStar, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
		SHORTEST_PATH_TEST_METHOD(roGraph, AStar, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
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

		SHORTEST_PATH_TEST_METHOD(negativeLoopGraph, Bellman, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(algo2Graph, Bellman, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(roGraph, Bellman, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
	};

	TEST_CLASS(DFS_Tests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(simpleTest, DFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
		SHORTEST_PATH_TEST_METHOD(littleMaze, DFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
		SHORTEST_PATH_TEST_METHOD(negativeLinksGraph, DFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
		SHORTEST_PATH_TEST_METHOD(negativeCycleGraph, DFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
		SHORTEST_PATH_TEST_METHOD(negativeLoopGraph, DFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
		SHORTEST_PATH_TEST_METHOD(notSimpleGraph, DFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
		SHORTEST_PATH_TEST_METHOD(algo2Graph, DFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
		SHORTEST_PATH_TEST_METHOD(roGraph, DFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
	};

	TEST_CLASS(BFS_Tests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(simpleTest, BFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(littleMaze, BFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(negativeCycleGraph, BFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(negativeLoopGraph, BFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(notSimpleGraph, BFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(algo2Graph, BFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(roGraph, BFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
	};

	TEST_CLASS(BellmanFordTests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(simpleTest, BellmanFord, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(littleMaze, BellmanFord, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(negativeLinksGraph, BellmanFord, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(negativeCycleGraph, BellmanFord, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(negativeLoopGraph, BellmanFord, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(notSimpleGraph, BellmanFord, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(algo2Graph, BellmanFord, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(roGraph, BellmanFord, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
	};

	TEST_CLASS(BellmanFordYenTests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(simpleTest, BellmanFordYen, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(littleMaze, BellmanFordYen, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(negativeLinksGraph, BellmanFordYen, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(negativeCycleGraph, BellmanFordYen, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(negativeLoopGraph, BellmanFordYen, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(notSimpleGraph, BellmanFordYen, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(algo2Graph, BellmanFordYen, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
		SHORTEST_PATH_TEST_METHOD(roGraph, BellmanFordYen, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
	};

	TEST_CLASS(FloydWarshallTests)
	{
		SHORTEST_PATH_TEST_METHOD(simpleTest, FloydWarshall, RUN_SHORTEST_PATH_FINDER_TEST_ALL_PAIRS_OF_NODES);

		// Floyd-Warshall est extrêmement lent pour ce test !
		//SHORTEST_PATH_TEST_METHOD(littleMaze, FloydWarshall, RUN_SHORTEST_PATH_FINDER_TEST_ALL_PAIRS_OF_NODES);

		SHORTEST_PATH_TEST_METHOD(negativeLinksGraph, FloydWarshall, RUN_SHORTEST_PATH_FINDER_TEST_ALL_PAIRS_OF_NODES);
		SHORTEST_PATH_TEST_METHOD(notSimpleGraph, FloydWarshall, RUN_SHORTEST_PATH_FINDER_TEST_ALL_PAIRS_OF_NODES);
		SHORTEST_PATH_TEST_METHOD(algo2Graph, FloydWarshall, RUN_SHORTEST_PATH_FINDER_TEST_ALL_PAIRS_OF_NODES);
		SHORTEST_PATH_TEST_METHOD(roGraph, FloydWarshall, RUN_SHORTEST_PATH_FINDER_TEST_ALL_PAIRS_OF_NODES);
	};
}
