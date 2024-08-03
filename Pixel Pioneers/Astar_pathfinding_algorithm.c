// Astar_pathfinding_algorithm.c
#include "game.h"

typedef struct Node {
    Point position;
    int gCost, hCost;
    struct Node* parent;
    struct Node* next;
} Node;

int heuristic(Point a, Point b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

int is_valid(Point p, int gridWidth, int gridHeight) {
    return p.x >= 0 && p.x < gridWidth && p.y >= 0 && p.y < gridHeight;
}

int is_walkable(Point p, World* grid) 
{
    if ( is_colliding( p.x, p.y, grid, 0))
    {
        return grid;//flag
    }
}

Node* find_node(Node* list, Point p) {
    while (list != NULL) {
        if (list->position.x == p.x && list->position.y == p.y) {
            return list;
        }
        list = list->next;
    }
    return NULL;
}

Node* a_star(Point start, Point end, World* grid, int gridWidth, int gridHeight) {
    Node* openList = NULL;
    Node* closedList = NULL;
    Node* startNode = (Node*)malloc(sizeof(Node));
    startNode->position = start;
    startNode->gCost = 0;
    startNode->hCost = heuristic(start, end);
    startNode->parent = NULL;
    startNode->next = NULL;
    openList = startNode;

    while (openList != NULL) {
        Node* current = openList;
        Node* previous = NULL;
        Node* temp = openList;
        while (temp != NULL) {
            if ((temp->gCost + temp->hCost) < (current->gCost + current->hCost)) {
                current = temp;
            }
            temp = temp->next;
        }

        if (current->position.x == end.x && current->position.y == end.y) {
            return current;
        }

        if (previous == NULL) {
            openList = openList->next;
        } else {
            previous->next = current->next;
        }

        current->next = closedList;
        closedList = current;

        Point directions[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        for (int i = 0; i < 4; i++) {
            Point neighbor = {current->position.x + directions[i].x, current->position.y + directions[i].y};

            if (!is_valid(neighbor, gridWidth, gridHeight) || !is_walkable(neighbor, grid)) {
                continue;
            }

            int tentative_gCost = current->gCost + 1;
            Node* neighborNode = find_node(openList, neighbor);
            if (neighborNode == NULL) {
                neighborNode = find_node(closedList, neighbor);
            }

            if (neighborNode == NULL) {
                neighborNode = (Node*)malloc(sizeof(Node));
                neighborNode->position = neighbor;
                neighborNode->gCost = tentative_gCost;
                neighborNode->hCost = heuristic(neighbor, end);
                neighborNode->parent = current;
                neighborNode->next = openList;
                openList = neighborNode;
            } else if (tentative_gCost < neighborNode->gCost) {
                neighborNode->gCost = tentative_gCost;
                neighborNode->parent = current;
            }
        }
    }
    return NULL;
}

void print_path(Node* node) {
    if (node == NULL) return;
    print_path(node->parent);
    printf("(%d, %d) ", node->position.x, node->position.y);
}
