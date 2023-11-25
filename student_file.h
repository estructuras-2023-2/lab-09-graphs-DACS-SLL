#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct Subset {
    int parent, rank;
};

class Carretera {
public:
    string id;
    int ciu1, ciu2, cost;

    Carretera(string id, int ciu1, int ciu2, int cost) : id(id), ciu1(ciu1), ciu2(ciu2), cost(cost) {}

    bool operator<(const Carretera& compareCarretera) const {
        int comparCosto = this->cost - compareCarretera.cost;
        if (comparCosto != 0) return comparCosto < 0;
        return this->id < compareCarretera.id;
    }
};

class Graph {
public:
    vector<Carretera> caminos;
    unordered_map<string, int> map;

    Graph() {}

    int find(Subset subsets[], int i) {
        if (subsets[i].parent != i) {
            subsets[i].parent = find(subsets, subsets[i].parent);
        }
        return subsets[i].parent;
    }

    void unionSets(Subset subsets[], int x, int y) {
        int xB = find(subsets, x);
        int yB = find(subsets, y);

        if (subsets[xB].rank < subsets[yB].rank) {
            subsets[xB].parent = yB;
        } else if (subsets[xB].rank > subsets[yB].rank) {
            subsets[yB].parent = xB;
        } else {
            subsets[xB].parent = yB;
            subsets[xB].rank++;
        }
    }

    void addEdge(string id, string ciu1, string ciu2, int cost) {
        if (map.find(ciu1) == map.end()) {
            map[ciu1] = map.size(); 
        }
        if (map.find(ciu2) == map.end()) {
            map[ciu2] = map.size(); 
        }
        caminos.emplace_back(id, map[ciu1], map[ciu2], cost);
    }

    string kruskalMethod() {
        if (caminos.empty()) {
            return "IMPOSIBLE";
        }

        sort(caminos.begin(), caminos.end());

        Subset subsets[map.size()];
        for (int i = 0; i < map.size(); ++i) {
            subsets[i].parent = i;
            subsets[i].rank = 0;
        }

        string carreteraReparable = "";
        int minCost = 5000000;

        int citiesConnected = 0;

        for (const Carretera& cam : caminos) {
            int x = find(subsets, cam.ciu1);
            int y = find(subsets, cam.ciu2);

            if (x != y) {
                if (cam.cost != 0 && cam.cost < minCost) {
                    carreteraReparable += cam.id;
                    minCost = cam.cost;
                }
                unionSets(subsets, x, y);
                citiesConnected++;
            }
        }

        if (citiesConnected == 0 && map.size() == 1) {
            return "";
        } else if (citiesConnected < map.size() - 1) {
            return "IMPOSIBLE";
        }

        return carreteraReparable.empty() ? "" : carreteraReparable;
    }
};

string reconstruye(vector<string>& carreteras) {
    if (carreteras.size() == 1) {
        return "";
    }

    Graph graph;

    for (const string& road : carreteras) {
        vector<string> partes;

        size_t start = 0, end = 0;
        while ((end = road.find(' ', start)) != string::npos) {
            partes.push_back(road.substr(start, end - start));
            start = end + 1;
        }
        partes.push_back(road.substr(start));
        try {
            int cost = stoi(partes[3]);
            graph.addEdge(partes[0], partes[1], partes[2], cost);
        } catch (invalid_argument& e) {
            return "IMPOSIBLE";
        }
    }

    return graph.kruskalMethod();
}
