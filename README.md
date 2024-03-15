
# Simple Street Map (SSM) for CSC209

## Background

[OpenStreetMap](https://www.openstreetmap.org) (OSM) is an innovative open-source mapping platform that empowers individuals to contribute, edit, and freely utilize geospatial data. It stands out for using user-generated content to craft detailed, contemporary maps that include a wide variety of geographical features like roads, buildings, rivers, and points of interest. OSM is celebrated for its versatility, inclusiveness, and utility across numerous applications — from navigation and urban planning to disaster response and humanitarian aid. However, given its complexity, OpenStreetMap poses challenges for simpler programming tasks, such as those encountered in a CSC209 course. Consequently, we have significantly simplified the OSM dataset for this assignment, focusing exclusively on navigating a motor vehicle within a mapped area.

## Concepts

The simplified street map comprises two fundamental elements: **nodes** and **ways**.

- **Node**: Represents a single point on the map, identified by latitude and longitude coordinates. In this assignment, nodes are stripped of metadata (e.g., turn restrictions).

- **Way**: An ordered sequence of nodes that forms a road segment. Although ways typically include various metadata, only the following attributes are relevant here:
  - **Name**: The road segment's name. Unnamed roads are assigned a unique OSM way ID.
  - **One-way**: Indicates whether the segment is one-way, prohibiting reverse direction travel.
  - **Speed Limit**: The maximum legal speed on the segment.

## Map File Structure

Each map is a human-readable text file, as described in the **Setup** section of the starter code. The file begins with the header "Simple Street Map," followed by counts of way and node objects (e.g., "410 ways," "1924 nodes"). Subsequently, the file details all way objects, then node objects, using the formats below:

- **Way Object**:
  ```
  way <id> <osmid> <name>
  <speed limit> <oneway or normal> <number of associated nodes>
  <list of node ids...>
  ```
  Example:
  ```
  way 0 3998177 Bloor Street West
  40.0 normal 3
  0 1 2
  ```

- **Node Object**:
  ```
  node <id> <osmid> <latitude> <longitude> <number of associated ways>
  <list of way ids...>
  ```
  Example:
  ```
  node 579 42606901 43.6606766 -79.3866647 3
  77 102 158
  ```

## SSM Shell Commands

After loading a map (e.g., `$ ./ssmap maps/uoft.txt`), the SSM shell provides several commands:

- **node**: Prints basic information for a given node ID.
  ```
  node 1
  Node 1: (43.6675534, -79.3997042)
  ```

- **way**: Displays basic information for a specified way ID.
  ```
  way 1
  Way 1: Queen's Park Crescent West
  ```

- **find**: Searches for ways or nodes matching given keywords.
  - Way search: Lists way IDs with a name matching the keyword.
    ```
    find way Bloor
    0 147 148 ...
    ```
  - Node search: With one argument, lists unique nodes associated with ways containing the keyword. With two arguments, finds nodes at intersections of two ways matching the keywords.
    ```
    find node Bloor
    0 1 2 ...

    find node Harbord Spadina
    695 974

    find node Ursula Franklin
    ```
    (No results for multi-word searches not supporting "St George" or "Ursula Franklin")

- **path**: Calculates paths between nodes.
  - Time estimate between sequential nodes:
    ```
    path time 199 200 201 202
    0.1201 minutes
    ```
  - Create path from start to end node:
    ```
    path create 199 186
    199 200 201 202 666 667 668 186
    ```

- **quit**: Exits the program gracefully.

This README provides a foundational understanding for engaging with the SSM project in CSC209, streamlining navigation and interaction within the simplified OpenStreetMap environment.
