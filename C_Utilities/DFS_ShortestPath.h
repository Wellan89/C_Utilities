#ifndef DEF_DFS_SHORTEST_PATH
#define DEF_DFS_SHORTEST_PATH

#include <vector>
#include <deque>
#include <set>
#include "Graph.h"

// D�sactive la d�tection des cycles dans cet algorithme :
// am�liore largement ses performances sur des graphes sans cycles,
// mais il fera des stack overflow sur des graphes avec cycles
// si sa fonction de calcul computeShortestPathFrom est appel�e avec un maxCost trop grand.
//#define DFS_NO_CYCLE_DETECTION

#ifndef DFS_NO_CYCLE_DETECTION

// Permet � cet algorithme de d�tecter les circuits absorbants durant son parcours,
// mais cette v�rification ralentit l�g�rement son ex�cution.
// Si cette macro n'est pas d�finie, DFS n'entrera pas dans une boucle infinie,
// mais ses r�sultats seront n�cessairement faux en cas de cycle n�gatif dans le graphe d'entr�e, et cela ne sera pas d�tect�.
#define DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION

#endif

// Algorithme du plus court chemin suivant un parcours en profondeur :
// parcours le graphe en profondeur (jusqu'� une profondeur maximale donn�e le cas �ch�ant),
// et s'arr�te d�s qu'un point final est trouv�.
template<class Graphe = Graph<> >
class DFS_ShortestPath
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
#ifndef DFS_NO_CYCLE_DETECTION
#ifndef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
	// L'ensemble des noeuds actuellement visit�s par r�cursivit�
	std::set<IndexNoeud> visitedNodes;
#else
	struct DFS_NodeInfo
	{
		IndexNoeud node;
		Cout totalCost;

		// Op�rateur inf�rieur strict, ne comparant que les indices des noeuds des structures.
		// Ceci permet de consid�rer deux DFS_NodeInfo �gales si leurs noeuds sont �gaux,
		// ind�pendamment du co�t du chemin trouv� pour les atteindre.
		bool operator<(const DFS_NodeInfo& other) const
		{
			return (node < other.node);
		}

		DFS_NodeInfo() : node(Graphe::INVALID_NODE_INDEX), totalCost(Graphe::INFINITE_COST)
		{ }

		// Ce constructeur pourrait aussi permettre de convertir des noeuds (IndexNoeud) en DFS_NodeInfo implicitement.
		// Cependant on l'interdit ici (car cela obfusque le code) avec le mot cl� explicit.
		explicit DFS_NodeInfo(IndexNoeud _node) : node(_node), totalCost(Graphe::INFINITE_COST)
		{ }

		DFS_NodeInfo(IndexNoeud _node, Cout _totalCost) : node(_node), totalCost(_totalCost)
		{ }
	};

	// L'ensemble des noeuds actuellement visit�s par r�cursivit�, ainsi que leur distance au noeud de d�part
	std::set<DFS_NodeInfo> visitedNodes;
#endif
#endif

	// Le chemin actuellement trouv� (dans l'ordre inverse)
	std::vector<IndexNoeud> reverseShortestPath;

	const Graphe& g;

	// Le noeud final trouv� par cet algorithme
	IndexNoeud endNode;

	// Le co�t pour acc�der � ce noeud depuis le noeud de d�part
	Cout pathCost;

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
#ifndef DFS_NO_CYCLE_DETECTION
		visitedNodes.clear();
