var GraphMLViewer = GraphMLViewer || {};

/**
 * Initilize a graph object.
 */
GraphMLViewer.GmlDoc = function() {
    this.keys = [];
    this.graphs = [];
    this.datas = {};
};

/**
 * @param {string} id - Id of node.
 * @param {string} edgedefault - Determines if graph is directed or undirected.
 * Initilize a graph object with id and edgedefault.
 */
GraphMLViewer.GmlGraph = function(id, edgedefault) {
    this.id = id;
    this.edgedefault = edgedefault;
    this.nodes = [];
    this.edges = [];
    this.datas = {};
};

/**
 * @param {string} id - Id of a desired node.
 * @return {Object} The node.
 * Search nodes and returns them if they exist.
 */
GraphMLViewer.GmlGraph.prototype.getNodeById = function(id) {
    for(var i=0;i<this.nodes.length;i++) {
        if (this.nodes[i].id == id) {
            return this.nodes[i];
        }
    }
    return undefined;
};

/**
 * @param {string} src - Id of source node.
 * @param {string} target - Id of target node.
 * Initilize an edge with source an target node.
 */
GraphMLViewer.GmlEdge = function(src, target) {
    this.src = src;
    this.target = target;
    this.datas = {};
};

/**
 * @param {string} id - Id of node.
 * Initilize a Node with a id.
 */
GraphMLViewer.GmlNode = function(id) {
    this.id = id;
    this.datas = {};
};

/**
 * @param {string} id - Id of key.
 * @param {string} fortype - Determines if its a key for a node or an edge.
 * @param {string} attr_name - Name of the attribute.
 * @param {string} attr_type - Type of the attribute.
 * Initilize a key.
 */
GraphMLViewer.GmlKey = function(id, fortype, attr_name, attr_type) {
    this.id = id;
    this.fortype = fortype;
    this.attr_name = attr_name;
    this.attr_type = attr_type;
};
