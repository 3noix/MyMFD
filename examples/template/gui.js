///////////////////////////////////////////////////////////////////////////////
// INITIALISATION /////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
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
