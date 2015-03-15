#include "CppUnitTest.h"
#include <cstdlib>
#include <vector>
#include <list>
#include <deque>

// ATTENTION : L'inclusion des .h ne marche pas ici : trouver pourquoi !
#include "..\C_Utilities\List.c"
#include "..\C_Utilities\Dijkstra.cpp"
#include "..\C_Utilities\AStar.cpp"

using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestUnit
{
	TEST_CLASS(ListTests)
	{
	public:
		TEST_METHOD(ListCreate)
		{
			List* l = list_create();
			Assert::AreNotEqual((List*)NULL, l);
			Assert::AreEqual(0u, list_getSize(l));
			Assert::AreEqual(true, list_isEmpty(l));
			list_delete(l);
		}

		TEST_METHOD(ListPushAndPop)
		{
			List* l = list_create();
			list_pushBack(l, 5);
			list_pushBack(l, 23);
			list_pushFront(l, 14);
			list_pushFront(l, 7);
			Assert::AreEqual(4u, list_getSize(l));
			Assert::AreEqual(false, list_isEmpty(l));
			Assert::AreEqual(7, list_front(l));
			Assert::AreEqual(23, list_back(l));
			
			list_popFront(l);
			list_popBack(l);
			Assert::AreEqual(14, list_front(l));
			Assert::AreEqual(5, list_back(l));

			list_popFront(l);
			list_popBack(l);
			Assert::AreEqual(0u, list_getSize(l));
			Assert::AreEqual(true, list_isEmpty(l));
			list_delete(l);
		}

	};

#ifdef _DEBUG
#define PATH_FINDERS_COMPUTE_LOOPS				800
#define PATH_FINDERS_PATH_RECONSTRUCTION_LOOPS	0

#define BIG_MAZE_SIZE							141		// Doit être impair
#else
#define PATH_FINDERS_COMPUTE_LOOPS				500000
#define PATH_FINDERS_PATH_RECONSTRUCTION_LOOPS	0

#define BIG_MAZE_SIZE							2401	// Doit être impair
#endif

#define ID(i, j)	((i) * width + (j))
	TEST_CLASS(DijkstraTests)
	{
	public:
		TEST_METHOD(DijkstraSimpleTest)
		{
#define CTI(c)	(c - 'a')

			// Wikipedia example test : http://fr.wikipedia.org/wiki/Algorithme_de_Dijkstra
			Graph g(CTI('j') + 1);
			g.addLink(CTI('a'), CTI('b'), 85);
			g.addLink(CTI('a'), CTI('c'), 217);
			g.addLink(CTI('a'), CTI('e'), 173);

			g.addLink(CTI('b'), CTI('f'), 80);

			g.addLink(CTI('c'), CTI('g'), 186);
			g.addLink(CTI('c'), CTI('h'), 103);

			g.addLink(CTI('d'), CTI('h'), 183);

			g.addLink(CTI('e'), CTI('j'), 502);

			g.addLink(CTI('f'), CTI('i'), 250);

			g.addLink(CTI('i'), CTI('j'), 84);

			Dijkstra<> dj(g);
			for (int i = 0; i < PATH_FINDERS_COMPUTE_LOOPS; i++)
				dj.computeShortestPathsFrom(0);
			for (int j = 0; j < PATH_FINDERS_PATH_RECONSTRUCTION_LOOPS; j++)
				dj.getShortestPathTo(CTI('j'));

			const unsigned int costs[] =
				{ 0, 85, 217, 503, 173, 165, 403, 320, 415, 499 };
			const deque<unsigned int> paths[] = {
				deque < unsigned int > { CTI('a') },
				deque < unsigned int > { CTI('a'), CTI('b') },
				deque < unsigned int > { CTI('a'), CTI('c') },
				deque < unsigned int > { CTI('a'), CTI('c'), CTI('h'), CTI('d') },
				deque < unsigned int > { CTI('a'), CTI('e') },
				deque < unsigned int > { CTI('a'), CTI('b'), CTI('f') },
				deque < unsigned int > { CTI('a'), CTI('c'), CTI('g') },
				deque < unsigned int > { CTI('a'), CTI('c'), CTI('h') },
				deque < unsigned int > { CTI('a'), CTI('b'), CTI('f'), CTI('i') },
				deque < unsigned int > { CTI('a'), CTI('b'), CTI('f'), CTI('i'), CTI('j') } };
			for (unsigned int i = 0; i <= CTI('j'); i++)
			{
				Assert::AreEqual(true, dj.canReachNode(i));
				Assert::AreEqual(costs[i], dj.getCostTo(i));
				Assert::AreEqual(paths[i], dj.getShortestPathTo(i));
			}
		}

		TEST_METHOD(DijkstraLittleMaze)
		{
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
			unsigned int width = map[0].size();
			unsigned int height = map.size();

			bool reachable = true;
			unsigned int cost = 12;
			deque<unsigned int> path = { ID(5, 1), ID(6, 1), ID(7, 1), ID(8, 1), ID(9, 1),
				ID(9, 2), ID(9, 3), ID(9, 4), ID(9, 5), ID(9, 6), ID(9, 7), ID(9, 8), ID(9, 9) };

			unsigned int start = (unsigned int)(-1);
			unsigned int finalNode = (unsigned int)(-1);

			Graph g(width * height);
			for (unsigned int i = 0; i < height; i++)
			{
				for (unsigned int j = 0; j < width; j++)
				{
					if (map[i][j] != '#')
					{
						unsigned int id = ID(i, j);

						if (i < height - 1 && map[i + 1][j] != '#')
							g.addLink(id, ID(i + 1, j), 1);
						if (j < width - 1 && map[i][j + 1] != '#')
							g.addLink(id, ID(i, j + 1), 1);

						if (map[i][j] == 'S')
							start = id;
						else if (map[i][j] == 'E')
						{
							finalNode = id;
							g.setNodeFinal(id);
						}
					}
				}
			}

			Dijkstra<> dj(g);
			for (int i = 0; i < PATH_FINDERS_COMPUTE_LOOPS; i++)
				dj.computeShortestPathsFrom(start);
			for (int j = 0; j < PATH_FINDERS_PATH_RECONSTRUCTION_LOOPS; j++)
				dj.getShortestPathTo(finalNode);
			Assert::AreEqual(reachable, dj.canReachNode(finalNode));
			Assert::AreEqual(cost, dj.getCostTo(finalNode));
			Assert::AreEqual(path, dj.getShortestPathTo(finalNode));
		}

		TEST_METHOD(DijkstraBigMaze)
		{
			unsigned int width = BIG_MAZE_SIZE;
			unsigned int height = BIG_MAZE_SIZE;

			unsigned int cost = (BIG_MAZE_SIZE / 2) + BIG_MAZE_SIZE - 4;
			deque<unsigned int> path;
			for (unsigned int i = (BIG_MAZE_SIZE / 2); i < BIG_MAZE_SIZE - 2; i++)
				path.push_back(ID(i, 1));
			for (unsigned int j = 1; j < BIG_MAZE_SIZE - 1; j++)
				path.push_back(ID(BIG_MAZE_SIZE - 2, j));

			unsigned int start = path[0];
			unsigned int finalNode = path[path.size() - 1];

			Graph g(width * height);
			g.setNodeFinal(finalNode);
			for (unsigned int i = 1; i < height - 1; i++)
			{
				for (unsigned int j = 1; j < width - 1; j++)
				{
					unsigned int id = ID(i, j);
					if (j == 1 && i != height - 1)
						g.addLink(id, ID(i + 1, j), 1);
					if (i % 2 == 1 && j != width - 1)
						g.addLink(id, ID(i, j + 1), 1);
				}
			}

			Dijkstra<> dj(g);
			dj.computeShortestPathsFrom(start);
			Assert::AreEqual(true, dj.canReachNode(finalNode));
			Assert::AreEqual(cost, dj.getCostTo(finalNode));
			Assert::AreEqual(path, dj.getShortestPathTo(finalNode));
		}
	};

	TEST_CLASS(AStarTests)
	{
	public:
		TEST_METHOD(AStarSimpleTest)
		{
#define CTI(c)	(c - 'a')

			// Wikipedia example test : http://fr.wikipedia.org/wiki/Algorithme_de_Dijkstra
			Graph g(CTI('j') + 1);
			g.addLink(CTI('a'), CTI('b'), 85);
			g.addLink(CTI('a'), CTI('c'), 217);
			g.addLink(CTI('a'), CTI('e'), 173);

			g.addLink(CTI('b'), CTI('f'), 80);

			g.addLink(CTI('c'), CTI('g'), 186);
			g.addLink(CTI('c'), CTI('h'), 103);

			g.addLink(CTI('d'), CTI('h'), 183);

			g.addLink(CTI('e'), CTI('j'), 502);

			g.addLink(CTI('f'), CTI('i'), 250);

			g.addLink(CTI('i'), CTI('j'), 84);

			const unsigned int finalNode = CTI('j');
			if (finalNode == CTI('j'))
			{
				const unsigned int heuristics[] =
					{ 450, 400, 250, 300, 500, 300, 400, 150, 50, 0 };
				for (unsigned int i = 0; i <= CTI('j'); i++)
					g.setNodeHeuristic(i, heuristics[i]);
			}
			g.setNodeFinal(finalNode);

			AStar<> as(g);
			for (int i = 0; i < PATH_FINDERS_COMPUTE_LOOPS; i++)
				as.computeShortestPathFrom(0);
			for (int j = 0; j < PATH_FINDERS_PATH_RECONSTRUCTION_LOOPS; j++)
				as.getShortestPath();

			const unsigned int costs[] =
			{ 0, 85, 217, 503, 173, 165, 403, 320, 415, 499 };
			const deque<unsigned int> paths[] = {
				deque < unsigned int > { CTI('a') },
				deque < unsigned int > { CTI('a'), CTI('b') },
				deque < unsigned int > { CTI('a'), CTI('c') },
				deque < unsigned int > { CTI('a'), CTI('c'), CTI('h'), CTI('d') },
				deque < unsigned int > { CTI('a'), CTI('e') },
				deque < unsigned int > { CTI('a'), CTI('b'), CTI('f') },
				deque < unsigned int > { CTI('a'), CTI('c'), CTI('g') },
				deque < unsigned int > { CTI('a'), CTI('c'), CTI('h') },
				deque < unsigned int > { CTI('a'), CTI('b'), CTI('f'), CTI('i') },
				deque < unsigned int > { CTI('a'), CTI('b'), CTI('f'), CTI('i'), CTI('j') } };
			Assert::AreEqual(true, as.hasFoundPath());
			Assert::AreEqual(finalNode, as.getFinalNode());
			Assert::AreEqual(costs[finalNode], as.getPathCost());
			Assert::AreEqual(paths[finalNode], as.getShortestPath());
		}

		TEST_METHOD(AStarLittleMaze)
		{
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
			unsigned int width = map[0].size();
			unsigned int height = map.size();

			bool reachable = true;
			unsigned int cost = 12;
			deque<unsigned int> path = { ID(5, 1), ID(6, 1), ID(7, 1), ID(8, 1), ID(9, 1),
				ID(9, 2), ID(9, 3), ID(9, 4), ID(9, 5), ID(9, 6), ID(9, 7), ID(9, 8), ID(9, 9) };

			unsigned int start = (unsigned int)(-1);
			unsigned int finalNode = (unsigned int)(-1);

			Graph g(width * height);
			for (unsigned int i = 0; i < height; i++)
			{
				for (unsigned int j = 0; j < width; j++)
				{
					if (map[i][j] != '#')
					{
						unsigned int id = ID(i, j);

						if (i < height - 1 && map[i + 1][j] != '#')
							g.addLink(id, ID(i + 1, j), 1);
						if (j < width - 1 && map[i][j + 1] != '#')
							g.addLink(id, ID(i, j + 1), 1);

						if (map[i][j] == 'S')
							start = id;
						else if (map[i][j] == 'E')
						{
							finalNode = id;
							g.setNodeFinal(id);
						}
					}
				}
			}
			for (unsigned int i = 0; i < height; i++)
				for (unsigned int j = 0; j < width; j++)
					g.setNodeHeuristic(ID(i, j),
						abs((int)(finalNode % width) - (int)i) + abs((int)(finalNode / width) - (int)j));

			AStar<> as(g);
			for (int i = 0; i < PATH_FINDERS_COMPUTE_LOOPS; i++)
				as.computeShortestPathFrom(start);
			for (int j = 0; j < PATH_FINDERS_PATH_RECONSTRUCTION_LOOPS; j++)
				as.getShortestPath();
			Assert::AreEqual(reachable, as.hasFoundPath());
			Assert::AreEqual(finalNode, as.getFinalNode());
			Assert::AreEqual(cost, as.getPathCost());
			Assert::AreEqual(path, as.getShortestPath());
		}

		TEST_METHOD(AStarBigMaze)
		{
			unsigned int width = BIG_MAZE_SIZE;
			unsigned int height = BIG_MAZE_SIZE;

			unsigned int cost = (BIG_MAZE_SIZE / 2) + BIG_MAZE_SIZE - 4;
			deque<unsigned int> path;
			for (unsigned int i = (BIG_MAZE_SIZE / 2); i < BIG_MAZE_SIZE - 2; i++)
				path.push_back(ID(i, 1));
			for (unsigned int j = 1; j < BIG_MAZE_SIZE - 1; j++)
				path.push_back(ID(BIG_MAZE_SIZE - 2, j));

			unsigned int start = path[0];
			unsigned int finalNode = path[path.size() - 1];

			Graph g(width * height);
			g.setNodeFinal(finalNode);
			for (unsigned int i = 1; i < height - 1; i++)
			{
				for (unsigned int j = 1; j < width - 1; j++)
				{
					unsigned int id = ID(i, j);
					if (j == 1 && i != height - 1)
						g.addLink(id, ID(i + 1, j), 1);
					if (i % 2 == 1 && j != width - 1)
						g.addLink(id, ID(i, j + 1), 1);
				}
			}
			for (unsigned int i = 0; i < height; i++)
				for (unsigned int j = 0; j < width; j++)
					g.setNodeHeuristic(ID(i, j),
						abs((int)(finalNode % width) - (int)i) + abs((int)(finalNode / width) - (int)j));

			AStar<> as(g);
			as.computeShortestPathFrom(start);
			Assert::AreEqual(true, as.hasFoundPath());
			Assert::AreEqual(finalNode, as.getFinalNode());
			Assert::AreEqual(cost, as.getPathCost());
			Assert::AreEqual(path, as.getShortestPath());
		}
	};
}

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			static wstring ToString(List* l)
			{
				if (!l)
					return wstring(L"NULL");

				wstringstream str;
				Cell* it = list_getFrontIterator(l);
				for (Cell* it = list_getFrontIterator(l); list_isValidIt(it); it = list_incrIt(it))
				{
					if (!list_isFirst(it))
						str << L" -> ";
					str << list_getElt(it);
				}

				wstring outStr;
				str >> outStr;
				return outStr;
			}
			static wstring ToString(const list<unsigned int>& l)
			{
				wstringstream str;
				for (auto it = l.cbegin(); it != l.cend(); ++it)
				{
					if (it != l.cbegin())
						str << L" -> ";
					str << (*it);
				}

				wstring outStr;
				str >> outStr;
				return outStr;
			}
			static wstring ToString(const deque<unsigned int>& l)
			{
				wstringstream str;
				for (auto it = l.cbegin(); it != l.cend(); ++it)
				{
					if (it != l.cbegin())
						str << L" -> ";
					str << (*it);
				}

				wstring outStr;
				str >> outStr;
				return outStr;
			}
		}
	}
};
