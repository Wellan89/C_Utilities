#include "FloydWarshall.h"

using namespace std;

template<class Graphe>
void FloydWarshall<Graphe>::computeShortestPaths()
{
	// Réinitialise les informations sur les noeuds
	reset();

	// Indique le coût des liens de base
	unsigned int nodesCount = g.size();
	for (unsigned int i = 0; i < nodesCount; i++)
		fw[i][i].totalCost = 0;
	for (unsigned int node = 0; node < nodesCount; node++)	// Parcours toutes les arêtes du graphe
	{
		const auto& links = g[node].getLinks();
		for (auto it = links.begin(); it != links.end(); ++it)
		{
			unsigned int targetNode = it->getTargetIndex();
			unsigned int linkCost = it->getCost();
			if (linkCost < fw[node][targetNode].totalCost)
			{
				fw[node][targetNode].totalCost = linkCost;
			}
		}
	}

	// Détermine pour tout noeud k s'il est plus court de passer par ce noeud pour chaque paire de noeuds (i, j)
	for (unsigned int k = 0; k < nodesCount; k++)
	{
		for (unsigned int i = 0; i < nodesCount; i++)
		{
			for (unsigned int j = 0; j < nodesCount; j++)
			{
				unsigned int i_k_cost = fw[i][k].totalCost;
				unsigned int k_j_cost = fw[k][j].totalCost;
				if ((i_k_cost == (unsigned int)(-1)) || (k_j_cost == (unsigned int)(-1)))
					continue;

				unsigned int newCost = i_k_cost + k_j_cost;
				if (newCost < fw[i][j].totalCost)
				{
					fw[i][j].totalCost = newCost;
				}
			}
		}
	}
}
template<class Graphe>
bool FloydWarshall<Graphe>::pathExists(unsigned int startNode, unsigned int endNode) const
{
	return (fw[startNode][endNode].totalCost != (unsigned int)(-1));
}
template<class Graphe>
unsigned int FloydWarshall<Graphe>::getPathCost(unsigned int startNode, unsigned int endNode) const
{
	return fw[startNode][endNode].totalCost;
}
