'use strict';

angular.module('processPage')
    .component('console', {
        templateUrl: 'app/process-page/console.html',
        controller: function ($scope, $http, $interval, $rootScope) {
            $scope.stdoutOn = true;
            $scope.stderrOn = true;

            $scope.signalSent = $rootScope.signalSent;

            $scope.stop = function () {
                $scope.signalSent[$scope.$parent.procId] = true;
                $http.post('/proc/' + $scope.$parent.procId + '/term');
            };

            $scope.hide = false;
            $scope.toggleHide = function() {
                $scope.hide = !$scope.hide;
            };

            $scope.lines = [];
            $scope.options = {
                color: 0
            };

            var next = {
                stdout: 0,
                stderr: 0
            };
            var lock = false;
            $scope.refresh = function () {
                if (lock) return;
                lock = true;
                 var funcs = ['stdout', 'stderr'].map(function (value) {
                    return function() {
                        return $http.get('/proc/' + $scope.$parent.procId + '/' + value, {params: {from: next[value]}})
                            .then(function (res) {
                                if (!res.data.data)
                                    return;

                                var processed = stringProcessing(res.data.data, $scope.options, value);
                                var last = $scope.lines.pop();
                                if (!processed.deleteLast)
                                    processed.lines[0] =
                                        (last && (last.slice(0, -11) + "</span>") || "") + processed.lines[0];
                                Array.prototype.push.apply($scope.lines, processed.lines);
                                next[value] = res.data.next;
                            });
                        };
                    });
                funcs[0]().then(funcs[1]).then(function () {
                    lock = false;
                }, function () {
                    lock = false;
                });
            };
            var loop = $interval($scope.refresh, 100);
            $scope.refresh();

            $scope.$on('$destroy', function () {
                $interval.cancel(loop);
            });

        }
    });

function stringProcessing(data, options, channel) {
    var suffix = "";
    var deleteLast  = false;

    var lines = data.split('\n');
    lines.forEach(function (value, index, array) {
        var lastClear = value.lastIndexOf('\u001b[2K\r');
        if (lastClear >= 0) {
            array[index] = value.slice(lastClear + 5);
            if (index == 0)
                deleteLast = true;
        }
    });

    lines.forEach(function (value, index, array) {
        var prefix = "<span class='" + channel + "'>";
        switch (options.color) {
            case 0: prefix += "<span>"; break;
            case 31: prefix += "<span class='red'>"; break;
            case 33: prefix += "<span class='yellow'>"; break;
            case 34: prefix += "<span class='blue'>"; break;
        }
        array[index] = prefix + value.replace(/\u001b\[0(?:(?:;(31|33|34))?m)/g,
            function (match, p1) {
                if (!p1) {
                    options.color = 0;
                    return "</span><span>";
                }
                else {
                    options.color = parseInt(p1);
                    switch (p1) {
                        case "31": return "</span><span class='red'>";
                        case "33": return "</span><span class='yellow'>";
                        case "34": return "</span><span class='blue'>";
                    }
                }
            }) + "</span><br></span>";
    });

    return {
        lines: lines,
        deleteLast: deleteLast
    };
}