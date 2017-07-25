'use strict';

angular.module('EAGUIApp')
    .directive('sideNav', function ($mdDialog) {
        return {
            templateUrl: 'app/app.side-nav.html',
            replace: true,
            controller: function ($scope, $http, $interval, $rootScope) {
                // Refresh status
                $scope.procs = [];
                $scope.refresh = function () {
                    $http.get('/proc/active').then(function (res) {
                        $scope.procs = res.data;
                    })
                };
                var loop = $interval(function () {
                    $scope.refresh();
                }, 250);

                // Kill and clear
                $rootScope.signalSent = {};
                $scope.signalSent = function (proc) {
                    return $rootScope.signalSent[proc.id];
                };
                $scope.clear = function (proc) {
                    $http.delete('/proc/' + proc.id).then(function (res) {
                        $scope.refresh();
                    });
                };
                $scope.stop = function (proc) {
                    $rootScope.signalSent[proc.id] = true;
                    $http.post('/proc/' + proc.id + '/term');
                };

                // Selected
                $rootScope.selected = "";
                $scope.color = function (str) {
                    if (str === $rootScope.selected)
                        return "accent-700";
                    else
                        return "background";
                };
                $scope.theme = function (str) {
                    if (str === $rootScope.selected)
                        return "dark";
                    else
                        return "default";
                };

                $scope.$on('$destroy', function () {
                    $interval.cancel(loop);
                });

                $scope.showAdvanced = function(ev) {
                    $mdDialog.show({
                        controller: DialogController,
                        templateUrl: 'app/aboutus.html',
                        parent: angular.element(document.body),
                        targetEvent: ev,
                        clickOutsideToClose:true
                    })
                };

                function DialogController($scope, $mdDialog) {
                    $scope.hide = function() {
                        $mdDialog.hide();
                    };

                    $scope.cancel = function() {
                        $mdDialog.cancel();
                    };

                    $scope.answer = function(answer) {
                        $mdDialog.hide(answer);
                    };
                }

            }
        };
    });
