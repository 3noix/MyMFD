///////////////////////////////////////////////////////////////////////////////
// PLATFORM ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
let pf = navigator.platform;
let useTouchEvents = (pf == "iPhone" || pf == "iPad" || pf == "Android");
let useMouseEvents = (pf == "Win32" || !useTouchEvents);


///////////////////////////////////////////////////////////////////////////////
// MY MFD FUNCTIONS ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
function sendKeyMouse(keyStr,down) {
	if (!useMouseEvents) return;
	sendKey(keyStr,down);
}

function sendKeyTouch(keyStr,down) {
	if (!useTouchEvents) return;
	sendKey(keyStr,down);
}

function sendKey(keyStr,down) {
	let url = window.location.origin + "/key/" + keyStr + "/" + (down ? "1" : "0");
	fetch(url);
}

function sendButtonMouse(vji,button,pressed) {
	if (!useMouseEvents) return;
	sendButton(vji,button,pressed);
}

function sendButtonTouch(vji,button,pressed) {
	if (!useTouchEvents) return;
	sendButton(vji,button,pressed);
}

function sendButton(vji,button,pressed) {
	let url = window.location.origin + "/button/" + vji + "/" + button + "/" + (pressed ? "1" : "0");
	fetch(url);
}

function sendAxis(vji,axis,value) {
	let url = window.location.origin + "/axis/" + vji + "/" + axis + "/" + value;
	fetch(url);
}

function sendPov(vji,pov,value) {
	let url = window.location.origin + "/pov/" + vji + "/" + pov + "/" + value;
	fetch(url);
}

