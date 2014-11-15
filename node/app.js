var Firebase = require("firebase");
var http = require('http');
var hbs = require('hbs');
var bodyParser = require('body-parser')
var SerialPort = require("serialport").SerialPort
var express = require('express');
var exec = require('child_process').exec;
var app = express();
var fs = require('fs');

app.set('view engine', 'html');
app.engine('html', require('hbs').__express);
app.use(express.static(__dirname + '/public'));
app.use(bodyParser.urlencoded({ extended: false }))

var PORT = "/dev/tty.usbserial-FTG97NL6"; // "/dev/tty.usbmodem1421"; // "/dev/ttyACM0"; 
var DEBUG = false;

// var asleepTimer = {};
var oriRef = new Firebase('https://poofytoo.firebaseio.com/origin');

/*
function say(phrase) {
	var c;
  exec('python ../talk.py "' + phrase + '"');
  signlogRef.child('count').transaction(function(count){
    c = count;
    return count+1;
  }, function() {
    var logmsg = {};
    logmsg[c] = phrase;
    signlogRef.update(logmsg);
  });
}
*/

if (DEBUG) {
	var serialPort = {};
	serialPort.write = function(id, callback) {
		console.log(id);
	}
	serialPort.on = function(id, callback) {
		console.log(id);
	}
} else {
	var serialPort = new SerialPort(PORT, {
		baudrate: 9600
	});
}

/*

app.get('/snappy', function(req, res) {
  res.render('snappycam');
});

app.post('/savephoto', function(req, res) {
  res.send('something happened');
  var b64string = req.body.photo;
  var buf = new Buffer(b64string, 'base64'); // Ta-da
  fs.writeFile("public/photos/save.png", buf, function(err) {
      if(err) {
          console.log(err);
      } else {
          console.log("Oh, hello there!");
          transporter.sendMail(mailOptions);
      }
  }); 

})

app.get('/:id', function(req, res){
		var id = req.params.id;
		if (req.params.id.length < 2) {
			console.log(id);
			serialPort.write(id, function(err, results) {
				console.log('err ' + err);
				console.log('results ' + results);
			});
			res.send(id);
		}
		else {
			res.end();
		}
});
*/
app.get('/m', function(req, res) {
  res.render('m');
})

/*
var ref = new Firebase('https://poofytoo.firebaseIO.com/exitsign');
*/

var buff = '';
serialPort.on("open", function () {
  serialPort.on('data', function(data) {
    buff += data;
    /*
    ref.child('easy').once('value', function(data) {
      say(data.val().summon);
      ref.child('easy').child('trigger').set(1);
    });
*/
    if (buff.indexOf('\n') > 0) {
      var d = buff.split(',');
      var id = buff.split('$')[0];
      var N = parseFloat(d[3], 10);
      var W = parseFloat(d[5], 10);

      var latm = Math.floor(N/100);
      var lonm = Math.floor(W/100);
      latm += N%100/60;
      lonm += W%100/60;
      console.log(id, d[0].substr(-3), buff);

      if (latm && lonm && d[0].substr(-3) == 'RMC') {
        console.log(latm, lonm);
        fdata = {};
        fdata[id] = {lat:latm, lon:lonm};
        oriRef.update(fdata);
      }
      buff = '';
    };
  });
});

/*
  ref.on('value', function(data) {
  	var v = data.val();
  		console.log(v.val);
  		serialPort.write(v.val, function(err, results) {
  			console.log('err ' + err);
  			console.log('results ' + results);
  		});
  	});
  })
*/
var server = app.listen(8001, function() {
    console.log('Listening on port %d', server.address().port);
});
