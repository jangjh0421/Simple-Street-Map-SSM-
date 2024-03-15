
Background

OpenStreetMapLinks to an external site. (OSM) is an open source mapping platform that allows people to contribute, edit, and use geospatial data freely. The platform employs user-generated data to create detailed and up-to-date maps, encompassing a diverse range of geographical features such as roads, buildings, rivers, and points of interest. OSM has gained prominence for its flexibility, inclusivity, and the ability to serve various applications, from navigation tools and urban planning to disaster response and humanitarian aid efforts. Unfortunately, OpenStreetMap is much too complicated for a small programming course such as CSC209, as such, we have greatly reduced the complexity and richness of its dataset so the result is distilled with the single purpose of driving a motor vehicle around an area inside the map. 

Simple Street Map: There are only two main concepts in simple street map: nodes and ways.

Node: A node in OSM is a single point on the map, denoted by latitude and longitude. A node can optionally contain metadata, but we have removed all of them for the simplicity of the assignment, e.g., turn restrictions like "no left turn".

Way: A way in OSM contains an ordered sequence of nodes that makes up a road segment. It also contains various metadata about the road segment. For this assignment, we only care about three pieces of information: 

- Name: the name of the road segment. Note that some roads are unnamed. In this case, they are given a unique OSM way ID.

- One-way: whether the road segment is one-way, signifying that you cannot drive in the reverse direction.

- Speed Limit: the maximum legal speed of the road segment.

Map File:

Each map of an area is encoded in a human-readable text file. You can find this in the starter code (described in Setup section). The first line of all SSM map files is "Simple Street Map", followed by a count of way objects and node objects that are defined in the file, e.g.:

 - Simple Street Map
 - 410 ways
 - 1924 nodes
 
 Then, all way objects are defined, followed by node objects. For a way object, it is formatted as follows:

way <id> <osmid> <name>
 <speed limit> <oneway or normal> <number of associated nodes>
 <list of node ids...>
For example:

way 0 3998177 Bloor Street West
 40.0 normal 3
 0 1 2
For a node object, it is formatted as follows:

node <id> <osmid> <latitude> <longitude> <number of associated ways>
 <list of way ids...>
For example:

node 579 42606901 43.6606766 -79.3866647 3
 77 102 158

$ ./ssmap maps/uoft.txt
maps/uoft.txt successfully loaded. 1924 nodes, 410 ways.
>>
Once inside the SSM shell, the following commands are available: node, way, find, path, and quit.

node
The node command takes a node id and prints its basic information, e.g.:

>> node 1
Node 1: (43.6675534, -79.3997042)
way
The way command takes a way id and prints its basic information, e.g.:

>> way 1
Way 1: Queen's Park Crescent West
find
The find command has two modes: way and node. 

The find way command takes an argument, which is the keyword that you are looking for in the name of a way object. The command will print a list of way ids that matches the keyword. e.g.:'

>> find way Bloor
0 147 148 149 205 206 207 264 282 290 291 292 334 335 351 352 353 386
The find node command takes one or two arguments. In the case of one argument, it will print a list of unique nodes that has at least one associated way object that contains the keyword, e.g.:

>> find node Bloor
0 1 2 175 290 465 511 652 867 895 980 981 982 983 984 985 986 1052 1168 1209 1210 1211 1212 1213 1214 1215 1317 1363 1479 1480 1513 1550 1551 1552 1553 1554 1555 1556 1703 1704 1705 1744 1745 1854
With two arguments, it will print a list of nodes where each of the two keywords matches a distinct way object associated with the node. This command is helpful for locating intersections, e.g.:

>> find node Harbord Spadina
695 974
>> find node Harbord Hoskin
709
>> find node Ursula Franklin

>>
Note: in the last case, there are no two streets connecting to a node where the first street matches "Ursula" and the second street matches "Franklin". Therefore, the search result is empty.

Note: for the simplicity of this assignment, we unfortunately do not support multi-word searches like "St George" or "Ursula Franklin".


>> path time 199 200 201 202
0.1201 minutes
The path create command takes two nodes, start and end, and prints a path of node ids from start to end. e.g.:

>> path create 199 186
199 200 201 202 666 667 668 186
quit
The quit command allows for graceful exiting from the program.
