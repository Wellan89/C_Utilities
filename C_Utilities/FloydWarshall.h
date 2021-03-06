#ifndef DEF_FLOYD_WARSHALL
#define DEF_FLOYD_WARSHALL

#include <vector>
#include <limits>
#include "Graph.h"

// Algorithme de Floyd-Warshall : d�termine toutes les plus courtes distances entre toutes les paires de points.
template<class Graphe = Graph<> >
class FloydWarshall
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	struct FwPathInfo
	{
		// Un noeud k par lequel passe le chemin (i, j).
		// Lorsque k = j, ce chemin est un simple lien.
		IndexNoeud middleNode;

		Cout totalCost;

		FwPathInfo() : totalCost(Graphe::INFINITE_COST), middleNode(Graphe::INVALID_NODE_INDEX)
		{ }
	};

	std::vector<std::vector<FwPathInfo> > fw;
	const Graphe& g;

	// Indique si un circuit absorbant a �t� d�tect� dans le graphe
	bool absorbCycleFound;

	// D�termine si la somme de deux co�ts d�passe les valeurs maximales ou minimales d'un co�t.
	bool costAddOverflow(Cout a, Cout b)
	{
		if (a >= 0)	// Ce test sera optimis� si Cout est un type non sign�
		{
			// On ne veut pas pouvoir ajouter "b = -1" � "a = Graphe::INFINITE_COST" par exemple.
			// De plus, on �vite d'obtenir un r�sultat �gal � Graphe::INFINITE_COST.
			return (a == Graphe::INFINITE_COST
				|| b >= std::numeric_limits<Cout>::max() - a);
		}
		else
		{
			// On ne veut pas pouvoir ajouter "a = -1" � "b = Graphe::INFINITE_COST" par exemple.
			return (b == Graphe::INFINITE_COST
				|| b < std::numeric_limits<Cout>::min() - a);
		}
	}

	// R�initialise les informations sur les noeuds
	void reset()
	{
		fw.clear();
		IndexNoeud nodesCount = g.size();
		fw.resize(nodesCount);
		for (IndexNoeud i = 0; i < nodesCount; i++)
			fw[i].resize(nodesCount);
		absorbCycleFound = false;
	}

	void appendShortestPath(std::vector<IndexNoeud>& v, IndexNoeud startNode, IndexNoeud endNode) const
	{
		if (startNode == endNode)
			return;

		IndexNoeud middleNode = fw[startNode][endNode].middleNode;
		if (middleNode == endNode)
		{
			// Il existe un simple lien entre startNode et endNode
			//v.push_back(startNode);	// Cette fonction n'ajoute jamais le premier noeud dans le vecteur
			v.push_back(endNode);
		}
		else
		{
			appendShortestPath(v, startNode, middleNode);
			appendShortestPath(v, middleNode, endNode);
		}
	}

public:
	FloydWarshall(const Graphe& gr) : g(gr)	{ reset(); }

	void computeShortestPaths()
	{
		// R�initialise les informations sur les noeuds
		reset();

		// Indique le co�t des liens de base
		IndexNoeud nodesCount = g.size();
		for (IndexNoeud i = 0; i < nodesCount; i++)
		{
			fw[i][i].totalCost = 0;
			fw[i][i].middleNode = i;
		}
		for (IndexNoeud node = 0; node < nodesCount; node++)	// Parcours toutes les ar�tes du graphe
		{
			const auto& links = g[node].getLinks();
			for (auto it = links.begin(); it != links.end(); ++it)
			{
				IndexNoeud targetNode = it->getTargetIndex();
				Cout linkCost = it->getCost();
				if (linkCost < fw[node][targetNode].totalCost)
				{
					fw[node][targetNode].totalCost = linkCost;
					fw[node][targetNode].middleNode = targetNode;
				}
			}
		}

		// D�termine pour tout noeud k s'il est plus court de passer par ce noeud pour chaque paire de noeuds (i, j)
		for (IndexNoeud k = 0; k < nodesCount; k++)
		{
			for (IndexNoeud i = 0; i < nodesCount; i++)
			{
				// Cette optimisation casse-t-elle l'algorithme ?
				// On ne dirait pas, en se basant sur tous les tests actuels.
				if (i == k)
					continue;

				Cout i_k_cost = fw[i][k].totalCost;
				if (i_k_cost == Graphe::INFINITE_COST)
					continue;

				for (IndexNoeud j = 0; j < nodesCount; j++)
				{
					Cout k_j_cost = fw[k][j].totalCost;

					// Evite de d�passer le co�t maximal en calculant le nouveau co�t ici :
					// On ignore ce chemin en cas de d�passement puisqu'on ne peut pas le g�rer.
					if (costAddOverflow(k_j_cost, i_k_cost))
						continue;

					Cout newCost = i_k_cost + k_j_cost;
					if (newCost < fw[i][j].totalCost)
					{
						fw[i][j].totalCost = newCost;
						fw[i][j].middleNode = k;
					}
				}
			}
		}

		// Cherche un circuit absorbant : un tel circuit existe si pour un noeud i donn�,
		// la longueur du chemin le plus court pour atteindre i est strictement n�gative.
		for (IndexNoeud i = 0; i < nodesCount; i++)
		{
			if (fw[i][i].totalCost < 0)
			{
				reset();
				absorbCycleFound = true;
				return;
			}
		}
	}

	bool absorbCycleDetected() const
	{
		return absorbCycleFound;
	}
	bool pathExists(IndexNoeud startNode, IndexNoeud endNode) const
	{
		return (fw[startNode][endNode].totalCost != Graphe::INFINITE_COST);
	}
	Cout getPathCost(IndexNoeud startNode, IndexNoeud endNode) const
	{
		return fw[startNode][endNode].totalCost;
	}
	std::vector<IndexNoeud> getShortestPath(IndexNoeud startNode, IndexNoeud endNode) const
	{
		std::vector<IndexNoeud> v(1, startNode);

		if (pathExists(startNode, endNode))
			appendShortestPath(v, startNode, endNode);

		return v;
	}
};

#endif
