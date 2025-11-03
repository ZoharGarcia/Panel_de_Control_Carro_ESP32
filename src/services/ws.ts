type MsgHandler = (data: any) => void;

export class WSClient {
  private ws: WebSocket | null = null;
  private url: string;
  private onMsg?: MsgHandler;
  private onConnChange?: (connected: boolean) => void;

  constructor(url: string) {
    this.url = url;
  }

  connect(onMessage?: MsgHandler, onConnectionChange?: (c: boolean) => void) {
    this.onMsg = onMessage;
    this.onConnChange = onConnectionChange;

    this.ws = new WebSocket(this.url);
    this.ws.onopen = () => {
      this.onConnChange?.(true);
      console.log('[WS] conectado:', this.url);
    };
    this.ws.onclose = () => {
      this.onConnChange?.(false);
      console.log('[WS] cerrado');
    };
    this.ws.onerror = (e) => {
      this.onConnChange?.(false);
      console.error('[WS] error', e);
    };
    this.ws.onmessage = (e) => {
      try { this.onMsg?.(JSON.parse(e.data)); }
      catch { this.onMsg?.(e.data); }
    };
  }

  send(data: unknown) {
    if (this.ws && this.ws.readyState === WebSocket.OPEN) {
      this.ws.send(JSON.stringify(data));
    }
  }

  isOpen() {
    return this.ws?.readyState === WebSocket.OPEN;
  }

  close() {
    this.ws?.close();
    this.ws = null;
  }
}
