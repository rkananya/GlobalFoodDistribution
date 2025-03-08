#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 10
#define INF INT_MAX

//Donor
typedef struct {
    int id;
    char name[50];
    char location[50];
    int food_quantity;
} FoodProvider;

//NGO
typedef struct {
    int id;
    char name[50];
    char location[50];
    int capacity;
} NGO;

//Volunteers
typedef struct {
    int id;
    char name[50];
    char location[50];
} Volunteer;

//Adjacency matrix for graph
int graph[MAX_NODES][MAX_NODES];

//Map to store location names and their corresponding integer IDs
typedef struct {
    char name[50];
    int id;
} LocationMap;
LocationMap location_map[MAX_NODES];
int location_count = 0;

//Priority queue node structure
typedef struct {
    int vertex;
    int distance;
} PQNode;

typedef struct {
    PQNode nodes[MAX_NODES];
    int size;
} PriorityQueue;

void init_priority_queue(PriorityQueue* pq) {
    pq->size = 0;
}

void insert_priority_queue(PriorityQueue* pq, int vertex, int distance) {
    pq->nodes[pq->size].vertex = vertex;
    pq->nodes[pq->size].distance = distance;
    pq->size++;
}

PQNode extract_min(PriorityQueue* pq) {
    int min_index = 0;
    for (int i = 1; i < pq->size; i++) {
        if (pq->nodes[i].distance < pq->nodes[min_index].distance) {
            min_index = i;
        }
    }
    PQNode min_node = pq->nodes[min_index];
    pq->nodes[min_index] = pq->nodes[pq->size - 1];
    pq->size--;
    return min_node;
}
void dijkstra(int start, int n, int parent[], int distance[]) {
    PriorityQueue pq;
    init_priority_queue(&pq);

    for (int i= 0;i < n;i++) {
        distance[i] = INF;
        parent[i] = -1;
    }
    distance[start] = 0;
    insert_priority_queue(&pq, start, 0);

    while (pq.size > 0) {
        PQNode min_node = extract_min(&pq);
        int u = min_node.vertex;

        for (int v=0;v<n; v++) {
            if (graph[u][v] != INF && distance[u] + graph[u][v] < distance[v]) {
                distance[v] = distance[u] + graph[u][v];
                parent[v] = u;
                insert_priority_queue(&pq, v, distance[v]);
            }
        }
    }
}

void print_path(int parent[], int j, int start) {
    if (parent[j] == -1) {
        if (j == start) {
            printf("%s", location_map[j].name);
        } else {
            printf("No path exists\n");
        }
        return;
    }

    if (parent[j] == j) {
        printf("%s", location_map[j].name);
        return;
    }

    print_path(parent, parent[j], start);
    printf(" -> %s", location_map[j].name);
}

void log_to_file_with_id(const char* operation, int id, const char* details) {
    FILE* log_file = fopen("delivery_log.txt", "a");
    if (log_file == NULL) {
        printf("Could not open log file for writing.\n");
        return;
    }
    fprintf(log_file, "Operation: %s | ID: %d | Details: %s\n", operation, id, details);
    fclose(log_file);
}

int get_location_id(char location[]) {
    for (int i = 0; i < location_count; i++) {
        if (strcmp(location_map[i].name, location) == 0)
            return location_map[i].id;
    }

    strcpy(location_map[location_count].name, location);
    location_map[location_count].id = location_count;
    return location_count++;
}

