///////////////////////////////////////////////////////////////////////////////
// INIT AND CONSTANTS /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
let msgDisplay = document.getElementById("msgDisplay");
let iError = 0;


///////////////////////////////////////////////////////////////////////////////
// PLATFORM ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
let pf = navigator.platform;
let useTouchEvents = (pf == "iPhone" || pf == "iPad" || pf == "Android");
let useMouseEvents = (pf == "Win32" || !useTouchEvents);
msgDisplay.innerHTML = pf;


///////////////////////////////////////////////////////////////////////////////
// TESTS //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
document.fullscreenEnabled = true;
let body = document.getElementById("body");

body.onfullscreenerror = function() {
	msgDisplay.innerHTML = "full screen error";
};

window.onorientationchange = function(event) {
	//msgDisplay.innerHTML = "Orientation is now " + event.target.screen.orientation.angle;
	msgDisplay.innerHTML = "orientation changed";
};

let requestFullScreen = () => {
	body.requestFullscreen();
	msgDisplay.innerHTML = "full screen request";
};


///////////////////////////////////////////////////////////////////////////////
// SLIDERS LABELS /////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
{
	// slider 1
	let slider1 = document.getElementById("slider1");
	let slabel1 = document.getElementById("slabel1");
	slabel1.innerHTML = 0;
	slider1.oninput = function() {
		let f = 0.001 * this.value - 1.0;
		slabel1.innerHTML = f.toString().substring(0,f<0?6:5);
	};
	
	// slider 2
	let slider2 = document.getElementById("slider2");
	let slabel2 = document.getElementById("slabel2");
	slabel2.innerHTML = 0;
	slider2.oninput = function() {
		let f = 0.001 * this.value - 1.0;
		slabel2.innerHTML = f.toString().substring(0,f<0?6:5);
	};
}


///////////////////////////////////////////////////////////////////////////////
// MY MFD /////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
let sendKeyMouse = (keyStr,down) => {
	if (!useMouseEvents) return;
	sendKey(keyStr,down);
};

let sendKeyTouch = (keyStr,down) => {
	if (!useTouchEvents) return;
	sendKey(keyStr,down);
};

let sendKey = (keyStr,down) => {
	let url = window.location.origin + "/key/" + keyStr + "/" + (down ? "1" : "0");
	let reqhttp = new XMLHttpRequest();
	reqhttp.onerror = function() {
		iError++;
		msgDisplay.innerHTML = "An error occured (#" + iError + ")";
	};
	reqhttp.open("GET",url,true);
	reqhttp.send();
};

let sendButtonMouse = (vji,button,pressed) => {
	if (!useMouseEvents) return;
	sendButton(vji,button,pressed);
};

let sendButtonTouch = (vji,button,pressed) => {
	if (!useTouchEvents) return;
	sendButton(vji,button,pressed);
};

let sendButton = (vji,button,pressed) => {
	let url = window.location.origin + "/button/" + vji + "/" + button + "/" + (pressed ? "1" : "0");
	let reqhttp = new XMLHttpRequest();
	reqhttp.onerror = function() {
		iError++;
		msgDisplay.innerHTML = "An error occured (#" + iError + ")";
	};
	reqhttp.open("GET",url,true);
	reqhttp.send();
};

let sendAxis = (vji,axis,value) => {
	let url = window.location.origin + "/axis/" + vji + "/" + axis + "/" + value;
	let reqhttp = new XMLHttpRequest();
	reqhttp.onerror = function() {
		iError++;
		msgDisplay.innerHTML = "An error occured (#" + iError + ")";
	};
	reqhttp.open("GET",url,true);
	reqhttp.send();
};

let sendPov = (vji,pov,value) => {
	let url = window.location.origin + "/pov/" + vji + "/" + pov + "/" + value;
	let reqhttp = new XMLHttpRequest();
	reqhttp.onerror = function() {
		iError++;
		msgDisplay.innerHTML = "An error occured (#" + iError + ")";
	};
	reqhttp.open("GET",url,true);
	reqhttp.send();
};

// initialisation
{
	let keys = document.querySelectorAll("button[data-type=key][data-key]");
	for (let key of keys) {
		let keyStr = key.getAttribute("data-key");
		
		key.addEventListener("mousedown", function() {
			sendKeyMouse(keyStr,true);
		});
		key.addEventListener("mouseup", function() {
			sendKeyMouse(keyStr,false);
		});
		key.addEventListener("touchstart", function() {
			sendKeyTouch(keyStr,true);
		});
		key.addEventListener("touchend", function() {
			sendKeyTouch(keyStr,false);
		});
	}
	
	let buttons = document.querySelectorAll("button[data-type=button][data-vjoy][data-number]");
	for (let button of buttons) {
		let dataVjoy = parseInt(button.getAttribute("data-vjoy"));
		let dataNumber = parseInt(button.getAttribute("data-number")) - 1;
		if (!isNaN(dataVjoy) && !isNaN(dataNumber)) {
			button.addEventListener("mousedown", function() {
				sendButtonMouse(dataVjoy,dataNumber,true);
			});
			button.addEventListener("mouseup", function() {
				sendButtonMouse(dataVjoy,dataNumber,false);
			});
			button.addEventListener("touchstart", function() {
				sendButtonTouch(dataVjoy,dataNumber,true);
			});
			button.addEventListener("touchend", function() {
				sendButtonTouch(dataVjoy,dataNumber,false);
			});
		}
	}
	
	let axes = document.querySelectorAll("input[type=range][data-type][data-vjoy][data-number]");
	for (let axis of axes) {
		let dataType = axis.getAttribute("data-type");
		let dataVjoy = parseInt(axis.getAttribute("data-vjoy"));
		let dataNumber = parseInt(axis.getAttribute("data-number")) - 1;
		if (dataType == "axis" && !isNaN(dataVjoy) && !isNaN(dataNumber)) {
			axis.addEventListener("input", function() {
				let f = 0.001 * axis.value - 1.0;
				sendAxis(dataVjoy,dataNumber,f);
			});
			axis.setAttribute("min",0);
			axis.setAttribute("max",2000);
			axis.setAttribute("value",1000);
			sendAxis(dataVjoy,dataNumber,0);
		}
	}
}
