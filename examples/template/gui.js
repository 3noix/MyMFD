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
