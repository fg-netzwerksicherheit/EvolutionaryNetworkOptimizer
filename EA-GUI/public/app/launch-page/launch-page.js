'use strict';

angular.module('launchPage', ['ngRoute', 'md.data.table', 'ngMessages'])
    .component('launchPage', {
        templateUrl: 'app/launch-page/launch-page.html',
        controller: function ($rootScope, $scope, $routeParams, $http) {
            $rootScope.selected = 'launch';
            $scope.procId = $routeParams.procId;
            $scope.eamlVar = [];
            $scope.log = {
                etoo: false,
                etoe: true,
                itoo: true,
                itoe: false,
                ttoo: false,
                ttoe: false,
                dtoo: true,
                dtoe: false
            };
            $scope.exec = function () {
                var info = {
                    "name": $("#configTextInput").val(),
                    "eaml": $("#editConfig").val(),
                    "multithreading": $("#proc").val(),
                    "log": {
                        "debug": "",
                        "error": "",
                        "info": "",
                        "trace": ""
                    },
                    "var": {},
                    "generation": $("#genLim").val(),
                    "evaluation": $("#evalLim").val(),
                };
                if ($scope.log.etoo == true) {
                    info.log.error += "stdout";
                }
                if ($scope.log.etoe == true) {
                    if (info.log.error) {
                        info.log.error += ", ";
                    }
                    info.log.error += "stderr";
                }

                if ($scope.log.itoo == true) {
                    info.log.info += "stdout";
                }
                if ($scope.log.itoe == true) {
                    if (info.log.info) {
                        info.log.info += ", ";
                    }
                    info.log.info += "stderr";
                }

                if ($scope.log.dtoo == true) {
                    info.log.debug += "stdout";
                }
                if ($scope.log.dtoe == true) {
                    if (info.log.debug) {
                        info.log.debug += ", ";
                    }
                    info.log.debug += "stderr";
                }


                if ($scope.log.ttoo == true) {
                    info.log.trace += "stdout";
                }
                if ($scope.log.ttoe == true) {
                    if (info.log.trace) {
                        info.log.trace += ", ";
                    }
                    info.log.trace += "stderr";
                }

                if ($scope.eamlVar.length) {
                    for (var i = 0; i < $scope.eamlVar.length; i++) {
                        if ($scope.eamlVar[i].newVal) {
                            info.var[$scope.eamlVar[i].variable] = $scope.eamlVar[i].newVal;
                        }
                    }
                }

                $http.post('/exec', info);
            };

            function findVar(element, iteration) {
                if (element.hasAttributes()) {
                    for (var i = 0, len = element.attributes.length; i < len; i++) {
                        var variable = element.attributes[i].nodeName;
                        var end, value;
                        if (element.attributes[i].nodeValue.includes("|")) {
                            end = element.attributes[i].nodeValue.indexOf("|");
                            value = element.attributes[i].nodeValue.substring(0, end);
                        }
                        else {
                            value = element.attributes[i].nodeValue;
                        }
                        var newObject = {
                            variable: variable,
                            value: value
                        };
                        $scope.eamlVar.push(newObject);
                    }

                    if (element.childElementCount != 0) {
                        findVar(element.firstElementChild, iteration + 1);
                    }
                    else {
                        while (!element.nextElementSibling) {
                            if (!element.parentNode.parentNode) {
                                return;
                            }
                            element = element.parentNode;
                        }
                        findVar(element.nextElementSibling, iteration + 1);
                    }
                }


                else {
                    if (element.childElementCount != 0) {
                        findVar(element.firstElementChild, iteration + 1);
                    }
                    else {
                        while (!element.nextElementSibling) {
                            if (!element.parentNode.parentNode) {
                                return;
                            }
                            element = element.parentNode;
                        }
                        findVar(element.nextElementSibling, iteration + 1);
                    }
                }
            }

            $("body").on(
                'input', '#configFileInput',
                function (event) {
                    var file = event.target.files[0];
                    var fr = new FileReader();
                    fr.onload = function (event) {
                        $("#editConfig").val(event.target.result);
                        $scope.eamlVar = [];
                        var parser = new DOMParser();
                        var xmlDoc = parser.parseFromString($("#editConfig").val(), "text/xml");
                        findVar(xmlDoc.documentElement, 0);
                    };
                    fr.readAsText(file);

                    $("#editConfig").change(function () {
                        $scope.eamlVar = [];
                        var parser = new DOMParser();
                        var xmlDoc = parser.parseFromString($("#editConfig").val(), "text/xml");
                        findVar(xmlDoc.documentElement, 0);
                    });
                }
            );

        }
    })
    .directive('modelUploadFile', uploadFile('model'))
    .directive('eventsUploadFile', uploadFile('events'))
    .directive('configUploadFile', uploadFile('config'))
    .directive('restoreUploadFile', uploadFile('restore'))
    .directive('backupChooseFolder', chooseFolder('backup'))
    .directive('errorChooseFolder', chooseFolder('error'))
    .directive('infoChooseFolder', chooseFolder('info'))
    .directive('debugChooseFolder', chooseFolder('debug'))
    .directive('traceChooseFolder', chooseFolder('trace'));


function uploadFile(name) {
    return function () {
        var directive = {
            restrict: 'E',
            templateUrl: 'app/launch-page/file-uploader/' + name + '-upload.html',
            link: uploadFileLink(name)
        };
        return directive;
    }
}
function uploadFileLink(name) {
    return function (scope, element, attrs) {
        var input = $(element[0].querySelector('#' + name + 'FileInput'));
        var button = $(element[0].querySelector('#' + name + 'UploadButton'));
        var textInput = $(element[0].querySelector('#' + name + 'TextInput'));

        if (input.length && button.length && textInput.length) {
            button.click(function (e) {
                input.click();
            });
            textInput.click(function (e) {
                input.click();
            });
        }

        input.on('change', function (e) {
            var files = e.target.files;
            if (files[0]) {
                scope[name + 'FileName'] = files[0].name;
            } else {
                scope[name + 'FileName'] = null;
            }
            scope.$apply();
        });
    };
}

function chooseFolder(name) {
    return function () {
        var directive = {
            restrict: 'E',
            templateUrl: 'app/launch-page/folder-chooser/' + name + '-choose.html',
            link: chooseFolderLink(name)
        };
        return directive;
    }
}


function chooseFolderLink(name) {
    return function (scope, element, attrs) {
        var input = $(element[0].querySelector('#' + name + 'FolderInput'));
        var button = $(element[0].querySelector('#' + name + 'ChooseButton'));
        var textInput = $(element[0].querySelector('#' + name + 'TextInput'));

        if (input.length && button.length && textInput.length) {
            button.click(function (e) {
                input.click();
            });
            textInput.click(function (e) {
                input.click();
            });
        }

        input.on('change', function (e) {
            var files = e.target.files;
            if (files[0]) {
                var relativePath = files[0].webkitRelativePath;
                var folder = relativePath.split("/");
                scope[name + 'FolderName'] = files[0].name;
            } else {
                scope[name + 'FolderName'] = null;
            }
            scope.$apply();
        });
    }
}
