'use strict';

var rylPacketAnalizeClientApp = angular.module('rylPacketAnalizeClientApp',
		[ 'ngRoute' ]);

rylPacketAnalizeClientApp.config([ '$routeProvider', '$locationProvider',
		function($routeProvider, $locationProvider) {
			$routeProvider.when('/', {
				redirectTo : '/home'
			}).when('/home', {
				templateUrl : 'views/home.html',
				controller : 'homeCtrl'
			}).when('/capture', {
				templateUrl : 'views/capture.html',
				controller : 'captureCtrl'
			}).when('/packets', {
				templateUrl : 'views/packets.html',
				controller : 'packetsCtrl'
			}).otherwise({
				redirectTo : '/home'
			});

			$locationProvider.html5Mode(true);
		} ]);
