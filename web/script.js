//  Firebase 
const firebaseConfig = {
  databaseURL: "https://carro-esp32-c0050-default-rtdb.firebaseio.com/",
};

// Inicializa Firebase
firebase.initializeApp(firebaseConfig);
const db = firebase.database();

function listenSensores() {
  const ref = db.ref("sensores");
  ref.on("value", (snapshot) => {
    const data = snapshot.val();
    if (!data) return;

    document.getElementById("mq7Val").textContent = data.mq7 || "-";
    document.getElementById("mq9Val").textContent = data.mq9 || "-";
    document.getElementById("mq135Val").textContent = data.mq135 || "-";
    document.getElementById("tempVal").textContent = data.temp || "-";
    document.getElementById("humVal").textContent = data.hum || "-";

    const fecha = new Date().toLocaleTimeString();
    document.getElementById("updateTime").textContent = fecha;
  });
}

// comandos
function sendCommand(accion) {
  db.ref("comandos").set({
    accion: accion,
    timestamp: Date.now(),
  });
  console.log("Comando enviado:", accion);
}

window.onload = listenSensores;
