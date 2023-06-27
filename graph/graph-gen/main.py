import networkx as nx
import random

N = 10
G = nx.complete_graph(N, nx.DiGraph())

# add random weights to edges: integer from 0 to 100
for (start, end) in G.edges:
    G.edges[start, end]['weight'] = random.randint(0, 15)

G.edges.data('weight')

# export graph to file with format: N - number of nodes, M - number of edges, M lines with (start, end, weight)
with open('graph.txt', 'w') as f:
    f.write(str(N) + '\n')
    f.write(str(G.number_of_edges()) + '\n')
    for (start, end, weight) in G.edges.data('weight'):
        f.write(str(start) + ' ' + str(end) + ' ' + str(weight) + '\n')