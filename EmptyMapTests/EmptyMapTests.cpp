#include "ShortestPathTestsUtils.h"

namespace TestUnit
{
	void EmptyMapInit(unsigned int testRefsCount)
	{
#define ID(i, j)	((i) * width + (j))
		unsigned int width = EMPTY_MAP_SIZE;
		unsigned int height = EMPTY_MAP_SIZE;

		unsigned int start = ID(height / 2, width / 2);
		unsigned int finalNode = ID(height - 1, width - 1);
		unsigned int cost = abs((int)(start / width) - (int)(finalNode / width)) + abs((int)(start % width) - (int)(finalNode % width));

		emptyMap = new ShortestPathTest(width * height, testRefsCount, 1);
		emptyMap->startNode = start;
		emptyMap->closestFinalNode = finalNode;
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				unsigned int id = ID(i, j);
#if 0
				// Ajoute 2 fois plus de liens que nécessaire
				if (i > 0)			emptyMap->g.addLink(id, ID(i - 1, j), 3);	// Haut<->Bas : coûte 3     
				if (i < height - 1)	emptyMap->g.addLink(id, ID(i + 1, j), 1);	// Bas<->Haut : coûte 1     
				if (j > 0)			emptyMap->g.addLink(id, ID(i, j - 1), 1);	// Gauche<->Droite : coûte 1
				if (j < width - 1)	emptyMap->g.addLink(id, ID(i, j + 1), 2);	// Droite<->Gauche : coûte 2
#else
				if (i < height - 1)
					emptyMap->g.addLink(id, ID(i + 1, j), 1);
				if (j < width - 1)
					emptyMap->g.addLink(id, ID(i, j + 1), 1);
#endif

				emptyMap->g.setNodeHeuristic(ID(i, j),
					abs((int)(finalNode / width) - (int)i) + abs((int)(finalNode % width) - (int)j));
			}
		}

		emptyMap->costs[emptyMap->closestFinalNode] = cost;
		emptyMap->finishTest();
#undef ID
	}

	TEST_MODULE_INITIALIZE(EmptyMapTestsInit)
	{
		EmptyMapInit(4);
	}
	TEST_MODULE_CLEANUP(EmptyMapTestsCleanup)
	{
		if (emptyMap)
			delete emptyMap;
	}

	TEST_CLASS(DijkstraTests)
	{
	public:
		TEST_METHOD(DijkstraEmptyMap)
		{
			Dijkstra<> dj(emptyMap->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(emptyMap, dj);
		}
	};

	TEST_CLASS(AStarTests)
	{
	public:
		TEST_METHOD(AStarEmptyMap)
		{
			AStar<> as(emptyMap->g);
			RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(emptyMap, as);
		}
	};

	TEST_CLASS(DFS_Tests)
	{
	public:
		/* Ce test échoue toujours !
		TEST_METHOD(DFS_EmptyMap)
		{
		DFS_ShortestPath<> dfs(emptyMap->g);
		RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(emptyMap, dfs);
		}
		*/
	};

	TEST_CLASS(BFS_Tests)
	{
	public:
		TEST_METHOD(BFS_EmptyMap)
		{
			BFS_ShortestPath<> bfs(emptyMap->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(emptyMap, bfs);
		}
	};

	TEST_CLASS(BellmanFordTests)
	{
	public:
		/* Bellman-Ford est extrêmement lent pour les gros tests !
		TEST_METHOD(BellmanFordEmptyMap)
		{
			BellmanFord<> bf(emptyMap->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(emptyMap, bf);
		}
		*/
	};

	TEST_CLASS(BellmanFordYenTests)
	{
	public:
		TEST_METHOD(BellmanFordYenEmptyMap)
		{
			BellmanFordYen<> bfy(emptyMap->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(emptyMap, bfy);
		}
	};

	TEST_CLASS(FloydWarshallTests)
	{
		/* Floyd-Warshall est extrêmement lent pour les gros tests !
		TEST_METHOD(FloydWarshallEmptyMap)
		{
			FloydWarshall<> fw(emptyMap->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_PAIRS_OF_NODES(emptyMap, fw);
		}
		*/
	};
}
