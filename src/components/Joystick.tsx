import { useEffect, useRef, useState } from 'react'
import clsx from 'clsx'

interface Vec2 { x:number; y:number }

export default function Joystick({
  radius = 140,
  deadZone = 0.12,
  onChange
}: {
  radius?: number
  deadZone?: number
  onChange?: (norm:Vec2, raw:Vec2) => void
}) {
  const wrapRef = useRef<HTMLDivElement>(null)
  const [center, setCenter] = useState<Vec2>({x:0,y:0})
  const [knob, setKnob] = useState<Vec2>({x:0,y:0})
  const [drag, setDrag] = useState(false)

  useEffect(()=>{
    const el = wrapRef.current!
    const rect = el.getBoundingClientRect()
    setCenter({ x: rect.width/2, y: rect.height/2 })
  }, [])

  const clamp = (v:number, min:number, max:number)=>Math.max(min, Math.min(max, v))

  const handle = (clientX:number, clientY:number, end=false) => {
    const el = wrapRef.current!
    const rect = el.getBoundingClientRect()
    const cx = rect.width/2, cy = rect.height/2
    let dx = clientX - (rect.left + cx)
    let dy = clientY - (rect.top + cy)
    const dist = Math.hypot(dx, dy)
    if (dist > radius) {
      const k = radius / dist
      dx *= k; dy *= k
    }
    if (end) { dx = 0; dy = 0 }
    setKnob({ x: dx, y: dy })

    // raw in [-1,1]
    const raw = { x: dx / radius, y: -dy / radius }
    // apply deadzone
    const mag = Math.hypot(raw.x, raw.y)
    let norm = { x:0, y:0 }
    if (mag > deadZone) {
      const s = (mag - deadZone) / (1 - deadZone)
      norm = { x: (raw.x/mag)*s, y: (raw.y/mag)*s }
    }
    onChange?.(norm, raw)
  }

  return (
    <div className="card p-6">
      <div className="text-center mb-1 font-medium">Control de Movimiento</div>
      <p className="text-center text-sm text-textMuted mb-6">Arrastra el joystick para mover el carrito</p>
      <div
        ref={wrapRef}
        onPointerDown={(e)=>{ setDrag(true); (e.target as HTMLElement).setPointerCapture?.(e.pointerId); handle(e.clientX, e.clientY) }}
        onPointerMove={(e)=>{ if (drag) handle(e.clientX, e.clientY) }}
        onPointerUp={(e)=>{ setDrag(false); handle(e.clientX, e.clientY, true) }}
        onPointerCancel={(e)=>{ setDrag(false); handle(e.clientX, e.clientY, true) }}
        className="relative mx-auto my-4 rounded-full border border-cardBorder"
        style={{ width: radius*2, height: radius*2, background: 'radial-gradient(circle at 50% 50%, rgba(255,255,255,0.03), rgba(0,0,0,0.08))' }}
      >
        {/* Deadzone */}
        <div
          className="absolute rounded-full border border-cardBorder/70"
          style={{
            width: radius*2*deadZone,
            height: radius*2*deadZone,
            left: '50%', top: '50%',
            transform: 'translate(-50%,-50%)'
          }}
        />
        {/* 100% ring */}
        <div
          className="absolute rounded-full border-2 border-cardBorder/80"
          style={{
            width: radius*2,
            height: radius*2,
            left: 0, top: 0
          }}
        />
        {/* Knob */}
        <div
          className={clsx("absolute rounded-full shadow-glow")}
          style={{
            width: 56, height: 56, background: '#4AA3FF',
            left: `calc(50% + ${knob.x - 28}px)`,
            top: `calc(50% + ${knob.y - 28}px)`,
            transition: drag ? 'none' : 'transform 120ms ease-out',
          }}
        />
        {/* Axis ticks */}
        <div className="absolute left-1/2 top-0 -translate-x-1/2 h-full w-px bg-cardBorder/40"></div>
        <div className="absolute top-1/2 left-0 -translate-y-1/2 w-full h-px bg-cardBorder/40"></div>
      </div>
    </div>
  )
}
