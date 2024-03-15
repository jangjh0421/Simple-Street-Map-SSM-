#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "streets.h"

struct node
{
    double lat;
    double lon;
    int id;
    int num_ways;
    int *way_ids;
};

typedef struct heapNode
{
    int from;
    int to;
    double time;
} HeapNode;

typedef struct min_heap
{
    HeapNode **element;
    int current_num_node;
    int max_size;
} MinHeap;

struct way
{
    char *name;
    int id;
    bool one_way;
    double speed_limit;
    int num_nodes;
    int *node_ids;
};

struct ssmap
{
    int max_nodes;
    int max_ways;
    int current_num_nodes;
    int current_num_ways;
    struct node **nodes;
    struct way **ways;
};

struct ssmap *
ssmap_create(int nr_nodes, int nr_ways)
{
    struct ssmap *map = malloc(sizeof(struct ssmap));
    if (map == NULL || nr_nodes == 0 || nr_ways == 0)
    {
        return NULL;
    }

    map->current_num_nodes = 0;
    map->current_num_ways = 0;
    map->max_nodes = nr_nodes;
    map->max_ways = nr_ways;
    map->nodes = malloc(sizeof(struct node *) * nr_nodes);
    map->ways = malloc(sizeof(struct way *) * nr_ways);

    if (map->nodes == 0 || map->ways == 0)
    {
        free(map);
        return NULL;
    }

    return map;
}

bool ssmap_initialize(struct ssmap *m)
{
    /* TODO: task 2
     * additional initialization code can be added here */
    return true;
}

void ssmap_destroy(struct ssmap *m)
{
    for (int i = 0; i < m->current_num_nodes; i++)
    {
        free(m->nodes[i]->way_ids);
        free(m->nodes[i]);
    }
    for (int j = 0; j < m->current_num_ways; j++)
    {
        free(m->ways[j]->name);
        free(m->ways[j]->node_ids);
        free(m->ways[j]);
    }
    free(m->nodes);
    free(m->ways);
    free(m);
}

struct way *
specify_way_by_id(const struct ssmap *m, int way_id)
{
    struct way *way = NULL;

    for (int k = 0; k < m->current_num_ways; k++)
    {
        if (m->ways[k]->id == way_id)
        {
            way = m->ways[k];
            break;
        }
    }
    return way;
}

struct node *
specify_node_by_id(const struct ssmap *m, int node_id)
{
    struct node *node = NULL;

    for (int k = 0; k < m->current_num_nodes; k++)
    {
        if (m->nodes[k]->id == node_id)
        {
            node = m->nodes[k];
            break;
        }
    }
    return node;
}

struct way *
ssmap_add_way(struct ssmap *m, int id, const char *name, float maxspeed, bool oneway,
              int num_nodes, const int node_ids[num_nodes])
{
    struct way *way = malloc(sizeof(struct way));

    if (way == NULL)
    {
        return NULL;
    }

    way->name = malloc(sizeof(char) * (strlen(name) + 1));
    if (way->name == NULL)
    {
        free(way);
        return NULL;
    }
    strcpy(way->name, name);

    way->node_ids = malloc(sizeof(int) * num_nodes);
    if (way->node_ids == NULL)
    {
        free(way->name);
        free(way);
        return NULL;
    }
    memcpy(way->node_ids, node_ids, sizeof(int) * num_nodes);

    way->id = id;
    way->speed_limit = maxspeed;
    way->one_way = oneway;
    way->num_nodes = num_nodes;

    m->ways[m->current_num_ways] = way;
    m->current_num_ways++;

    return way;
}

struct node *
ssmap_add_node(struct ssmap *m, int id, double lat, double lon,
               int num_ways, const int way_ids[num_ways])
{
    struct node *node = malloc(sizeof(struct node));
    if (node == NULL)
    {
        return NULL;
    }
    node->id = id;
    node->lat = lat;
    node->lon = lon;
    node->num_ways = num_ways;
    node->way_ids = malloc(sizeof(int) * num_ways);
    if (node->way_ids == NULL)
    {
        free(node);
        return NULL;
    }
    memcpy(node->way_ids, way_ids, sizeof(int) * num_ways);
    m->nodes[m->current_num_nodes] = node;
    m->current_num_nodes++;
    return node;
}

void ssmap_print_way(const struct ssmap *m, int id)
{
    if (id < 0 || id >= m->current_num_ways)
    {
        printf("error: way %d does not exist.\n", id);
    }
    else
    {
        printf("Way %d: %s\n", id, m->ways[id]->name);
    }
}

