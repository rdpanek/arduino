var wsUri = "ws://" + location.hostname + ":81/";
//var wsUri = "ws://192.168.1.107:81/";
var commsup = 0;
var websocket;

window.addEventListener("load", function load(event){
    window.removeEventListener("load", load, false); //remove listener, no longer needed
    Ticker();
},false);

var msg = 0;
var tickmessage = 0;
var lasthz = 0;
var time_since_hz = 0;

function wsNotify(message) {
  document.getElementById("wsNotifyBox").innerHTML = "<strong>" + message + "</strong>";
}

function Ticker()
{
	setTimeout( Ticker, 1000 );

	lasthz = (msg - tickmessage);
	tickmessage = msg;
	if( lasthz == 0 )
	{
		time_since_hz++;
		if( time_since_hz > 3)
		{
			if( commsup != 0 ) { wsNotify("Spojení bylo ztraceno."); }
			commsup = 0;
			StartWebSocket();
		}
	}
	else
	{
		time_since_hz = 0;
		//console.log("Online: " + lasthz + " Hz");
	}
}


function progress(percent, $element) {
    var progressBarWidth = percent * $element.width() / 100;
    $element.find('div').animate({ width: progressBarWidth }, 0).html(percent + "% ");
}

function QueueOperation( command, callback )
{
	if( workarray[command] == 1 ){
		return;
	}

	workarray[command] = 1;
	var vp = new Object();
	vp.callback = callback;
	vp.request = command;
	workqueue.push( vp );
}

function StartWebSocket()
{
  wsNotify("Připojuji ...");
  if( websocket ) websocket.close();
	workarray = {};
	workqueue = [];
	lastitem = null;
	websocket = new WebSocket(wsUri, ['arduino']);
	websocket.onopen = function(evt) { onOpen(evt) };
	websocket.onclose = function(evt) { onClose(evt) };
	websocket.onmessage = function(evt) { onMessage(evt) };
	websocket.onerror = function(evt) { onError(evt) };
}

function onOpen(evt)
{
  wsNotify("Připojeno.");
	websocket.send('ping');
}

function onClose(evt)
{
	commsup = 0;
	console.log("WS Close");
}

function onMessage(evt)
{
	msg++;
	if( commsup != 1 ){
		commsup = 1;
	}

	if( lastitem ){
		if( lastitem.callback ){
			lastitem.callback( lastitem, evt.data );
			lastitem = null;
		}
	}
	else{
    document.getElementById("dbmIndicator").innerHTML = evt.data.substr(0);
    document.getElementById("rssiIndicator").innerHTML = Math.round((getRSIIQuality(parseInt(evt.data.substr(0))) * 100)) + "%";
    $("#rssiIndicator").width((getRSIIQuality(parseInt(evt.data.substr(0))) * 100) + "%");
	}

	if( workqueue.length ){
		var elem = workqueue.shift();
		delete workarray[elem.request];

		if( elem.request ){
			doSend( elem.request );
			lastitem = elem;
			return;
		}
	}

	doSend('wx');
}

function onError(evt)
{
	wsNotify("Spojení bylo ztraceno.");
	commsup = 0;
}

function doSend(message)
{
	websocket.send(message);
}

function getQuality(dBm) {
  return parseFloat(((2 * (dBm + 100)) / 100).toFixed(2));
}

function getRSIIQuality(dBm) {
  var quality = dBm <= -100 ? 0 : dBm >= -50 ? 100 : getQuality(dBm);
  return quality;
};

// @string button with val A1 or A2
function toogleButton(button) {
  QueueOperation( button, buttonUpdate );
}

function buttonUpdate(req, data) {
  console.log(req);
  console.log(data);
}
