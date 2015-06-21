#ifndef DEF_DFS_SHORTEST_PATH
#define DEF_DFS_SHORTEST_PATH

#include <vector>
#include <deque>
#include <set>
#include "Graph.h"

// Permet � cet algorithme de d�tecter les circuits absorbants durant son parcours,
// mais cette v�rification ralentit l�g�rement son ex�cution.
// Si cette macro n'est pas d�finie, DFS n'entrera pas dans une boucle infinie,
// mais ses r�sultats seront n�cessairement faux en cas de cycle n�gatif dans le graphe d'entr�e, et cela ne sera pas d�tect�.
#define DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION

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

		DFS_NodeInfo() : node(Graphe::INVALID_NODE_INDEX()), totalCost(Graphe::INFINITE_COST())
			{ }

		// Ce constructeur pourrait aussi permettre de convertir des noeuds (IndexNoeud) en DFS_NodeInfo implicitement.
		// Cependant on l'interdit ici (car cela obfusque le code) avec le mot cl� explicit.
		explicit DFS_NodeInfo(IndexNoeud _node) : node(_node), totalCost(Graphe::INFINITE_COST())
			{ }

		DFS_NodeInfo(IndexNoeud _node, Cout _totalCost) : node(_node), totalCost(_totalCost)
			{ }
	};

	// L'ensemble des noeuds actuellement visit�s par r�cursivit�, ainsi que leur distance au noeud de d�part
	std::set<DFS_NodeInfo> visitedNodes;
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

	// R�initialise les informations sur les noeuds
	void reset()
	{
		visitedNodes.clear();
		reverseShortestPath.clear();
		endNode = Graphe::INVALID_NODE_INDEX();
		pathCost = Graphe::INFINITE_COST();
		absorbCycleFound = false;
	}

	bool computeShortestPath_Rec(IndexNoeud node, Cout currentCost);

public:
	DFS_ShortestPath(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathFrom(IndexNoeud startNode, Cout maxCost = Graphe::INFINITE_COST());

	bool absorbCycleDetected() const
	{
		return absorbCycleFound;
	}
	bool hasFoundPath() const;
	IndexNoeud getFinalNode() const;
	Cout getPathCost() const;
	std::deque<IndexNoeud> getShortestPath() const;
	const std::vector<IndexNoeud>& getReverseShortestPath() const;
};

#endif
