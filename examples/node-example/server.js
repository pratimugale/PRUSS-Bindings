
var express = require('express'),
	app = express(),
 	parser = require('body-parser'),
	fs = require('fs'),
	path = require('path'),
 	pruss = require('./node_modules/pruss');

var p = pruss.PRUSS.get();
var pru = [p.pru0, p.pru1];
var port = process.env.PORT || 8088;
var router = express.Router();
var prussrouter = express.Router();

app.use(parser.json({ type: 'application/json' }));
app.use(parser.urlencoded({ extended: true }));

var states = ["NONE", "STOPPED", "RUNNING", "HALTED"];

router.get('/', function(req, res){
	res.json({ 
		"state0": states[pru[0].getState()],
		"state1": states[pru[1].getState()]
	});
});

router.get('/:no([0-1])', function(req, res){
	res.json({ "status": states[pru[req.params.no].getState()] });	
});

router.get('/:no([0-1])/stop', function(req, res){
	res.json({ "return": pru[req.params.no].disable() });
});

router.get('/:no([0-1])/start', function(req, res){
	res.json({ "return": pru[req.params.no].enable() });
});

router.get('/:no([0-1])/pause', function(req, res){
	res.json({ "return": pru[req.params.no].pause() });
});

router.get('/:no([0-1])/resume', function(req, res){
	res.json({ "return": pru[req.params.no].resume() });
});

router.post('/:no([0-1])/load', function(req, res){
	var filepath = req.body.file;
	res.json( { "return": pru[req.params.no].load(filepath) } )
});

router.get('/:no([0-1])/message', function(req, res){
	res.json({ "message": pru[req.params.no].getMsg() });
});

router.post('/:no([0-1])/message', function(req, res){
	var msg = req.body.message;
	res.json({ "return": pru[req.params.no].sendMsg(msg) });
});

prussrouter.get('/', function(req, res){
	var state = "off";
	if(p.isOn())
		state = "on";
	res.json({ "status": state });
});

prussrouter.get('/on', function(req, res){
	res.json({ "return": p.bootUp() });
});

prussrouter.get('/off', function(req, res){
	res.json({ "return": p.shutDown() });
});

app.use('/pru', router);
app.use('/', prussrouter);
app.listen(port);
console.log('server started at port:8888');



