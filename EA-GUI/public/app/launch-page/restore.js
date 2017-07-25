'use strict';

angular.module('launchPage')
    .component('restore', {
        templateUrl: 'app/launch-page/restore.html',
        controller: function ($rootScope, $scope) {
            $scope.reset = function(){
                $("#restoreTextInput")[0].value = "";
            }
        }
    });