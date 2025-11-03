import { useState } from 'react'

interface Props {
  label: string
  min?: number
  max?: number
  step?: number
  value?: number
  onChange?: (v:number) => void
}

export default function Slider({ label, min=0, max=100, step=1, value=50, onChange }: Props) {
  const [v, setV] = useState(value)
  return (
    <div className="card p-5 w-full">
      <div className="flex items-center justify-between mb-3">
        <span className="font-medium">{label}</span>
        <span className="px-2 py-1 bg-primary text-black rounded-md text-sm">{v}%</span>
      </div>
      <input
        type="range"
        min={min}
        max={max}
        step={step}
        value={v}
        onChange={(e)=>{
          const nv = Number(e.target.value)
          setV(nv)
          onChange?.(nv)
        }}
        className="w-full accent-primary"
      />
      <div className="flex justify-between text-sm text-textMuted">
        <span>{min}%</span><span>{Math.round((min+max)/2)}%</span><span>{max}%</span>
      </div>
    </div>
  )
}
