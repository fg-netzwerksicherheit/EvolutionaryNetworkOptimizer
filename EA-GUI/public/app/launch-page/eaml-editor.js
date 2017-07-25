'use strict';

angular.module('launchPage')
    .component('eamlEditor', {
        templateUrl: 'app/launch-page/eaml-editor.html',
        controller: function ($rootScope, $scope) {
            $scope.resetContent = function () {
                var file = $("#configFileInput")[0].files[0];

                var reader = new FileReader();
                reader.onload = function (event) {
                    $("#editConfig").val(reader.result);
                };
                reader.readAsText(file);
            }
        }
    });