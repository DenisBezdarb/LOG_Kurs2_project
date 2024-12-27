#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>
#include <random>
#include <locale>
#include <Windows.h>

typedef struct {
    unsigned short path[2048];
    int size;
} Path;

int_fast8_t** create_graph(int size);
void initialize_unorient_graph(int_fast8_t** graph_pointer, int size, float edge_probability);
void initialize_orient_graph(int_fast8_t** graph_pointer, int size, float edge_probability);
void print_graph(int_fast8_t** graph_pointer, int size);
void free_graph(int_fast8_t** graph_pointer, int size);
bool findEulerianCycle(int_fast8_t** graph_pointer, int size, bool directed, Path* result);
bool isEulerian(int_fast8_t** graph_pointer, int size, bool directed);
void makeEulerian(int_fast8_t** graph_pointer, int size, bool directed);

int main(void) {
    setlocale(LC_ALL, "Russian");
    int size, choice = 0;
    float edge_probability;
    char file_name[128];

    while (1) {
        system("cls");
        printf(" Это программа для поиска Эйлеровых циклов в графе.\n");
        printf(" Выберите тип графа:\n");
        printf(" 1. Случайный неориентированный граф\n");
        printf(" 2. Случайный ориентированный граф\n");
        printf(" 3. Загрузить граф из файла\n");
        printf(" 4. Выход\n");
        printf("\n Ввод: ");
        scanf("%d", &choice);
        int_fast8_t** graph = NULL;

        switch (choice) {
        case 1:
        {
            printf(" Введите количество вершин в неориентированном графе: ");
            if (!scanf("%d", &size) || size <= 1) {
                fprintf(stderr, " Ошибка ввода количества вершин.");
                choice = 0;
                getchar();
                break;
            }

            graph = create_graph(size);
            fprintf(stdout, " Введите вероятность создания ребра в неориентированном графе (от 0 до 100): ");
            if (!scanf("%f", &edge_probability) || edge_probability < 0 || edge_probability > 100) {
                fprintf(stderr, " Ошибка ввода вероятности.");
                choice = 0;
                getchar();
                break;
            }

            srand((unsigned int)time(NULL));
            initialize_unorient_graph(graph, size, edge_probability);
            printf(" Матрица смежности неориентированного графа:\n\n");
            print_graph(graph, size);

            int make_eulerian = 0;
            if (!isEulerian(graph, size, false)) {
                printf("\n Граф не является Эйлеровым. Хотите сделать его Эйлеровым? (0 - нет, 1 - да): ");
                if (!scanf("%d", &make_eulerian) || make_eulerian < 0 || make_eulerian > 1) {
                    fprintf(stderr, " Неправильный ввод.");
                    choice = 0;
                    getchar();
                    break;
                }
            }

            if (make_eulerian == 1) {
                makeEulerian(graph, size, false);
                printf("\n Преобразованная матрица смежности:\n\n");
                print_graph(graph, size);
            }

            Path result1;
            printf("\n Введите название файла для сохранения: ");
            scanf("%s", file_name);

            FILE* file = fopen(file_name, "w");
            if (file == NULL) {
                printf("Ошибка открытия файла\n");
                break;
            }

            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    fprintf(file, "%d ", graph[j][i]);
                }
                fprintf(file, "\n");
            }

            if (findEulerianCycle(graph, size, false, &result1)) {
                fprintf(file, "\nЭйлеров цикл: ");
                for (int i = 1; i < result1.size; i++) {
                    fprintf(file, "%d-%d ", result1.path[i - 1] + 1, result1.path[i] + 1);
                }
                fprintf(file, "\n");

                printf("\n Эйлеров цикл существует: ");
                for (int i = 1; i < result1.size; i++) {
                    printf("%d-%d ", result1.path[i - 1] + 1, result1.path[i] + 1);
                }
                printf("\n");
            }
            else {
                fprintf(file, "\nЭйлеров цикл не существует.\n");
                printf("\n Эйлеров цикл не существует.\n");
            }

            fclose(file);
            free_graph(graph, size);
            break;
        }
        case 2:
        {
            printf(" Введите количество вершин в ориентированном графе: ");
            if (!scanf(" %d", &size) || size <= 1) {
                fprintf(stderr, " Ошибка ввода количества вершин.");
                choice = 0;
                getchar();
                break;
            }

            graph = create_graph(size);
            fprintf(stdout, " Введите вероятность создания ребра в ориентированном графе (от 0 до 100): ");
            if (!scanf("%f", &edge_probability) || edge_probability < 0 || edge_probability > 100) {
                fprintf(stderr, " Ошибка ввода вероятности.");
                choice = 0;
                getchar();
                break;
            }

            srand((unsigned int)time(NULL));
            initialize_orient_graph(graph, size, edge_probability);
            printf(" Матрица смежности ориентированного графа:\n\n");
            print_graph(graph, size);

            int make_eulerian = 0;
            if (!isEulerian(graph, size, true)) {
                printf("\n Граф не является Эйлеровым. Хотите сделать его Эйлеровым? (0 - нет, 1 - да): ");
                if (!scanf("%d", &make_eulerian) || make_eulerian < 0 || make_eulerian > 1) {
                    fprintf(stderr, " Неправильный ввод.");
                    choice = 0;
                    getchar();
                    break;
                }
            }

            if (make_eulerian == 1) {
                makeEulerian(graph, size, true);
                printf("\n Преобразованная матрица смежности:\n\n");
                print_graph(graph, size);
            }

            Path result2;
            printf("\n Введите название файла для сохранения: ");
            scanf("%s", file_name);

            FILE* file = fopen(file_name, "w");
            if (file == NULL) {
                printf("Ошибка открытия файла\n");
                break;
            }

            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    fprintf(file, "%d ", graph[j][i]);
                }
                fprintf(file, "\n");
            }

            if (findEulerianCycle(graph, size, true, &result2)) {
                fprintf(file, "\nЭйлеров цикл: ");
                for (int i = 1; i < result2.size; i++) {
                    fprintf(file, "%d-%d ", result2.path[i - 1] + 1, result2.path[i] + 1);
                }
                fprintf(file, "\n");

                printf("\n Эйлеров цикл существует: ");
                for (int i = 1; i < result2.size; i++) {
                    printf("%d-%d ", result2.path[i - 1] + 1, result2.path[i] + 1);
                }
                printf("\n");
            }
            else {
                fprintf(file, "\nЭйлеров цикл не существует.\n");
                printf("\n Эйлеров цикл не существует.\n");
            }

            fclose(file);
            free_graph(graph, size);
            break;
        }
        case 3:
        {
            printf(" Введите название файла для чтения матрицы смежности: ");
            scanf("%s", file_name);

            FILE* file = fopen(file_name, "r");
            if (file == NULL) {
                printf(" Ошибка: файл не существует или не может быть открыт.\n");
                break;
            }

            size = 0;
            char line[1024];
            if (fgets(line, sizeof(line), file)) {
                char* token = strtok(line, " \n");
                while (token != NULL) {
                    size++;
                    token = strtok(NULL, " \n");
                }
            }

            if (size < 2 || size > 1024) {
                printf(" Ошибка: некорректный размер матрицы.\n");
                fclose(file);
                break;
            }

            rewind(file);

            graph = create_graph(size);
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if (fscanf(file, "%hhd", &graph[j][i]) != 1) {
                        printf(" Ошибка чтения матрицы смежности.\n");
                        fclose(file);
                        free_graph(graph, size);
                        break;
                    }
                }
            }
            fclose(file);

            printf("\n Матрица смежности графа из файла:\n\n");
            print_graph(graph, size);

            bool is_directed = false;
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if (graph[i][j] != graph[j][i]) {
                        is_directed = true;
                        break;
                    }
                }
                if (is_directed) break;
            }

            int make_eulerian = 0;
            if (!isEulerian(graph, size, is_directed)) {
                printf("\n Граф не является Эйлеровым. Хотите сделать его Эйлеровым? (0 - нет, 1 - да): ");
                if (!scanf("%d", &make_eulerian) || make_eulerian < 0 || make_eulerian > 1) {
                    fprintf(stderr, " Неправильный ввод.");
                    choice = 0;
                    getchar();
                    break;
                }
            }

            if (make_eulerian == 1) {
                makeEulerian(graph, size, is_directed);
                printf("\n Преобразованная матрица смежности:\n\n");
                print_graph(graph, size);
            }

            Path result3;
            char output_file_name[128];
            printf("\n Введите название файла для сохранения результатов: ");
            scanf("%s", output_file_name);

            FILE* output_file = fopen(output_file_name, "w");
            if (output_file == NULL) {
                printf(" Ошибка создания выходного файла.\n");
                break;
            }

            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    fprintf(output_file, "%d ", graph[j][i]);
                }
                fprintf(output_file, "\n");
            }

            if (findEulerianCycle(graph, size, is_directed, &result3)) {
                fprintf(output_file, "\nЭйлеров цикл: ");
                printf("\n Эйлеров цикл существует: ");
                for (int i = 1; i < result3.size; i++) {
                    fprintf(output_file, "%d-%d ", result3.path[i - 1] + 1, result3.path[i] + 1);
                    printf("%d-%d ", result3.path[i - 1] + 1, result3.path[i] + 1);
                }
                fprintf(output_file, "\n");
                printf("\n");
            }
            else {
                fprintf(output_file, "\nЭйлеров цикл не существует.\n");
                printf("\n Эйлеров цикл не существует.\n");
            }

            fclose(output_file);
            free_graph(graph, size);
            break;
        }
        case 4:
            return 0;

        default:
            printf(" Неправильный ввод.\n");
            choice = 0;
            getchar();
            break;
        }

        printf("\n Нажмите Esc чтобы вернуться в меню.\n");
        while (GetKeyState(VK_ESCAPE) >= 0) {
            Sleep(100);
        }
        system("cls");
    }
    return 0;
}

