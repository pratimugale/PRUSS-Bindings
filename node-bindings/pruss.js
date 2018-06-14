var pru = require('./pru'),
    send = require('./util/sock');

class PRUSS {

	constructor() {
		this._on = false;
		this._prus = [new pru(0), new pru(1)];
		this.bootUp();

		process.on('exit', () => {
			this._prus.forEach(pru => pru.disable());
		});
	}

	get isOn() {
		return this._on;
	}

	get pru() {
		return this._prus;
	}

	bootUp(cb) {
		if(this._on)
			return cb? cb(errno.EALREADY): errno.EALREADY;
		send('PROBE_RPROC', (ret) => {
			if(ret == 0) {
				this._on = true;
				this._prus.forEach((pru) => {
					send('DISABLE_'+pru._no, (ret) => {
						pru.setState('STOPPED');
					});
				});
			}
			return cb? cb(ret): ret;
		});
	}

	shutDown(cb) {
		if(!this._on)
			return cb? cb(errno.EALREADY): errno.EALREADY;
		this._prus.forEach(pru => pru.disable());
		send('UNPROBE_RPROC', (ret) => {
			if(ret == 0){
				this._on = false;
				this._prus.forEach(pru => pru.setState('NONE'));
			}
			return cb? cb(ret): ret;
		});
	}

	restart(cb) {
		shutDown((ret) => {
			bootUp((ret) => {
				return cb? cb(ret): ret;
			});
		});
	}


}

module.exports = new PRUSS;