void ssmap_print_node(const struct ssmap *m, int id)
{
    if (id < 0 || id >= m->current_num_nodes)
    {
        printf("error: node %d does not exist.\n", id);
    }
    else
    {
        printf("Node %d: (%.7lf, %.7lf)\n", id, m->nodes[id]->lat, m->nodes[id]->lon);
    }
}

void ssmap_find_way_by_name(const struct ssmap *m, const char *name)
{
    bool first = 0;
    for (int i = 0; i < (m->current_num_ways); i++)
    {
        if (strstr(m->ways[i]->name, name) != NULL)
        {
            if (first == 0)
            {
                printf("%d", m->ways[i]->id);
                first = 1;
            }
            else
            {
                printf(" %d", m->ways[i]->id);
            }
        }
    }
    printf("\n");
}

void ssmap_find_node_by_names(const struct ssmap *m, const char *name1, const char *name2)
{
    bool first = true;
    int test = 0;

    // We look at each node one by one.
    for (int i = 0; i < (m->current_num_nodes); ++i)
    {
        struct node *current_node = m->nodes[i];
        bool matchName1 = false;
        bool matchName2 = false;

        // We look at every single way associated with each node.
        for (int j = 0; j < current_node->num_ways; ++j)
        {
            int way_id = current_node->way_ids[j];
            struct way *current_way = NULL;

            // Find the way by way_id
            current_way = specify_way_by_id(m, way_id);

            // If we have successfully specified a way based on its ID, proceed.
            if (current_way != NULL)
            {
                // (Common Case) If the second name is not given OR two names provided are different.
                if (name2 == NULL || strcmp(name1, name2) != 0)
                {

                    // If we find a way that matches the first name given.
                    if ((strstr(current_way->name, name1) != NULL))
                    {
                        matchName1 = true;
                        if (name2 == NULL)
                            break; // If name2 is NULL, no need to search further.
                    }
                    else if (name2 != NULL && strstr(current_way->name, name2) != NULL)
                    {
                        matchName2 = true;
                    }
                }

                // ("Rare" Case) If two names provided are the same and has their own unique way object.
                else
                {

                    // If we find a way that matches the first name given.
                    if ((strstr(current_way->name, name1) != NULL) && test == 0)
                    {
                        matchName1 = true;
                        test = 1;
                        if (name2 == NULL)
                            break; // If name2 is NULL, no need to search further.
                    }
                    else if (name2 != NULL && strstr(current_way->name, name2) != NULL)
                    {
                        matchName2 = true;
                    }
                }
            }
        }

        // Determine if the node meets the criteria for printing -- There are different criteria for
        // different input numbers/options
        if (name2 == NULL && matchName1)
        { // Case 1: Only name1 needs to be found
            if (!first)
                printf(" ");
            printf("%d", current_node->id);
            first = false;
        }
        else if (name2 != NULL && matchName1 && matchName2)
        { // Case 2: Both names must be found since both name1 and name2 are given
          // -- either the common case or the rare case.
            if (!first)
                printf(" ");
            printf("%d", current_node->id);
            first = false;
        }
    }
    printf("\n");
}

/**
 * Converts from degree to radian
 *
 * @param deg The angle in degrees.
 * @return the equivalent value in radian
 */
#define d2r(deg) ((deg) * M_PI / 180.)

/**
 * Calculates the distance between two nodes using the Haversine formula.
 *
 * @param x The first node.
 * @param y the second node.
 * @return the distance between two nodes, in kilometre.
 */
