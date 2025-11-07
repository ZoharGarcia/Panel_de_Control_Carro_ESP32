const VALID_USER = "Admin";
const VALID_PASS = "Admin2025";

const SESS_KEY = "esp32_panel_session";

const app = document.getElementById("app");
const overlay = document.getElementById("authOverlay");
const form = document.getElementById("loginForm");
const msg = document.getElementById("authMsg");
const logoutBtn = document.getElementById("logoutBtn");

// Actualiza UI según sesión
function updateUI() {
  const logged = localStorage.getItem(SESS_KEY) === "1";
  overlay.style.display = logged ? "none" : "grid";
  app.style.display = logged ? "" : "none";

  // Si hay funciones de inicialización en script.js, llámalas
  if (logged) {
    if (window.appControls && typeof window.appControls.onAppShow === "function") {
      try { window.appControls.onAppShow(); } catch(e){ console.error(e); }
    }
  } else {
    if (window.appControls && typeof window.appControls.onAppHide === "function") {
      try { window.appControls.onAppHide(); } catch(e){ console.error(e); }
    }
  }
}

function login(user, pass) {
  if (user === VALID_USER && pass === VALID_PASS) {
    localStorage.setItem(SESS_KEY, "1");
    msg.textContent = "";
    updateUI();
    // Opcional: mostrar saludo temporal
    setTimeout(()=> {
      if (overlay) overlay.style.display = "none";
    }, 100);
    return true;
  } else {
    msg.textContent = "Usuario o contraseña incorrectos.";
    msg.className = "authMsg error";
    return false;
  }
}

function logout() {
  localStorage.removeItem(SESS_KEY);
  updateUI();
}

form.addEventListener("submit", (e) => {
  e.preventDefault();
  msg.textContent = "Verificando...";
  msg.className = "authMsg";
  const user = document.getElementById("email").value.trim();
  const pass = document.getElementById("password").value;
  login(user, pass);
});

logoutBtn.addEventListener("click", logout);

// On load
updateUI();

