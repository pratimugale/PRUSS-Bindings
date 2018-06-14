const net = require('net');
const SOCKFILE = '/tmp/prussd.sock'

function socketSend(message, callback) {
	const client = net.createConnection(SOCKFILE, () => {
		client.write(message+'\n');
		client.end();
	})
	.on('data', (data) => {
		callback(data.toString())
	})
}

module.exports = socketSend;
