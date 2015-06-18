#include "ShortestPathTestsUtils.h"

namespace TestUnit
{
	void SimpleTestInit(unsigned int testRefsCount)
	{
#define CTI(c)	(Graph<long>::IndexNoeud)(c - 'a')
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

		const Graph<>::Cout heuristics[] = { 450, 400, 250, 300, 500, 300, 400, 150, 50, 0 };
		for (Graph<>::IndexNoeud i = 0; i <= CTI('j'); i++)
			simpleTest->g.setNodeHeuristic(i, heuristics[i]);

		simpleTest->costs = { 0, 85, 217, 503, 173, 165, 403, 320, 415, 499 };
		simpleTest->paths = {
			deque < Graph<>::IndexNoeud > { CTI('a') },
			deque < Graph<>::IndexNoeud > { CTI('a'), CTI('b') },
			deque < Graph<>::IndexNoeud > { CTI('a'), CTI('c') },
			deque < Graph<>::IndexNoeud > { CTI('a'), CTI('c'), CTI('h'), CTI('d') },
			deque < Graph<>::IndexNoeud > { CTI('a'), CTI('e') },
			deque < Graph<>::IndexNoeud > { CTI('a'), CTI('b'), CTI('f') },
			deque < Graph<>::IndexNoeud > { CTI('a'), CTI('c'), CTI('g') },
			deque < Graph<>::IndexNoeud > { CTI('a'), CTI('c'), CTI('h') },
			deque < Graph<>::IndexNoeud > { CTI('a'), CTI('b'), CTI('f'), CTI('i') },
			deque < Graph<>::IndexNoeud > { CTI('a'), CTI('b'), CTI('f'), CTI('i'), CTI('j') } };
		simpleTest->finishTest();
#undef CTI
	}
	void LittleMazeInit(unsigned int testRefsCount)
	{
#define ID(i, j)	(Graph<>::IndexNoeud)((i) * width + (j))
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
		const Graph<>::IndexNoeud width = map[0].size();
		const Graph<>::IndexNoeud height = map.size();

		const Graph<>::Cout cost = 12;
		const deque<Graph<>::IndexNoeud> path = { ID(5, 1), ID(6, 1), ID(7, 1), ID(8, 1), ID(9, 1),
			ID(9, 2), ID(9, 3), ID(9, 4), ID(9, 5), ID(9, 6), ID(9, 7), ID(9, 8), ID(9, 9) };

		littleMaze = new ShortestPathTest<>(width * height, testRefsCount);
		for (Graph<>::IndexNoeud i = 0; i < height; i++)
		{
			for (Graph<>::IndexNoeud j = 0; j < width; j++)
			{
				if (map[i][j] != '#')
				{
					Graph<>::IndexNoeud id = ID(i, j);

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
		for (Graph<>::IndexNoeud i = 0; i < height; i++)
			for (Graph<>::IndexNoeud j = 0; j < width; j++)
				littleMaze->g.setNodeHeuristic(ID(i, j),
					(Graph<>::Cout)(abs((long)(littleMaze->closestFinalNode / width) - (long)i)
					+ abs((long)(littleMaze->closestFinalNode % width) - (long)j)));

		littleMaze->costs[littleMaze->closestFinalNode] = cost;
		littleMaze->paths[littleMaze->closestFinalNode] = path;
		littleMaze->finishTest();
#undef ID
	}

	TEST_MODULE_INITIALIZE(ShortestPathsTestsInit)
	{
		SimpleTestInit(7);
		LittleMazeInit(6);
	}
	TEST_MODULE_CLEANUP(ShortestPathsTestsCleanup)
	{
		if (simpleTest)
			delete simpleTest;
		if (littleMaze)
			delete littleMaze;
	}

	TEST_CLASS(DijkstraTests)
	{
	public:
		TEST_METHOD(DijkstraSimpleTest)
		{
			Dijkstra<> dj(simpleTest->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(simpleTest, dj);
		}

		TEST_METHOD(DijkstraLittleMaze)
		{
			Dijkstra<> dj(littleMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(littleMaze, dj);
		}
	};

	TEST_CLASS(AStarTests)
	{
	public:
		TEST_METHOD(AStarSimpleTest)
		{
			AStar<> as(simpleTest->g);
			RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(simpleTest, as);
		}

		TEST_METHOD(AStarLittleMaze)
		{
			AStar<> as(littleMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(littleMaze, as);
		}
	};

	TEST_CLASS(BellmanTests)
	{
		TEST_METHOD(BellmanSimpleTest_Algo2)
		{
			// Exemple tiré du cours d'Algorithmique 2 (Ensimag 1A) :
			Graph<> g(8);
			g.addLink(7, 6, 1, true);
			g.addLink(7, 5, 1, true);
			g.addLink(6, 3, 2, true);
			g.addLink(5, 3, 1, true);
			g.addLink(4, 2, 1, true);
			g.addLink(3, 1, 1, true);
			g.addLink(2, 1, 1, true);
			g.addLink(1, 0, 1, true);

			Bellman<> bm(g);
			bm.computeShortestPathsFrom(7);

			const Graph<>::Cout costs[8] =
				{ 4, 3, Graph<>::INFINITE_COST(), 2, Graph<>::INFINITE_COST(), 1, 1, 0 };
			const deque<Graph<>::IndexNoeud> paths[8] = {
				deque < Graph<>::IndexNoeud > { 7, 5, 3, 1, 0 },
				deque < Graph<>::IndexNoeud > { 7, 5, 3, 1 },
				deque < Graph<>::IndexNoeud > { 2 },
				deque < Graph<>::IndexNoeud > { 7, 5, 3 },
				deque < Graph<>::IndexNoeud > { 4 },
				deque < Graph<>::IndexNoeud > { 7, 5 },
				deque < Graph<>::IndexNoeud > { 7, 6 },
				deque < Graph<>::IndexNoeud > { 7 } };
			for (Graph<>::IndexNoeud i = 0; i < g.size(); i++)
			{
				Assert::AreEqual((costs[i] != Graph<>::INFINITE_COST()), bm.canReachNode(i));
				Assert::AreEqual(costs[i], bm.getCostTo(i));
				Assert::AreEqual(paths[i], bm.getShortestPathTo(i));
				Assert::AreEqual(reverse_vect(paths[i]), bm.getReverseShortestPathTo(i));
			}
		}
		TEST_METHOD(BellmanSimpleTest_RO)
		{
			// Exemple tiré du cours de Rercherche Opérationnelle (Ensimag 1A) :
			Graph<> g(8);
			g.addLink(0, 2, 1, true);
			g.addLink(0, 3, 1, true);
			g.addLink(1, 3, 1, true);
			g.addLink(2, 5, 1, true);
			g.addLink(2, 6, 1, true);
			g.addLink(3, 4, 1, true);
			g.addLink(3, 5, 2, true);
			g.addLink(3, 7, 1, true);
			g.addLink(5, 6, 1, true);
			g.addLink(5, 7, 1, true);

			Bellman<> bm(g);
			bm.computeShortestPathsFrom(0);

			const Graph<>::Cout costs[8] =
				{ 0, Graph<>::INFINITE_COST(), 1, 1, 2, 2, 2, 2 };
			const deque<Graph<>::IndexNoeud> paths[8] = {
				deque < Graph<>::IndexNoeud > { 0 },
				deque < Graph<>::IndexNoeud > { 1 },
				deque < Graph<>::IndexNoeud > { 0, 2 },
				deque < Graph<>::IndexNoeud > { 0, 3 },
				deque < Graph<>::IndexNoeud > { 0, 3, 4 },
				deque < Graph<>::IndexNoeud > { 0, 2, 5 },
				deque < Graph<>::IndexNoeud > { 0, 2, 6 },
				deque < Graph<>::IndexNoeud > { 0, 3, 7 } };
			for (Graph<>::IndexNoeud i = 0; i < g.size(); i++)
			{
				Assert::AreEqual((costs[i] != Graph<>::INFINITE_COST()), bm.canReachNode(i));
				Assert::AreEqual(costs[i], bm.getCostTo(i));
				Assert::AreEqual(paths[i], bm.getShortestPathTo(i));
				Assert::AreEqual(reverse_vect(paths[i]), bm.getReverseShortestPathTo(i));
			}
		}
		TEST_METHOD(BellmanImpossibleTopologicalOrder)
		{
			// Exemple tiré du cours de Rercherche Opérationnelle (Ensimag 1A) :
			Graph<> g(6);
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

			Bellman<> bm(g);
			bm.computeShortestPathsFrom(0);
			for (Graph<>::IndexNoeud i = 0; i < g.size(); i++)
				Assert::IsFalse(bm.canReachNode(i));
		}
	};

	TEST_CLASS(DFS_Tests)
	{
	public:
		TEST_METHOD(DFS_SimpleTest)
		{
			DFS_ShortestPath<> dfs(simpleTest->g);
			RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(simpleTest, dfs);
		}

		TEST_METHOD(DFS_LittleMaze)
		{
			DFS_ShortestPath<> dfs(littleMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(littleMaze, dfs);
		}
	};

	TEST_CLASS(BFS_Tests)
	{
	public:
		TEST_METHOD(BFS_SimpleTest)
		{
			BFS_ShortestPath<> bfs(simpleTest->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(simpleTest, bfs);
		}

		TEST_METHOD(BFS_LittleMaze)
		{
			BFS_ShortestPath<> bfs(littleMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(littleMaze, bfs);
		}
	};

	TEST_CLASS(BellmanFordTests)
	{
	public:
		TEST_METHOD(BellmanFordSimpleTest)
		{
			BellmanFord<> bf(simpleTest->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(simpleTest, bf);
		}

		TEST_METHOD(BellmanFordLittleMaze)
		{
			BellmanFord<> bf(littleMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(littleMaze, bf);
		}
	};

	TEST_CLASS(BellmanFordYenTests)
	{
	public:
		TEST_METHOD(BellmanFordYenSimpleTest)
		{
			BellmanFordYen<> bfy(simpleTest->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(simpleTest, bfy);
		}

		TEST_METHOD(BellmanFordYenLittleMaze)
		{
			BellmanFordYen<> bfy(littleMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(littleMaze, bfy);
		}
	};

	TEST_CLASS(FloydWarshallTests)
	{
		TEST_METHOD(FloydWarshallSimpleTest)
		{
			FloydWarshall<> fw(simpleTest->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_PAIRS_OF_NODES(simpleTest, fw);
		}

		/* Floyd-Warshall est extrêmement lent pour ce test !
		TEST_METHOD(FloydWarshallLittleMaze)
		{
			FloydWarshall<> fw(littleMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_PAIRS_OF_NODES(littleMaze, fw);
		}
		*/
	};
}
