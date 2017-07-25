var GraphMLViewer = GraphMLViewer || {};

/**
 * @param {Object} doc - GraphML-file.
 * @param {Object} object - Graph object.
 * @return {Object} The parsed graph object
 * Parse the graphML-file.
 */
GraphMLViewer.parseGraphml = function(doc) {
    var object = new GraphMLViewer.GmlDoc();
    parseRec(doc, object);
    return object;

    function parseRec(elem, parent) {
        for(var i=0, e=elem.key;e!=undefined && i<e.length;i++) {
            var k = new GraphMLViewer.GmlKey(e[i].id, e[i]['for'], getAttributeContaining(e[i], ".name"), getAttributeContaining(e[i],".type"));
            parent.keys.push(k);
        }

        for(i=0, e=elem.data;e!=undefined && i<e.length;i++) {
          if (e[i]._children && e[i]._children.length > 0) {
              parent.datas[e[i]._children[0]] = e[i][e[i]._children[0]];
            }else{
              parent.datas[e[i].key] = e[i].Text;
            }
        }

        for(i=0, e=elem.node;e!=undefined && i<e.length;i++) {
            var n = new GraphMLViewer.GmlNode(e[i].id);
            parseRec(e[i],n);
            parent.nodes.push(n);
        }

        for(i=0, e=elem.graph;e!=undefined && i<e.length;i++) {
            var g = new GraphMLViewer.GmlGraph(e[i].id, e[i].edgedefault);
            parseRec(e[i],g);
            parent.graphs.push(g);

        }
        for(i=0, e=elem.edge;e!=undefined && i<e.length;i++) {
            var ed = new GraphMLViewer.GmlEdge(e[i].source, e[i].target);
            parseRec(e[i],ed);
            parent.edges.push(ed);
        }
    }

    function getAttributeContaining(el, string) {
        for (var i=0, attrs=el._attributes, l=attrs.length; i<l; i++){
            if (attrs[i].indexOf(string) !== -1  ) {
                return el[attrs[i]];
            }
        }
        return "";
    }
};
