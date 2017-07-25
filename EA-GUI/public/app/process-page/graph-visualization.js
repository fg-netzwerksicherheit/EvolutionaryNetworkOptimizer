'use strict';

/**
 * @class angular_module.processPage
 * @memberOf angular_module
 * @description This component takes a graphML-file and will visualize it.
 */
angular.module('processPage')
    .component('graphVisualization', {
        templateUrl: 'app/process-page/graph-visualization.html',
        controller: function () {

          var doc;
          var xmlDoc;
          var parser;
          var network;
          var nodes;
          var graphNodes;
          var edges;
          var data;
          var keys = [];
          var cIDs = new Set();
          var cIDsNumbers = {};

          /**
           * Returns the nodes with added cid from the attributes included in the graphML-file.
           * @param {Object} nodes - Includes the nodes of the graph with visualization parameters.
           * @param {Object} graphNodes - Includes the nodes of the graph.
           * @param {string} clusterAttribute - The new cid attribute.
           * @param {set} cIDs - Includes all node attributes.
           * @param {Object} cIDsNumbers - Includes the number of frequency of every node attribute.
           * @return {Object} The node objekt.
           */
          function getDynamicInputValue(nodes, graphNodes){
              var clusterAttribute = $('#dynamicInput :selected').text();
              cIDs = new Set();
              cIDsNumbers = {};
              for (var i = 0; i < nodes['length']; i++) {
                nodes['_data']['n'+i].cid = graphNodes[i].datas[clusterAttribute];
                if (!cIDs.has(graphNodes[i].datas[clusterAttribute])){
                  cIDs.add(graphNodes[i].datas[clusterAttribute]);
                  cIDsNumbers[graphNodes[i].datas[clusterAttribute]] = 0;
                }
                cIDsNumbers[graphNodes[i].datas[clusterAttribute]] = cIDsNumbers[graphNodes[i].datas[clusterAttribute]]+1;
              }
              return nodes;
          }

          /**
           * Add the node attributes into a drop-down list.
           * @param {string} divName - Name of the drop-down list.
           * @param {string[]} keys - Array of attributes of the graph.
           * @param {html} select - Includes the node attributes.
           */
          function addInput(divName, keys) {
            var select = $('<select/>')
            $.each(keys, function(a, b) {
              select.append($('<option/>').attr('value', b).text(b));
            });
            $('#' + divName).html('');
            $('#' + divName).append(select);
            $('#dynamicInput')[0].style.visibility = 'visible';
          }

          /**
           * Returns the keys from the graphML-file.
           * @param {Objekt} graphML - GraphML-file.
           * @param {string[]} keys - Array of attributes of the graph.
           * @return {string[]} keys.
           */
          function getKeys(graphML, keys){
            for (var i = 0, j = 0; i < graphML.keys.length; i++) {
              if(graphML.keys[i].fortype == 'node'){
                keys[j] = graphML.keys[i].id;
                j++;
              }
            }
            return keys;
          }

          /**
           * Returns true if coordinates exist.
           * @param {string} coordinates - coordinate of a node
           * @return {boolean}
           */
          function getPhysics(coordinates){
              if ('undefined' != typeof coordinates)return false;
              return true;
          }

          /**
           * Returns the atributes of the graph.
           * @param {Object} datas - Includes the attributes of the graph.
           * @param {string} string
           * @return {string} string.
           */
          function getTitle(datas){
            var string = JSON.stringify(datas.datas, null, '<br>');
            string = string.replace(/["',}]/g, '');
            if('undefined' == typeof datas.id){
                string = string.replace(/[{]/g, 'Source: ' + datas.src + '  ' + 'Target: ' + datas.target);
            }else{
                string = string.replace(/[{]/g, 'ID: ' + datas.id);
            }
            return string;
          }

          /**
           * @param {Object} clusterPosition - Includes the coordinates of a cluster
           * @param {Object} containedNodesPositions - Includes the coordinates of all nodes
           * @return {Object} containedNodesPositions
           * Returns the original positions of the clustered nodes.
           */
          function releaseFunction(clusterPosition, containedNodesPositions) {
              return containedNodesPositions;
          }

          /**
           * @param {Object} data - Includes nodes and edges of the network.
           * @param {Object} network - Includes the data, container and the options of the visualization.
           * @param {set} cIDs - Includes all node attributes.
           * @param {Object} cIDsNumbers - Includes the number of frequency of every node attribute.
           * Creates the clusters.
           */
          function clusterByCid(data, network, cIDs, cIDsNumbers) {
              network.setData(data);
              var clusterOptionsByData;
              for (let cID of cIDs){
                  clusterOptionsByData = {
                      joinCondition:function(childOptions) {
                        if('undefined' != typeof cID)return childOptions.cid == cID;
                      },
                      clusterNodeProperties: {id: cID, size: cIDsNumbers[cID], label: cID, borderWidth:3, shape:'triangle'}
                  };
                  network.cluster(clusterOptionsByData);
              }
          }

          function handleFileSelect(evt) {
              $('#dynamicInput')[0].style.visibility = 'hidden';
              var files = evt.target.files;
              for (var i = 0, f; f = files[i]; i++) {
                  var reader = new FileReader();
                  reader.onload = (function(theFile) {
                      return function(e) {
                          parser=new DOMParser();
                          xmlDoc=parser.parseFromString(e.target.result,'text/xml');
                          load(evt);
                      };
                  })(f);
                  reader.readAsText(f);
              }
          }

          function load(e) {
              var xM = 0, yM = 0;
              keys = [];
              var json = $.xmlToJSON(xmlDoc);
              var graphML = new GraphMLViewer.parseGraphml(json);
              var graph = graphML.graphs[0];
              var graphKeys = graphML.keys;
              graphNodes = graph.nodes;
              var graphEdges = graph.edges;
              var graphId = graph.id;
              var edgeDefault = graph.edgedefault;
              keys = getKeys(graphML, keys);

              $('#graphProperties')[0].innerHTML = edgeDefault + ' Network with ID: ' + graphId;

              nodes = new vis.DataSet([]);
              if ('undefined' != typeof graphNodes[0].datas.v_x) {
                  for (var i = 0; i < graphNodes.length; i++) {
                      if (Number(graphNodes[i].datas.v_x) < xM)xM = Number(graphNodes[i].datas.v_x);
                      if (Number(graphNodes[i].datas.v_y) < yM)yM = Number(graphNodes[i].datas.v_y);
                  }
                  var xMabs = Math.abs(xM)+1;
                  var yMabs = Math.abs(yM)+1;
                  for (var i = 0; i < graphNodes.length; i++) {
                      nodes.add({id: graphNodes[i].id, title: getTitle(graphNodes[i]), cid:'', x: (Number(graphNodes[i].datas.v_x)+xMabs)*100, y: (Number(graphNodes[i].datas.v_y)+yMabs)*100, label: graphNodes[i].datas.v_label, font: {size:3}});
                  }
              }else{
                  for (var i = 0; i < graphNodes.length; i++) {
                      nodes.add({id: graphNodes[i].id, label: graphNodes[i].datas.v_label, title: getTitle(graphNodes[i]), font: {size:5}});
                  }
              }

              edges = new vis.DataSet([]);
              for (var i = 0; i < graphEdges.length; i++) {
                  edges.add({from: graphEdges[i].src, to: graphEdges[i].target, title: getTitle(graphEdges[i]), label: graphEdges[i].datas.e_label, font: {size:3}});
              }

              var container = $('#mynetwork')[0];

              data = {
                  nodes: nodes,
                  edges: edges
              };

              if (edgeDefault == 'directed'){
                  var options = {
                      physics: getPhysics(graphNodes[0].datas.v_x),
                      edges:{
                          arrows: 'to',
                      },
                  };
              }else{
                  var options = {
                      physics: getPhysics(graphNodes[0].datas.v_x),
                      edges: {
                          smooth: {
                              type: 'continuous'
                          }
                      }
                  };
              }

              network = new vis.Network(container, data, options);

              network.on('selectNode', function(params) {
                  if (params.nodes.length == 1) {
                      if (network.isCluster(params.nodes[0]) == true) {
                          network.openCluster(params.nodes[0], {releaseFunction: releaseFunction});
                      }
                  }
              });
              addInput('dynamicInput', keys);
          }

          $('#cluster')[0].addEventListener('click', function() {
            nodes = getDynamicInputValue(nodes, graphNodes);
            clusterByCid(data, network, cIDs, cIDsNumbers);
          }, false);

          $('#files')[0].addEventListener('change', handleFileSelect, false);
      }
  });
