
    var network;
    var newData;
    var events;
    var nodeSrcId;
    var nodeDstId;
    var offsetx, offsety, scale, duration, easingFunction;
    var options;
    var showInterval = false;
    var showPhase = 1;
    var showPhaseNode = 1;
    var i;
    var flag = true;

    $(window).blur(function(e) {
      flag = false;
    });

    $(window).focus(function(e) {
      flag = true;
    });

    function updateValues() {
      offsetx = 0;
      offsety = 0;
      duration = 1500;
      scale = 2.0;
      easingFunction = 'easeInOutQuad';
    }

    function toggleFlag() {
      if(flag == true){
        flag = false;
      }else{
        flag = true;
      }
    }

    function round10(wert, dez) {
           wert = parseFloat(wert);
           if (!wert) return 0;

           dez = parseInt(dez);
           if (!dez) dez=0;

           var umrechnungsfaktor = Math.pow(10,dez);

           return Math.round(wert * umrechnungsfaktor) / umrechnungsfaktor;
    }

    function switchColor(src, dst, color) {
      var focusedNodeSrc = newData.nodes.get(src);
      focusedNodeSrc.color = {
        background: color
      }
      newData.nodes.update(focusedNodeSrc);

      var focusedNodeDst = newData.nodes.get(dst);
      focusedNodeDst.color = {
        background: color
      }
      newData.nodes.update(focusedNodeDst);
    }

    function draw(networkEx, eventFile, dataEx) {
      $('#statusUpdate').html('');
      newData = dataEx;
      network = networkEx;
      events = eventFile;
      i = -1;
      updateValues();
      startShow();
    }

    function fitAnimated() {
      var options = {offset: {x:offsetx,y:offsety},
        duration: duration,
        easingFunction: easingFunction
      };
      network.fit({animation:options});
    }

    function focusOnNode() {
      if(flag){
      if(showPhaseNode == 1){
        i = i+1;
        nodeSrcId = 'n'+events[i].src;
        nodeDstId = 'n'+events[i].dst;
        options = {
          scale: scale,
          offset: {x:offsetx,y:offsety},
          animation: {
            duration: duration,
            easingFunction: easingFunction
          }
        };
        $('#statusUpdate').html($('#statusUpdate').html() + '<pre>' + round10((Number(events[i].datatransfer)/1000000000),2) + 'Gb' + ' from ' + events[i].src + " to " + events[i].dst + " with " + events[i].protocol + ' starts at ' + events[i].start + '</pre>');
        var div = $('#statusUpdate');
        div.scrollTop(div[0].scrollHeight);
        network.focus(nodeSrcId, options);
        switchColor(nodeSrcId, nodeDstId, '#FF0000');
        showPhaseNode = 0;
      }else{
        network.focus(nodeDstId, options);
        showPhaseNode = 1;
      }
      }
    }

    function startShow() {
      if (showInterval !== false) {
        showInterval = false;
        network.fit();
      }
      else {
        focusOnNode();
        setTimeout(doTheShow, duration);
        showInterval = true;
      }
    }

    function doTheShow() {
      if (showInterval == true) {
        if (showPhase == 0) {
          focusOnNode();
          showPhase = 1;
        }
        else if(showPhaseNode != 0) {
          fitAnimated();
          showPhase = 0;
          switchColor(nodeSrcId, nodeDstId, '#33cccc');
        }else{
          showPhase = 0;
        }
        setTimeout(doTheShow, duration);
      }

      if(events.length-1 == i && showPhaseNode == 1){
        showInterval = false;
        showPhaseNode = 0;
        switchColor(nodeSrcId, nodeDstId, '#33cccc');
        $('#statusUpdate').html($('#statusUpdate').html()+ 'DONE');
        var div = $('#statusUpdate');
        div.scrollTop(div[0].scrollHeight);
        return;
      }
    }
