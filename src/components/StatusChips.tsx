import { Link } from 'react-router-dom' // unused but shows future-ready routing
import { useConnectionStatus } from '../hooks/useConnectionStatus'

export default function StatusChips() {
  const stats = useConnectionStatus()

  return (
    <div className="flex items-center gap-3">
      <div className="chip">
        <span className={`w-2 h-2 rounded-full ${stats.connected ? 'bg-success' : 'bg-danger'}`}></span>
        <span>{stats.connected ? 'Conectado' : 'Desconectado'}</span>
      </div>
      <div className="chip">
        <span className="opacity-70">RSSI</span>
        <span> {stats.rssi} dBm</span>
      </div>
      <div className="chip">
        <span className="opacity-70">Latencia</span>
        <span> {stats.latency} ms</span>
      </div>
      <div className="chip">
        <span className="opacity-70">Batería</span>
        <span> {stats.batteryPct}% ({stats.batteryV} V)</span>
      </div>
    </div>
  )
}
