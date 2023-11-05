// Import the functions you need from the SDKs you need
import { initializeApp } from 'https://www.gstatic.com/firebasejs/10.5.2/firebase-app.js';
import {
	getDatabase,
	ref,
	update,
	onValue,
} from 'https://www.gstatic.com/firebasejs/10.5.2/firebase-database.js';
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
const firebaseConfig = {
	apiKey: 'AIzaSyDU7HjH3cAn_2EbQXs7O6JYTRYb1u3WYu0',
	authDomain: 'esp32-rtdb-tutorial.firebaseapp.com',
	databaseURL: 'https://esp32-rtdb-tutorial-default-rtdb.firebaseio.com',
	projectId: 'esp32-rtdb-tutorial',
	storageBucket: 'esp32-rtdb-tutorial.appspot.com',
	messagingSenderId: '216960855948',
	appId: '1:216960855948:web:1e6335906384f79655f0a9',
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);

// Initialize Realtime Database and get a reference to the service
const db = getDatabase(app);
const ldrValueRef = ref(db, 'Sensor/ldr_data');
const ldrVoltageRef = ref(db, 'Sensor/voltage');
const ledAnalogRef = ref(db, 'LED/analog');
const ledDigitalRef = ref(db, 'LED/digital');

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
	update(ref(db), { 'LED/digital': true })
		.then(() => {
			console.log('LED Digital Write: OK: ON');
		})
		.catch((error) => {
			console.log(`LED Digital Write: ERROR: ${error}`);
		});
};
document.getElementById('ledOff').onclick = (event) => {
	update(ref(db), { 'LED/digital': false })
		.then(() => {
			console.log('LED Digital Write: OK: ON');
		})
		.catch((error) => {
			console.log(`LED Digital Write: ${error}`);
		});
};
document.getElementById('ledSlider').onchange = (event) => {
	const value = Number(event.target.value);
	update(ref(db), { 'LED/analog': value })
		.then(() => {
			console.log('LED Analog Write: OK: ON');
		})
		.catch((error) => {
			console.log(`LED Analog Write: ${error}`);
		});
};
