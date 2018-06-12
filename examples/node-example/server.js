
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
app.use(express.static('app'))

var states = ["NONE", "STOPPED", "RUNNING", "HALTED"];


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


router.get('/:no([0-1])/event/:time([0-9])?', function(req, res){
	
	function eventwait(number, tout, cb){
		var retr = pru[number].waitForEvent(tout)
		console.log("wait over")
		cb(retr)
	}
	function async_eventwait(no, timeout){
		return new Promise(function(resolve){
			eventwait(no, timeout, function(data){
				resolve(data)
			})
		})
	}

	var timeout = (req.params.time==undefined)?-1:req.params.time;
	async_eventwait(req.params.no, parseInt(timeout)).then(function(data){
		res.json({ "return": data })
	})
	console.log("exited");
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
app.use('/pruss', prussrouter);
app.listen(port);
console.log('server started at port:8888');



