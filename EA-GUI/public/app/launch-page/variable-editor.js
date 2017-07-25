'use strict';

angular.module('launchPage')
    .component('variableEditor', {
        templateUrl: 'app/launch-page/variable-editor.html',
        controller: function ($rootScope, $scope, $mdEditDialog) {
            $scope.edit = function (event, attr) {
                var editDialog = {
                    modelValue: attr.newVal,
                    placeholder: 'Edit',
                    save: function (input) {
                        attr.newVal = input.$modelValue;
                    },
                    targetEvent: event,
                    title: 'Edit this Value',
                    validators: {
                        'md-maxlength': 100
                    }
                };
                var promise;
                promise = $mdEditDialog.large(editDialog);
                promise.then(function (ctrl) {
                    var input = ctrl.getInput();
                    input.$viewChangeListeners.push(function () {
                        input.$setValidity('test', input.$modelValue !== 'test');
                    });
                });
            };
        }
    });