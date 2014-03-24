'use strict';

var rylPacketAnalizeClientApp = angular.module('rylPacketAnalizeClientApp');

rylPacketAnalizeClientApp.controller("packetsCtrl", [ '$scope', function(
		$scope) {
	$scope.teste = function() {
		alert("testta");
	}
}]);