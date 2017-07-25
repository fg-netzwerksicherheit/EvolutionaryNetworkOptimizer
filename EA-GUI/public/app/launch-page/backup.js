'use strict';

angular.module('launchPage')
    .component('backup', {
        templateUrl: 'app/launch-page/backup.html',
        controller: function ($rootScope, $scope) {
            $scope.reset = function(){
                $("#backupTextInput")[0].value = "";
            }
        }
    });