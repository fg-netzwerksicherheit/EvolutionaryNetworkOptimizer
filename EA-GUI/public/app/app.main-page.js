'use strict';

angular.module('EAGUIApp')
    .controller('MainPageCtrl', function ($scope) {
        $scope.showText = false;
        $scope.toggleSideNav = function () {
            $scope.showText = !$scope.showText;
        };
    });
