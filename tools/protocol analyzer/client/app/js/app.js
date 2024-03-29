'use strict';

var rylPacketAnalizeClientApp = angular.module('rylPacketAnalizeClientApp',
		[ 'ngRoute' ]);

rylPacketAnalizeClientApp.config([ '$routeProvider', '$locationProvider',
		function($routeProvider, $locationProvider) {
			$routeProvider.when('/', {
				redirectTo : '/home'
			}).when('/home', {
				templateUrl : '/views/home.html',
				controller : 'homeCtrl'
			}).when('/capture', {
				templateUrl : '/views/capture.html',
				controller : 'captureCtrl'
			}).when('/protocol', {
				templateUrl : '/views/protocol.html',
				controller : 'protocolCtrl'
			}).when('/packet/create', {
				templateUrl : '/views/packetCreate.html',
				controller : 'packetCreateCtrl'
			}).when('/packet/modify', {
				templateUrl : '/views/packetModify.html',
				controller : 'packetModifyCtrl'
			}).otherwise({
				redirectTo : '/home'
			});

			$locationProvider.html5Mode(true);
		} ]);