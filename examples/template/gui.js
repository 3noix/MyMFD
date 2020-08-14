///////////////////////////////////////////////////////////////////////////////
// PLATFORM ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
let pf = navigator.platform;
let useTouchEvents = (pf == "iPhone" || pf == "iPad" || pf == "Android");
let useMouseEvents = (pf == "Win32" || !useTouchEvents);


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
	reqhttp.open("GET",url,true);
	reqhttp.send();
};

let sendAxis = (vji,axis,value) => {
	let url = window.location.origin + "/axis/" + vji + "/" + axis + "/" + value;
	let reqhttp = new XMLHttpRequest();
	reqhttp.open("GET",url,true);
	reqhttp.send();
};

let sendPov = (vji,pov,value) => {
	let url = window.location.origin + "/pov/" + vji + "/" + pov + "/" + value;
	let reqhttp = new XMLHttpRequest();
	reqhttp.open("GET",url,true);
	reqhttp.send();
};

// initialisation
{
	let keys = document.querySelectorAll("button[data-type=key]");
	for (let key of keys) {
		if (key.hasAttribute("data-key")){
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
	}
	
	let buttons = document.querySelectorAll("button[data-type=button]");
	for (let button of buttons) {
		if (button.hasAttribute("data-vjoy") && button.hasAttribute("data-number")) {
			let vji = parseInt(button.getAttribute("data-vjoy"));
			let number = parseInt(button.getAttribute("data-number")) - 1;
			if (!isNaN(vji) && !isNaN(number)) {
				button.addEventListener("mousedown", function() {
					sendButtonMouse(vji,number,true);
				});
				button.addEventListener("mouseup", function() {
					sendButtonMouse(vji,number,false);
				});
				button.addEventListener("touchstart", function() {
					sendButtonTouch(vji,number,true);
				});
				button.addEventListener("touchend", function() {
					sendButtonTouch(vji,number,false);
				});
			}
		}
	}
	
	let axes = document.querySelectorAll("input[type=range]");
	for (let axis of axes) {
		if (axis.hasAttribute("data-type") && axis.hasAttribute("data-vjoy") && axis.hasAttribute("data-number")) {
			let dataType = axis.getAttribute("data-type");
			let vji = parseInt(axis.getAttribute("data-vjoy"));
			let number = parseInt(axis.getAttribute("data-number")) - 1;
			if (dataType == "axis" && !isNaN(vji) && !isNaN(number)) {
				axis.addEventListener("input", function() {
					let f = 0.001 * axis.value - 1.0;
					sendAxis(vji,number,f);
				});
				axis.setAttribute("min",0);
				axis.setAttribute("max",2000);
				axis.setAttribute("value",1000);
				sendAxis(vji,number,0);
			}
		}
	}
}