int_fast8_t** create_graph(int size) {
    int_fast8_t** array = (int_fast8_t**)malloc(sizeof(int_fast8_t*) * size);
    for (int i = 0; i < size; i++) {
        array[i] = (int_fast8_t*)calloc(size, sizeof(int_fast8_t));
    }
    if (array == NULL) {
        fprintf(stderr, " Ошибка создания массива.");
        exit(1);
    }
    return array;
}

void initialize_unorient_graph(int_fast8_t** graph_pointer, int size, float edge_probability) {
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if ((float)rand() / (RAND_MAX / 100) < edge_probability) {
                graph_pointer[i][j] = 1;
                graph_pointer[j][i] = 1;
            }
            else {
                graph_pointer[i][j] = 0;
                graph_pointer[j][i] = 0;
            }
        }
        graph_pointer[i][i] = 0;
    }
}

void initialize_orient_graph(int_fast8_t** graph_pointer, int size, float edge_probability) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if ((float)rand() / (RAND_MAX / 100) < edge_probability) {
                graph_pointer[i][j] = 1;
            }
            else {
                graph_pointer[i][j] = 0;
            }
        }
        graph_pointer[i][i] = 0;
    }
}

void print_graph(int_fast8_t** graph_pointer, int size) {
    printf(" ");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fprintf(stdout, "%d ", graph_pointer[j][i]);
        }
        fprintf(stdout, "\n ");
    }
}

