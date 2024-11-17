#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Graph {
private:
    vector<vector<int>> matrix;
    vector<vector<int>> matrix_weight;

    map<int, int> _vertices;

public:
    Graph() = default;
    void ADD_V(int v, int mark = 0) {
        _vertices[v] = mark;

        matrix.resize(_vertices.size());
        for (auto& row : matrix) {
            row.resize(_vertices.size(), 0);
        }

        matrix_weight.resize(_vertices.size());
        for (auto& row : matrix_weight) {
            row.resize(_vertices.size(), 0);
        }
    }

    void ADD_E(int start, int end, int weight = 1) {
        //if(_vertices.find(start) != _vertices.end()) {
        //    ADD_V(start);
        //}
        if (start >= matrix.size() || end >= matrix.size()) {
            throw std::out_of_range("Invalid vertex index in ADD_E");
        }

        matrix[start][end] = 1;
        matrix_weight[start][end] = weight;
    }

    void DEL_V(int v) {
        _vertices.erase(v);

        matrix.erase(matrix.begin() + v);
        for (auto& row : matrix) {
            row.erase(row.begin() + v);
        }

        matrix_weight.erase(matrix_weight.begin() + v);
        for (auto& row : matrix_weight) {
            row.erase(row.begin() + v);
        }

        map<int, int> updated_vertices;
        for (const auto& [key, value] : _vertices) {
            updated_vertices[key > v ? key - 1 : key] = value;
        }
        _vertices = std::move(updated_vertices);
    }


    void DEL_E(int start, int end) {
        matrix[start][end] = 0;
        matrix_weight[start][end] = 0;
    }

    void EDIT_V(int v, int mark){
        _vertices[v] = mark;
    }

    void EDIT_E(int start, int end, int weight) {
        matrix_weight[start][end] = weight;
    }

    int FIRST(int v) {
        for (int i = 0; i < matrix[v].size(); ++i) {
            if (matrix[v][i] == 1) {
                return i;
            }
        }
        return -1;
    }

    int NEXT(int v, int i) {
        for (int j = i + 1; j < matrix[v].size(); ++j) {
            if (matrix[v][j] == 1) {
                return j;
            }
        }
        return -1;
    }

    int VERTEX(int v, int i) {
        for (int j = 0; j < matrix[v].size(); ++j) {
            if (matrix[v][j] == 1 && i == 0) {
                return j;
            }
            else if (matrix[v][j] == 1) i--;
        }

        return -1;
    }
    void print()  {
        std::cout << "Vertices:\n";
        for (const auto& [vertex, mark] : _vertices) {
            std::cout << "Vertex " << vertex << " (Mark: " << mark << ")\n";
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
};

// Функция для поиска циклов в графе
// void task(Graph& graph) {
//     auto adjMatrix = graph.toAdjMatrix(); // Матрица смежности графа
//     vector<T_PATH> cycles; // Список циклов
//
//     // Поиск в глубину для нахождения циклов
//     function<void(T_PATH)> dfs = [&](T_PATH path) {
//         int v = path.back();
//         for (int i = 0; i < adjMatrix.size(); ++i) {
//             if (adjMatrix[v][i]) {
//                 if (i == path[0]) { // Найден цикл
//                     cycles.push_back(path);
//                     cycles.back().push_back(i);
//                 } else if (find(path.begin(), path.end(), i) == path.end()) {
//                     T_PATH newPath = path;
//                     newPath.push_back(i);
//                     dfs(newPath);
//                 }
//             }
//         }
//     };
//
//     // Запуск DFS с каждой вершины
//     for (int i = 0; i < adjMatrix.size(); ++i) {
//         dfs({i});
//     }
//
//     // Вывод найденных циклов
//     cout << "Количество циклов: " << cycles.size() << "\n";
//     cout << "Варианты обхода образующие циклы:\n";
//     for (const auto& cycle : cycles) {
//         for (int v : cycle) {
//             cout << v << " ";
//         }
//         cout << "\n";
//     }
// }

// Точка входа в программу
int main() {
    Graph g;

    g.ADD_V(0,0);
    g.ADD_V(1,1);
    g.ADD_V(2,2);
    g.ADD_V(3,3);
    g.ADD_V(4,4);

    g.ADD_E(0,1);
    g.ADD_E(0,2);
    g.ADD_E(1,3);
    g.ADD_E(1,4);
    g.ADD_E(2,4);
    g.ADD_E(3,4);

    g.print();

    g.DEL_V(0);

    g.print();
}
