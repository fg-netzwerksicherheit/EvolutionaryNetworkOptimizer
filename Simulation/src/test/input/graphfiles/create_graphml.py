import igraph
import json
import numpy as np

numNodes = 6
numComputers = 4
numEvents = 20

#### Topology
# create computer list
compList = []
for i in range(numComputers):
	compList.append("comp" + str(i))

# create basic graph with 5 nodes
g = igraph.Graph(numNodes)
g.add_edges([(0, 4), (1, 4), (2, 4), (3, 4), (5, 4)])

# create node attributes and labels
g.vs['DataRate'] = '1000Mbps'
g.vs['MTU'] = 1500
g.vs['InterframeGap'] = '96ns'
for i in range(g.vcount()):
    g.vs[i]['label'] = 'ID: {0}\n{1}'.format(i, g.vs[i]['DataRate'])
g.vs['size'] = 80
g.vs['color'] = 'green'
g.vs['Computer'] = ['{0}'.format(d) for d in compList]
g.vs['cost'] = 2

# create edge attributes and labels
g.es['Delay'] = ['{0}ms'.format(d) for d in [2, 10, 30, 200, 56]]
g.es['label'] = g.es['Delay']

# output graph as *.graphml
g.save('use_case1.graphml')

# plot graph as pdf
igraph.plot(g, 'use_case1_graphml.pdf', bbox=(0, 0, 1024, 768), margin=100)

#### Events
events = []
for i in range(numEvents):
    e = {}
    e['name'] = 'event{0}'.format(i)
    e['start'] = str(np.random.randint(10000))
    e['protocol'] = np.random.choice(["tcp", "udp", "udptest"])
    e['datatransfer'] = '{0}'.format(np.random.randint(3 * 1024))

    e['src'] = str(np.random.choice(compList))
    e['dst'] = str(np.random.choice(compList))
    while e['src'] == e['dst']:
        e['dst'] = str(np.random.choice(compList))

    events.append(e)

events_json = {'events' : events}
with open('use_case1_events.json', 'w') as ofile:
    json.dump(events_json, ofile, sort_keys=True, indent=2)