void free_graph(int_fast8_t** graph_pointer, int size) {
    for (int i = 0; i < size; i++) {
        free(graph_pointer[i]);
    }
    free(graph_pointer);
}

// Функция для проверки степеней вершин в неориентированном графе
bool checkDegrees(int_fast8_t** graph_pointer, int size) {
    for (int i = 0; i < size; i++) {
        int degree = 0;
        for (int j = 0; j < size; j++) {
            degree += graph_pointer[i][j];
        }
        if (degree % 2 != 0) return false;
    }
    return true;
}

// Функция для проверки входящих и исходящих степеней в ориентированном графе
bool checkInOutDegrees(int_fast8_t** graph_pointer, int size) {
    for (int i = 0; i < size; i++) {
        int in_degree = 0, out_degree = 0;
        for (int j = 0; j < size; j++) {
            out_degree += graph_pointer[i][j];
            in_degree += graph_pointer[j][i];
        }
        if (in_degree != out_degree) return false;
    }
    return true;
}

// Функция для проверки связности графа
bool isConnected(int_fast8_t** graph_pointer, int size) {
    bool visited[1024] = { false };
    int stack[1024], top = -1;

    // Начинаем с первой вершины
    stack[++top] = 0;
    visited[0] = true;

    while (top >= 0) {
        int vertex = stack[top--];
        for (int i = 0; i < size; i++) {
            if (graph_pointer[vertex][i] && !visited[i]) {
                stack[++top] = i;
                visited[i] = true;
            }
        }
    }

    // Проверяем, все ли вершины были посещены
    for (int i = 0; i < size; i++) {
        if (!visited[i]) return false;
    }
    return true;
}

