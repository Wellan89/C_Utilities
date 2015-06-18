#include "CounterCulture.h"
#include "AStarDynamic.cpp"
#include "AStar.cpp"
#include "Dijkstra.cpp"
#include <iostream>
#include <ctime>

using namespace std;

void printResult_AStarDynamic(CCDynGraph::IndexNoeud finalNode)
{
	try
	{
		cout << "AStarDynamic : ";
		clock_t startTime = clock();

		CCNodeLinksGenerator linksGen(finalNode);
		CCNodeFinalGenerator finalGen(finalNode);
		CCNodeHeuristicGenerator heuristicGen(finalNode);
		CCDynGraph ccdg(linksGen, finalGen, heuristicGen);

		AStarDynamic<CCDynGraph> asd(ccdg);
		asd.computeShortestPathFrom(0);

		time_t diffTime = clock() - startTime;
		cout << "Result = " << asd.getPathCost() << " ; Time = " << diffTime << " ms" << endl;
	}
	catch (...)
	{
		cout << "Error !" << endl;
	}
}
void printResult_Dijkstra(CCDynGraph::IndexNoeud finalNode)
{
	try
	{
		cout << "Dijkstra     : ";
		clock_t startTime = clock();

		CCGraph ccg = generateCCGraph(finalNode);

		Dijkstra<CCGraph> dj(ccg);
		dj.computeShortestPathsFrom(0);

		time_t diffTime = clock() - startTime;
		cout << "Result = " << dj.getCostTo(finalNode) << " ; Time = " << diffTime << " ms" << endl;
	}
	catch (...)
	{
		cout << "Error !" << endl;
	}
}
void printResult_AStar(CCDynGraph::IndexNoeud finalNode)
{
	try
	{
		cout << "AStar        : ";
		clock_t startTime = clock();

		CCGraph ccg = generateCCGraph(finalNode);

		AStar<CCGraph> as(ccg);
		as.computeShortestPathFrom(0);

		time_t diffTime = clock() - startTime;
		cout << "Result = " << as.getPathCost() << " ; Time = " << diffTime << " ms" << endl;
	}
	catch (...)
	{
		cout << "Error !" << endl;
	}
}
int main()
{
	cout << "Entrez le nombre a atteindre :" << endl;

	CCDynGraph::IndexNoeud finalNode;
	cin >> finalNode;

	cout << endl;
	printResult_AStarDynamic(finalNode);
	printResult_Dijkstra(finalNode);
	printResult_AStar(finalNode);
	cout << endl;

	system("PAUSE");

	return EXIT_SUCCESS;
}
