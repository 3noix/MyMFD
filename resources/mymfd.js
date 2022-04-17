// TOUCH DEVICE? //////////////////////////////////////////////////////////////
function isTouchDevice() {
	return ["iPhone","iPad","Android"].includes(navigator.platform);
	// return (window.ontouchstart != undefined || navigator.maxTouchPoints > 0); // e.g. does not work on iPhone+Safari
}

const useTouchEvents = isTouchDevice();
const useMouseEvents = !useTouchEvents;
const ujpsHttpServerUrl = window.location.origin;


// MY MFD FUNCTIONS ///////////////////////////////////////////////////////////
function sendKeyMouse(keyStr, down) {
	if (!useMouseEvents) return;
	sendKey(keyStr,down);
}

function sendKeyTouch(keyStr, down) {
	if (!useTouchEvents) return;
	sendKey(keyStr,down);
}

function sendKey(keyStr, down) {
	fetch(`${ujpsHttpServerUrl}/key/${keyStr}/${down ? "1" : "0"}`);
}

function sendButtonMouse(vji, button, pressed) {
	if (!useMouseEvents) return;
	sendButton(vji,button,pressed);
}

function sendButtonTouch(vji, button, pressed) {
	if (!useTouchEvents) return;
	sendButton(vji,button,pressed);
}

function sendButton(vji, button, pressed) {
	fetch(`${ujpsHttpServerUrl}/button/${vji}/${button}/${pressed ? "1" : "0"}`);
}

function sendAxis(vji, axis, value) {
	fetch(`${ujpsHttpServerUrl}/axis/${vji}/${axis}/${value}`);
}

function sendPov(vji, pov, value) {
	fetch(`${ujpsHttpServerUrl}/pov/${vji}/${pov}/${value}`);
}

function sendLog(msg) {
	fetch(`${ujpsHttpServerUrl}/log/${msg}`);
}

