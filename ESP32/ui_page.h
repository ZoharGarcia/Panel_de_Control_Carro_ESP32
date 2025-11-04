#pragma once

const char INDEX_HTML[] PROGMEM = R"HTML(<!doctype html>
<html lang="es">
<head>
<meta charset="utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<title>Panel - Carrito ESP32</title>
<style>
  :root{
    --bg:#0e0f12; --panel:#14161a; --card:#1a1d22; --muted:#8b93a1;
    --primary:#3d6cff; --primary2:#5b8bff; --ok:#23c55e; --warn:#f59e0b; --bad:#ef4444;
    --ring:#2a2e37; --on:#cdd6f4; --soft:#c3ccdb;
  }
  *{box-sizing:border-box}
  html,body{height:100%}
  body{
    margin:0; background:radial-gradient(1200px 600px at 20% -10%, #131620 0, transparent 60%),
               radial-gradient(1100px 700px at 80% 10%, #0f1320 0, transparent 60%),
               var(--bg);
    color:var(--on); font:16px/1.45 system-ui, -apple-system, Segoe UI, Roboto, Helvetica, Arial;
  }
  .wrap{max-width:1200px; margin:24px auto; padding:0 16px; display:grid; grid-template-columns: 1fr 1fr; gap:24px}
  header{
    grid-column:1/-1; display:flex; align-items:center; justify-content:space-between;
    padding:14px 18px; border-radius:14px; background:var(--panel); box-shadow:0 10px 24px rgba(0,0,0,.35) inset, 0 4px 20px rgba(0,0,0,.25);
  }
  .badge{display:inline-flex; align-items:center; gap:8px; padding:6px 10px; border-radius:999px; background:#0f1319; color:var(--soft); border:1px solid var(--ring); font-size:13px}
  .badge.ok{color:#d1fae5; background:#0f1913; border-color:#1b2e23}
  .badge .dot{width:8px; height:8px; border-radius:999px; background:var(--ok)}
  .sliderCard,.stateCard,.sensorsCard,.padCard{
    background:var(--panel); border:1px solid var(--ring); border-radius:18px; padding:18px;
    box-shadow: 0 6px 22px rgba(0,0,0,.35), inset 0 1px 0 rgba(255,255,255,.02);
  }
  .sectionTitle{font-weight:600; margin-bottom:12px; color:#e6e9ef}
  /* Grid derecha */
  .right{display:grid; gap:18px}
  /* Slider */
  .sliderRow{display:flex; align-items:center; gap:12px}
  input[type=range]{width:100%}
  .percent{min-width:52px; text-align:right; color:#d1d7e2}
  /* State */
  .stateGroup{display:grid; gap:12px}
  .stateItem{display:flex; align-items:center; justify-content:space-between; background:var(--card); border:1px solid var(--ring); border-radius:14px; padding:12px 14px}
  .stateItem .name{display:flex; align-items:center; gap:10px; color:#cfd7e6}
  .stateItem .sub{color:var(--muted); font-size:14px}
  /* Sensors */
  .sensorsList{display:grid; gap:10px}
  .sensorRow{display:flex; align-items:center; justify-content:space-between; background:var(--card); border:1px solid var(--ring); border-radius:14px; padding:12px 14px}
  .sensorRow .name{display:flex; align-items:center; gap:10px}
  .pill{width:26px; height:26px; display:grid; place-items:center; border-radius:8px; background:#0f1a12; border:1px solid #1c2a21}
  .value{color:#a7f3d0}
  /* D-Pad */
  .pad{
    width:360px; max-width:100%; margin:0 auto; display:grid; gap:12px;
    grid-template-columns: repeat(3, 1fr);
  }
  .padCard{display:flex; flex-direction:column; gap:18px}
  .padCard p{margin:0; color:var(--muted)}
  .btn{
    user-select:none; -webkit-user-select:none; touch-action:none;
    display:grid; place-items:center; height:78px; border-radius:16px;
    background:linear-gradient(180deg, var(--primary) 0%, var(--primary2) 100%);
    color:white; font-weight:600; border:none; outline:none;
    box-shadow:0 10px 30px rgba(61,108,255,.35), inset 0 1px 0 rgba(255,255,255,.18);
    cursor:pointer; transition:transform .05s ease-in-out, filter .15s;
  }
  .btn:active{transform:translateY(2px); filter:saturate(1.1)}
  .btn.center{background:#2a2e37; box-shadow:inset 0 0 0 2px #353b47}
  .btn svg{width:24px; height:24px}
  .muted{color:var(--muted); font-size:14px}
  footer{grid-column:1/-1; text-align:center; color:#9aa3b5; padding:12px 4px}
  @media (max-width:980px){ .wrap{grid-template-columns: 1fr} }
</style>
</head>
<body>
  <div class="wrap">
    <header>
      <div class="title"> <strong>Panel - Carrito ESP32</strong> </div>
      <div style="display:flex; gap:10px; align-items:center">
        <div class="badge ok" id="wifiBadge"><span class="dot"></span> Conectado</div>
        <div class="badge" id="batBadge">75%</div>
      </div>
    </header>

    <!-- Izquierda: D-Pad -->
    <div class="padCard">
      <div class="sectionTitle">Control de Movimiento</div>
      <p>Presiona una dirección</p>
      <div class="pad">
        <div></div>
        <button class="btn" id="btnUp"    data-cmd="adelante" aria-label="Up">
          <svg viewBox="0 0 24 24" fill="none"><path d="M6 14l6-6 6 6" stroke="white" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/></svg>
        </button>
        <div></div>

        <button class="btn" id="btnLeft"  data-cmd="izquierda" aria-label="Left">
          <svg viewBox="0 0 24 24" fill="none"><path d="M14 6l-6 6 6 6" stroke="white" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/></svg>
        </button>
        <button class="btn center" id="btnStop" data-cmd="detener" aria-label="Stop">
          <svg viewBox="0 0 24 24" fill="none"><circle cx="12" cy="12" r="4" stroke="#8b93a1" stroke-width="2"/></svg>
        </button>
        <button class="btn" id="btnRight" data-cmd="derecha" aria-label="Right">
          <svg viewBox="0 0 24 24" fill="none"><path d="M10 6l6 6-6 6" stroke="white" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/></svg>
        </button>

        <div></div>
        <button class="btn" id="btnDown"  data-cmd="atras" aria-label="Down">
          <svg viewBox="0 0 24 24" fill="none"><path d="M18 10l-6 6-6-6" stroke="white" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/></svg>
        </button>
        <div></div>
      </div>
      <div class="muted">Al soltar, el carro se detiene.</div>
    </div>

    <!-- Derecha: Velocidad (visual) + Estado + Sensores -->
    <div class="right">
      <div class="sliderCard">
        <div class="sectionTitle">Velocidad</div>
        <div class="sliderRow">
          <span>0%</span>
          <input type="range" min="0" max="100" value="50" id="speedSlider" />
          <span class="percent" id="speedLbl">50%</span>
        </div>
        <div class="muted">*Solo visual por ahora (el ESP32 no usa este valor).</div>
      </div>

      <div class="stateCard">
        <div class="sectionTitle">Estado del Sistema</div>
        <div class="stateGroup">
          <div class="stateItem">
            <div class="name">
              <div class="pill">⚙️</div>
              <div>ESP32<br><span class="sub" id="espState">• Conectado</span></div>
            </div>
            <div id="ipLbl" class="muted">IP —</div>
          </div>
          <div class="stateItem">
            <div class="name">
              <div class="pill">🛞</div>
              <div>Motores<br><span class="sub" id="motState">• Apagados</span></div>
            </div>
            <div class="muted" id="lastCmd">—</div>
          </div>
        </div>
      </div>

      <div class="sensorsCard">
        <div class="sectionTitle">Sensores</div>
        <div class="sensorsList" id="sensorsList">
          <!-- Filas se rellenan por JS -->
        </div>
      </div>
    </div>

    <footer>Proyecto Carrito ESP32 — Tu Nombre</footer>
  </div>

<script>
  const sensorsUI = [
    {key:'mq7',    label:'MQ-7',    unit:'ppm'},
    {key:'mq9',    label:'MQ-9',    unit:'ppm'},
    {key:'mq135',  label:'MQ-135',  unit:'ppm'},
    {key:'dht_t',  label:'DHT22 Temp', unit:'°C'},
    {key:'dht_h',  label:'DHT22 Hum',  unit:'%'},
  ];

  // Construir filas de sensores
  const list = document.getElementById('sensorsList');
  sensorsUI.forEach(s=>{
    const row = document.createElement('div');
    row.className = 'sensorRow';
    row.innerHTML = `
      <div class="name">
        <div class="pill">🟢</div>
        <div>${s.label}</div>
      </div>
      <div class="value" id="val_${s.key}">—</div>
    `;
    list.appendChild(row);
  });

  // Slider (visual)
  const speed = document.getElementById('speedSlider');
  const speedLbl = document.getElementById('speedLbl');
  speed.addEventListener('input', ()=>{ speedLbl.textContent = speed.value + '%'; });

  // Botones: mantener pulsado => mueve; soltar => detener
  const bindHold = (el, cmd) => {
    const go = ()=> fetch('/'+cmd).catch(()=>{});
    const stop = ()=> fetch('/detener').catch(()=>{});
    // mouse
    el.addEventListener('mousedown', go);
    el.addEventListener('mouseup',   stop);
    el.addEventListener('mouseleave',stop);
    // touch
    el.addEventListener('touchstart', (e)=>{ e.preventDefault(); go(); }, {passive:false});
    el.addEventListener('touchend',   (e)=>{ e.preventDefault(); stop(); }, {passive:false});
    el.addEventListener('touchcancel',(e)=>{ e.preventDefault(); stop(); }, {passive:false});
  };

  bindHold(document.getElementById('btnUp'),    'adelante');
  bindHold(document.getElementById('btnDown'),  'atras');
  bindHold(document.getElementById('btnLeft'),  'izquierda');
  bindHold(document.getElementById('btnRight'), 'derecha');

  // Botón central: click corto => detener inmediato
  document.getElementById('btnStop').addEventListener('click', ()=> fetch('/detener'));

  // Ping /status y /sensors
  const ipLbl   = document.getElementById('ipLbl');
  const espState= document.getElementById('espState');
  const motState= document.getElementById('motState');
  const lastCmd = document.getElementById('lastCmd');

  async function poll() {
    try{
      const s = await fetch('/status');     // texto: "alive|ip:x.x.x.x|mot:0|last:cmd"
      const t = await s.text();
      // parseo simple
      // ejemplo: "alive|ip:192.168.1.23|mot:0|last:-"
      const parts = t.split('|').reduce((a,x)=>{
        const p=x.split(':'); if(p.length>1) a[p[0]]=p.slice(1).join(':'); else a[p[0]]=true; return a;
      }, {});
      if(parts['ip'])  ipLbl.textContent = 'IP ' + parts['ip'];
      if(parts['mot']) motState.textContent = (parts['mot']=='1'?'• Encendidos':'• Apagados');
      if(parts['last']) lastCmd.textContent = parts['last'];
      espState.textContent = '• Conectado';
    }catch(e){
      espState.textContent = '• Desconectado';
    }

    try{
      const r = await fetch('/sensors');
      const j = await r.json();
      // Nota: el ESP envía tanto voltaje como "ppm" aproximado no calibrado.
      const fmt = (n)=> (typeof n==='number' ? n.toFixed(1) : '—');
      document.getElementById('val_mq7').textContent   = fmt(j.mq7_ppm)   + ' ppm';
      document.getElementById('val_mq9').textContent   = fmt(j.mq9_ppm)   + ' ppm';
      document.getElementById('val_mq135').textContent = fmt(j.mq135_ppm) + ' ppm';
      document.getElementById('val_dht_t').textContent = fmt(j.dht_t)     + ' °C';
      document.getElementById('val_dht_h').textContent = fmt(j.dht_h)     + ' %';
    }catch(e){}
  }
  setInterval(poll, 1000);
  poll();
</script>
</body>
</html>)HTML";
