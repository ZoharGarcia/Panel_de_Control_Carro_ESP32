const VALID_USER = "Admin";
const VALID_PASS = "Admin2025";

// Clave localStorage
const SESS_KEY = "esp32_panel_session";

// Referencias de elementos
const app = document.getElementById("app");
const overlay = document.getElementById("authOverlay");
const form = document.getElementById("loginForm");
const msg = document.getElementById("authMsg");
const logoutBtn = document.getElementById("logoutBtn");

// Mostrar u ocultar panel según sesión
function updateUI() {
  const logged = localStorage.getItem(SESS_KEY) === "1";
  overlay.style.display = logged ? "none" : "grid";
  app.style.display = logged ? "" : "none";
}

// Iniciar sesión
function login(user, pass) {
  if (user === VALID_USER && pass === VALID_PASS) {
    localStorage.setItem(SESS_KEY, "1");
    updateUI();
    msg.textContent = "";
    return true;
  } else {
    msg.textContent = "Usuario o contraseña incorrectos.";
    msg.className = "authMsg error";
    return false;
  }
}

// Cerrar sesión
function logout() {
  localStorage.removeItem(SESS_KEY);
  updateUI();
}

// Eventos
form.addEventListener("submit", (e) => {
  e.preventDefault();
  msg.textContent = "Verificando...";
  msg.className = "authMsg";
  const user = document.getElementById("email").value.trim();
  const pass = document.getElementById("password").value;
  login(user, pass);
});

logoutBtn.addEventListener("click", logout);

// Inicia al cargar la página
updateUI();
