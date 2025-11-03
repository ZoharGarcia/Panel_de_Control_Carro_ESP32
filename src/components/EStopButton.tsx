import { useRef, useState } from 'react'

export default function EStopButton({
  onStop,
  onRearm
}: {
  onStop?: () => void
  onRearm?: () => void
}) {
  const [armed, setArmed] = useState(true)
  const holdRef = useRef<number | null>(null)

  const triggerStop = () => {
    if (!armed) return
    console.log('E-STOP: STOP sent')
    setArmed(false)
    onStop?.()
  }

  const onHoldToArm = () => {
    if (armed) return
    if (holdRef.current) return
    const started = Date.now()
    holdRef.current = window.setInterval(() => {
      if (Date.now() - started >= 2000) {
        clearInterval(holdRef.current!)
        holdRef.current = null
        setArmed(true)
        console.log('E-STOP: RE-ARMED')
        onRearm?.()
      }
    }, 50)
  }

  const cancelHold = () => {
    if (holdRef.current) {
      clearInterval(holdRef.current)
      holdRef.current = null
    }
  }

  return (
    <div className="fixed top-4 right-4 z-50">
      {armed ? (
        <button
          onPointerDown={triggerStop}
          className="btn-danger shadow-glow"
          aria-label="Paro de emergencia"
          title="Paro inmediato — Mantén 2 s para rearmar"
        >
          E-STOP
        </button>
      ) : (
        <button
          onPointerDown={onHoldToArm}
          onPointerUp={cancelHold}
          onPointerCancel={cancelHold}
          className="btn-ghost border-danger text-danger"
          title="Mantén 2 s para rearmar"
        >
          Re-armar
        </button>
      )}
    </div>
  )
}
