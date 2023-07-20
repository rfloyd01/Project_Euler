#pragma once

#include <set>
#include <map>

//Classes needed for graph based algorithms

template <class T>
class GraphVertex
{
	//a simple class that represents a vertex in a graph. It only holds data for the vertex 
	//as well as some overloaded operators for printing and comparing data to other vertices
	T data;

public:
	//CONSTRUCTORS
	GraphVertex()
	{
		//blank default constructor
	}
	GraphVertex(T data)
	{
		this->data = data;
	}

	//GETTERS and SETTERS
	T getData() const { return this->data; }
	void setData(const T& data) { this->data = data; }
	void alterData() { this->data += 1; }

	//BOOLEAN OPERATORS
	friend bool operator==(const GraphVertex& d1, const GraphVertex& d2) { return d1.data == d2.data;}
	friend bool operator!=(const GraphVertex& d1, const GraphVertex& d2) { return d1.data != d2.data; }
	friend bool operator<(const GraphVertex& d1, const GraphVertex& d2) { return d1.data < d2.data; }
	friend bool operator>(const GraphVertex& d1, const GraphVertex& d2) { return d1.data > d2.data; }
	friend bool operator<=(const GraphVertex& d1, const GraphVertex& d2) { return d1.data <= d2.data; }
	friend bool operator>=(const GraphVertex& d1, const GraphVertex& d2) { return d1.data >= d2.data; }

	//ASSIGNMENT OPERATORS
	GraphVertex& operator=(const GraphVertex& d)
	{
		//check for mistaken self assignment
		if (this != &d) this->data = d.data;
		return *this;
	}
};

template <class T>
class GraphEdge
{
	//The edges of the graph can only be created from existing vertices of the graph, therefore we shouldn't be able 
	//to directly modify the vertices from the edges. By using a pointer to a const GraphVertex<T> we can access the 
	//data of each vertex from the graph class, however, we can't modify the values. We can, however, modify the weight 
	//of the edge between the vertices as this value is exclusive to the edge itself.

	const GraphVertex<T> *u, *v; //The vertices connected by the edge. The edge starts at u and ends at v
	int weight; //The weight of the edge

public:
	//CONSTRUCTORS
	GraphEdge(const GraphVertex<T>& u, const GraphVertex<T>& v, int weight = 0)
	{
		this->u = &u;
		this->v = &v;
		this->weight = weight;
	}

	//GETTERS and SETTERS
	int getWeight() const { return this->weight; }
	const GraphVertex<T>* getU() const { return this->u; }
	const GraphVertex<T>* getV() const { return this->v; }

	//BOOLEAN OPERATORS (comparison of edge weights)
	friend bool operator==(const GraphEdge& e1, const GraphEdge& e2) { return e1.weight == e2.weight; }
	friend bool operator!=(const GraphEdge& e1, const GraphEdge& e2) { return e1.weight != e2.weight; }
	friend bool operator<(const GraphEdge& e1, const GraphEdge& e2) { return e1.weight < e2.weight; }
	friend bool operator>(const GraphEdge& e1, const GraphEdge& e2) { return e1.weight > e2.weight; }
	friend bool operator<=(const GraphEdge& e1, const GraphEdge& e2) { return e1.weight <= e2.weight; }
	friend bool operator>=(const GraphEdge& e1, const GraphEdge& e2) { return e1.weight >= e2.weight; }
};

/*
* There are two ways to go about storing data for the graph class. Either it can create it's own copies of the vertices
* and edges, or it can reference existing vertices and edges. If the graph class creates it's own copies it will take 
* more memory but be easier to manage. Using references to existing vertices and edges means that the graph class won't 
* take up that much memory itself, however, care will need to be taken elsewhere to make sure that the vertices and edges 
* won't get deleted before the graph is done using them. For example, if we want to create a graph based on some table of 
* values we might have a loop that reads the values one by one and creates a vertex variable for each piece of data and adds 
* it to a vector (which will later get passed to the initializer of the graph class). Unless each of these vertices is 
* created with the 'new' keyword and their pointers stored in the vector, the vector will only hold copies of the original 
* vertices. 
* 
* When using the below graph class it's important to 
*/
template <class T>
class Graph
{
	std::set<const GraphVertex<T>*> vertices; //store pointers to allow vertices with the same value
	std::set<const GraphEdge<T>*> edges; //store pointers to allow edges with the same edge weights
	std::map<const GraphVertex<T>*, std::vector<const GraphEdge<T>*>> adjacencyList;

public:
	std::set<const GraphVertex<T>*>::iterator first_vertex, last_vertex; //save the locations of the first and last vertex added to set

