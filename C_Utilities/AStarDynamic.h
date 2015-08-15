#ifndef DEF_A_STAR_DYNAMIC
#define DEF_A_STAR_DYNAMIC

#include <vector>
#include <deque>
#include <map>
#include "cost_priority_queue.h"
#include "DynamicGraph.h"

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

		ASDNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX), totalCost(Graphe::INFINITE_COST)
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
		asd.clear();
		endNode = Graphe::INVALID_NODE_INDEX;
#ifdef _DEBUG
		nbExploredNodes = 0;
#endif
		negativeLinkFound = false;
	}

public:
	AStarDynamic(const Graphe& gr) : g(gr)	{ reset(); }

	void computeShortestPathFrom(IndexNoeud startNode)
	{
		// R�initialise les informations sur les noeuds
		reset();

		// Indique le co�t du premier noeud et l'ajoute � la liste des noeuds � parcourir
		asd[startNode].totalCost = 0;
		cost_priority_queue<IndexNoeud, Cout> nodesToSee;
		nodesToSee.push(startNode, g.getNodeHeuristic(startNode));

		// La priorit� du dernier noeud valid� par l'algorithme : celle-ci est toujours croissante,
		// et permet ainsi de d�terminer les noeuds d�j� visit�s simplement par leur priorit�.
		Cout currentPriority = 0;

		while (!nodesToSee.empty())
		{
			IndexNoeud node = nodesToSee.top();
			Cout nodePriority = nodesToSee.top_cost();
			if (g.isNodeFinal(node))
			{
				endNode = node;
				return;
			}
			nodesToSee.pop();

			if (nodePriority < currentPriority)
				continue;
			currentPriority = nodePriority;

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

				// Evite de d�passer le co�t maximal en calculant le nouveau co�t ici :
				// On ignore ce chemin en cas de d�passement pusiqu'on ne peut pas le g�rer.
				if (costAddOverflow(linkCost, nodeTotalCost))
					continue;

				Cout newCost = nodeTotalCost + linkCost;
				IndexNoeud targetNode = l.getTargetIndex();
				if (newCost < asd[targetNode].totalCost)
				{
					asd[targetNode].previousNode = node;
					asd[targetNode].totalCost = newCost;

					// Evite de d�passer la priorit� maximale ici.
					Cout targetPriority;
					Cout targetNodeHeuristic = g.getNodeHeuristic(targetNode);
					if (costAddOverflow(newCost, targetNodeHeuristic))
						targetPriority = Graphe::INFINITE_COST;
					else
						targetPriority = newCost + targetNodeHeuristic;

					// On v�rifie que ce noeud aura bien une priorit� sup�rieure � la priorit� du noeud actuel,
					// afin qu'il ne soit pas consid�r� comme d�j� vu lorsqu'il sera retir� de la file de priorit�.
					if (targetPriority < currentPriority)
						targetPriority = currentPriority;

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
