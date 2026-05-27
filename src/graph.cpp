#include "graph.h"
#include <stdexcept>

// Basic error logging helper
void error(string msg) {
    cerr << "Error: " << msg << endl;
    throw runtime_error(msg);
}

// DisjointSet Implementation

DisjointSet::DisjointSet(int numVertices) {
    subsets.resize(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        makeSet(i);
    }
}

void DisjointSet::makeSet(Vertex x) {
    if (x >= 0 && x < static_cast<int>(subsets.size())) {
        subsets[x].parent = x;
    }
}

Vertex DisjointSet::findSet(Vertex x) {
    // Path compression optimization: flattens the tree structure
    if (subsets[x].parent != x) {
        subsets[x].parent = findSet(subsets[x].parent);
    }
    return subsets[x].parent;
}

void DisjointSet::unionSets(Vertex x, Vertex y) {
    Vertex rootX = findSet(x);
    Vertex rootY = findSet(y);
    
    if (rootX != rootY) {
        subsets[rootY].parent = rootX;
    }
}

// Helper to open file stream and parse file data into the graph
void file_to_graph(string filename, Graph & G) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        error("unable to find input file: " + filename);
    }
    infile >> G;
    infile.close();
}

// Argument helper for checking execution flags
string get_arg(int argc, char *argv[], string def) {
    if (argc > 1) {
        return string(argv[1]);
    }
    return def;
}

// Graph Methods

// Returns a copy of the graph with edges sorted in ascending order by weight
Graph Graph::sort_edges() const {
    Graph sortedG = *this;
    sort(sortedG.begin(), sortedG.end(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight;
    });
    return sortedG;
}

// Finds all vertices adjacent to the given vertex
VertexList Graph::edges_from(Vertex vertex) const {
    VertexList neighbors;
    for (const auto& edge : *this) {
        if (edge.u == vertex) {
            neighbors.push_back(edge.v);
        }
    }
    return neighbors;
}


// Graph Algorithms & Utilities

// Kruskal's algorithm to compute the Minimum Spanning Tree
EdgeList Kruskals(const Graph& G) {
    EdgeList mst;
    DisjointSet ds(G.numVertices);
    
    // Process edges in ascending order of weight
    Graph sortedGraph = G.sort_edges();
    
    for (const auto& edge : sortedGraph) {
        if (ds.findSet(edge.u) != ds.findSet(edge.v)) {
            mst.push_back(edge);
            ds.unionSets(edge.u, edge.v);
        }
    }
    return mst;
}

// Calculates the total cost/weight of the edge list
int sum_weights(EdgeList const& L) {
    int total = 0;
    for (const auto& edge : L) {
        total += edge.weight;
    }
    return total;
}

// Depth First Search Traversal
VertexList dfs(const Graph& graph, Vertex startVertex) {
    VertexList result;
    if (graph.numVertices == 0) return result;

    vector<bool> visited(graph.numVertices, false);
    stack<Vertex> S;

    visited[startVertex] = true;
    S.push(startVertex);

    while (!S.empty()) {
        Vertex v = S.top();
        S.pop();
        result.push_back(v);

        for (Vertex w : graph.edges_from(v)){
            if (!visited[w]) {
                visited[w] = true;
                S.push(w);
            }
        }
    }
    return result;
}

// Breadth First Search Traversal
VertexList bfs(const Graph& graph, Vertex startVertex) {
    VertexList result;
    if (graph.numVertices == 0) return result;

    vector<bool> visited(graph.numVertices, false);
    queue<Vertex> Q;

    visited[startVertex] = true;
    Q.push(startVertex);

    while (!Q.empty()) {
        Vertex v = Q.front();
        Q.pop();
        result.push_back(v);

        for (Vertex w : graph.edges_from(v)) {
            if (!visited[w]) {
                visited[w] = true;
                Q.push(w);
            }
        }
    }
    return result;
}