int main() {
    int n = MAX_NODES;
    int choice, food_provider_count = 0, ngo_count = 0, volunteer_count = 0;
    FoodProvider food_providers[MAX_NODES];
    NGO ngos[MAX_NODES];
    Volunteer volunteers[MAX_NODES];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) graph[i][j] = INF;
        }
    }

    while (1) {
        printf("\n--- Food Redistribution System ---\n");
        printf("1. Add Food Provider\n");
        printf("2. View Food Providers\n");
        printf("3. Add NGO\n");
        printf("4. View NGOs\n");
        printf("5. Add Volunteer\n");
        printf("6. View Volunteers\n");
        printf("7. Add Delivery Path\n");
        printf("8. Find Shortest Path\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                printf("Enter Food Provider ID, Name, Location, and Food Quantity: ");
                scanf("%d %s %s %d", &food_providers[food_provider_count].id, food_providers[food_provider_count].name,
                      food_providers[food_provider_count].location, &food_providers[food_provider_count].food_quantity);
                get_location_id(food_providers[food_provider_count].location);

                char details[100];
                sprintf(details, "Name: %s, Location: %s, Quantity: %d",
                        food_providers[food_provider_count].name,
                        food_providers[food_provider_count].location,
                        food_providers[food_provider_count].food_quantity);

                log_to_file_with_id("Added Food Provider", food_providers[food_provider_count].id, details);
                food_provider_count++;
                break;
            }

            case 2: {
                printf("Food Providers:\n");
                for (int i = 0; i < food_provider_count; i++) {
                    printf("ID: %d, Name: %s, Location: %s, Quantity: %d\n", food_providers[i].id,
                           food_providers[i].name, food_providers[i].location, food_providers[i].food_quantity);
                }
                break;
            }

            case 3: {
                printf("Enter NGO ID, Name, Location, and Capacity: ");
                scanf("%d %s %s %d", &ngos[ngo_count].id, ngos[ngo_count].name, ngos[ngo_count].location,
                      &ngos[ngo_count].capacity);
                get_location_id(ngos[ngo_count].location);

                char details[100];
                sprintf(details, "Name: %s, Location: %s, Capacity: %d",
                        ngos[ngo_count].name,
                        ngos[ngo_count].location,
                        ngos[ngo_count].capacity);

                log_to_file_with_id("Added NGO", ngos[ngo_count].id, details);
                ngo_count++;
                break;
            }

            case 4: {
                printf("NGOs:\n");
                for (int i = 0; i < ngo_count; i++) {
                    printf("ID: %d, Name: %s, Location: %s, Capacity: %d\n", ngos[i].id, ngos[i].name, ngos[i].location, ngos[i].capacity);
                }
                break;
            }

            case 5: {
                printf("Enter Volunteer ID, Name, and Location: ");
                scanf("%d %s %s", &volunteers[volunteer_count].id, volunteers[volunteer_count].name, volunteers[volunteer_count].location);
                get_location_id(volunteers[volunteer_count].location);

                char details[100];
                sprintf(details, "Name: %s, Location: %s",
                        volunteers[volunteer_count].name,
                        volunteers[volunteer_count].location);

                log_to_file_with_id("Added Volunteer", volunteers[volunteer_count].id, details);
                volunteer_count++;
                break;
            }

            case 6: {
                printf("Volunteers:\n");
                for (int i = 0; i < volunteer_count; i++) {
                    printf("ID: %d, Name: %s, Location: %s\n", volunteers[i].id, volunteers[i].name, volunteers[i].location);
                }
                break;
            }

            case 7: {
                char src[50], dest[50];
                int weight;
                printf("Enter Source Location, Destination Location, and Weight of the Edge: ");
                scanf("%s %s %d", src, dest, &weight);

                int src_id = get_location_id(src);
                int dest_id = get_location_id(dest);

                graph[src_id][dest_id] = weight;
                graph[dest_id][src_id] = weight;

                char details[100];
                sprintf(details, "Source: %s (ID: %d), Destination: %s (ID: %d), Weight: %d",
                        src, src_id, dest, dest_id, weight);

                log_to_file_with_id("Added Delivery Path", -1, details);
                break;
            }

            case 8: {
                char source[50], destination[50];
                printf("Enter Source and Destination Locations: ");
                scanf("%s %s", source, destination);

                int src_id = get_location_id(source);
                int dest_id = get_location_id(destination);

                int parent[MAX_NODES], distance[MAX_NODES];
                dijkstra(src_id, location_count, parent, distance);

                if (distance[dest_id] == INF) {
                    printf("No path exists\n");

                    char details[100];
                    sprintf(details, "Source: %s (ID: %d), Destination: %s (ID: %d) - No path exists",
                            source, src_id, destination, dest_id);

                    log_to_file_with_id("Shortest Path Query", -1, details);
                } else {
                    printf("Shortest Path from %s to %s: ", source, destination);
                    print_path(parent, dest_id, src_id);
                    printf("\nDistance: %d\n", distance[dest_id]);

                    char details[200];
                    sprintf(details, "Source: %s (ID: %d), Destination: %s (ID: %d), Distance: %d",
                            source, src_id, destination, dest_id, distance[dest_id]);

                    log_to_file_with_id("Shortest Path Query", -1, details);
                }
                break;
            }

            case 9:
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
