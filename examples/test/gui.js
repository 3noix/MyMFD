// INIT AND CONSTANTS /////////////////////////////////////////////////////////
const msgDisplay = document.getElementById("msgDisplay");
let iError = 0;


// PLATFORM ///////////////////////////////////////////////////////////////////
msgDisplay.innerHTML = `${navigator.platform}, ${isTouchDevice()}`;


// TESTS //////////////////////////////////////////////////////////////////////
document.fullscreenEnabled = true;
const body = document.getElementById("body");

body.onfullscreenerror = function() {
	msgDisplay.innerHTML = "full screen error";
};

window.onorientationchange = function(event) {
	//msgDisplay.innerHTML = "Orientation is now " + event.target.screen.orientation.angle;
	msgDisplay.innerHTML = "orientation changed";
};

function requestFullScreen() {
	body.requestFullscreen();
	msgDisplay.innerHTML = "full screen request";
};


// SLIDERS LABELS /////////////////////////////////////////////////////////////
{
	// slider 1
	const slider1 = document.getElementById("slider1");
	const slabel1 = document.getElementById("slabel1");
	slabel1.innerHTML = 0;
	slider1.oninput = function() {
		let f = 0.001 * this.value - 1.0;
		slabel1.innerHTML = f.toString().substring(0,f<0?6:5);
	};
	
	// slider 2
	const slider2 = document.getElementById("slider2");
	const slabel2 = document.getElementById("slabel2");
	slabel2.innerHTML = 0;
	slider2.oninput = function() {
		let f = 0.001 * this.value - 1.0;
		slabel2.innerHTML = f.toString().substring(0,f<0?6:5);
	};
}


// INITIALISATION /////////////////////////////////////////////////////////////
{
	document.querySelectorAll("button[data-type=key][data-key]").forEach(keyElt => {
		const keyStr = keyElt.getAttribute("data-key");

		keyElt.addEventListener("mousedown",  () => sendKeyMouse(keyStr, true));
		keyElt.addEventListener("mouseup",    () => sendKeyMouse(keyStr, false));
		keyElt.addEventListener("touchstart", () => sendKeyTouch(keyStr, true));
		keyElt.addEventListener("touchend",   () => sendKeyTouch(keyStr, false));
	});
	
	document.querySelectorAll("button[data-type=button][data-vjoy][data-number]").forEach(buttonElt => {
		const dataVjoy = parseInt(buttonElt.getAttribute("data-vjoy"));
		const dataNumber = parseInt(buttonElt.getAttribute("data-number")) - 1;

		if (!isNaN(dataVjoy) && !isNaN(dataNumber)) {
			buttonElt.addEventListener("mousedown",  () => sendButtonMouse(dataVjoy, dataNumber, true));
			buttonElt.addEventListener("mouseup",    () => sendButtonMouse(dataVjoy, dataNumber, false));
			buttonElt.addEventListener("touchstart", () => sendButtonTouch(dataVjoy, dataNumber, true));
			buttonElt.addEventListener("touchend",   () => sendButtonTouch(dataVjoy, dataNumber, false));
		}
	});
	
	document.querySelectorAll("input[type=range][data-type][data-vjoy][data-number]").forEach(axisElt => {
		const dataType = axisElt.getAttribute("data-type");
		const dataVjoy = parseInt(axisElt.getAttribute("data-vjoy"));
		const dataNumber = parseInt(axisElt.getAttribute("data-number")) - 1;

		if (dataType == "axis" && !isNaN(dataVjoy) && !isNaN(dataNumber)) {
			axisElt.addEventListener("input", () => {sendAxis(dataVjoy, dataNumber, 0.001*axisElt.value-1.0);});
			axisElt.setAttribute("min",0);
			axisElt.setAttribute("max",2000);
			axisElt.setAttribute("value",1000);
			sendAxis(dataVjoy, dataNumber, 0);
		}
	});
}
