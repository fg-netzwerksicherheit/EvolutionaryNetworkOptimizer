'use strict';

angular.module('graphPage', ['ngRoute'])
    .component('graphPage', {
        templateUrl: 'app/graph-page/graph-page.html',
        controller: function ($scope, $routeParams, $location, $http, $interval, $rootScope) {
            $rootScope.selected = 'graphviz';
        }
    });