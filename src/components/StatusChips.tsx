import { useConnectionStatus } from '../hooks/useConnectionStatus'

export default function StatusChips() {
  const stats = useConnectionStatus()

  return (
    <div className="flex items-center gap-3">
      <span className="chip">
        <span
          className={`w-2 h-2 rounded-full ${
            stats.connected ? 'bg-success' : 'bg-danger'
          }`}
        />
        <span>{stats.connected ? 'Conectado' : 'Desconectado'}</span>
      </span>

      <span className="chip">
        <span className="opacity-70">RSSI</span>
        <span> {stats.rssi} dBm</span>
      </span>

      <span className="chip">
        <span className="opacity-70">Latencia</span>
        <span> {stats.latency} ms</span>
      </span>

      <span className="chip">
        <span className="opacity-70">Batería</span>
        <span>
          {' '}
          {stats.batteryPct}% ({stats.batteryV} V)
        </span>
      </span>
    </div>
  )
}
