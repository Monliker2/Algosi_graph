#include <iostream>
#include <vector>
#include <map>
#include <algorithm>


using namespace std;

struct Vertie {
    char name;
    int mark;
};

class Graph {

private:
    vector<vector<int>> matrix;
    vector<vector<int>> matrix_weight;

    map<int, Vertie> _vertices;

public:
    Graph() = default;

    int indexOfName(char c) {
        for (const auto& [key, value] : _vertices) {
            if (value.name == c) {
                return key;
            }
        }
        return -1;
    }

    char nameOfIndex(int i) {
        return _vertices[i].name;
    }

    void ADD_V(char v, int mark = 0) {
        _vertices[_vertices.size()] = {v, mark};

        matrix.resize(_vertices.size());
        for (auto& row : matrix) {
            row.resize(_vertices.size(), 0);
        }

        matrix_weight.resize(_vertices.size());
        for (auto& row : matrix_weight) {
            row.resize(_vertices.size(), 0);
        }
    }

    void ADD_E(char s, char e, int weight = 1) {
        int start = indexOfName(s);
        int end = indexOfName(e);

        if (start >= matrix.size() || end >= matrix.size()) {
            throw std::out_of_range("Invalid vertex index in ADD_E");
        }

        matrix[start][end] = 1;
        matrix_weight[start][end] = weight;
    }

    void DEL_V(char v) {
        int idx = indexOfName(v);
        if (idx == -1) return;

        _vertices.erase(idx);

        matrix.erase(matrix.begin() + idx);
        for (auto& row : matrix) {
            row.erase(row.begin() + idx);
        }

        matrix_weight.erase(matrix_weight.begin() + idx);
        for (auto& row : matrix_weight) {
            row.erase(row.begin() + idx);
        }

        /*std::map<int, Vertie> updated_vertices;

        for (const auto& [key, value] : _vertices) {
            int new_key = key > idx ? key - 1 : key;
            updated_vertices[new_key] = value;
        }
        _vertices = std::move(updated_vertices);*/
    }


    void DEL_E(char s, char e) {
        int start = indexOfName(s);
        int end = indexOfName(e);

        matrix[start][end] = 0;
        matrix_weight[start][end] = 0;
    }

    void EDIT_V(char v, int mark){
        _vertices[indexOfName(v)] = {v, mark};
    }

    void EDIT_E(char s, char e, int weight) {
        int start = indexOfName(s);
        int end = indexOfName(e);
        matrix_weight[start][end] = weight;
    }

    int FIRST(char v) {
        for (int i = 0; i < matrix[indexOfName(v)].size(); ++i) {
            if (matrix[indexOfName(v)][i] == 1) {
                return i;
            }
        }
        return -1;
    }

    int NEXT(char v, int i) {
        for (int j = i + 1; j < matrix[indexOfName(v)].size(); ++j) {
            if (matrix[indexOfName(v)][j] == 1) {
                return j;
            }
        }
        return -1;
    }

    int VERTEX(char v, int i) {
        for (int j = 0; j < matrix[indexOfName(v)].size(); ++j) {
            if (matrix[indexOfName(v)][j] == 1 && i == 0) {
                return j;
            }
            else if (matrix[indexOfName(v)][j] == 1) i--;
        }

        return -1;
    }
    void print()  {
        std::cout << "Vertices:\n";
        for (const auto& [vertex, vertie] : _vertices) {
            std::cout << "Vertex " << vertex << " (name: " << vertie.name << " mark: " << vertie.mark << ")\n";
        }

        // Вывод матрицы смежности
        std::cout << "\nAdjacency Matrix:\n";
        for (const auto& row : matrix) {
            for (int cell : row) {
                std::cout << cell << " ";
            }
            std::cout << "\n";
        }

        // Вывод матрицы весов
        std::cout << "\nWeight Matrix:\n";
        for (const auto& row : matrix_weight) {
            for (int weight : row) {
                std::cout << weight << " ";
            }
            std::cout << "\n";
        }
    }
    int size() const {
        return matrix.size();
    }
};

void dfs(char v, vector<int>& path, Graph& graph, vector<vector<int>>& cycles) {
    // Получаем индекс первой смежной вершины
    int w = graph.FIRST(v);
    if (w == -1) return; // Если нет смежных вершин, возвращаемся

    while (w != -1) {
        if (w == path[0]) { // Найден цикл
            path.push_back(w);
            cycles.push_back(path);
            path.pop_back(); // Убираем последний элемент для продолжения поиска
        } else if (find(path.begin(), path.end(), w) == path.end()) { // Если вершина еще не в пути
            path.push_back(w);
            dfs(graph.nameOfIndex(w), path, graph, cycles);
            path.pop_back(); // Убираем вершину после рекурсивного вызова
        }

        // Переходим к следующей смежной вершине
        w = graph.NEXT(v, w);
    }
}

void task(Graph& graph) {
    vector<vector<int>> cycles;

    for (int i = 0; i < graph.size(); ++i) { // Перебор всех вершин
        vector<int> path = {i}; // Стартовый путь
        dfs(graph.nameOfIndex(i), path, graph, cycles); // Запуск DFS для вершины i
    }

    // Вывод результатов
    cout << "Количество циклов: " << cycles.size() << "\n";
    cout << "Варианты обхода, образующие циклы:\n";
    for (const auto& cycle : cycles) {
        for (int v : cycle) {
            cout << v << " ";
        }
        cout << "\n";
    }
}


// Точка входа в программу
int main() {
    Graph g;

    /*g.ADD_V('a',0);
    g.ADD_V('b',1);
    g.ADD_V('c',2);
    g.ADD_V('d',3);
    g.ADD_V('e',4);

    g.ADD_E('a','b');
    g.ADD_E('a','c');
    g.ADD_E('b','d');
    g.ADD_E('b','e');
    g.ADD_E('c','e');
    g.ADD_E('d','e');
    g.ADD_E('e','b');*/



    g.ADD_V('a','a');
    g.ADD_V('b','b');
    g.ADD_V('c','c');
    g.ADD_V('d','d');
    g.ADD_V('e','e');
    g.ADD_V('f','f');
    g.ADD_V('j','j');

    g.ADD_E('a','b');
    g.ADD_E('a','d');
    g.ADD_E('b','d');
    g.ADD_E('c','a');
    g.ADD_E('d','e');
    g.ADD_E('d','f');
    g.ADD_E('d','a');
    g.ADD_E('e','b');
    g.ADD_E('e','c');
    g.ADD_E('f','a');


    g.print();

    task(g);

    return 0;
}
