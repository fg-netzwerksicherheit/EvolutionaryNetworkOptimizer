'use strict';

angular.module('launchPage')
    .component('fileChooser', {
        templateUrl: 'app/launch-page/file-chooser.html',
        controller: function ($rootScope, $scope) {
            $scope.resetConfig = function(){
                $("#configTextInput").val("");
            }
        }
    });