const char* scan_js = R"(
mainWrap = document.getElementsByClassName("main-wrap")[0]

function getE(name) {
	return document.getElementById(name);
}
function showLoading(state) {
	if(state == "hide") {
		sL.className="";
	} else {
		sL.classList.add("show-loading");
	}
}
function notify(msg, closeAfter) {
	if (msg) {
		notification.innerHTML = msg;
		document.body.className = "show";
	} else {
		document.body.className = "";
	}
	if (closeAfter !== undefined) {
		setTimeout(function() {
			document.body.classList.remove("show")
		}, closeAfter);
	}
}

function escapeHTML(str) {
	return str
	  .replace(/&/g, '&amp;')
	  .replace(/</g, '&lt;')
	  .replace(/>/g, '&gt;')
	  .replace(/\"/g, '&quot;')
	  .replace(/\'/g, '&#39;')
	  .replace(/\//g, '&#x2F;')
}

function log(one, two, three) {
	var color = "#369BF2", success
	if (typeof three !== 'undefined')
		success = three
	else
		success = two
	if (success != "default" && typeof success !== 'undefined') {
		if (success == true)
			color = "#3ec563"
		else
			color = "#de363c"
	}
	if (typeof three !== 'undefined')
		console.log('%c[ ] %c' + one, 'color:' + color, 'color:#777', two)
	else
		console.log('%c[ ] ', 'color:' + color, one)
}



function getResponse(adr, callback, timeoutCallback, timeout, method) {
	log('FETCH     ~ ', adr, "default")
	if (timeoutCallback == null) {
		timeoutCallback = function() {
			getResponse("ClientScanTime.json", function() {
				notify("Didn't receive a response! (E97)");
			}, function() {
				notify("Reconnect and reload this page (E96)");
				autoReload();
			})
		};
	}
	if (timeout == null) timeout = 8000;
	if (method == null) method = "GET";
	var xmlhttp = new XMLHttpRequest();
	xmlhttp.onreadystatechange = function() {
		if (xmlhttp.readyState == 4) {
			if (xmlhttp.status == 200) {
				if(typeof callback !== 'undefined') callback(xmlhttp.responseText);
			} else timeoutCallback();
		}
	};
	xmlhttp.open(method, adr, true);
	xmlhttp.send();
	xmlhttp.timeout = timeout;
	xmlhttp.ontimeout = timeoutCallback;
}



var table = document.getElementsByTagName('table')[0],

//var table = $("#apscan");
	mainWrap = document.getElementsByClassName("main-wrap")[0],
	sL = getE('spinner-container'),
	networkInfo = getE('networksFound'),
	scanInfo = getE('spinner-container'),
	apMAC = getE('apMAC'),
	startStopScan = getE('startStopScan'),
	selectAll = getE('selectAll'),
	autoScan = false,
	//tableHeaderHTML = '<thead><tr class="active"><th width="8%"></th><th width="17%">Signal</th><th width="22%">SSID</th><th width="15%">Security</th><th width="8%">Ch.</th></tr></thead>',

	tableHeaderHTML=table.innerHTML,
	selectAllState = 'not-checked',
	previousCall = new Date().getTime(),
	url = window.location.href,
	wifiIndicator, securityState, res;

function toggleScan(onoff) {
	if (onoff && !autoScan) {
		showLoading("hide");
	} else {
		showLoading();
	}
}

function compare(a, b) {
	if (a.r > b.r) return -1;
	if (a.r < b.r) return 1;
	return 0;
}

function getStatus(enc, hid) {
	var buff = "";
		  if (enc == 8) buff = "WPA* &nbsp;&#128274;";
	else  if (enc == 4) buff = "WPA2 &nbsp;&#128274;";
	else  if (enc == 2) buff = "WPA";
	else  if (enc == 7) buff = "Open";
	else  if (enc == 5) buff = "WEP";
		  if (hid == 1) buff += "&#128123;";
	return buff
}
function fadeIn() {
	if (!mainWrap.className.match('fadeIn')) {
		setTimeout(function(){document.getElementsByClassName('main-wrap')[0].classList.add('fadeIn')}, 10)
		setTimeout(function(){document.getElementsByClassName('main-wrap')[0].classList.add('animation')}, 1000)
	}
}

var sS = '<svg viewBox="0 0 48 48"xmlns=http://www.w3.org/2000/svg>';
var sN = '<path d="M24.02 42.98L47.28 14c-.9-.68-9.85-8-23.28-8S1.62 13.32.72 14l23.26 28.98.02.02.02-.02z"fill-opacity=.3 />'
var sE = '</svg>'
var s0 = sS + sN + sE;
var s1 = sS + sN + '<path d="M13.34 29.72l10.65 13.27.01.01.01-.01 10.65-13.27C34.13 29.31 30.06 26 24 26s-10.13 3.31-10.66 3.72z"/>' + sE;
var s2 = sS + sN + '<path d="M9.58 25.03l14.41 17.95.01.02.01-.02 14.41-17.95C37.7 24.47 32.2 20 24 20s-13.7 4.47-14.42 5.03z"/>' + sE;
var s3 = sS + sN + '<path d="M7.07 21.91l16.92 21.07.01.02.02-.02 16.92-21.07C40.08 21.25 33.62 16 24 16c-9.63 0-16.08 5.25-16.93 5.91z"/>' + sE;
var s4 = sS + '<path d="M24.02 42.98L47.28 14c-.9-.68-9.85-8-23.28-8S1.62 13.32.72 14l23.26 28.98.02.02.02-.02z"/>' + sE;
/* Combine strings to form WiFi icons with lock icon */
var s0L = sS + '<path d="m41,19c0.7,0 1.4,0.1 2.1,0.2l4.2,-5.2c-0.9,-0.7 -9.8,-8 -23.3,-8s-22.4,7.3 -23.3,8l23.3,29l7,-8.7l0,-5.3c0,-5.5 4.5,-10 10,-10z"opacity=0.3 /><path d="m46,32l0,-3c0,-2.8 -2.2,-5 -5,-5s-5,2.2 -5,5l0,3c-1.1,0 -2,0.9 -2,2l0,8c0,1.1 0.9,2 2,2l10,0c1.1,0 2,-0.9 2,-2l0,-8c0,-1.1 -0.9,-2 -2,-2zm-2,0l-6,0l0,-3c0,-1.7 1.3,-3 3,-3s3,1.3 3,3l0,3z">' + sE;
var s1L = sS + '<path d="M41 19c.7 0 1.4.1 2.1.2l4.2-5.2c-.9-.7-9.8-8-23.3-8S1.6 13.3.7 14L24 43l7-8.7V29c0-5.5 4.5-10 10-10z"opacity=.3 /><path d="M46 32v-3c0-2.8-2.2-5-5-5s-5 2.2-5 5v3c-1.1 0-2 .9-2 2v8c0 1.1.9 2 2 2h10c1.1 0 2-.9 2-2v-8c0-1.1-.9-2-2-2zm-2 0h-6v-3c0-1.7 1.3-3 3-3s3 1.3 3 3v3zm-30.7-2.3l9 11.2L24 43l7-8.8V29c0-.5 0-1 .1-1.4-1.8-.8-4.2-1.6-7.1-1.6-6.1 0-10.1 3.3-10.7 3.7z">' + sE;
var s2L = sS + '<path d="M41 19c.7 0 1.4.1 2.1.2l4.2-5.2c-.9-.7-9.9-8-23.3-8S1.6 13.3.7 14L24 43l7-8.7V29c0-5.5 4.5-10 10-10z"opacity=.3 /><path d="M46 32v-3c0-2.8-2.2-5-5-5s-5 2.2-5 5v3c-1.1 0-2 .9-2 2v8c0 1.1.9 2 2 2h10c1.1 0 2-.9 2-2v-8c0-1.1-.9-2-2-2zm-2 0h-6v-3c0-1.7 1.3-3 3-3s3 1.3 3 3v3zM9.6 25L24 43l7-8.7V29c0-2.6 1-5 2.7-6.8C31.2 21 27.9 20 24 20c-8.2 0-13.7 4.5-14.4 5z">' + sE;
var s3L = sS + '<path d="M41 19c.7 0 1.4.1 2.1.2l4.2-5.2c-.9-.7-9.8-8-23.3-8S1.6 13.3.7 14L24 43l7-8.7V29c0-5.5 4.5-10 10-10z"enable-background=new opacity=.3 /><path d="M46 32v-3c0-2.8-2.2-5-5-5s-5 2.2-5 5v3c-1.1 0-2 .9-2 2v8c0 1.1.9 2 2 2h10c1.1 0 2-.9 2-2v-8c0-1.1-.9-2-2-2zm-2 0h-6v-3c0-1.7 1.3-3 3-3s3 1.3 3 3v3zM7.1 21.9L24 43l7-8.7V29c0-4.3 2.7-8 6.5-9.4C34.6 18 29.9 16 24 16c-9.6 0-16.1 5.2-16.9 5.9z">' + sE;
var s4L = sS + '<path d="M41 19c.72 0 1.41.08 2.09.22L47.28 14c-.9-.68-9.85-8-23.28-8S1.62 13.32.72 14l23.26 28.98.02.02.02-.02 6.98-8.7V29c0-5.52 4.48-10 10-10zm5 13v-3c0-2.76-2.24-5-5-5s-5 2.24-5 5v3c-1.1 0-2 .9-2 2v8c0 1.1.9 2 2 2h10c1.1 0 2-.9 2-2v-8c0-1.1-.9-2-2-2zm-2 0h-6v-3c0-1.66 1.34-3 3-3s3 1.34 3 3v3z">' + sE;

function getResults() {

	toggleScan(true);
	getResponse("APScanResults.json", function(responseText) {
		try {
			res = JSON.parse(responseText);
			log("RESPONSE  ~ ", res,  true)
			notify()
		} catch(err) {
			log("INVALID   ~ ", responseText, false)
			console.error(err)
			notify('Failed to parse scan results!');
			return
		}
		res.aps = res.aps.sort(compare);
		networkInfo.innerHTML = res.aps.length;
		if (res.aps.length == 0) scan()
		apMAC.innerHTML = "";
		if (res.multiAPs == 1) tableHeaderHTML = '<tr><th width="8%"><input type="checkbox" name="selectAll" id="selectAll" value="false" onclick="selAll()" ' + selectAllState + '><label class="checkbox" for="selectAll"></th><th width="17%">Signal</th><th width="22%">SSID</th><th width="15%">Security</th><th width="8%">Ch.</th></tr>';
		var tr = '';
		if (res.aps.length > 0) tr += tableHeaderHTML;

		for (var i = 0; i < res.aps.length; i++) {

			if (res.aps[i].se == 1) tr += '<tr class="selected" onclick="select(' + res.aps[i].i + ')">';
			else tr += '<tr onclick="select(' + res.aps[i].i + ')">';

			if (res.aps[i].se) {
				tr += '<td><input type="checkbox" name="check' + res.aps[i].i + '" id="check' + res.aps[i].i + '" value="false" checked><label class="checkbox" for="check' + res.aps[i].i + '"></label></td>';
				apMAC.innerHTML = res.aps[i].m;
			} else tr += '<td><input type="checkbox" name="check' + res.aps[i].i + '" id="check' + res.aps[i].i + '" value="false"><label class="checkbox" for="check' + res.aps[i].i + '"></label></td>';

			if (getStatus(res.aps[i].e) != 'Open') {securityState = 'L'} else {securityState = ''}
			if (-89 > res.aps[i].r) {
				wifiIndicator = 's0'+securityState
			} else if (-88 > res.aps[i].r) {
				wifiIndicator = 's1'+securityState
			} else if (-77 > res.aps[i].r) {
				wifiIndicator = 's2'+securityState
			} else if (-66 > res.aps[i].r) {
				wifiIndicator = 's3'+securityState
			} else {
				wifiIndicator = 's4'+securityState
			}

			var signalPercent = Math.round((1-((res.aps[i].r+30)/-70))*100);
			//if (signalPercent > 100) signalPercent = 100;
			//if (i == 0) {var tdID = ' id="resizeEventTD"'} else {var tdID = ''}
			if (i == 0) {var tdID = ''} else {var tdID = ''}
			tr += '<td class="WiFi"'+tdID+'><div>' + eval(wifiIndicator) + '</div><div><span style="background:linear-gradient(135deg, '+getColor(signalPercent)+' '+signalPercent+'%,rgba(0,0,0,0.15) '+signalPercent+'%)"></span><span style="color:'+getColor(signalPercent, true)+'">' + signalPercent + '</span></div></td>';
			tr += '<td>' + escapeHTML(res.aps[i].ss) + '</td>';
			tr += '<td>' + getStatus(res.aps[i].e, res.aps[i].h) + '</td>';
			tr += '<td>' + res.aps[i].c + '</td>';
			tr += '</tr>';
		}

		//console.log(table);

		table.innerHTML = tr;
		//console.log(tr);

		//table.className="table";
		//table.setAttribute("class","table"); 
		//console.log(table);
		checkSize()
		fadeIn();
	}, function() {
		toggleScan(true);
		fadeIn();
		notify("Failed to scan for networks! E0")
	});
}

function getColor(value, lighten){
	var lightness = 50;
	var saturation = 75;
	if (lighten == true) lightness = 90
	if (value > 120) value = 100
	if (value > 90) saturation = 60;
	value = 100 - value;
	var hue=((1-(value/87))*100).toString(10);
	return ["hsl(",hue,","+saturation+"%,"+lightness+"%)"].join("");
}

function scan() {
	toggleScan(false);
	getResponse("APScan.json", function(responseText) {
		if (responseText == "true") getResults();
		else notify("ERROR: Bad response 'APScan.json' (E3)");
		setTimeout(function(){toggleScan(true)}, 700);
	});
}

function select(num) {
	var time = new Date().getTime();
	if ((time - previousCall) >= 80) {
		previousCall = time;
		getResponse("APSelect.json?num=" + num, function(responseText) {
			if (responseText == "true") getResults();
			else notify("ERROR: Bad response 'APSelect.json' (E4)");
		});
	}
}
function selAll() {
	if (selectAllState == 'not-checked') {
		select(-1);
		selectAllState = 'checked';
	} else {
		select(-2);
		selectAllState = 'not-checked';
	}
}

/* Add event listener for WiFi signal icons */
	window.onresize = function(event){checkSize()}
	function checkSize() {
		try {
			var w = document.getElementById('resizeEventTD');
			if (w.clientWidth <= 99) {
				document.getElementById('apscan').className = 'pointUp'
			} else {
				document.getElementById('apscan').className = ''
			}
		} catch(e) {}
	}

getResults();
)";
