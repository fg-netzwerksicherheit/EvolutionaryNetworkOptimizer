'use strict';

angular.module('EAGUIApp',
    [
        'ngMaterial',
        'ngRoute',
        'processPage',
        'launchPage',
        'graphPage'
    ])
    .config(function ($mdThemingProvider) {
        $mdThemingProvider.theme('default')
            .primaryPalette('blue')
            .accentPalette('light-blue')
            .warnPalette('red');

        $mdThemingProvider.theme('dark').dark();
    })
    .config(function ($locationProvider, $routeProvider) {
        $locationProvider.hashPrefix('!');

        $routeProvider.
        when('/dashboard', {
            template: '<dashboard-page></dashboard-page>'
        }).
        when('/launch', {
            template: '<launch-page></launch-page>'
        }).
        when('/graphviz', {
            template: '<graph-page></graph-page>'
        }).
        when('/process/:procId', {
            template: '<process-page></process-page>'
        }).
        otherwise('/launch');
    });
