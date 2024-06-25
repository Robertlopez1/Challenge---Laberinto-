

#include <iostream>
#include <vector>

// Definición de constantes para los elementos del laberinto
const char WALL = '1';
const char PATH = '0';
const char ENTRY = 'E';
const char EXIT = 'S';
const char VISITED = '*'; // Marcar el camino recorrido en la búsqueda

// Definición de la estructura para las coordenadas
struct Position {
    int x, y;
};

// Función para crear un laberinto de dimensiones dinámicas
std::vector<std::vector<char>> createMaze(int width, int height) {
    // Inicializar la matriz con paredes
    std::vector<std::vector<char>> maze(height, std::vector<char>(width, WALL));
    
    // Crear caminos (0) en el laberinto
    for (int i = 1; i < width - 1; ++i) {
        if (height > 1) {
            maze[1][i] = PATH;
            maze[height - 2][i] = PATH;
        }
    }
    for (int i = 1; i < height - 1; ++i) {
        if (width > 1) {
            maze[i][1] = PATH;
            maze[i][width - 2] = PATH;
        }
    }

    // Definir la entrada (E) y la salida (S)
    maze[2][2] = ENTRY; // Ejemplo de entrada en una posición interna
    maze[height - 3][width - 3] = EXIT; // Ejemplo de salida en una posición interna

    return maze;
}

// Función para imprimir el laberinto
void printMaze(const std::vector<std::vector<char>>& maze) {
    for (const auto& row : maze) {
        for (const auto& cell : row) {
            std::cout << cell << ' ';
        }
        std::cout << std::endl;
    }
}

// Verifica si una posición es válida para avanzar en el laberinto
bool isValidMove(const std::vector<std::vector<char>>& maze, int x, int y) {
    int height = maze.size();
    int width = maze[0].size();
    return (x >= 0 && x < height && y >= 0 && y < width && 
            (maze[x][y] == PATH || maze[x][y] == EXIT));
}

// Función para resolver el laberinto utilizando backtracking
bool solveMaze(std::vector<std::vector<char>>& maze, int x, int y) {
    if (maze[x][y] == EXIT) {
        return true; // Se ha encontrado la salida
    }

    if (!isValidMove(maze, x, y)) {
        return false; // Movimiento no válido
    }

    // Marcar la celda como visitada
    maze[x][y] = VISITED;

    // Movimientos posibles: derecha, abajo, izquierda, arriba
    std::vector<Position> moves = {{x, y + 1}, {x + 1, y}, {x, y - 1}, {x - 1, y}};

    for (const auto& move : moves) {
        if (solveMaze(maze, move.x, move.y)) {
            return true; // Si uno de los movimientos lleva a la salida, retornamos verdadero
        }
    }

    // Desmarcar la celda, retroceder en el camino
    maze[x][y] = PATH;
    return false;
}

// Función para encontrar la posición de la entrada en el laberinto
Position findEntry(const std::vector<std::vector<char>>& maze) {
    for (int x = 0; x < maze.size(); ++x) {
        for (int y = 0; y < maze[0].size(); ++y) {
            if (maze[x][y] == ENTRY) {
                return {x, y};
            }
        }
    }
    return {-1, -1}; // Retorna una posición no válida si no encuentra la entrada
}

int main() {
    int width, height;

    // Pedir al usuario las medidas del laberinto
    std::cout << "Introduce el ancho del laberinto: ";
    std::cin >> width;
    std::cout << "Introduce el alto del laberinto: ";
    std::cin >> height;

    // Validar las medidas mínimas para un laberinto
    if (width < 2 || height < 2) {
        std::cerr << "Las dimensiones del laberinto deben ser al menos 2x2." << std::endl;
        return 1;
    }

    // Crear el laberinto
    std::vector<std::vector<char>> maze = createMaze(width, height);

    // Encontrar la posición de la entrada
    Position entry = findEntry(maze);
    if (entry.x == -1 || entry.y == -1) {
        std::cerr << "No se encontró una entrada válida en el laberinto." << std::endl;
        return 1;
    }

    // Resolver el laberinto desde la posición de la entrada
    if (solveMaze(maze, entry.x, entry.y)) {
        std::cout << "El laberinto ha sido resuelto:" << std::endl;
    } else {
        std::cout << "No se encontró solución para el laberinto." << std::endl;
    }

    // Mostrar el laberinto con el camino resuelto
    printMaze(maze);

    return 0;
}