static double
distance_between_nodes(const struct node *x, const struct node *y)
{
    double R = 6371.;
    double lat1 = x->lat;
    double lon1 = x->lon;
    double lat2 = y->lat;
    double lon2 = y->lon;
    double dlat = d2r(lat2 - lat1);
    double dlon = d2r(lon2 - lon1);
    double a = pow(sin(dlat / 2), 2) + cos(d2r(lat1)) * cos(d2r(lat2)) * pow(sin(dlon / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c;
}

double ssmap_path_travel_time(const struct ssmap *m, int size, int node_ids[size])
{
    double time = 0.0;

    // Check if any invalid node is passed in.
    for (int i = 0; i < size; i++)
    {
        if (node_ids[i] < 0 || node_ids[i] >= m->current_num_nodes)
        {
            printf("error: node %d does not exist.\n", node_ids[i]);
            return -1.0;
        }
    }

    // Check if there are any nodes duplicated.
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (node_ids[i] == node_ids[j])
            {
                printf("error: node %d appeared more than once.\n", node_ids[i]);
                return -1.0;
            }
        }
    }

    // Check each nodes in sequence one by one.
    for (int i = 0; i < size - 1; i++)
    {

        bool road_exists = false;
        bool adjacent = false;
        bool one_way_travel_error = false;

        // Check each one of the way objects in this map.
        for (int j = 0; j < m->current_num_ways && !one_way_travel_error; j++)
        {

            for (int k = 0; k < m->ways[j]->num_nodes - 1; k++)
            {

                // If the order on the provided path matches the order in a way object.
                if (m->ways[j]->node_ids[k] == node_ids[i] && m->ways[j]->node_ids[k + 1] == node_ids[i + 1])
                {
                    road_exists = true;
                    adjacent = true;
                    one_way_travel_error = false;
                    double distance = distance_between_nodes(m->nodes[node_ids[i]], m->nodes[node_ids[i + 1]]);
                    time += (distance / m->ways[j]->speed_limit) * 60; // convert hours to minutes
                }

                // If the order of nodes is inversed.
                else if (m->ways[j]->node_ids[k] == node_ids[i + 1] && m->ways[j]->node_ids[k + 1] == node_ids[i])
                {

                    // If this specific way object is one-way-only, then error.
                    if (m->ways[j]->one_way == true)
                    {
                        printf("error: cannot go in reverse from node %d to node %d.\n", node_ids[i], node_ids[i + 1]);
                        return -1.0;
                    }

                    else
                    {
                        road_exists = true;
                        adjacent = true;
                        one_way_travel_error = false;
                        double distance = distance_between_nodes(m->nodes[node_ids[i]], m->nodes[node_ids[i + 1]]);
                        time += (distance / m->ways[j]->speed_limit) * 60; // convert hours to minutes
                    }
                }

                else if (m->ways[j]->node_ids[k] == node_ids[i] && m->ways[j]->node_ids[k + 1] != node_ids[i + 1])
                {
                    for (int q = k + 1; q < m->ways[j]->num_nodes; q++)
                    {
                        if (m->ways[j]->node_ids[q] == node_ids[i + 1])
                        {
                            road_exists = true;
                            adjacent = false;
                        }
                    }
                }
                else if (m->ways[j]->node_ids[k] == node_ids[i + 1] && m->ways[j]->node_ids[k + 1] != node_ids[i])
                {
                    for (int q = k + 1; q < m->ways[j]->num_nodes; q++)
                    {
                        if (m->ways[j]->node_ids[q] == node_ids[i])
                        {
                            road_exists = true;
                            adjacent = false;
                        }
                    }
                }
            }
        }
        if (!road_exists)
        {
            printf("error: there are no roads between node %d and node %d.\n", node_ids[i], node_ids[i + 1]);
            return -1.0;
        }
        else if (!adjacent)
        {
            printf("error: cannot go directly from node %d to node %d.\n", node_ids[i], node_ids[i + 1]);
            return -1.0;
        }
    }

    return time;
}

MinHeap *
create_min_heap(int max_size)
{
    MinHeap *heap = malloc(sizeof(MinHeap));
    if (!heap)
    {
        printf("error: out of memory.\n");
        return NULL;
    }

    heap->element = malloc(sizeof(HeapNode *) * max_size);
    if (!heap->element)
    {
        free(heap);
        printf("error: out of memory.\n");
        return NULL;
    }

    heap->current_num_node = 0;
    heap->max_size = max_size;
    return heap;
}

// This algorithm is the one I learned from CSC263. I did not copy anything, but
// it might potentially contain some similarities since I follows the
// pseudocode on the CSC263 textbook for some general idea.
void minHeapify(MinHeap *heap, int current_index)
{
    int smallest = current_index;
    int left = 2 * current_index + 1;
    int right = 2 * current_index + 2;

    if ((left < heap->current_num_node) && (heap->element[left]->time < heap->element[smallest]->time))
    {
        smallest = left;
    }
    if ((right < heap->current_num_node) && (heap->element[right]->time < heap->element[smallest]->time))
    {
        smallest = right;
    }
    if (smallest != current_index)
    {
        HeapNode *to_be_swapped = heap->element[current_index];
        heap->element[current_index] = heap->element[smallest];
        heap->element[smallest] = to_be_swapped;
        minHeapify(heap, smallest);
    }
}

