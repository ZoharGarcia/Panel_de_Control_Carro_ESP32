//  Firebase 
const firebaseConfig = {
  databaseURL: "https://carro-esp32-c0050-default-rtdb.firebaseio.com/",
};

// Inicializa Firebase
firebase.initializeApp(firebaseConfig);
const db = firebase.database();

// ===== UI refs =====
const appRoot = document.getElementById("app");
const lastCmdLbl = document.getElementById("lastCmd");
const espStateLbl = document.getElementById("espState");
const ipLbl = document.getElementById("ipLbl");
const motStateLbl = document.getElementById("motState");

const lbls = {
  mq7: document.getElementById("mq7Val"),
  mq9: document.getElementById("mq9Val"),
  mq135: document.getElementById("mq135Val"),
  temp: document.getElementById("tempVal"),
  hum: document.getElementById("humVal"),
  update: document.getElementById("updateTime"),
};

// ===== Estado local (demo) =====
let lastCommand = null;
let simulatedSensorsInterval = null;

// ===== Vincular controles =====
function bindControls() {
  const btnUp = document.getElementById("btnUp");
  const btnDown = document.getElementById("btnDown");
  const btnLeft = document.getElementById("btnLeft");
  const btnRight = document.getElementById("btnRight");
  const btnStop = document.getElementById("btnStop");
  const slider = document.getElementById("speedSlider");
  const speedLbl = document.getElementById("speedLbl");

  const press = (accion) => () => sendCommand(accion);
  const release = () => sendCommand("stop");

  [
    [btnUp, "forward"],
    [btnDown, "backward"],
    [btnLeft, "left"],
    [btnRight, "right"],
  ].forEach(([el, acc]) => {
    el.addEventListener("mousedown", press(acc));
    el.addEventListener("touchstart", press(acc));
    el.addEventListener("mouseup", release);
    el.addEventListener("mouseleave", release);
    el.addEventListener("touchend", release);
    el.addEventListener("touchcancel", release);
  });

  btnStop.addEventListener("click", release);

  slider.addEventListener("input", () => {
    speedLbl.textContent = slider.value + "%";
  });
}

// ===== Comando (demo local) =====
function sendCommand(accion) {
  lastCommand = {
    accion,
    timestamp: Date.now(),
  };
  // Guardamos localmente para ver la última acción
  localStorage.setItem("esp32_last_cmd", JSON.stringify(lastCommand));
  // Actualizar etiqueta en UI
  if (lastCmdLbl) lastCmdLbl.textContent = accion.toUpperCase();
  // Simular cambio de estado de motores
  if (accion === "stop") motStateLbl.textContent = "• Apagados";
  else motStateLbl.textContent = "• Ejecutando: " + accion;
  console.log("Comando (demo):", accion);
}

// ===== Simulación de sensores (solo demo) =====
function startSensorSimulation() {
  // Una primera lectura inmediata
  simulateSensorRead();

  // Luego actualizaciones periódicas
  simulatedSensorsInterval = setInterval(simulateSensorRead, 4000);
}

function stopSensorSimulation() {
  if (simulatedSensorsInterval) {
    clearInterval(simulatedSensorsInterval);
    simulatedSensorsInterval = null;
  }
}

function simulateSensorRead() {
  // Valores aleatorios razonables para demo
  const mq7 = (Math.random() * 10).toFixed(2);     // ppm
  const mq9 = (Math.random() * 5).toFixed(2);
  const mq135 = (Math.random() * 30).toFixed(2);
  const temp = (20 + Math.random() * 10).toFixed(1); // °C
  const hum = (30 + Math.random() * 50).toFixed(1);  // %

  if (lbls.mq7) lbls.mq7.textContent = mq7 + " ppm";
  if (lbls.mq9) lbls.mq9.textContent = mq9 + " ppm";
  if (lbls.mq135) lbls.mq135.textContent = mq135 + " ppm";
  if (lbls.temp) lbls.temp.textContent = temp + " °C";
  if (lbls.hum) lbls.hum.textContent = hum + " %";

  if (lbls.update) lbls.update.textContent = new Date().toLocaleTimeString();
}

// ===== Estado visual del ESP32 (demo) =====
function setEspConnected(connected = true) {
  if (espStateLbl) espStateLbl.textContent = connected ? "• Conectado" : "• Desconectado";
  if (document.getElementById("wifiBadge"))
    document.getElementById("wifiBadge").classList.toggle("ok", connected);
  if (ipLbl) ipLbl.textContent = connected ? "IP — 192.168.4.1" : "IP —";
}

// ===== Inicialización cuando el app se muestre (después de login) =====
function onAppShow() {
  bindControls();
  startSensorSimulation();
  setEspConnected(true);

  // Restaurar último comando si existe
  const raw = localStorage.getItem("esp32_last_cmd");
  if (raw) {
    try {
      const cmd = JSON.parse(raw);
      if (cmd && cmd.accion && lastCmdLbl) lastCmdLbl.textContent = cmd.accion.toUpperCase();
    } catch (e) { /* ignore */ }
  }
}

// ===== Cleanup cuando oculten la app =====
function onAppHide() {
  stopSensorSimulation();
}

// Exponer funciones para auth.js
window.appControls = {
  onAppShow,
  onAppHide
};

