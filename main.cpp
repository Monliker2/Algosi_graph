#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <map>
#include <set>

using namespace std;

using T_ADJ_MATRIX = vector<vector<int>>;
using T_PATH = vector<int>;

class Vertex {
    int _index;

public:
    explicit Vertex(int index) : _index(index) {}

    int index() const {
        return _index;
    }

    friend ostream& operator<<(ostream& os, const Vertex& vertex) {
        os << "Vertex[i: " << vertex._index << "]";
        return os;
    }
};

class Edge {
    pair<int, int> _vertices;
    int len;

public:
    Edge(int v1 = -1, int v2 = -1, int length = 1)
        : _vertices(v1, v2), len(length) {}

    pair<int, int> vertices() const {
        return _vertices;
    }

    int length() const {
        return len;
    }

    friend ostream& operator<<(ostream& os, const Edge& edge) {
        os << "Edge[" << edge._vertices.first << " -> "
           << edge._vertices.second << ", " << edge.len << "]";
        return os;
    }
};

class Graph {
    vector<Vertex> _vertices;
    vector<Edge> _edges;

    void _initVertices(int dim) {
        _vertices.clear();
        for (int i = 0; i < dim; ++i) {
            _vertices.emplace_back(i);
        }
    }

public:
    Graph() = default;

    void load(const string& filename) {
        vector<vector<int>> data;
        ifstream file(filename);

        if (!file.is_open()) {
            throw runtime_error("Не удалось открыть файл");
        }

        int dim;
        file >> dim;
        file.ignore();

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            vector<int> row;
            int x;
            while (ss >> x) {
                row.push_back(x);
            }
            data.push_back(row);
        }

        _initVertices(dim);
        for (int i = 0; i < data.size(); ++i) {
            for (int j = 0; j < data[i].size(); ++j) {
                if (data[i][j] > 0) {
                    addEdge(i, j, data[i][j]);
                }
            }
        }
    }

    void addVertex(int index) {
        for (const auto& vertex : _vertices) {
            if (vertex.index() == index) {
                throw runtime_error("Вершина с таким индексом уже существует!");
            }
        }
        _vertices.emplace_back(index);
    }

    void addEdge(int v1, int v2, int edgeLen = 1) {
        bool foundV1 = false, foundV2 = false;

        for (const auto& vertex : _vertices) {
            if (vertex.index() == v1) foundV1 = true;
            if (vertex.index() == v2) foundV2 = true;
        }

        if (!(foundV1 && foundV2) || v1 == v2) {
            throw runtime_error("Невозможная пара индексов вершин!");
        }

        _edges.emplace_back(v1, v2, edgeLen);
    }

    void deleteVertex(int index) {
        auto it = find_if(_vertices.begin(), _vertices.end(),
                          [index](const Vertex& v) { return v.index() == index; });

        if (it == _vertices.end()) {
            throw runtime_error("Вершина с таким индексом не существует!");
        }

        _vertices.erase(it);

        _edges.erase(remove_if(_edges.begin(), _edges.end(),
                               [index](const Edge& e) {
                                   return e.vertices().first == index ||
                                          e.vertices().second == index;
                               }),
                     _edges.end());
    }

    void deleteEdge(int v1, int v2) {
        auto it = find_if(_edges.begin(), _edges.end(),
                          [v1, v2](const Edge& e) {
                              return e.vertices() == make_pair(v1, v2);
                          });

        if (it == _edges.end()) {
            throw runtime_error("Ребра с таким набором вершин не существует!");
        }

        _edges.erase(it);
    }

    T_ADJ_MATRIX toAdjMatrix() const {
        T_ADJ_MATRIX adjMatrix(_vertices.size(), vector<int>(_vertices.size(), 0));
        for (const auto& edge : _edges) {
            adjMatrix[edge.vertices().first][edge.vertices().second] = edge.length();
        }
        return adjMatrix;
    }

    void print() const {
        cout << "Vertices:\n";
        for (const auto& vertex : _vertices) {
            cout << vertex << "\n";
        }

        cout << "Edges:\n";
        for (const auto& edge : _edges) {
            cout << edge << "\n";
        }

        cout << "Adjacency Matrix:\n";
        for (const auto& row : toAdjMatrix()) {
            for (int val : row) {
                cout << val << " ";
            }
            cout << "\n";
        }
    }
};

void task(Graph& graph) {
    auto adjMatrix = graph.toAdjMatrix();
    vector<T_PATH> cycles;

    function<void(T_PATH)> dfs = [&](T_PATH path) {
        int v = path.back();
        for (int i = 0; i < adjMatrix.size(); ++i) {
            if (adjMatrix[v][i]) {
                if (i == path[0]) {
                    cycles.push_back(path);
                    cycles.back().push_back(i);
                } else if (find(path.begin(), path.end(), i) == path.end()) {
                    T_PATH newPath = path;
                    newPath.push_back(i);
                    dfs(newPath);
                }
            }
        }
    };

    for (int i = 0; i < adjMatrix.size(); ++i) {
        dfs({i});
    }

    cout << "Количество циклов: " << cycles.size() << "\n";
    cout << "Варианты обхода образующие циклы:\n";
    for (const auto& cycle : cycles) {
        for (int v : cycle) {
            cout << v << " ";
        }
        cout << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Использование: " << argv[0] << " [ПУТЬ_ДО_ФАЙЛА_С_ГРАФОМ] [РЕЖИМ]\n";
        return -1;
    }

    string filename = argv[1];
    string mode = argv[2];

    Graph graph;
    try {
        graph.load(filename);
    } catch (const exception& e) {
        cerr << e.what() << "\n";
        return -1;
    }

    if (mode == "example") {
        graph.print();
    } else if (mode == "task") {
        task(graph);
    } else {
        cerr << "Неизвестный режим: " << mode << "\n";
        return -1;
    }

    return 0;
}
