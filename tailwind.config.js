/** @type {import('tailwindcss').Config} */
export default {
  content: ['./index.html', './src/**/*.{ts,tsx}'],
  theme: {
    extend: {
      colors: {
        bg: '#0F1115',
        card: '#171A20',
        cardBorder: '#262A33',
        primary: '#4AA3FF',
        success: '#22C55E',
        danger: '#EF4444',
        text: '#E6E8EE',
        textMuted: '#B7BCC5',
      },
      boxShadow: {
        'soft': '0 2px 16px rgba(0,0,0,0.25)',
        'glow': '0 0 0 3px rgba(74,163,255,0.35)',
      },
      borderRadius: {
        'xl2': '1rem'
      }
    },
  },
  plugins: [],
}
