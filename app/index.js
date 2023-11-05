// Import the functions you need from the SDKs you need
import { initializeApp } from 'https://www.gstatic.com/firebasejs/10.5.2/firebase-app.js';
import {
	getDatabase,
	ref,
	update,
	onValue,
} from 'https://www.gstatic.com/firebasejs/10.5.2/firebase-database.js';

// TODO: Add your web app's Firebase configuration
const firebaseConfig = {};

// Initialize Firebase
const app = initializeApp(firebaseConfig);

// Initialize Realtime Database and get a reference to the service
const db = getDatabase(app);

// TODO: Add paths for the values you refer to from the database
const ldrValueRef = ref(db, '');
const ldrVoltageRef = ref(db, '');
const ledAnalogRef = ref(db, '');
const ledDigitalRef = ref(db, '');

onValue(ldrValueRef, (snapshot) => {
	const value = snapshot.val();
	console.log(`LDR Value Read: OK: ${value}`);
	document.getElementById('ldrValue').innerText = value;
});
onValue(ldrVoltageRef, (snapshot) => {
	const value = snapshot.val();
	console.log(`LDR Voltage Read: OK: ${value}`);
	document.getElementById('ldrVoltage').innerText = value;
});
onValue(ledDigitalRef, (snapshot) => {
	const value = snapshot.val();
	console.log(`LED Digital Read: OK: ${value}`);
	const text = value ? 'ON' : 'OFF';
	document.getElementById('ledDigitalValue').innerText = text;
});
onValue(ledAnalogRef, (snapshot) => {
	const value = snapshot.val();
	console.log(`LED Analog Read: OK: ${value}`);
	document.getElementById('ledAnalogValue').innerText = value;
	document.getElementById('ledSlider').value = value;
});

document.getElementById('ledOn').onclick = (event) => {
	update(ref(db), {
		/* TODO: Add appropriate path as the key and the value you want to set it to */
	})
		.then(() => {
			console.log('LED Digital Write: OK: ON');
		})
		.catch((error) => {
			console.log(`LED Digital Write: ERROR: ${error}`);
		});
};
document.getElementById('ledOff').onclick = (event) => {
	update(ref(db), {
		/* TODO: Add appropriate path as the key and the value you want to set it to */
	})
		.then(() => {
			console.log('LED Digital Write: OK: ON');
		})
		.catch((error) => {
			console.log(`LED Digital Write: ${error}`);
		});
};
document.getElementById('ledSlider').onchange = (event) => {
	const value = Number(event.target.value);
	update(ref(db), {
		/* TODO: Add appropriate path as the key and the value you want to set it to */
	})
		.then(() => {
			console.log('LED Analog Write: OK: ON');
		})
		.catch((error) => {
			console.log(`LED Analog Write: ${error}`);
		});
};
