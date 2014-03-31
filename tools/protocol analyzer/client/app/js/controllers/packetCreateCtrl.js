'use strict';

var rylPacketAnalizeClientApp = angular.module('rylPacketAnalizeClientApp');

rylPacketAnalizeClientApp.controller("packetCreateCtrl", [ '$scope', 
	function($scope) {
		$(function() {
    		$( "#sortable" ).sortable();
    		$( "#sortable" ).disableSelection();
  		});
	}
]);
