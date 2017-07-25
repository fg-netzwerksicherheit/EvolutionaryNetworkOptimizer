import igraph
import json
import numpy as np

#### Topology
# create basic graph with 5 nodes
g = igraph.Graph(6)
g.add_edges([(0, 4), (1, 4), (2, 4), (3, 4), (5, 4)])

# create node attributes and labels
g.vs['size'] = 80
g.vs['color'] = 'green'

# create edge attributes and labels
g.es['Delay'] = ['{0}ms'.format(d) for d in [2, 10, 30, 200, 56]]
g.es['label'] = g.es['Delay']
g.es['MTUSrc'] = 1500
g.es['MTUDst'] = 1500
g.es['InterframeGapSrc'] = '96ns'
g.es['InterframeGapDst'] = '96ns'
g.es['DataRateSrc'] = '1000Mbps'
g.es['DataRateDst'] = '1000Mbps'

# output graph as *.graphml
g.save('use_case1.p2pnetml', "graphml")

# plot graph as pdf
igraph.plot(g, 'use_case1_p2pnetml.pdf', bbox=(0, 0, 1024, 768), margin=100)

#### Events
events = []
for i in range(20):
    e = {}
    e['name'] = 'event{0}'.format(i)
    e['start'] = str(np.random.randint(10000))
    e['protocol'] = np.random.choice(["tcp", "udp", "udptest"])
    e['datatransfer'] = '{0}'.format(np.random.randint(3 * 1024))
    e['src'] = "n" + str(np.random.choice(range(g.vcount())))
    e['dst'] = "n" + str(np.random.choice(range(g.vcount())))
    while e['src'] == e['dst']:
        e['dst'] = "n" + str(np.random.choice(range(g.vcount())))
    events.append(e)
events_json = {'events' : events}
with open('use_case1_events.json', 'w') as ofile:
    json.dump(events_json, ofile, sort_keys=True, indent=2)
