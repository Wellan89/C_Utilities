#ifndef DEF_GRAPH
#define DEF_GRAPH

#include <deque>
#include <vector>

// Un graphe est un tableau de noeuds dont chacun contient la liste des liens vers ses voisins.
// Un lien contient un noeud (le voisin) et son coût de trajet.

// Fonctions nécessaires de la classe Graphe :
//		unsigned int size() const;
//		const Noeud& operator[](unsigned int index) const;
// Fonctions supplémentaires pour l'algorithme de Bellman :
//		std::vector<unsigned int> getReverseTopologicalyOrderedNodes() const;
//			Note :	std::vector peut être remplacé par tout autre conteneur de unsigned int pouvant être itéré.

// Fonctions nécessaires de la classe Noeud :
//		unsigned int getIndex() const;
//		const std::vector<Lien*>& getLinks() const;
//			Note :	std::vector peut être remplacé par tout autre conteneur de List* pouvant être itéré.
// Fonctions supplémentaires nécessaires pour l'algorithme A* :
//		bool isFinal() const;
//		unsigned int getHeuristic() const;
//			Note :	Cette fonction doit renvoyer la distance minimale estimée du noeud jusqu'à un noeud final.
//					Afin que l'algorithme fourni ici fonctionne, cette fonction doit vérifier :
//						Pour tous noeuds x et y : h(x) <= h(y) + d(x, y).
//					La classe fournie par défaut ici implémente une heuristique nulle :
//					son utilisation revient à effectuer l'algorithme de Dijkstra sur le graphe fourni.
// Fonction supplémentaire pour l'algorithme de Bellman :
//		const std::vector<Lien*>& getIncomingLinks() const;
//			Note :	std::vector peut être remplacé par tout autre conteneur de List* pouvant être itéré.

// Fonctions nécessaires de la classe Lien :
//		unsigned int getCost() const;
//		unsigned int getTargetIndex() const;
// Fonction supplémentaire pour l'algorithme de Bellman :
//		unsigned int getFromIndex() const;



// Implémentation minimale de ces classes
class Graph;
class Link
{
protected:
	unsigned int fromIndex;
	unsigned int targetIndex;
	unsigned int cost;

public:
	unsigned int getFromIndex() const	{ return fromIndex; }
	unsigned int getTargetIndex() const	{ return targetIndex; }
	unsigned int getCost() const		{ return cost; }

	Link(unsigned int _fromIndex, unsigned int _targetIndex, unsigned int _cost)
		: fromIndex(_fromIndex), targetIndex(_targetIndex), cost(_cost) { }
};
class Node
{
	friend Graph;

protected:
	std::vector<Link> links;
	std::vector<Link> incomingLinks;
	unsigned int index;

	unsigned int heuristic;
	bool finalNode;

public:
	unsigned int getIndex() const						{ return index; }
	const std::vector<Link>& getLinks() const			{ return links; }
	const std::vector<Link>& getIncomingLinks() const	{ return incomingLinks; }

	bool isFinal() const								{ return finalNode; }
	unsigned int getHeuristic() const					{ return heuristic; }

	Node(unsigned int _index)
		: index(_index), finalNode(false), heuristic((unsigned int)(-1)) { }
};
class Graph
{
protected:
	// Enum définissant l'état d'un noeud lors de la construction d'un ordre topologique
	enum E_TOPOLOGICAL_ORDER_NODE_STATE
	{
		ETONS_UNSEEN,
		ETONS_SEEN,
		ETONS_ADDED
	};

	std::vector<Node> nodes;

