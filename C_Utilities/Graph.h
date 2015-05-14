#ifndef DEF_GRAPH
#define DEF_GRAPH

#include <vector>

// Un graphe est un tableau de noeuds dont chacun contient la liste des liens vers ses voisins.
// Un lien contient un noeud (le voisin) et son co�t de trajet.

// Fonctions n�cessaires de la classe Graphe :
//		unsigned int size() const;
//		const Noeud& operator[](unsigned int index) const;

// Fonctions n�cessaires de la classe Noeud :
//		unsigned int getIndex() const;
//		const std::vector<Lien*>& getLinks() const;
//			Note :	std::vector peut �tre remplac� par tout autre conteneur de List* pouvant �tre it�r�.
// Fonctions suppl�mentaires n�cessaires pour l'algorithme A* :
//		bool isFinal() const;
//		unsigned int getHeuristic() const;
//			Note :	Cette fonction doit renvoyer la distance minimale estim�e du noeud jusqu'� un noeud final.
//					Afin que l'algorithme fourni ici fonctionne, cette fonction doit v�rifier :
//						Pour tous noeuds x et y : h(x) <= h(y) + d(x, y).
//					La classe fournie par d�faut ici impl�mente une heuristique nulle :
//					son utilisation revient � effectuer l'algorithme de Dijkstra sur le graphe fourni.

// Fonctions n�cessaires de la classe Lien :
//		unsigned int getCost() const;
//		unsigned int getTargetIndex() const;



// Impl�mentation minimale de ces classes
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

public:
	unsigned int getIndex() const				{ return index; }
	const std::vector<Link>& getLinks() const	{ return links; }

	bool isFinal() const						{ return finalNode; }
	unsigned int getHeuristic() const			{ return heuristic; }

	Node(unsigned int _index)
		: index(_index), finalNode(false), heuristic((unsigned int)(-1))	{ }
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

	void addLink(unsigned int start, unsigned int end, unsigned int cost, bool oneDirection = false)
	{
		nodes[start].links.push_back(Link(start, end, cost));
		if (!oneDirection)
			nodes[end].links.push_back(Link(end, start, cost));
	}
	void removeLink(unsigned int start, unsigned int end, bool oneDirection = false, bool costCheck = false, unsigned int cost = 0)
	{
		// Si costCheck == true : tous les liens de start � end sont supprim�s, peu importe leur co�t ;
		// sinon on ne supprime que les liens de start � end avec le co�t sp�cifi�.
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