	Graph(const std::vector<T>& vertices, const std::vector<std::pair<std::pair<int, int>, int>>& edges, bool undirected = true)
	{
		//The graph class is given a vector of type T, which will be used to create vertices and a vector of double integer pairs 
		//which represent edges between the vertices as well as their weight
		std::vector< std::set<const GraphVertex<T>*>::iterator> vertex_locations; //stores the locations of the newly created vertices temporarily
		for (int i = 0; i < vertices.size(); i++)
		{
			//create a new graph vertex and add add it to the vertices list
			const GraphVertex<T>* v = new const GraphVertex<T>(vertices[i]);
			auto location = this->vertices.insert(v).first;
			//std::cout << &location.first << std::endl;
			vertex_locations.push_back(location);

			//create key for the vertex in the adjacency map
			std::vector<const GraphEdge<T>*> new_edge_list;
			std::pair<const GraphVertex<T>*, std::vector<const GraphEdge<T>*>> new_vertex = { v, new_edge_list };
			this->adjacencyList.emplace(new_vertex);
		}
		for (int i = 0; i < edges.size(); i++)
		{
			const GraphEdge<T>* e = new const GraphEdge<T>(*(*vertex_locations[edges[i].first.first]), *(*vertex_locations[edges[i].first.second]), edges[i].second);
			for (int j = 0; j <= undirected; j++)
			{
				this->edges.insert(e);
				auto location = this->adjacencyList.find(e->getU());
				location->second.push_back(e);

				//If the graph is undirected the current loop will execute one more time, adding a newly created edge
				//that's the reverse of the current one.
				if (undirected && j == 0)
				{
					e = new const GraphEdge<T>(*(*vertex_locations[edges[i].first.second]), *(*vertex_locations[edges[i].first.first]), edges[i].second);
				}
			}
		}

		this->first_vertex = vertex_locations[0];
		this->last_vertex = vertex_locations.back();
	}

	~Graph()
	{
		//Need to delete all edges and vertices
		for (auto it = this->vertices.begin(); it != this->vertices.end(); it++) delete (*it);
		for (auto it = this->edges.begin(); it != this->edges.end(); it++) delete (*it);
		this->vertices.clear();
		this->edges.clear();
		this->adjacencyList.clear();
	}

	std::map<const GraphVertex<T>*, std::vector<const GraphEdge<T>*>>* getAdjacencyList() { return &(this->adjacencyList); }
	std::set<const GraphVertex<T>*>* getVertices() { return &(this->vertices); }
	std::set<const GraphVertex<T>*>* getEdges() { return &(this->edges); }

	void print() const
	{
		//Just a method that prints each vertex, and the edges that originate from them
		for (auto it = this->adjacencyList.begin(); it != this->adjacencyList.end(); it++)
		{
			std::cout << "Vertex " << *(it->first) << ":" << std::endl;
			for (int i = 0; i < it->second.size(); i++) std::cout << *(it->second[i]);
			std::cout << std::endl;
		}
	}
};

//Overloaded printing functions
template<class T>
std::ostream& operator<<(std::ostream& os, const GraphVertex<T>& v)
{
	return os << v.getData();
}

template<class T>
std::ostream& operator<<(std::ostream& os, const GraphEdge<T>& e)
{
	return os << "u = " << *(e.getU()) << ", v = " << *(e.getV()) << ", weight = " << e.getWeight() << std::endl;
}