void insertMinHeap(MinHeap *heap, int from, int to, double time)
{
    if (heap->current_num_node >= heap->max_size)
    {
        printf("The heap is full. Node is not added");
        return;
    }
    HeapNode *node = malloc(sizeof(HeapNode));
    node->from = from;
    node->to = to;
    node->time = time;

    int i = heap->current_num_node++;
    heap->element[i] = node;

    while (i != 0 && heap->element[(i - 1) / 2]->time > heap->element[i]->time)
    {
        HeapNode *temp = heap->element[i];
        heap->element[i] = heap->element[(i - 1) / 2];
        heap->element[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

HeapNode *extractMin(MinHeap *heap)
{
    if (heap->current_num_node <= 0)
    {
        printf("The heap is empty");
    }
    if (heap->current_num_node == 1)
    {
        heap->current_num_node -= 1;
        return heap->element[0];
    }

    HeapNode *root = heap->element[0];
    heap->element[0] = heap->element[heap->current_num_node - 1];
    heap->current_num_node -= 1;
    minHeapify(heap, 0);

    return root;
}

void destroyMinHeap(MinHeap *heap)
{
    if (heap != NULL)
    {
        for (int i = 0; i < heap->current_num_node; i++)
        {
            free(heap->element[i]);
        }
        free(heap->element);
        free(heap);
    }
}

void ssmap_path_create(const struct ssmap *m, int start_id, int end_id)
{

    // Initial error ehecking
    if (start_id < 0 || start_id >= m->current_num_nodes)
    {
        printf("error: node %d does not exist.\n", start_id);
        return;
    }
    if (end_id < 0 || end_id >= m->current_num_nodes)
    {
        printf("error: node %d does not exist.\n", end_id);
        return;
    }

    // Initializing arrays required for Dijkstra's Algo
    double time[m->current_num_nodes];
    bool visited[m->current_num_nodes];
    int previous[m->current_num_nodes];

    // Fill those arrays with default values
    for (int i = 0; i < m->current_num_nodes; i++)
    {
        time[i] = INFINITY;
        visited[i] = false;
        previous[i] = -1; // This is required to check that initially we don't have any "previous paths".
    }

    MinHeap *heap = create_min_heap(m->current_num_nodes * 10);

    // Start with the start node
    time[start_id] = 0;
    insertMinHeap(heap, start_id, start_id, 0); // Obviously, the cost it takes for moving to itself is zero

    while (heap->current_num_node > 0)
    {
        HeapNode *extracted_min = extractMin(heap); // Extracting the one with the lowest cost
        int u = extracted_min->to;

        if (visited[u])
        {
            free(extracted_min); // Free the extracted node
            continue;
        }

        visited[u] = true;
        if (u == end_id)
        {
            break; // The end node is already reached. NO NEED TO CHECK FURTHER
        }

        struct node *u_node = specify_node_by_id(m, u);
        for (int i = 0; i < u_node->num_ways; i++)
        {
            struct way *way = specify_way_by_id(m, u_node->way_ids[i]);
            for (int j = 0; j < way->num_nodes - 1; j++)
            {
                // Manually checking for connections among nodes via a way
                if (way->node_ids[j] == u || (!way->one_way && way->node_ids[j + 1] == u))
                {
                    int v = way->node_ids[j] == u ? way->node_ids[j + 1] : way->node_ids[j];
                    if (!visited[v])
                    {
                        struct node *v_node = specify_node_by_id(m, v);
                        double alt = time[u] + distance_between_nodes(u_node, v_node) / way->speed_limit;

                        // Dijkstra's algoritm -- it this alternative updated time is < current time required? then replace it and update
                        if (alt < time[v])
                        {
                            time[v] = alt;
                            previous[v] = u;
                            insertMinHeap(heap, u, v, alt);
                        }

                        // Do nothing with time otherwise.
                    }
                }
            }
        }
        free(extracted_min);
    }

    // Reconstruct the shortest path since it is currently reversed.
    if (time[end_id] == INFINITY)
    {
        printf("error: could not find a path from node %d to node %d.", start_id, end_id);
    }
    else
    {
        // Reverse the array back so that it can print the nodes in the correct sequence.
        bool first = true;

        int path[m->current_num_nodes];
        int path_size = 0;
        for (int at = end_id; at != -1; at = previous[at])
        {
            path[path_size++] = at;
        }

        for (int i = path_size - 1; i >= 0; i--)
        {
            if (first)
            {
                printf("%d", path[i]);
                first = false;
            }
            else
            {
                printf(" %d", path[i]);
            }
        }
        printf("\n");
    }

    destroyMinHeap(heap);
}

