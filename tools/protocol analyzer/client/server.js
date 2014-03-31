var express = require('express');
var app = express();
port = process.argv[2] || 8000;
 
app.configure(function () {
    app.use(express.static(__dirname + '/app')); 		// set the static files location /public/img will be /img for users
		app.use(express.logger('dev')); 						// log every request to the console
		app.use(express.bodyParser()); 							// pull information from html in POST
		app.use(express.methodOverride()); 						// simulate DELETE and PUT
});

app.get('*', function(req, res) {
	res.sendfile('./app/index.html'); // load the single view file (angular will handle the page changes on the front-end)
});

app.listen(port); //the port you want to use
console.log("Express server running at => http://localhost:" + port + "/\nCTRL + C to shutdown");