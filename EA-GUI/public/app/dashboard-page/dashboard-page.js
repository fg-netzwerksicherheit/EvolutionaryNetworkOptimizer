'use strict';

angular.module('dashboardPage', ['ngRoute'])
    .component('dashboardPage', {
        templateUrl: 'app/dashboard-page/dashboard-page.html',
        controller: function ($scope, $routeParams) {
            $scope.procId = $routeParams.procId;
        }

    });
