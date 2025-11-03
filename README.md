# ESP32 Car Control UI (React + Vite + Tailwind)

Panel de control en **dark UI** con joystick, E‑Stop fijo, slider de **Velocidad máx.**, y chips de estado (Conectado/RSSI/Latencia/Batería). Listo para conectar por **WebSocket** a ESP32.

## Stack
- React + Vite + TypeScript
- TailwindCSS
- Componente **Joystick** con zona muerta y anillo de saturación
- Botón **E‑Stop** con re-armado manteniendo 2 s

## Estructura
```
src/
  components/
    EStopButton.tsx
    Joystick.tsx
    Slider.tsx
    StatusChips.tsx
  hooks/
    useConnectionStatus.ts
  App.tsx
  index.css
  main.tsx
tailwind.config.js
postcss.config.js
vite.config.ts
tsconfig.json
package.json
```
