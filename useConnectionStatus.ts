import { useEffect, useState } from 'react'

export interface LinkStats {
  connected: boolean
  rssi: number   // dBm
  latency: number // ms
  batteryPct: number
  batteryV: number
}

export function useConnectionStatus() {
  const [stats, setStats] = useState<LinkStats>({
    connected: true,
    rssi: -63,
    latency: 42,
    batteryPct: 75,
    batteryV: 11.9,
  })

  // Demo: jitter update
  useEffect(() => {
    const id = setInterval(() => {
      setStats(s => ({
        ...s,
        rssi: -60 - Math.floor(Math.random()*8),
        latency: 35 + Math.floor(Math.random()*25),
        batteryPct: Math.max(5, Math.min(100, s.batteryPct + (Math.random() > 0.6 ? -1 : 0))),
        batteryV: parseFloat((11.4 + s.batteryPct/100 * 1.0).toFixed(2))
      }))
    }, 1000)
    return () => clearInterval(id)
  }, [])

  return stats
}