void dfs_helper(int v, int size, int_fast8_t** graph_pointer, bool* visited, int* stack, int* stack_size) {
    visited[v] = true;
    for (int i = 0; i < size; i++) {
        if (graph_pointer[v][i] && !visited[i]) {
            dfs_helper(i, size, graph_pointer, visited, stack, stack_size);
        }
    }
    stack[*stack_size] = v;
    (*stack_size)++;
}

void makeEulerian(int_fast8_t** graph_pointer, int size, bool directed) {
    if (directed) {
        int* in_degree = (int*)calloc(size, sizeof(int));
        int* out_degree = (int*)calloc(size, sizeof(int));

        // Подсчет степеней
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (graph_pointer[i][j]) {
                    out_degree[i]++;
                    in_degree[j]++;
                }
            }
        }

        // Проверка связности и поиск компонент
        bool* visited = (bool*)calloc(size, sizeof(bool));
        int* stack = (int*)calloc(size, sizeof(int));
        int stack_size = 0;

        // Находим первую вершину с несбалансированными степенями
        int start_vertex = 0;
        for (int i = 0; i < size; i++) {
            if (in_degree[i] != out_degree[i]) {
                start_vertex = i;
                break;
            }
        }

        dfs_helper(start_vertex, size, graph_pointer, visited, stack, &stack_size);

        // Проверяем, все ли вершины достижимы
        bool needs_connection = false;
        for (int i = 0; i < size; i++) {
            if (!visited[i]) {
                needs_connection = true;
                break;
            }
        }

        // Соединяем компоненты только если необходимо
        if (needs_connection) {
            memset(visited, 0, size * sizeof(bool));
            stack_size = 0;

            // Повторный DFS для построения полного пути
            for (int i = 0; i < size; i++) {
                if (!visited[i]) {
                    dfs_helper(i, size, graph_pointer, visited, stack, &stack_size);
                }
            }

            // Соединяем компоненты минимальным количеством рёбер
            for (int i = 0; i < stack_size - 1; i++) {
                int v = stack[i];
                int u = stack[i + 1];
                if (!graph_pointer[v][u] && (in_degree[v] != out_degree[v] || in_degree[u] != out_degree[u])) {
                    graph_pointer[v][u] = 1;
                    out_degree[v]++;
                    in_degree[u]++;
                }
            }
        }

        // Балансировка степеней только для вершин с разными степенями
        for (int i = 0; i < size; i++) {
            if (in_degree[i] != out_degree[i]) {
                while (in_degree[i] > out_degree[i]) {
                    bool found = false;
                    // Ищем вершину, где входящих меньше исходящих
                    for (int j = 0; j < size; j++) {
                        if (i != j && in_degree[j] < out_degree[j] && !graph_pointer[i][j]) {
                            graph_pointer[i][j] = 1;
                            out_degree[i]++;
                            in_degree[j]++;
                            found = true;
                            break;
                        }
                    }
                    if (!found) break; // Если не нашли подходящую вершину, прерываем цикл
                }
                while (in_degree[i] < out_degree[i]) {
                    bool found = false;
                    // Ищем вершину, где входящих больше исходящих
                    for (int j = 0; j < size; j++) {
                        if (i != j && in_degree[j] > out_degree[j] && !graph_pointer[j][i]) {
                            graph_pointer[j][i] = 1;
                            in_degree[i]++;
                            out_degree[j]++;
                            found = true;
                            break;
                        }
                    }
                    if (!found) break; // Если не нашли подходящую вершину, прерываем цикл
                }
            }
        }

        if (!isEulerian(graph_pointer, size, directed)) {
            // Добавляем недостающие рёбра для обеспечения эйлеровости
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if (i != j && !graph_pointer[i][j] && in_degree[i] < out_degree[i] && in_degree[j] > out_degree[j]) {
                        graph_pointer[i][j] = 1;
                        out_degree[i]++;
                        in_degree[j]++;
                    }
                }
            }
        }

        free(visited);
        free(stack);
        free(in_degree);
        free(out_degree);
    }
    else {
        unsigned int tries = 0;
        while (!isEulerian(graph_pointer, size, directed)) {
            int action = rand() % 2; // 0 - добавить ребро, 1 - удалить ребро

            if (action == 0) {
                // Попытка добавить ребро
                int v1 = rand() % size;
                int v2 = rand() % size;

                if (v1 != v2 && graph_pointer[v1][v2] == 0) {
                    graph_pointer[v1][v2] = 1;
                    graph_pointer[v2][v1] = 1;
                }
            }
            else {
                // Попытка удалить ребро, только если есть что удалять
                int edge_count = 0;
                for (int i = 0; i < size; i++) {
                    for (int j = i + 1; j < size; j++) {
                        if (graph_pointer[i][j]) {
                            edge_count++;
                        }
                    }
                }

                if (edge_count > 0) {
                    int attempts = 0;
                    while (attempts < 100) { // Ограничение на количество попыток найти существующее ребро
                        int v1 = rand() % size;
                        int v2 = rand() % size;
                        if (v1 != v2 && graph_pointer[v1][v2] == 1) {
                            graph_pointer[v1][v2] = 0;
                            graph_pointer[v2][v1] = 0;
                            break;
                        }
                        attempts++;
                    }
                }
            }

            // Ограничение на количество ребер в строке/столбце
            for (int i = 0; i < size; i++) {
                int row_degree = 0;
                for (int j = 0; j < size; j++) {
                    row_degree += graph_pointer[i][j];
                }
                if (row_degree > (size / 2) + 1) { // Если ребер слишком много
                    // Случайное удаление ребра из этой строки
                    int attempts = 0;
                    while (attempts < 100) {
                        int j = rand() % size;
                        if (i != j && graph_pointer[i][j] == 1) {
                            graph_pointer[i][j] = 0;
                            graph_pointer[j][i] = 0;
                            break;
                        }
                        attempts++;
                    }
                }
            }
            tries++;
            if (tries > size * size * 2000) break;
        }
    }
}

