'use strict';

/**
 * @class angular_module.processPage
 * @memberOf angular_module
 * @description This component takes the fitness values from the EA and plots them.
 */
angular.module('processPage')
    .component('fitnessPlot', {
        templateUrl: 'app/process-page/fitness-plot.html',
        controller: function ($scope, $http, $interval) {

            var xZoom, yZoom, updateVar = true;

            function round10(wert, dez) {
                wert = parseFloat(wert);
                if (!wert) return 0;

                dez = parseInt(dez);
                if (!dez) dez = 0;

                var umrechnungsfaktor = Math.pow(10, dez);

                return Math.round(wert * umrechnungsfaktor) / umrechnungsfaktor;
            }

            var chart = AmCharts.makeChart("chartdiv", {
                "fontFamily": "Roboto",
                "type": "serial",
                "theme": "light",
                "zoomOutButton": {
                    "backgroundColor": '#000000',
                    "backgroundAlpha": 0.15
                },

                "dataLoader": {
                    "url": '/proc/' + $scope.$parent.procId + '/realtime/fitness',
                    "format": "json",

                    "load": function (options, chart) {

                        var data = chart.dataProvider;
                        chart.dataProvider = Object.keys(chart.dataProvider).map(function (gen) {
                            var obj = data[gen];
                            obj.ID = parseInt(gen);
                            obj.mean = round10(Number(data[gen].mean), 4);
                            obj.sdM = round10(Number(data[gen].mean) - Number(data[gen].sd), 4);
                            obj.sdP = round10(Number(data[gen].mean) + Number(data[gen].sd), 4);
                            return obj;
                        });

                        for (var key in chart.dataProvider[0]) {
                            if (chart.dataProvider[0].hasOwnProperty(key) && key != chart.categoryField && key != "sd" && key != "ID") {
                                var graph = new AmCharts.AmGraph();
                                graph.valueField = key;
                                graph.type = "smoothedLine";
                                graph.title = key;
                                graph.lineThickness = 1;
                                graph.showBalloon = false;

                                switch (key) {
                                    case "mean":
                                        graph.id = "g1";
                                        graph.fillAlphas = 0;
                                        graph.showBalloon = true;
                                        graph.balloonText = "ID: [[category]]<br><span style='font-size:14px;'>Max: [[max]]</span><br><span style='font-size:14px;'>Min: [[min]]</span><br><span style='font-size:14px;'>Mean: [[mean]]</span><br><span style='font-size:14px;'>sd+: [[sdP]]</span><br><span style='font-size:14px;'>sd-: [[sdM]]</span>";
                                        break;

                                    case "sdP":
                                        graph.lineColor = "#ccffff";
                                        graph.fillAlphas = 0.3;
                                        graph.fillToGraph = "g1";
                                        break;

                                    case "sdM":
                                        graph.lineColor = "#ccffff";
                                        graph.fillAlphas = 0.3;
                                        graph.fillToGraph = "g1";
                                        break;

                                    default:
                                }
                                chart.addGraph(graph);
                            }
                        }
                    }
                },

                "categoryField": "ID",

                categoryAxis: {
                    parseDates: false,
                    gridAlpha: 0.15,
                    minorGridEnabled: true,
                    axisColor: "#DADADA"
                },

                valueAxes: [{
                    axisAlpha: 0.2,
                    id: "v1"
                }],

                chartCursor: {
                    fullWidth: false,
                    cursorAlpha: 0.1
                },

                chartScrollbar: {
                    scrollbarHeight: 40,
                    color: "#FFFFFF",
                    autoGridCount: true,
                    graph: "g"
                },

                legend: {
                    position: "bottom",
                    valueText: "[[value]]",
                    valueWidth: 1000,
                    valueAlign: "left",
                    equalWidths: false,
                    periodValueText: "total: [[value.sum]]"
                },
                mouseWheelZoomEnabled: true
            })

            chart.ignoreZoomed = false;
            chart.addListener("zoomed", function (event) {
                if (chart.ignoreZoomed) {
                    chart.ignoreZoomed = false;
                    return;
                }

                if (chart.dataProvider.length == yZoom + 2) {
                    yZoom = event.endIndex + 2;
                } else {
                    yZoom = event.endIndex;
                }
                xZoom = event.startIndex;
            });

            chart.addListener("dataUpdated", function (event) {
                chart.zoomToIndexes(xZoom, yZoom);
            });

            var loop = $interval(function () {
                if (updateVar) {

                    $http.get('/proc/' + $scope.$parent.procId + '/realtime/fitness/last')
                        .then(function (res) {
                            chart.dataProvider.push({
                                ID: Number(res.data.generation),
                                mean: round10(Number(res.data.mean), 4),
                                min: Number(res.data.min),
                                max: Number(res.data.max),
                                sdP: round10(Number(res.data.mean) + Number(res.data.sd), 4),
                                sdM: round10(Number(res.data.mean) - Number(res.data.sd), 4)
                            });
                            chart.ignoreZoomed = true;
                            chart.validateData();
                        });
                }
            }, 1000);

            $scope.$on('$destroy', function () {
                $interval.cancel(loop);
            });

            $scope.setPan = function () {
                var chartCursor = chart.chartCursor;
                chartCursor.pan = true;
                chart.validateNow();
            }

            $scope.setSelect = function () {
                var chartCursor = chart.chartCursor;
                chartCursor.pan = false;
                chartCursor.zoomable = true;
                chart.validateNow();
            }

            $scope.setUpdate = function () {
                if (updateVar) {
                    updateVar = false;
                    return;
                }
                updateVar = true;
            };
        }
    });
