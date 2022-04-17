// TOUCH DEVICE? //////////////////////////////////////////////////////////////
function isTouchDevice(): boolean {
	return ["iPhone","iPad","Android"].includes(navigator.platform);
	// return (window.ontouchstart != undefined || navigator.maxTouchPoints > 0); // e.g. does not work on iPhone+Safari
}

const useTouchEvents = isTouchDevice();
const useMouseEvents = !useTouchEvents;
const ujpsHttpServerUrl = window.location.origin;


// MY MFD FUNCTIONS ///////////////////////////////////////////////////////////
function sendKeyMouse(keyStr: string, down: boolean): void {
	if (!useMouseEvents) return;
	sendKey(keyStr,down);
}

function sendKeyTouch(keyStr: string, down: boolean): void {
	if (!useTouchEvents) return;
	sendKey(keyStr,down);
}

function sendKey(keyStr: string, down: boolean): void {
	fetch(`${ujpsHttpServerUrl}/key/${keyStr}/${down ? "1" : "0"}`);
}

function sendButtonMouse(vji: number, button: number, pressed: boolean): void {
	if (!useMouseEvents) return;
	sendButton(vji,button,pressed);
}

function sendButtonTouch(vji: number, button: number, pressed: boolean): void {
	if (!useTouchEvents) return;
	sendButton(vji,button,pressed);
}

function sendButton(vji: number, button: number, pressed: boolean): void {
	fetch(`${ujpsHttpServerUrl}/button/${vji}/${button}/${pressed ? "1" : "0"}`);
}

function sendAxis(vji: number, axis: number, value: number) {
	fetch(`${ujpsHttpServerUrl}/axis/${vji}/${axis}/${value}`);
}

function sendPov(vji: number, pov: number, value: number): void {
	fetch(`${ujpsHttpServerUrl}/pov/${vji}/${pov}/${value}`);
}

