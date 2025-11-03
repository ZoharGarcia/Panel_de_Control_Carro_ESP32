# ESP32 Car Control UI (React + Vite + Tailwind)

Panel de control en **dark UI** con joystick, E‑Stop fijo, slider de **Velocidad máx.**, y chips de estado (Conectado/RSSI/Latencia/Batería). Listo para conectar por **WebSocket** a tu ESP32.

## Stack
- React + Vite + TypeScript
- TailwindCSS
- Componente **Joystick** con zona muerta y anillo de saturación
- Botón **E‑Stop** con re-armado manteniendo 2 s

## Inicio rápido
```bash
npm install
npm run dev
```

Abrir: http://localhost:5173

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

## Conexión con ESP32 (WebSocket)
1. En `App.tsx` dentro de `onJoy` está el lugar para emitir comandos:
   ```ts
   // ws.send(JSON.stringify({ type:'cmd', x:norm.x, y:norm.y, max:maxSpeed }))
   ```
2. Crea una instancia de `WebSocket` al montar la app y guarda la referencia (o usa Zustand/Context).
3. A nivel de firmware, recomienda enviar **STOP** triple paquete al E‑Stop.

## Accesibilidad
- Contraste ≥ 4.5:1 en texto principal
- Targets táctiles ≥ 56 px
- `outline` visible para foco por teclado

## Licencia
MIT
