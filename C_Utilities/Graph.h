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

// Fonctions nécessaires de la classe Lien :
//		unsigned int getCost() const;
//		unsigned int getTargetIndex() const;



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
	unsigned int index;
	std::vector<Link> links;

	bool finalNode;
	unsigned int heuristic;
	unsigned int topologicIndex;

public:
	unsigned int getIndex() const				{ return index; }
	const std::vector<Link>& getLinks() const	{ return links; }

	bool isFinal() const						{ return finalNode; }
	unsigned int getHeuristic() const			{ return heuristic; }
	unsigned int getTopologicIndex() const		{ return topologicIndex; }

	Node(unsigned int _index)
		: index(_index), finalNode(false), heuristic((unsigned int)(-1)), topologicIndex((unsigned int)(-1)) { }
};
class Graph
{
protected:
	std::vector<Node> nodes;

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
		if (!unidirectionnal)
			nodes[end].links.push_back(Link(end, start, cost));
	}
	void removeLink(unsigned int start, unsigned int end, bool oneDirection = false, bool costCheck = false, unsigned int cost = 0)
	{
		// Si costCheck == true : tous les liens de start à end sont supprimés, peu importe leur coût ;
		// sinon on ne supprime que les liens de start à end avec le coût spécifié.
		{
			auto& links = nodes[start].links;
			for (auto it = links.begin(); it != links.end(); ++it)
				if (it->getTargetIndex() == end && (!costCheck || it->getCost() == cost))
					links.erase(it);
		}

		if (!oneDirection)
		{
			auto& links = nodes[end].links;
			for (auto it = links.begin(); it != links.end(); ++it)
				if (it->getTargetIndex() == start && (!costCheck || it->getCost() == cost))
					links.erase(it);
		}
	}

	void setNodeFinal(unsigned int node, bool isFinal = true)
	{
		nodes[node].finalNode = isFinal;
	}
	void setNodeHeuristic(unsigned int node, unsigned int heuristic)
	{
		nodes[node].heuristic = heuristic;
	}

protected:
	// Enum définissant l'état d'un noeud lors de la construction d'un ordre topologique
	enum E_TOPOLOGICAL_ORDER_NODE_STATE
	{
		ETONS_UNSEEN,
		ETONS_SEEN,
		ETONS_ADDED
	};

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
