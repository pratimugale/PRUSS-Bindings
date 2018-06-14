const path = require('path'),
      send = require('./util/sock');
      errno = require('./util/errno');

class PRU {
	constructor(no) {
		this._no = no;
		this._channelName = 'rpmsg_pru';
		this._channelPort = no?31:30;
		this._state = 'NONE';
		this.reset();
	}

	setState(state) {
		this._state = state;
	}

	reset(cb) {
		this.disable((ret) => {
			this.enable((ret) => {
				return cb? cb(data): ret;
			});
		});
	}

	enable(cb) {
		if(this._state == 'NONE')
			return cb? cb(errno.ENODEV): errno.ENODEV;
		if(this._state == 'RUNNING' || this._state == 'HALTED')
			return cb? cb(errno.EALREADY): errno.EALREADY;
		send('ENABLE_'+this._no, (ret) => {
			ret = parseInt(ret);
			if(!ret)
				this._state = 'RUNNING';
			return cb? cb(ret): ret;
		});
	}	
	
	disable(cb) {
		if(this._state == 'NONE')
			return cb? cb(errno.ENODEV): errno.ENODEV;
		if(this._state == 'STOPPED')
			return cb? cb(errno.EALREADY): errno.EALREADY;
		send('DISABLE_'+this._no, (ret) => {
			ret = parseInt(ret);
			if(!ret)
				this._state = 'STOPPED';
			return cb? cb(ret): ret;
		});
	}

	pause(cb) {
		if(this._state == 'NONE')
			return cb? cb(errno.ENODEV): errno.ENODEV;
		if(this._state == 'HALTED' || this._state == 'STOPPED')
			return cb? cb(errno.EALREADY): errno.EALREADY;
		send('PAUSE_'+this._no, (ret) => {
			ret = parseInt(ret);
			if(!ret)
				this._state = 'HALTED';
			return cb? cb(ret): ret;
		});
	}
	resume(cb) {
		if(this._state == 'NONE' || this._state == 'STOPPED')
			return cb? cb(errno.ENODEV): errno.ENODEV;
		if(this._state == 'RUNNING')
			return cb? cb(errno.EALREADY): errno.EALREADY;
		send('DISABLE_'+this._no, (data) => {
			ret = parseInt(ret);
			if(!ret)
				this._state = 'RUNNING';
			return cb? cb(data): data;
		});
	}
	getRegs(cb) {
		send('GETREGS_'+this._no, (data) => {
			return cb? cb(data): data;
		});
	}

	load(fw, cb) {
		if(this.state == 'NONE')
			return cb? cb(errno.ENODEV): errno.ENODEV;
		this.disable((ret) => {
			send('LOAD_'+this._no+" "+path.resolve(fw), (ret) => {
				ret = parseInt(ret);
				if(!ret)
					this.enable((ret) => {
						return cb? cb(ret): ret;
					});
				else	
					return cb? cb(ret): ret;
			});
		});
	}

	setChannel(port, name) {
		if(port < 0)
			return errno.EINVAL;
		this._channelPort = port;
		this._channelName = name;
		return 0;
	}

	getState() {
		return this._state;
	}

	sendMsg(msg, cb) {
		send('SENDMSG '+this._channelName+" "+this._channelPort+" "+msg, (ret) => {
			return cb? cb(ret): ret;
		})
	}
	getMsg(cb){
		send('GETMSG '+this._channelName+" "+this._channelPort, (data) => {
			return cb? cb(data): data;
		})
	}
	waitForEvent(time, cb){
		if(time < 0)
			return cb? cb(errno.EINVAL): errno.EINVAL;
		if(time == undefined) {
			send('EVENTWAIT '+this._channelName+" "+this._channelPort, (ret) => {
				return cb? cb(ret): ret;
			});
		}
		else {
			send('EVENTWAIT '+this._channelName+" "+this._channelPort+" "+time, (ret) => {
				return cb? cb(ret): ret;
			});
		}
	}

}

module.exports = PRU;
