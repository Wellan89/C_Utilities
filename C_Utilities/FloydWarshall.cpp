#include "FloydWarshall.h"

using namespace std;

template<class Graphe, class Noeud, class Lien>
void FloydWarshall<Graphe, Noeud, Lien>::computeShortestPaths()
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
				if ((fw[i][k].totalCost == (unsigned int)(-1)) || (fw[k][j].totalCost == (unsigned int)(-1)))
					continue;

				unsigned int newCost = fw[i][k].totalCost + fw[k][j].totalCost;
				if (newCost < fw[i][j].totalCost)
				{
					fw[i][j].totalCost = newCost;
				}
			}
		}
	}
}
template<class Graphe, class Noeud, class Lien>
bool FloydWarshall<Graphe, Noeud, Lien>::pathExists(unsigned int startNode, unsigned int endNode) const
{
	return (fw[startNode][endNode].totalCost != (unsigned int)(-1));
}
template<class Graphe, class Noeud, class Lien>
unsigned int FloydWarshall<Graphe, Noeud, Lien>::getPathCost(unsigned int startNode, unsigned int endNode) const
{
	return fw[startNode][endNode].totalCost;
}