#endif
		reverseShortestPath.clear();
		endNode = Graphe::INVALID_NODE_INDEX;
		pathCost = Graphe::INFINITE_COST;
		absorbCycleFound = false;
	}

	bool computeShortestPath_Rec(IndexNoeud node, Cout currentCost)
	{
		// V�rifie que le chemin actuellement parcouru n'est pas plus long que le chemin le plus court d�j� trouv�.
		// On quitte lorsqu'il y a �galit� entre la longueur du chemin actuel et celle du chemin le plus court si un noeud final a d�j� �t� trouv�.
		if (currentCost > pathCost
			|| (currentCost >= pathCost && endNode != Graphe::INVALID_NODE_INDEX))
			return false;

		// V�rifie que ce noeud est un noeud final
		if (g[node].isFinal())
		{
			reverseShortestPath.clear();
			reverseShortestPath.push_back(node);

			endNode = node;
			pathCost = currentCost;
			return true;
		}

#ifndef DFS_NO_CYCLE_DETECTION
		// Indique que ce noeud est en cours d'exploration
#ifndef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
		visitedNodes.insert(node);
#else
		// Indique aussi que le co�t du chemin actuellement trouv� est currentCost.
		visitedNodes.insert(DFS_NodeInfo(node, currentCost));
#endif
#endif

		// Parcourt en profondeur tous les fils de ce noeud
		bool pathFound = false;
		const auto& links = g[node].getLinks();
		for (auto it = links.begin(); it != links.end(); ++it)
		{
			Cout linkCost = it->getCost();

			// Evite de d�passer le co�t maximal en calculant le nouveau co�t ici :
			// On ignore ce chemin en cas de d�passement puisqu'on ne peut pas le g�rer.
			if (costAddOverflow(currentCost, linkCost))
				continue;

			Cout targetCost = currentCost + linkCost;
			IndexNoeud targetNode = it->getTargetIndex();

#ifndef DFS_NO_CYCLE_DETECTION
			// V�rifie que ce noeud n'est pas explor� actuellement
#ifndef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
			auto targetNodeIt = visitedNodes.find(targetNode);
#else
			// Cette conversion est n�cessaire � cause du mot-cl� explicit dans le constructeur DFS_NodeInfo(IndexNoeud)
			auto targetNodeIt = visitedNodes.find(DFS_NodeInfo(targetNode));
#endif
			if (targetNodeIt != visitedNodes.end())
			{
#ifdef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
				// S'il l'est, on v�rifie bien que ce chemin pour l'atteindre est plus long que le chemin pr�c�demment trouv�,
				// sinon c'est qu'on est en pr�sence d'un circuit absorbant.
				if (targetCost < targetNodeIt->totalCost)
				{
					absorbCycleFound = true;
					return false;
				}
#endif
				continue;
			}
#endif

			if (computeShortestPath_Rec(targetNode, targetCost))
			{
				pathFound = true;
				reverseShortestPath.push_back(node);
			}

#ifdef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
			// Si on a d�tect� un circuit absorbant, on quitte imm�diatement
			if (absorbCycleFound)
				return false;
#endif
		}

#ifndef DFS_NO_CYCLE_DETECTION
		// Efface ce noeud de la liste des noeuds en cours d'exploration
#ifndef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
		visitedNodes.erase(node);
#else
		visitedNodes.erase(DFS_NodeInfo(node));
#endif
#endif

		return pathFound;
	}

public:
	DFS_ShortestPath(const Graphe& gr) : g(gr)	{ reset(); }

	void computeShortestPathFrom(IndexNoeud startNode, Cout maxCost = Graphe::INFINITE_COST)
	{
		// R�initialise les informations sur les noeuds
		reset();

		// Indique la profondeur maximale � laquelle on va chercher un noeud final
		pathCost = maxCost;

		// Traite r�cursivement chacun des noeuds pouvant �tre atteints depuis le noeud de d�part
		if (!computeShortestPath_Rec(startNode, 0))
		{
			// R�initialise les informations sur les noeuds si aucun noeud final n'a �t� trouv�,
			// en retenant si un circuit absorbant a �t� d�tect�.
			bool absorbCycle = absorbCycleFound;
			reset();
			absorbCycleFound = absorbCycle;
		}
	}

	bool absorbCycleDetected() const
	{
		return absorbCycleFound;
	}
	bool hasFoundPath() const
	{
		return (endNode != Graphe::INVALID_NODE_INDEX);
	}
	IndexNoeud getFinalNode() const
	{
		return endNode;
	}
	Cout getPathCost() const
	{
		return pathCost;
	}
	std::deque<IndexNoeud> getShortestPath() const
	{
		std::deque<IndexNoeud> l;
		for (auto it = reverseShortestPath.begin(); it != reverseShortestPath.end(); ++it)
			l.push_front(*it);
		return l;
	}
	const std::vector<IndexNoeud>& getReverseShortestPath() const
	{
		return reverseShortestPath;
	}
};

#endif
