#ifndef DEF_A_STAR_DYNAMIC
#define DEF_A_STAR_DYNAMIC

#include <vector>
#include <deque>
#include <map>
#include "cost_priority_queue.h"
#include "DynamicGraph.h"

// Permet de diminuer la m�moire utilis�e par cette classe (et permet ainsi � cet algorithme de travailler sur des graphes plus grands),
// en contre-partie l'algorithme se r�v�lera l�g�rement plus lent sur des graphes contenant de nombreux liens au co�t nul,
// mais il sera l�g�rement plus rapide sur des graphes sans ce type de lien.
#define A_STAR_DYNAMIC_CLOSED_SET_REMOVAL_OPTIMIZATION

// Algorithme A* : parcours le graphe depuis un point de d�part en suivant une heuristique,
// et s'arr�te d�s qu'un point final est trouv�.
// G�re aussi les graphes dynamiques.
template<class Graphe = DynamicGraph<> >
class AStarDynamic
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	struct ASDNodeInfo
	{
		IndexNoeud previousNode;
		Cout totalCost;
#ifndef A_STAR_DYNAMIC_CLOSED_SET_REMOVAL_OPTIMIZATION
		bool alreadyVisited;
#endif

		ASDNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX), totalCost(Graphe::INFINITE_COST)
#ifndef A_STAR_DYNAMIC_CLOSED_SET_REMOVAL_OPTIMIZATION
			, alreadyVisited(false)
#endif
			{ }
	};

	std::map<IndexNoeud, ASDNodeInfo> asd;
	const Graphe& g;

	// Le noeud final trouv� par cet algorithme
	IndexNoeud endNode;

#ifdef _DEBUG
	// Le nombre de noeuds explor�s (marqu�s comme visit�s) par cet algorithme
	IndexNoeud nbExploredNodes;
#endif

	// Indique si un lien au co�t n�gatif a �t� d�tect� dans le graphe
	bool negativeLinkFound;

	// R�initialise les informations sur les noeuds
	void reset()
	{
		asd.clear();
		endNode = Graphe::INVALID_NODE_INDEX;
#ifdef _DEBUG
		nbExploredNodes = 0;
#endif
		negativeLinkFound = false;
	}

public:
	AStarDynamic(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathFrom(IndexNoeud startNode)
	{
		// R�initialise les informations sur les noeuds
		reset();

		// Indique le co�t du premier noeud et l'ajoute � la liste des noeuds � parcourir
		asd[startNode].totalCost = 0;
		cost_priority_queue<IndexNoeud, Cout> nodesToSee;
		nodesToSee.push(startNode, g.getNodeHeuristic(startNode));

#ifdef A_STAR_DYNAMIC_CLOSED_SET_REMOVAL_OPTIMIZATION
		// La priorit� du dernier noeud valid� par l'algorithme : celle-ci est toujours croissante,
		// et permet ainsi de d�terminer les noeuds d�j� visit�s simplement par leur priorit�.
		Cout currentPriority = 0;
#endif

		while (!nodesToSee.empty())
		{
			IndexNoeud node = nodesToSee.top();
#ifdef A_STAR_DYNAMIC_CLOSED_SET_REMOVAL_OPTIMIZATION
			Cout nodePriority = nodesToSee.top_cost();
#endif
			if (g.isNodeFinal(node))
			{
				endNode = node;
				return;
			}
			nodesToSee.pop();

#ifndef A_STAR_DYNAMIC_CLOSED_SET_REMOVAL_OPTIMIZATION
			if (asd[node].alreadyVisited)
				continue;
			asd[node].alreadyVisited = true;
#else
			if (nodePriority < currentPriority)
				continue;
			currentPriority = nodePriority;
#endif

#ifdef _DEBUG
			nbExploredNodes++;
#endif
			Cout nodeTotalCost = asd[node].totalCost;

			IndexNoeud linksCount = g.getNodeLinksCount(node);
			for (IndexNoeud i = 0; i < linksCount; i++)
			{
				Graphe::Lien l = g.getNodeLink(node, i);
				Cout linkCost = l.getCost();
				if (linkCost < 0)
				{
					// On a trouv� une ar�te avec un co�t n�gatif : on quitte ici.
					reset();
					negativeLinkFound = true;
					return;
				}

				IndexNoeud targetNode = l.getTargetIndex();
				Cout newCost = nodeTotalCost + linkCost;
				if (newCost < asd[targetNode].totalCost)
				{
					asd[targetNode].previousNode = node;
					asd[targetNode].totalCost = newCost;

					Cout targetPriority = newCost + g.getNodeHeuristic(targetNode);
#ifdef A_STAR_DYNAMIC_CLOSED_SET_REMOVAL_OPTIMIZATION
					// On v�rifie que ce noeud aura bien une priorit� sup�rieure � la priorit� du noeud actuel,
					// afin qu'il ne soit pas consid�r� comme d�j� vu lorsqu'il sera retir� de la file de priorit�.
					if (targetPriority < currentPriority)
						targetPriority = currentPriority;
#endif

					nodesToSee.push(targetNode, targetPriority);
				}
			}
		}
	}

	bool negativeLinkDetected() const
	{
		return negativeLinkFound;
	}
	bool hasFoundPath() const
	{
		return (endNode != Graphe::INVALID_NODE_INDEX);
	}
	IndexNoeud getFinalNode() const
	{
		return endNode;
	}
	Cout getPathCost()
	{
		if (hasFoundPath())
			return asd[endNode].totalCost;
		return Graphe::INFINITE_COST;
	}
	std::deque<IndexNoeud> getShortestPath()
	{
		std::deque<IndexNoeud> l;
		IndexNoeud node = endNode;
		while (node != Graphe::INVALID_NODE_INDEX)
		{
			l.push_front(node);
			node = asd[node].previousNode;
		}
		return l;
	}
	std::vector<IndexNoeud> getReverseShortestPath()
	{
		std::vector<IndexNoeud> l;
		IndexNoeud node = endNode;
		while (node != Graphe::INVALID_NODE_INDEX)
		{
			l.push_back(node);
			node = asd[node].previousNode;
		}
		return l;
	}

#ifdef _DEBUG
	IndexNoeud getNbExploredNodes() const
	{
		return nbExploredNodes;
	}
#endif
};

#endif
