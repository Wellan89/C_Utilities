#include "ShortestPathTestsUtils.h"

namespace TestUnit
{
	void BigMazeInit(unsigned int testRefsCount)
	{
#define ID(i, j)	(Graph<>::IndexNoeud)((i) * width + (j))
		Graph<>::IndexNoeud width = BIG_MAZE_SIZE;
		Graph<>::IndexNoeud height = BIG_MAZE_SIZE;

		Graph<>::Cout cost = (BIG_MAZE_SIZE / 2) + BIG_MAZE_SIZE - 4;
		deque<Graph<>::IndexNoeud> path;
		for (Graph<>::IndexNoeud i = (BIG_MAZE_SIZE / 2); i < BIG_MAZE_SIZE - 2; i++)
			path.push_back(ID(i, 1));
		for (Graph<>::IndexNoeud j = 1; j < BIG_MAZE_SIZE - 1; j++)
			path.push_back(ID(BIG_MAZE_SIZE - 2, j));

		bigMaze = new ShortestPathTest<>(width * height, testRefsCount, 1);
		bigMaze->startNode = path[0];
		bigMaze->closestFinalNode = path[path.size() - 1];

		for (Graph<>::IndexNoeud i = 1; i < height - 1; i++)
		{
			for (Graph<>::IndexNoeud j = 1; j < width - 1; j++)
			{
				Graph<>::IndexNoeud id = ID(i, j);
				if (j == 1 && i != height - 1)
					bigMaze->g.addLink(id, ID(i + 1, j), 1);
				if (i % 2 == 1 && j != width - 1)
					bigMaze->g.addLink(id, ID(i, j + 1), 1);
			}
		}
		for (Graph<>::IndexNoeud i = 0; i < height; i++)
			for (Graph<>::IndexNoeud j = 0; j < width; j++)
				bigMaze->g.setNodeHeuristic(ID(i, j),
					(Graph<>::Cout)(abs((long)(bigMaze->closestFinalNode / width) - (long)i)
					+ abs((long)(bigMaze->closestFinalNode % width) - (long)j)));

		bigMaze->costs[bigMaze->closestFinalNode] = cost;
		bigMaze->paths[bigMaze->closestFinalNode] = path;
		bigMaze->finishTest();
#undef ID
	}

	TEST_MODULE_INITIALIZE(BigMazeTestsInit)
	{
		BigMazeInit(5);
	}
	TEST_MODULE_CLEANUP(BigMazeTestsCleanup)
	{
		if (bigMaze)
			delete bigMaze;
	}

	TEST_CLASS(DijkstraTests)
	{
	public:
		TEST_METHOD(DijkstraBigMaze)
		{
			Dijkstra<> dj(bigMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(bigMaze, dj);
		}
	};

	TEST_CLASS(AStarTests)
	{
	public:
		TEST_METHOD(AStarBigMaze)
		{
			AStar<> as(bigMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(bigMaze, as);
		}
	};

	TEST_CLASS(DFS_Tests)
	{
	public:
		TEST_METHOD(DFS_BigMaze)
		{
			DFS_ShortestPath<> dfs(bigMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(bigMaze, dfs);
		}
	};

	TEST_CLASS(BFS_Tests)
	{
	public:
		TEST_METHOD(BFS_BigMaze)
		{
			BFS_ShortestPath<> bfs(bigMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(bigMaze, bfs);
		}
	};

	TEST_CLASS(BellmanFordTests)
	{
	public:
		/* Bellman-Ford est extrêmement lent pour les gros tests !
		TEST_METHOD(BellmanFordBigMaze)
		{
			BellmanFord<> bf(bigMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(bigMaze, bf);
		}
		*/
	};

	TEST_CLASS(BellmanFordYenTests)
	{
	public:
		TEST_METHOD(BellmanFordYenBigMaze)
		{
			BellmanFordYen<> bfy(bigMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(bigMaze, bfy);
		}
	};

	TEST_CLASS(FloydWarshallTests)
	{
		/* Floyd-Warshall est extrêmement lent pour les gros tests !
		TEST_METHOD(FloydWarshallBigMaze)
		{
			FloydWarshall<> fw(bigMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_PAIRS_OF_NODES(bigMaze, fw);
		}
		*/
	};
}
