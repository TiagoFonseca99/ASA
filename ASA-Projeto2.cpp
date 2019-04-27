/*=====================================*/
/*      ASA 2018/2019 - Projeto 2      */
/*	        Joao Dinis 89485	       */
/*	      Tiago Fonseca 89542	       */
/*=====================================*/

/* Libraries */
#include <iostream>
#include <climits>
#include <vector>

using namespace std;

// Structs
struct Edge
{
    int flow, capacity;

    // An edge u--->v has start vertex as u and end
    // vertex as v.
    int u, v;

    Edge(int flow, int capacity, int u, int v)
    {
        this->flow = flow;
        this->capacity = capacity;
        this->u = u;
        this->v = v;
    }
};

struct Vertex
{
    int h, e_flow;

    Vertex(int h, int e_flow)
    {
        this->h = h;
        this->e_flow = e_flow;
    }
};

// Classes
class Graph {
    int V;    // No. of vertices
    vector<Vertex> ver;
    vector<Edge> edge;

    // Function to push excess flow from u
    bool push(int u) {
        // Traverse through all edges to find an adjacent (of u)
        // to which flow can be pushed
        for (int i = 0; i < edge.size(); i++) {
            // Checks u of current edge is same as given
            // overflowing vertex
            if (edge[i].u == u) {
                // if flow is equal to capacity then no push
                // is possible
                if (edge[i].flow == edge[i].capacity)
                    continue;

                // Push is only possible if height of adjacent
                // is smaller than height of overflowing vertex
                if (ver[u].h > ver[edge[i].v].h) {
                    // Flow to be pushed is equal to minimum of
                    // remaining flow on edge and excess flow.
                    int flow = min(edge[i].capacity - edge[i].flow,
                                   ver[u].e_flow);

                    // Reduce excess flow for overflowing vertex
                    ver[u].e_flow -= flow;

                    // Increase excess flow for adjacent
                    ver[edge[i].v].e_flow += flow;

                    // Add residual flow (With capacity 0 and negative
                    // flow)
                    edge[i].flow += flow;

                    updateReverseEdgeFlow(i, flow);

                    return true;
                }
            }
        }
        return false;
    }

    // Function to relabel a vertex u
    void relabel(int u) {
        // Initialize minimum height of an adjacent
        int mh = INT_MAX;

        // Find the adjacent with minimum height
        for (int i = 0; i < edge.size(); i++)
        {
            if (edge[i].u == u)
            {
                // if flow is equal to capacity then no
                // relabeling
                if (edge[i].flow == edge[i].capacity)
                    continue;

                // Update minimum height
                if (ver[edge[i].v].h < mh)
                {
                    mh = ver[edge[i].v].h;

                    // updating height of u
                    ver[u].h = mh + 1;
                }
            }
        }
    }

    // This function is called to initialize
    // preflow
    void preflow(int s) {
        // Making h of source Vertex equal to no. of vertices
        // Height of other vertices is 0.
        ver[s].h = ver.size();

        //
        for (int i = 0; i < edge.size(); i++) {
            // If current edge goes from source
            if (edge[i].u == s) {
                // Flow is equal to capacity
                edge[i].flow = edge[i].capacity;

                // Initialize excess flow for adjacent v
                ver[edge[i].v].e_flow += edge[i].flow;

                // Add an edge from v to s in residual graph with
                // capacity equal to 0
                edge.push_back(Edge(-edge[i].flow, 0, edge[i].v, s));
            }
        }
    }

    // Function to reverse edge
    void updateReverseEdgeFlow(int i, int flow) {
        int u = edge[i].v, v = edge[i].u;

        for (int j = 0; j < edge.size(); j++) {
            if (edge[j].v == v && edge[j].u == u) {
                edge[j].flow -= flow;
                return;
            }
        }

        // adding reverse Edge in residual graph
        Edge e = Edge(0, flow, u, v);
        edge.push_back(e);
    }

public:
    Graph() {} // Constructor

    void graphInit(int V)
    {
        this->V = V;

        // all vertices are initialized with 0 height
        // and 0 excess flow
        for (int i = 0; i < V; i++)
            ver.push_back(Vertex(0, 0));
    }

    // function to add an edge to graph
    void addEdge(int u, int v, int capacity) {
        // flow is initialized with 0 for all edge
        edge.push_back(Edge(0, capacity, u, v));
    }

    // returns index of overflowing Vertex
    int overFlowVertex(vector<Vertex>& ver, int t) {
        for (int i = 1; i < ver.size(); i++)
            if ((ver[i].e_flow > 0) & (i != t))
                return i;

        return -1;
    }

    // returns maximum flow from s to t
    int getMaxFlow(int s, int t) {
        preflow(s);

        while (overFlowVertex(ver, t) != -1) {
            int u = overFlowVertex(ver, t);
            if (!push(u))
                relabel(u);
        }

        return ver[t].e_flow;
    }
};

/* Prototypes */


/* Variables */
Graph g;
int fornecedores, abastecimento;

void readInput() {
	int ligacoes, edge1, edge2, size, totalEdges;

    scanf("%d %d %d", &fornecedores, &abastecimento, &ligacoes);

	if (fornecedores < 0){
		printf("Numero de fornecedores tem que ser maior que 0\n");
		exit(-1);
	}

	if (abastecimento < 0){
		printf("Numero de estacoes de abastecimento tem que ser maior ou igual a 0\n");
		exit(-1);
	}

	if (ligacoes < 0){
		printf("Numero de ligacoes tem que ser maior ou igual a 0\n");
		exit(-1);
	}

    totalEdges = 2 + fornecedores + 2*abastecimento;
	g.graphInit(totalEdges);

	for (int i = 0; i < fornecedores; i++) {
		scanf("%d ", &size);
        edge2 = i + 2;
		g.addEdge(0, edge2, size);
	}

    for (int i = 0; i < abastecimento; i++) {
		scanf("%d ", &size);
        edge1 = fornecedores + i + 2;
        edge2 = edge1 + abastecimento;
		g.addEdge(edge1, edge2, size);
	}

    for (int i = 0; i < ligacoes; i++) {
		scanf("%d %d %d", &edge1, &edge2, &size);

        if (edge1 >= fornecedores + 2) {
            g.addEdge(edge1 + abastecimento, edge2, size);
        }
        else {
            g.addEdge(edge1, edge2, size);
        }
	}
}

/* Codigo */
int main(int argc, char *argv[]) {

	readInput();
    int s = 0, t = 1;

    printf("%d\n", g.getMaxFlow(s, t));

    return 0;
}
