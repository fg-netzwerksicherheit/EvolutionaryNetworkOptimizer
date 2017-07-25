function Sorter(events){
  this.events = events;
  this.sortedEvents;
  this.swap = true;
}

function Sorter(){
  this.events;
}

Sorter.prototype.sort = function(){
  while (this.swap) {
    this.swap = false;
    for (var i = 0; i < this.events.length-1; i++) {
      if (Number(this.events[i].start) > Number(this.events[i+1].start)) {
        this.sortedEvents = this.events[i];
        this.events[i] = this.events[i+1];
        this.events[i+1] = this.sortedEvents;
        this.swap = true;
      }
    }
  }
  return this.events;
};

Sorter.prototype.quickSort = function(events, a, b){
    var anfang = a;
    var ende = b;
    this.events = events;
    if(this.events.length > 1){
        var pivot = Number(this.events[anfang].start);
        var links = anfang;
        var rechts = ende;
        while(links <= rechts){
            while(Number(this.events[links].start) < pivot)links = links+1;
            while(Number(this.events[rechts].start) > pivot)rechts = rechts-1;
            if(links <= rechts){
                if(links < rechts){
                    var h = Number(this.events[links].start);
                    this.events[links].start = Number(this.events[rechts].start);
                    this.events[rechts].start = h;
                }
                links = links+1;
                rechts = rechts-1;
                if(rechts < anfang)rechts = anfang;
            }
          }
        }
        if(anfang < rechts)this.events = this.quickSort(this.events, anfang, rechts);
        if(links < ende)this.events = this.quickSort(this.events, links, ende);
    return this.events;
}

// L = [25, 17, 32, 56, 25, 19, 8, 66, 29, 6, 20, 29]
// L = quicksort(L, 0, len(L)-1)
