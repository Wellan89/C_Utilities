#include "ShortestPathTestsUtils.h"

namespace TestUnit
{
	void EmptyMapInit(unsigned int testRefsCount)
	{
#define ID(i, j)	(Graph<>::IndexNoeud)((i) * width + (j))
		Graph<>::IndexNoeud width = EMPTY_MAP_SIZE;
		Graph<>::IndexNoeud height = EMPTY_MAP_SIZE;

		Graph<>::IndexNoeud start = ID(height / 2, width / 2);
		Graph<>::IndexNoeud finalNode = ID(height - 1, width - 1);
		unsigned int cost = abs((long)(start / width) - (long)(finalNode / width)) + abs((long)(start % width) - (long)(finalNode % width));

		emptyMap = new ShortestPathTest<>(width * height, testRefsCount, 1);
		emptyMap->startNode = start;
		emptyMap->closestFinalNode = finalNode;
		for (Graph<>::IndexNoeud i = 0; i < height; i++)
		{
			for (Graph<>::IndexNoeud j = 0; j < width; j++)
			{
				Graph<>::IndexNoeud id = ID(i, j);
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
					(Graph<>::Cout)(abs((long)(finalNode / width) - (long)i) + abs((long)(finalNode % width) - (long)j)));
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