	// Ajoute un noeud et ses fils à un ordre topologique.
	// Renvoit false si on détecte une erreur lors de la construction de l'ordre topologique.
	bool addToTopologicalOrder(unsigned int index, std::vector<unsigned int>& v,
		std::vector<E_TOPOLOGICAL_ORDER_NODE_STATE>& nodesState) const
	{
		nodesState[index] = ETONS_SEEN;
		for (auto it = nodes[index].links.begin(); it != nodes[index].links.end(); ++it)
		{
			unsigned int node = it->getTargetIndex();
			if (nodesState[node] == ETONS_UNSEEN)
			{
				if (!addToTopologicalOrder(node, v, nodesState))
					return false;
			}
			else if (nodesState[node] == ETONS_SEEN)
				return false;
		}
		nodesState[index] = ETONS_ADDED;
		v.push_back(index);
		return true;
	}

public:
	Graph(unsigned int nodesNb)
	{
		nodes.reserve(nodesNb);
		for (unsigned int i = 0; i < nodesNb; i++)
			nodes.push_back(Node(i));
	}

	void addLink(unsigned int start, unsigned int end, unsigned int cost, bool unidirectionnal = false)
	{
		nodes[start].links.push_back(Link(start, end, cost));
		nodes[end].incomingLinks.push_back(Link(start, end, cost));
		if (!unidirectionnal)
		{
			nodes[end].links.push_back(Link(end, start, cost));
			nodes[start].incomingLinks.push_back(Link(end, start, cost));
		}
	}
	void removeLink(unsigned int start, unsigned int end, bool oneDirection = false, bool costCheck = false, unsigned int cost = 0)
	{
#define REMOVE_LINKS_FROM_ITERABLE(links, target, costCheck, cost)									\
			do {																					\
				auto& nodeLinks = (links);															\
				for (auto it = nodeLinks.end(); it != nodeLinks.end(); ++it)						\
					if (it->getTargetIndex() == (end) && (!(costCheck) || it->getCost() == (cost)))	\
						it = nodeLinks.erase(it);													\
			} while(false)

		// On parcours les liens de la fin vers le début, pour obtenir une meilleure complexité en moyenne

		// Si costCheck == true : tous les liens de start à end sont supprimés, peu importe leur coût ;
		// sinon on ne supprime que les liens de start à end avec le coût spécifié.
		REMOVE_LINKS_FROM_ITERABLE(nodes[start].links, end, costCheck, cost);
		REMOVE_LINKS_FROM_ITERABLE(nodes[end].incomingLinks, start, costCheck, cost);

		if (!oneDirection)
		{
			REMOVE_LINKS_FROM_ITERABLE(nodes[end].links, start, costCheck, cost);
			REMOVE_LINKS_FROM_ITERABLE(nodes[start].incomingLinks, end, costCheck, cost);
		}
#undef REMOVE_LINKS_FROM_ITERABLE
	}

	void setNodeFinal(unsigned int node, bool isFinal = true)
	{
		nodes[node].finalNode = isFinal;
	}
	void setNodeHeuristic(unsigned int node, unsigned int heuristic)
	{
		nodes[node].heuristic = heuristic;
	}

	// Détermine un ordre topologique inverse sur ce graphe :
	// les noeuds puits seront au début de ce vecteur, et les noeuds source seront à la fin.
	// Renvoit le vecteur vide s'il n'existe pas de tel ordre topologique.
	std::vector<unsigned int> getReverseTopologicalyOrderedNodes() const
	{
		std::vector<unsigned int> v;
		v.reserve(nodes.size());

		std::vector<E_TOPOLOGICAL_ORDER_NODE_STATE> nodesState(nodes.size(), ETONS_UNSEEN);
		for (unsigned int i = 0; i < nodes.size(); i++)
		{
			if (nodesState[i] == ETONS_UNSEEN)
			{
				if (!addToTopologicalOrder(i, v, nodesState))
				{
					v.clear();
					break;
				}
			}
		}
		return v;
	}

	void addNodes(unsigned int nbOfNodesToAdd = 1)
	{
		nodes.reserve(nodes.size() + nbOfNodesToAdd);
		for (unsigned int i = 0; i < nbOfNodesToAdd; i++)
			nodes.push_back(Node(nodes.size()));
	}
	
	unsigned int size() const
	{
		return nodes.size();
	}
	Node& operator[](unsigned int index)
	{
		return nodes[index];
	}
	const Node& operator[](unsigned int index) const
	{
		return nodes[index];
	}
};

#endif
