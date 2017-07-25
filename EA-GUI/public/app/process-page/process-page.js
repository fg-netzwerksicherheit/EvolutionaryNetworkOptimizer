'use strict';

angular.module('processPage', ['ngRoute', 'ngSanitize', 'luegg.directives'])
    .component('processPage', {
        templateUrl: 'app/process-page/process-page.html',
        controller: function ($scope, $routeParams, $location, $http, $interval, $rootScope) {
            var id = $scope.procId = $routeParams.procId;
            $rootScope.selected = id;

            $scope.running = false;

            $scope.refresh = function () {
                $http.get('/proc/' + id + '/status').then(function (res) {
                    $scope.name = res.data.name;
                    $scope.running = res.data.running;
                }, function (res) {
                    $location.path("#!/dashboard");
                });
            };
            var loop = $interval(function () {
                $scope.refresh();
            }, 250);
            $scope.refresh();

            $scope.$on('$destroy', function () {
                $interval.cancel(loop);
            });

        }
    });