bool isEulerian(int_fast8_t** graph_pointer, int size, bool directed) {
    if (directed) {
        if (!checkInOutDegrees(graph_pointer, size)) return false;
    }
    else {
        if (!checkDegrees(graph_pointer, size)) return false;
    }

    if (!isConnected(graph_pointer, size)) return false;

    return true;
}

void findEulerPath(int_fast8_t** graph_pointer, int size, int start, Path* result, bool directed) {
    short stack[2048];
    int top = -1;

    stack[++top] = start;

    while (top >= 0) {
        int current = stack[top];
        bool found = false;

        for (int i = 0; i < size; i++) {
            if (graph_pointer[current][i] > 0) {
                stack[++top] = i;
                graph_pointer[current][i]--;
                if (!directed) {
                    graph_pointer[i][current]--;
                }
                found = true;
                break;
            }
        }

        if (!found) {
            result->path[result->size++] = stack[top];
            top--;
        }
    }
}

bool findEulerianCycle(int_fast8_t** graph_pointer, int size, bool directed, Path* result) {
    result->size = 0;

    if (!isEulerian(graph_pointer, size, directed)) {
        return false;
    }

    int_fast8_t** temp_graph = (int_fast8_t**)malloc(sizeof(int_fast8_t*) * size);
    for (int i = 0; i < size; i++) {
        temp_graph[i] = (int_fast8_t*)malloc(sizeof(int_fast8_t) * size);
        for (int j = 0; j < size; j++) {
            temp_graph[i][j] = graph_pointer[i][j];
        }
    }

    findEulerPath(temp_graph, size, 0, result, directed);

    for (int i = 0; i < size; i++) {
        free(temp_graph[i]);
    }
    free(temp_graph);

    return true;
}