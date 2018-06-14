
var express = require('express'),
	app = express(),
 	parser = require('body-parser'),
	fs = require('fs'),
	path = require('path'),
 	pruss = require('pruss');

var pru = pruss.pru;
var port = process.env.PORT || 8088;
var router = express.Router();
var prussrouter = express.Router();

app.use(parser.json({ type: 'application/json' }));
app.use(parser.urlencoded({ extended: true }));
app.use(express.static('app'))

router.get('/:no([0-1])', function(req, res){
	res.json({ "status": pru[req.params.no].getState() });	
});

router.get('/:no([0-1])/stop', function(req, res){
	pru[req.params.no].disable((ret) => {
		res.json({ "return": ret });
	});
});

router.get('/:no([0-1])/start', function(req, res){
	pru[req.params.no].enable((ret) => {
		res.json({ "return": ret });
	});
});

router.get('/:no([0-1])/pause', function(req, res){
	pru[req.params.no].pause((ret) => {
		res.json({ "return": ret });
	});
});

router.get('/:no([0-1])/resume', function(req, res){
	pru[req.params.no].resume((ret) => {
		res.json({ "return": ret });
	});
});

router.post('/:no([0-1])/load', function(req, res){
	var filepath = req.body.file;
	pru[req.params.no].load(filepath, (ret) => {
		res.json({ "return": ret });
	});
});

router.get('/:no([0-1])/message', function(req, res){
	pru[req.params.no].getMsg((ret) => {
		res.json({ "message": ret });
	});
});


router.get('/:no([0-1])/event/:time([0-9])?', function(req, res){
	pru[req.params.no].waitForEvent(req.params.time, (ret) => {
		res.json({ "return": ret });
	});

});

router.post('/:no([0-1])/message', function(req, res){
	var msg = req.body.message;
	pru[req.params.no].sendMsg(msg, (ret) => {
		res.json({ "return": ret });
	});
});

prussrouter.get('/', function(req, res){
	var state = "off";
	if(pruss.isOn())
		state = "on";
	res.json({ "status": state });
});

prussrouter.get('/on', function(req, res){
	pruss.bootUp((ret) => {
		res.json({ "return": ret });
	});
});

prussrouter.get('/off', function(req, res){
	pruss.shutDown((ret) => {
		res.json({ "return": ret });
	});
});

app.use('/pru', router);
app.use('/pruss', prussrouter);
app.listen(port);
console.log('server started at port:8888');



