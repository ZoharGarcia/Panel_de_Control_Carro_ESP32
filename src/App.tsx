import Joystick from './components/Joystick'
import Slider from './components/Slider'
import StatusChips from './components/StatusChips'
import EStopButton from './components/EStopButton'
import { useRef, useState } from 'react'

export default function App() {
  const [maxSpeed, setMaxSpeed] = useState(50)
  const xRef = useRef(0)
  const yRef = useRef(0)

  // This is where you'd emit WS messages to ESP32
  const onJoy = (norm:{x:number;y:number}) => {
    xRef.current = norm.x
    yRef.current = norm.y
    // Example packet you might send:
    // ws.send(JSON.stringify({ type:'cmd', x:norm.x, y:norm.y, max:maxSpeed }))
  }

  return (
    <div className="min-h-screen">
      <EStopButton />
      <header className="sticky top-0 z-40 backdrop-blur bg-bg/80 border-b border-cardBorder">
        <div className="max-w-6xl mx-auto px-6 py-4 flex items-center justify-between">
          <div className="text-lg font-semibold">Panel de Control — Carrito ESP32</div>
          <StatusChips />
        </div>
      </header>

      <main className="max-w-6xl mx-auto px-6 py-8">
        <div className="grid grid-cols-1 lg:grid-cols-3 gap-6">
          <div className="lg:col-span-2">
            <Joystick onChange={(norm)=>onJoy(norm)} />
            <div className="mt-3 text-sm text-textMuted">
              X: {xRef.current.toFixed(2)} | Y: {yRef.current.toFixed(2)}
            </div>
          </div>
          <div className="lg:col-span-1">
            <div className="card p-5">
              <div className="title mb-4">Límites y Modos</div>
              <Slider label="Velocidad máx." value={maxSpeed} onChange={setMaxSpeed} />
              <div className="mt-4 text-sm text-textMuted">
                Nota: “Velocidad máx.” actúa como límite superior de PWM.
              </div>
            </div>
          </div>
        </div>

        <section className="mt-8">
          <div className="title mb-3">Datos de Sensores</div>
          <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
            <div className="card p-5">
              <div className="text-sm opacity-80 mb-1">ESP32</div>
              <div className="flex items-center gap-2">
                <span className="w-2 h-2 rounded-full bg-success"></span>
                <span>Conectado</span>
              </div>
              <div className="mt-2 text-sm text-textMuted">Último paquete: hace 0.2 s</div>
            </div>
            <div className="card p-5">
              <div className="text-sm opacity-80 mb-1">Motores</div>
              <div className="flex items-center gap-2">
                <span className="w-2 h-2 rounded-full bg-cardBorder"></span>
                <span>Apagados</span>
              </div>
              <div className="mt-2 text-sm text-textMuted">Listos para armar</div>
            </div>
          </div>
        </section>

        <footer className="mt-12 text-center text-sm text-textMuted">
          Proyecto Carrito ESP32 — Zohar
        </footer>
      </main>
    </div>
  )
}
