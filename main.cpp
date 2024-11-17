#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <functional>

using namespace std;

// Определяем типы для матрицы смежности и пути
using T_ADJ_MATRIX = vector<vector<int>>;
using T_PATH = vector<int>;

// Класс для представления вершины графа
class Vertex {
    int _index; // Индекс вершины

public:
    // Конструктор для инициализации вершины
    explicit Vertex(int index) : _index(index) {}

    // Метод для получения индекса вершины
    int index() const {
        return _index;
    }

    // Оператор вывода для отображения информации о вершине
    friend ostream& operator<<(ostream& os, const Vertex& vertex) {
        os << "Vertex[i: " << vertex._index << "]";
        return os;
    }
};

// Класс для представления ребра графа
class Edge {
    pair<int, int> _vertices; // Пара вершин (начало и конец ребра)
    int len; // Длина ребра

public:
    // Конструктор для инициализации ребра
    Edge(int v1 = -1, int v2 = -1, int length = 1)
        : _vertices(v1, v2), len(length) {}

    // Метод для получения пар вершин
    pair<int, int> vertices() const {
        return _vertices;
    }

    // Метод для получения длины ребра
    int length() const {
        return len;
    }

    // Оператор вывода для отображения информации о ребре
    friend ostream& operator<<(ostream& os, const Edge& edge) {
        os << "Edge[" << edge._vertices.first << " -> "
           << edge._vertices.second << ", " << edge.len << "]";
        return os;
    }
};

// Класс для представления графа
class Graph {
    vector<Vertex> _vertices; // Список вершин
    vector<Edge> _edges;      // Список ребер

    // Инициализация вершин
    void _initVertices(int dim) {
        _vertices.clear();
        for (int i = 0; i < dim; ++i) {
            _vertices.emplace_back(i);
            std::cout << "Инициализирована вершина: " << i << std::endl;
        }
    }

public:
    Graph() = default; // Конструктор по умолчанию

    // Загрузка графа из файла
    void load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Не удалось открыть файл");
        }

        int dim; // Размер матрицы смежности
        file >> dim;

        // Инициализация вершин
        _vertices.clear();
        for (int i = 0; i < dim; ++i) {
            _vertices.push_back(Vertex(i));
            std::cout << "Инициализирована вершина: " << i << std::endl;
        }

        // Загрузка матрицы смежности
        for (int i = 0; i < dim; ++i) {
            for (int j = 0; j < dim; ++j) {
                int edge_len;
                file >> edge_len; // Считываем длину ребра
                std::cout << "Матрица: [" << i << "][" << j << "] = " << edge_len << std::endl;
                // Добавляем ребро, если его длина > 0 и это не самоссылка
                if (edge_len > 0 && i != j) {
                    addEdge(i, j, edge_len);
                }
            }
        }
    }

    // Добавление вершины
    void addVertex(int index) {
        for (const auto& vertex : _vertices) {
            if (vertex.index() == index) {
                throw runtime_error("Вершина с таким индексом уже существует!");
            }
        }
        _vertices.emplace_back(index);
    }

    // Добавление ребра
    void addEdge(int v1, int v2, int edge_len = 1) {
        std::cout << "Пытаемся добавить ребро: (" << v1 << " -> " << v2 << ") длина: " << edge_len << std::endl;

        bool foundV1 = false;
        bool foundV2 = false;
        // Проверяем существование вершин
        for (const auto& vertex : _vertices) {
            if (vertex.index() == v1) foundV1 = true;
            if (vertex.index() == v2) foundV2 = true;
        }

        // Если вершины не найдены или ребро - петля, выбрасываем исключение
        if (!foundV1 || !foundV2 || v1 == v2) {
            throw std::runtime_error("Невозможная пара индексов вершин!");
        } else {
            _edges.push_back(Edge(v1, v2, edge_len)); // Добавляем ребро
        }
    }

    // Удаление вершины
    void deleteVertex(int index) {
        // Находим вершину по индексу
        auto it = find_if(_vertices.begin(), _vertices.end(),
                          [index](const Vertex& v) { return v.index() == index; });

        if (it == _vertices.end()) {
            throw runtime_error("Вершина с таким индексом не существует!");
        }

        // Удаляем вершину
        _vertices.erase(it);

        // Удаляем связанные с вершиной ребра
        _edges.erase(remove_if(_edges.begin(), _edges.end(),
                               [index](const Edge& e) {
                                   return e.vertices().first == index ||
                                          e.vertices().second == index;
                               }),
                     _edges.end());
    }

    // Удаление ребра
    void deleteEdge(int v1, int v2) {
        // Находим ребро
        auto it = find_if(_edges.begin(), _edges.end(),
                          [v1, v2](const Edge& e) {
                              return e.vertices() == make_pair(v1, v2);
                          });

        if (it == _edges.end()) {
            throw runtime_error("Ребра с таким набором вершин не существует!");
        }

        // Удаляем ребро
        _edges.erase(it);
    }

    // Создание матрицы смежности
    T_ADJ_MATRIX toAdjMatrix() const {
        T_ADJ_MATRIX adjMatrix(_vertices.size(), vector<int>(_vertices.size(), 0));
        for (const auto& edge : _edges) {
            adjMatrix[edge.vertices().first][edge.vertices().second] = edge.length();
        }
        return adjMatrix;
    }

    // Печать графа
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

// Функция для поиска циклов в графе
void task(Graph& graph) {
    auto adjMatrix = graph.toAdjMatrix(); // Матрица смежности графа
    vector<T_PATH> cycles; // Список циклов

    // Поиск в глубину для нахождения циклов
    function<void(T_PATH)> dfs = [&](T_PATH path) {
        int v = path.back();
        for (int i = 0; i < adjMatrix.size(); ++i) {
            if (adjMatrix[v][i]) {
                if (i == path[0]) { // Найден цикл
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

    // Запуск DFS с каждой вершины
    for (int i = 0; i < adjMatrix.size(); ++i) {
        dfs({i});
    }

    // Вывод найденных циклов
    cout << "Количество циклов: " << cycles.size() << "\n";
    cout << "Варианты обхода образующие циклы:\n";
    for (const auto& cycle : cycles) {
        for (int v : cycle) {
            cout << v << " ";
        }
        cout << "\n";
    }
}

// Точка входа в программу
int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " [PATH_TO_A_FILE_WITH_GRAPH] [MODE]\n";
        return -1;
    }

    string filename = argv[1]; // Путь к файлу с графом
    string mode = argv[2];     // Режим работы

    Graph graph;
    try {
        graph.load(filename); // Загрузка графа
    } catch (const exception& e) {
        cerr << e.what() << "\n";
        return -1;
    }

