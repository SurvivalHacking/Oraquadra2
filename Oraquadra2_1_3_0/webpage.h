// Pagina web di controllo OraQuadra2 - servita da handleRoot()
// Tenuta in un header separato: il generatore di prototipi di Arduino non
// analizza gli header, quindi il contenuto HTML/JS (virgolette, apostrofi)
// non puo piu confondere la compilazione dello sketch.
#pragma once

const char webpage[] PROGMEM = R"rawliteral(    

<!DOCTYPE html>
<html lang="it">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>OraQuadra2 Plus</title>
    <meta name="theme-color" content="#000000">
    <link rel="icon" href="data:image/svg+xml,%3Csvg xmlns=%22http://www.w3.org/2000/svg%22 viewBox=%220 0 16 16%22%3E%3Crect width=%2216%22 height=%2216%22 rx=%223%22 fill=%22%23111%22/%3E%3Crect x=%223%22 y=%223%22 width=%222%22 height=%222%22 fill=%22%232196F3%22/%3E%3Crect x=%227%22 y=%223%22 width=%222%22 height=%222%22 fill=%22%23eee%22/%3E%3Crect x=%2211%22 y=%223%22 width=%222%22 height=%222%22 fill=%22%23eee%22/%3E%3Crect x=%223%22 y=%227%22 width=%222%22 height=%222%22 fill=%22%23eee%22/%3E%3Crect x=%227%22 y=%227%22 width=%222%22 height=%222%22 fill=%22%234CAF50%22/%3E%3Crect x=%2211%22 y=%227%22 width=%222%22 height=%222%22 fill=%22%23eee%22/%3E%3Crect x=%223%22 y=%2211%22 width=%222%22 height=%222%22 fill=%22%23eee%22/%3E%3Crect x=%227%22 y=%2211%22 width=%222%22 height=%222%22 fill=%22%23FF9800%22/%3E%3Crect x=%2211%22 y=%2211%22 width=%222%22 height=%222%22 fill=%22%23F44336%22/%3E%3C/svg%3E">
    <style>
        :root {
            --primary: #2196F3;
            --primary-dark: #1976D2;
            --secondary: #FF9800;
            --success: #4CAF50;
            --danger: #F44336;
            --dark: #333;
            --light: #f4f4f4;
        }

        /* Pallino stato connessione */
        .conn-dot { display:inline-block;width:10px;height:10px;border-radius:50%;background:var(--danger);margin-left:8px;vertical-align:middle;transition:background 0.3s; }
        .conn-dot.online { background:var(--success); }

        /* Colore attualmente selezionato */
        .color-btn.selected { outline:3px solid white; outline-offset:2px; }

        /* Bottone con modifiche non salvate */
        .btn-dirty { animation: dirtyPulse 1.5s infinite; }
        @keyframes dirtyPulse { 0%,100% { box-shadow:0 0 0 0 rgba(76,175,80,0.7); } 50% { box-shadow:0 0 0 9px rgba(76,175,80,0); } }

        /* Badge fascia oraria attiva */
        .active-band-badge { display:inline-block;background:var(--success);color:white;font-size:0.7rem;padding:2px 8px;border-radius:10px;margin-left:8px;vertical-align:middle;font-weight:bold; }

        /* Hint modalita di salvataggio della sezione */
        .save-hint { color:#999;font-size:0.72rem;font-weight:normal;margin-left:10px;vertical-align:middle; }

        /* Sezioni collassabili */
        .sec-head { cursor:pointer; user-select:none; }
        .control-section.collapsed > :not(.sec-head) { display:none !important; }
        .container > .control-section > h2.sec-head::before,
        .container > .control-section > div.sec-head h2::before { content:"\25BE "; color:#888; font-size:0.85em; }
        .container > .control-section.collapsed > h2.sec-head::before,
        .container > .control-section.collapsed > div.sec-head h2::before { content:"\25B8 "; }

        /* Card stato compatta su mobile */

        
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: #000000;
            color: var(--light);
            min-height: 100vh;
            padding: 20px;
        }
        
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background: rgba(255, 255, 255, 0.1);
            backdrop-filter: blur(10px);
            border-radius: 15px;
            padding: 30px;
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3);
            border: 1px solid rgba(255, 255, 255, 0.2);
        }
        
        header {
            text-align: center;
            margin-bottom: 30px;
            padding-bottom: 20px;
            border-bottom: 1px solid rgba(255, 255, 255, 0.2);
        }
        
        h1 {
            font-size: 2.5rem;
            margin-bottom: 10px;
            text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.5);
            color: white;
        }
        
        .subtitle {
            font-size: 1.1rem;
            opacity: 0.9;
            color: #cccccc;
        }
        
        .status-card {
            background: rgba(255, 255, 255, 0.15);
            border-radius: 10px;
            padding: 20px;
            margin-bottom: 25px;
            display: flex;
            justify-content: space-between;
            align-items: center;
            flex-wrap: wrap;
            gap: 15px;
            border: 1px solid rgba(255, 255, 255, 0.1);
        }
        
        .status-item {
            display: flex;
            flex-direction: column;
            align-items: center;
            min-width: 120px;
        }
        
        .status-label {
            font-size: 0.9rem;
            opacity: 0.8;
            margin-bottom: 5px;
            color: #cccccc;
        }
        
        .status-value {
            font-size: 1.3rem;
            font-weight: bold;
        }
        
        .status-time {
            color: #FFD700 !important; /* Oro per l'orario */
        }
        
        .status-mode {
            color: #FF9800 !important; /* Arancione per la modalità */
        }
        
        .status-color {
            color: #4CAF50 !important; /* Verde per il colore */
        }
        
        .status-blink {
            color: #2196F3 !important; /* Blu per il lampeggio */
        }
        
        .status-brightness {
            color: #9C27B0 !important; /* Viola per la luminosità */
        }
        
        .status-power {
            color: #F44336 !important; /* Rosso per lo stato */
        }
        .status-scroll {
            color: #00BCD4 !important; /* Azzurro/Ciano per lo scroll */
        }
        .control-section {
            background: rgba(255, 255, 255, 0.15);
            border-radius: 10px;
            padding: 25px;
            margin-bottom: 25px;
            border: 1px solid rgba(255, 255, 255, 0.1);
        }
        
        .day-night-settings {
            margin-top: 20px;
            padding: 20px;
            background: rgba(255, 255, 255, 0.1);
            border-radius: 8px;
            border: 1px solid rgba(255, 255, 255, 0.2);
        }
        
        .time-brightness-row {
            display: flex;
            gap: 20px;
            align-items: center;
            margin-bottom: 20px;
            flex-wrap: wrap;
        }
        
        .time-input {
            display: flex;
            align-items: center;
            gap: 10px;
            min-width: 250px;
        }
        
        .time-input label {
            color: white;
            font-weight: bold;
            min-width: 130px;
        }
        
        .time-input input[type="time"] {
            padding: 8px;
            border-radius: 4px;
            border: 1px solid rgba(255, 255, 255, 0.3);
            background: rgba(255, 255, 255, 0.1);
            color: white;
            font-size: 1rem;
        }
        
        .brightness-control-inline {
            flex: 1;
            min-width: 250px;
        }
        
        .brightness-control-inline label {
            display: block;
            color: white;
            margin-bottom: 5px;
            font-weight: bold;
        }
        
        .brightness-control-inline .brightness-slider {
            width: 100%;
        }
        
        @media (max-width: 768px) {
            .time-brightness-row {
                flex-direction: column;
                align-items: stretch;
            }
            
            .time-input {
                width: 100%;
            }
            
            .brightness-control-inline {
                width: 100%;
            }
        }

        h2 {
            font-size: 1.5rem;
            margin-bottom: 20px;
            display: flex;
            align-items: center;
            gap: 10px;
            color: white;
        }
        
        h2 i {
            color: var(--secondary);
        }
        
        .mode-grid {
            display: flex;
            flex-direction: column;
            gap: 6px;
            margin-bottom: 20px;
        }
        
        .mode-btn {
            background: rgba(255, 255, 255, 0.2);
            border: 1px solid rgba(255, 255, 255, 0.3);
            border-radius: 8px;
            padding: 15px;
            color: white;
            font-size: 1rem;
            cursor: pointer;
            transition: all 0.3s ease;
            text-align: center;
        }
        
        .mode-btn:hover {
            background: rgba(255, 255, 255, 0.3);
            transform: translateY(-2px);
        }
        
        .mode-btn.active {
            background: var(--primary);
            box-shadow: 0 4px 15px rgba(33, 150, 243, 0.4);
            border-color: var(--primary);
        }
        
        .color-grid {
            display: grid;
            grid-template-columns: repeat(auto-fill, minmax(80px, 1fr));
            gap: 10px;
            margin-bottom: 20px;
        }
        
        .color-btn {
            width: 60px;
            height: 60px;
            border: 2px solid rgba(255, 255, 255, 0.5);
            border-radius: 50%;
            cursor: pointer;
            transition: all 0.3s ease;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
        }
        
        .color-btn:hover {
            transform: scale(1.1);
            border-color: white;
        }
        
        .toggle-container {
            display: flex;
            align-items: center;
            gap: 20px;
            margin-bottom: 20px;
            width: 100%;
            padding: 10px 0;
        }
        
        .toggle {
            position: relative;
            display: inline-block;
            width: 80px;
            height: 40px;
            flex-shrink: 0;
        }
        
        .toggle input {
            opacity: 0;
            width: 0;
            height: 0;
        }
        
        .slider {
            position: absolute;
            cursor: pointer;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: #ccc;
            transition: .4s;
            border-radius: 34px;
        }
        
        .slider:before {
            position: absolute;
            content: "";
            height: 32px;
            width: 32px;
            left: 4px;
            bottom: 4px;
            background-color: white;
            transition: .4s;
            border-radius: 50%;
        }
        
        input:checked + .slider {
            background-color: var(--success);
        }
        
        input:checked + .slider:before {
            transform: translateX(40px);
        }
        
        .slider-text {
            font-size: 1.1rem;
            color: white;
            flex: 1;
            min-width: 200px;
            text-align: left;
            white-space: nowrap;
            overflow: hidden;
            text-overflow: ellipsis;
        }
        
        .action-buttons {
            display: flex;
            gap: 15px;
            flex-wrap: wrap;
            margin-top: 20px;
        }
        
        .btn {
            padding: 12px 25px;
            border: none;
            border-radius: 8px;
            font-size: 1rem;
            cursor: pointer;
            transition: all 0.3s ease;
            font-weight: bold;
            display: flex;
            align-items: center;
            gap: 8px;
        }
        
        .btn-primary {
            background: var(--primary);
            color: white;
        }
        
        .btn-primary:hover {
            background: var(--primary-dark);
            transform: translateY(-2px);
        }
        
        .btn-danger {
            background: var(--danger);
            color: white;
        }
        
        .btn-danger:hover {
            background: #d32f2f;
            transform: translateY(-2px);
        }
        
        .btn-warning {
            background: var(--secondary);
            color: white;
        }
        
        .btn-warning:hover {
            background: #F57C00;
            transform: translateY(-2px);
        }
        
        .btn-success {
            background: var(--success);
            color: white;
        }
        
        .btn-success:hover {
            background: #388E3C;
            transform: translateY(-2px);
        }
        
        .notification {
            position: fixed;
            top: 20px;
            right: 20px;
            padding: 15px 25px;
            border-radius: 8px;
            color: white;
            font-weight: bold;
            box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2);
            transform: translateX(150%);
            transition: transform 0.3s ease;
            z-index: 1000;
        }
        
        .notification.show {
            transform: translateX(0);
        }
        
        .notification.success {
            background: var(--success);
        }
        
        .notification.error {
            background: var(--danger);
        }
        
        .brightness-control {
            margin-top: 15px;
        }
        
        .brightness-slider {
            width: 100%;
            margin: 10px 0;
        }
        
        .brightness-value {
            text-align: center;
            font-weight: bold;
            color: white;
        }
        
        .night-mode-settings {
            margin-top: 15px;
            padding: 15px;
            background: rgba(255, 255, 255, 0.1);
            border-radius: 8px;
            border: 1px solid rgba(255, 255, 255, 0.2);
        }
        
        .time-input {
            display: flex;
            align-items: center;
            gap: 10px;
            margin-bottom: 10px;
        }
        
        .time-input label {
            min-width: 80px;
            color: white;
        }
        
        .time-input input {
            padding: 8px;
            border-radius: 4px;
            border: 1px solid rgba(255, 255, 255, 0.3);
            background: rgba(255, 255, 255, 0.1);
            color: white;
        }
        
        @media (max-width: 768px) {
            body {
                padding: 10px;
            }

            .container {
                padding: 15px;
            }

            h1 {
                font-size: 1.8rem;
            }

            h2 {
                font-size: 1.2rem;
            }

            .mode-btn {
                padding: 12px 8px;
                font-size: 0.9rem;
            }

            .status-card {
                display: grid;
                grid-template-columns: 1fr 1fr;
                gap: 8px;
                padding: 15px;
            }

            .status-item {
                flex-direction: column;
                align-items: flex-start;
                min-width: 0;
            }

            .status-item:first-child {
                grid-column: 1 / -1;
                flex-direction: row;
                justify-content: space-between;
                align-items: center;
            }

            .toggle-container {
                gap: 15px;
                flex-wrap: wrap;
            }

            .toggle {
                width: 60px;
                height: 30px;
                flex-shrink: 0;
            }

            .slider:before {
                height: 22px;
                width: 22px;
            }

            input:checked + .slider:before {
                transform: translateX(30px);
            }

            .slider-text {
                font-size: 0.9rem;
                min-width: 0;
                flex: 1;
                white-space: normal;
                word-break: break-word;
            }

            .control-section {
                padding: 15px;
            }

            .color-grid {
                grid-template-columns: repeat(4, 1fr);
                gap: 8px;
            }

            .color-btn {
                width: 45px;
                height: 45px;
            }

            .btn {
                padding: 10px 15px;
                font-size: 0.9rem;
                width: 100%;
                justify-content: center;
            }

            .action-buttons {
                flex-direction: column;
            }

            .day-night-settings {
                padding: 10px;
            }

            .digital-timing-settings {
                padding: 10px;
            }

            .digital-timing-settings > div {
                flex-direction: column;
                gap: 10px;
            }

            /* Impostazioni giorno/notte su mobile */
            .day-night-settings {
                padding: 12px !important;
            }

            .day-night-settings h3 {
                font-size: 1.1rem !important;
                margin-bottom: 12px !important;
            }

            .time-brightness-row {
                flex-direction: column !important;
                gap: 12px !important;
                margin-bottom: 15px !important;
                padding-bottom: 15px;
                border-bottom: 1px solid rgba(255,255,255,0.1);
            }

            .time-brightness-row:last-of-type {
                border-bottom: none;
                margin-bottom: 10px !important;
            }

            .time-input {
                width: 100% !important;
                min-width: auto !important;
                flex-direction: row !important;
                justify-content: space-between !important;
            }

            .time-input label {
                min-width: auto !important;
                flex: 1;
                font-size: 0.95rem !important;
            }

            .time-input input[type="time"] {
                width: 110px !important;
                font-size: 16px !important;
                padding: 10px !important;
            }

            .brightness-control-inline {
                width: 100% !important;
                min-width: auto !important;
            }

            .brightness-control-inline label {
                font-size: 0.9rem !important;
                margin-bottom: 8px !important;
            }

            .brightness-slider {
                width: 100% !important;
                height: 10px !important;
                margin: 5px 0 !important;
            }

            /* Toggle spegnimento notturno su mobile */
            .time-brightness-row .brightness-control-inline[style*="display: flex"] {
                flex-wrap: wrap !important;
                justify-content: flex-start !important;
            }
        }

        @media (max-width: 400px) {
            .color-grid {
                grid-template-columns: repeat(3, 1fr);
            }
        }

        /* Mobile responsive */
        input[type="range"] { width: 100%; }
        input[type="number"], input[type="text"], input[type="time"] {
            font-size: 16px;
            max-width: 100%;
            box-sizing: border-box;
        }
        .mode-category { margin-bottom: 12px; }
        .mode-category .mode-btn { flex: 1 1 auto; min-width: 70px; }
    </style>
</head>
<body>
    <div class="container">
        <header>
            <h1>OraQuadra2 Plus</h1>
            <p class="subtitle">Gestisci tutti gli effetti del orologio LED <span id="connDot" class="conn-dot" title="Stato connessione col dispositivo"></span></p>
        </header>
        
        <div class="status-card">
            <div class="status-item">
                <div class="status-label">Orario</div>
                <div class="status-value status-time" id="currentTime">--:--:--</div>
            </div>
            <div class="status-item">
                <div class="status-label">Modalità</div>
                <div class="status-value status-mode" id="currentMode">---</div>
            </div>
            <div class="status-item">
                <div class="status-label">Colore</div>
                <div class="status-value status-color" id="currentColor">---</div>
            </div>
            <div class="status-item">
                <div class="status-label">Lampeggio</div>
                <div class="status-value status-blink" id="currentBlink">---</div>
            </div>
            <div class="status-item">
                <div class="status-label">Luminosità</div>
                <div class="status-value status-brightness" id="currentBrightness">---</div>
            </div>
            <div class="status-item">
                <div class="status-label">Stato</div>
                <div class="status-value status-power" id="currentPower">---</div>
            </div>
            <div class="status-item">
                <div class="status-label">Scroll</div>
                <div class="status-value status-scroll" id="currentScroll">---</div>
            </div>
        </div>
        
        <div class="control-section">
            <h2>🎭 Modalità Visualizzazione <span class="save-hint">salvataggio automatico</span></h2>
            <div class="mode-grid" id="modeButtons">
                <!-- I pulsanti delle modalità verranno generati dinamicamente -->
            </div>
        </div>
        
        <div class="control-section">
            <h2>🎨 Colori Predefiniti <span class="save-hint">salvataggio automatico</span></h2>
            <div class="color-grid" id="colorButtons">
                <!-- I pulsanti dei colori verranno generati dinamicamente -->
            </div>
            <!-- Palette colori avanzata -->
            <div class="control-section">
                <h2>🎨 Palette Colori Avanzata</h2>
                <div style="display:flex;align-items:center;gap:20px;flex-wrap:wrap;">
                    <input type="color" id="advancedColorPicker" value="#ff8800"
                           style="width:80px;height:40px;border:none;background:transparent;cursor:pointer;"
                           oninput="applyAdvancedColor()">
                </div>
            </div>


        </div>

        <div class="control-section">
            <h2>⚙️ Impostazioni</h2>
            
            <div class="toggle-container">
                <label class="toggle">
                    <input type="checkbox" id="blinkToggle">
                    <span class="slider"></span>
                </label>
                <span class="slider-text">Lampeggio Secondi</span>
            </div>
            
            <div class="toggle-container">
                <label class="toggle">
                    <input type="checkbox" id="digitalOverlayToggle">
                    <span class="slider"></span>
                </label>
                <span class="slider-text">Mostra orologio digitale automatico</span>
            </div>

            <div class="digital-timing-settings" style="margin-top:15px;padding:15px;background:rgba(255,255,255,0.1);border-radius:8px;">
                <h4 style="color:white;margin-bottom:10px;">⏱️ Tempistica Orologio Digitale</h4>
                <div style="display:flex;gap:20px;flex-wrap:wrap;align-items:center;">
                    <div>
                        <label style="color:#ccc;">Appare ogni (sec):</label>
                        <input type="number" id="digitalInterval" min="5" max="120" value="30"
                               style="width:60px;padding:5px;border-radius:4px;border:none;"
                               onfocus="startEditingDigital()" onchange="scheduleEndEditingDigital()" oninput="startEditingDigital()">
                    </div>
                    <div>
                        <label style="color:#ccc;">Per (sec):</label>
                        <input type="number" id="digitalDuration" min="5" max="59" value="10"
                               style="width:60px;padding:5px;border-radius:4px;border:none;"
                               onfocus="startEditingDigital()" onchange="scheduleEndEditingDigital()" oninput="startEditingDigital()">
                    </div>
                    <button class="btn btn-primary" onclick="saveDigitalTiming()">Salva</button>
                </div>
                <p style="color:#888;font-size:12px;margin-top:8px;">Animazioni partono al cambio minuto</p>
            </div>

            <div class="day-night-settings">
                <h3 style="color: white; margin-bottom: 15px;">☀️ Luminosità Giorno/Sera/Notte <span class="save-hint">premi Salva per memorizzare</span></h3>
                
                <div class="time-brightness-row">
                    <div class="time-input">
                        <label for="dayStartTime">🌅 Inizio Giorno: <span id="dayActiveBadge" class="active-band-badge" style="display:none;">attiva ora</span></label>
                        <input type="time" id="dayStartTime" value="07:00">
                    </div>
                    <div class="brightness-control-inline">
                        <label>Luminosità: <span id="dayBrightnessValue">80</span>%</label>
                        <input type="range" min="10" max="100" value="80" 
                               class="brightness-slider" id="dayBrightnessSlider">
                    </div>
                </div>
                
                <div class="time-brightness-row">
                    <div class="time-input">
                        <label for="nightStartTime">🌆 Inizio Sera: <span id="nightActiveBadge" class="active-band-badge" style="display:none;">attiva ora</span></label>
                        <input type="time" id="nightStartTime" value="22:00">
                    </div>
                    <div class="brightness-control-inline">
                        <label>Luminosità: <span id="nightBrightnessValue">10</span>%</label>
                        <input type="range" min="1" max="100" value="10" 
                               class="brightness-slider" id="nightBrightnessSlider">
                    </div>
                </div>

                <div class="time-brightness-row" style="border-top: 1px solid rgba(255,255,255,0.2); padding-top: 20px; margin-top: 20px;">
                    <div class="time-input">
                        <label for="sleepStartTime">🌙 Spegnimento Notturno:</label>
                        <input type="time" id="sleepStartTime" value="23:00">
                    </div>
                    <div class="brightness-control-inline" style="display: flex; align-items: center; gap: 15px;">
                        <label class="toggle" style="margin: 0;">
                            <input type="checkbox" id="sleepModeToggle">
                            <span class="slider"></span>
                        </label>
                        <span style="color: white; font-weight: bold;">Abilita Spegnimento</span>
                    </div>
                </div>

                <button class="btn btn-success" id="dayNightSaveBtn" onclick="saveDayNightSettings()" 
                        style="margin-top: 15px;">
                    💾 Salva Impostazioni Giorno/Sera/Notte
                </button>
            </div>
            
            <div class="toggle-container" style="margin-top: 20px;">
                <label class="toggle">
                    <input type="checkbox" id="powerToggle">
                    <span class="slider"></span>
                </label>
                <span class="slider-text">Accendi/Spegni Display</span>
            </div>
            
            <div class="action-buttons">
                <button class="btn btn-primary" onclick="updateStatus()">
                    🔄 Aggiorna Stato
                </button>
                <button class="btn btn-warning" onclick="resetWiFi()">
                    🔌 Reset WiFi
                </button>
                <button class="btn btn-danger" onclick="openFactoryResetModal()">
                    🏭 Reset di Fabbrica
                </button>
                <button class="btn btn-primary" onclick="rebootDevice()">
                    🔄 Riavvia Dispositivo
                </button>
            </div>
        </div>

        <div class="control-section">
            <div style="display: flex; justify-content: space-between; align-items: center; margin-bottom: 20px;">
                <h2 style="margin: 0;">📝 Testo Scorrevole <span class="save-hint">premi Salva per memorizzare</span></h2>
                <label class="toggle">
                <input type="checkbox" id="scrollEnabledToggle" onchange="handleScrollEnabledChange()">
                <span class="slider"></span>
                </label>
            </div>
            <div style="margin-bottom: 15px;">
                <input type="text" id="scrollTextInput" placeholder="Inserisci testo da scorrere..." maxlength="200"
                style="width:100%;padding:12px;font-size:1rem;border-radius:8px;border:1px solid rgba(255,255,255,0.3);
                background:rgba(255,255,255,0.1);color:white;" oninput="updateCharCount()">
                <div style="display:flex;justify-content:space-between;font-size:0.8rem;margin-top:4px;">
                    <span style="color:#888;">Totale stimato: <span id="totalCharCount" style="color:#4CAF50;">0</span> caratteri</span>
                    <span><span id="charCount" style="color:#888;">0</span><span style="color:#888;">/200</span></span>
                </div>
            </div>
            
            <div style="display:flex;gap:15px;align-items:center;margin-bottom:15px;flex-wrap:wrap;">
                <label style="color:#ccc;">Testo scorrevole ogni (sec):</label>
                <input type="number" id="scrollPause" min="5" max="60" value="5"
                    style="width:60px;padding:5px;border-radius:4px;border:none;">
                <label style="color:#ccc;display:flex;align-items:center;gap:8px;cursor:pointer;margin-left:10px;">
                    <input type="checkbox" id="scrollShowPreset" onchange="saveScrollShowPreset()" checked>
                    <span>🕐 Mostra oraquadra quando non si vede il testo scorrevole</span>
                </label>
             </div>
            
            <div style="margin-bottom:15px;">
                <label style="color:#ccc;display:block;margin-bottom:8px;">⚡ Velocità scorrimento: <span id="scrollSpeedValue" style="color:#4CAF50;font-weight:bold;">50</span> ms</label>
                <input type="range" id="scrollSpeedSlider" min="10" max="500" value="50" step="5"
                    style="width:100%;cursor:pointer;" onchange="saveScrollSpeed()" oninput="updateScrollSpeedLabel(this.value)">
                <div style="display:flex;justify-content:space-between;color:#888;font-size:0.8rem;">
                    <span>🐇 Veloce (10ms)</span><span>Normale (50ms)</span><span>🐢 Lento (500ms)</span>
                </div>
            </div>
            
            <div style="margin-bottom:15px;">
                <label style="color:#ccc;display:block;margin-bottom:8px;">Dimensione Font: <span id="fontSizeLabel">Medio</span></label>
                <input type="range" id="scrollFontSize" min="1" max="3" value="2"
                    style="width:100%;cursor:pointer;" onchange="saveScrollFontSize(this.value)" oninput="updateFontSizeLabel(this.value)">
                <div style="display:flex;justify-content:space-between;color:#888;font-size:0.8rem;">
                    <span>Piccolo</span><span>Medio</span><span>Grande</span>
                </div>
            </div>

            <div style="margin-top: 15px; margin-bottom: 25px;">
                <label style="color: white; font-weight: bold;">Colore Testo Scorrevole:</label>
                <div style="display: flex; gap: 10px; margin-top: 10px; align-items: center;">
                    <input type="color" id="scrollTextColorPicker" value="#FFFFFF" style="width: 60px; height: 40px; cursor: pointer;">
                    <button onclick="applyScrollTextColor()" class="mode-btn" style="padding: 10px 20px;">Applica Colore</button>
                    <button onclick="setScrollTextRainbow()" class="mode-btn" style="padding: 10px 20px; background: linear-gradient(90deg, red, orange, yellow, green, blue, violet);">Rainbow</button>
                </div>
            </div>
            
            <div style="background:rgba(255,255,255,0.1);padding:15px;border-radius:8px;margin-bottom:15px;">
                <h4 style="color:white;margin-bottom:10px;">📋 Opzioni Visualizzazione</h4>
                <div style="display:flex;gap:20px;margin-bottom:10px;flex-wrap:wrap;">
                    <label style="color:#ccc;display:flex;align-items:center;gap:8px;cursor:pointer;">
                        <input type="checkbox" id="scrollShowText" onchange="saveScrollOptions();updateTotalCharCount()" checked>
                        <span>📝 Mostra Testo</span>
                    </label>
                    <label style="color:#ccc;display:flex;align-items:center;gap:8px;cursor:pointer;">
                        <input type="checkbox" id="scrollShowDate" onchange="saveScrollOptions();updateTotalCharCount()">
                        <span>📅 Mostra Data</span>
                    </label>
                    <label style="color:#ccc;display:flex;align-items:center;gap:8px;cursor:pointer;">
                        <input type="checkbox" id="scrollShowTime" onchange="saveScrollOptions();updateTotalCharCount()">
                        <span>🕐 Mostra Ora</span>
                    </label>
                </div>
                <div style="display:flex;gap:10px;align-items:center;margin-top:10px;">
                    <label style="color:#ccc;display:flex;align-items:center;gap:8px;cursor:pointer;">
                        <input type="checkbox" id="scrollTextualDateTime" onchange="saveScrollTextualDateTime()">
                        <span>💬 Mostra data/ora testuale</span>
                    </label>
                    <span style="color:#888;font-size:0.85rem;">(Es: "SONO LE OTTO E DODICI MINUTI")</span>
                </div>
            </div>
            
            <button class="btn btn-primary" onclick="saveScrollOptions(); saveScrollText();">Salva impostazioni Testo Scorrevole</button>
        </div>

    </div> <!-- chiusura .container: mancava, e il browser annidava notification e modale dentro il container
                il cui backdrop-filter rompe position:fixed (modale centrata sulla pagina invece che sullo schermo) -->

    <div class="notification" id="notification"></div>

    <!-- Modale di conferma generica / Generic confirmation modal -->
    <!-- Fuori da .container: il suo backdrop-filter romperebbe position:fixed (la modale resterebbe a centro pagina invece che a centro schermo) -->
    <div id="confirmModal" style="display:none;position:fixed;top:0;left:0;width:100%;height:100%;
         background:rgba(0,0,0,0.7);z-index:1000;justify-content:center;align-items:center;">
        <div style="background:#1e1e2f;border:1px solid rgba(255,255,255,0.2);border-radius:12px;
             max-width:420px;width:90%;padding:25px;text-align:center;color:white;">
            <h2 id="confirmModalTitle" style="margin-top:0;"></h2>
            <p id="confirmModalText" style="color:#ccc;line-height:1.5;white-space:pre-line;"></p>
            <div style="display:flex;gap:15px;justify-content:center;margin-top:20px;">
                <button class="btn btn-primary" onclick="closeConfirmModal()">Annulla</button>
                <button id="confirmModalOk" class="btn btn-danger" onclick="confirmModalProceed()"></button>
            </div>
        </div>
    </div>
    
    <script>
    // Modalità disponibili organizzate per categoria
    const modeCategories = [
        {
            name: "Lento",
            icon: "🐢",
            modes: [
                { id: 0, name: "Casuale" },
                { id: 3, name: "Arancione" },
                { id: 7, name: "Blu" }
            ]
        },
        {
            name: "Veloce",
            icon: "⚡",
            modes: [
                { id: 10, name: "Giallo" },
                { id: 11, name: "Bianco" },
                { id: 12, name: "Ciano" }
            ]
        },
        {
            name: "Sfumato",
            icon: "🌈",
            modes: [
                { id: 1, name: "Blu" },
                { id: 6, name: "Verde" },
                { id: 9, name: "Rosa" }
            ]
        },
        {
            name: "Matrix",
            icon: "💊",
            modes: [
                { id: 2, name: "Verde" },
                { id: 4, name: "Blu" },
                { id: 5, name: "Giallo" },
                { id: 8, name: "Ciano" }
            ]
        },
        {
            name: "Effetti Speciali",
            icon: "✨",
            modes: [
                { id: 13, name: "TRON 💠" },
                { id: 14, name: "Goccia 💧" },
                { id: 15, name: "Moto 🏍️" },
                { id: 22, name: "Laser 🔦" },
                { id: 24, name: "Black Hole 🌌" },
                { id: 26, name: "Firework 🎆" },
                { id: 27, name: "Drift 🌊" },
                { id: 30, name: "Radar 📡" },
                { id: 32, name: "Battito ❤️" },
                { id: 35, name: "Sabbia ⏳" },
                { id: 36, name: "Temporale ⛈️" },
                { id: 37, name: "Bolle 🫧" }
            ]
        },
        {
            name: "Giochi",
            icon: "🎮",
            modes: [
                { id: 16, name: "Galaga 👾" },
                { id: 17, name: "Pacman 🟡" },
                { id: 19, name: "Arkanoid 🎱" },
                { id: 33, name: "Tetris 🧱" },
                { id: 34, name: "Snake 🐍" },
                { id: 38, name: "Pong 🏓" }
            ]
        },
        {
            name: "Ambient",
            icon: "🌿",
            modes: [
                { id: 20, name: "Natale 🎄" },
                { id: 21, name: "Neve ❄️" },
                { id: 23, name: "Fuoco 🔥" },
                { id: 25, name: "Camino 🔥" },
                { id: 28, name: "Plasma 🌀" },
                { id: 29, name: "Aurora 🌌" },
                { id: 31, name: "Stellato ⭐" },
                { id: 39, name: "Lava Lamp 🌋" }
            ]
        },
        {
            name: "Altri",
            icon: "🔢",
            modes: [
                { id: 18, name: "Digitale 🕐" }
            ]
        }
    ];
    
    // Colori disponibili
    const colors = [
        { name: "Bianco", value: "FFFFFF" },
        { name: "Blu", value: "0000FF" },
        { name: "Rosso", value: "FF0000" },
        { name: "Verde", value: "00FF00" },
        { name: "Giallo", value: "FFFF00" },
        { name: "Arancione", value: "FFA500" },
        { name: "Rainbow", value: "RAINBOW", isRainbow: true }
    ];
    
    // Variabili globali
    let userIsEditingDayNight = false;
    let userIsEditingDigital = false;
    let userIsEditingScrollText = false;
    let userIsEditingScrollPause = false;
    let editingTimeout = null;
    let digitalEditingTimeout = null;
    let scrollTextEditingTimeout = null;
    let scrollPauseEditingTimeout = null;
    let brightnessUpdateTimeout = null;
    
    // Inizializzazione
    document.addEventListener('DOMContentLoaded', function() {
        console.log('Inizializzazione pagina...');
        
        initializeModeButtons();
        initializeColorButtons();
        updateStatus();
        
        // Aggiorna l'orario ogni secondo
        setInterval(updateTime, 1000);
        
        // secondi / seconds
        setInterval(updateStatusIfNotEditing, 5000);

        // Modifiche non salvate: accendi il bottone Salva della sezione giorno/sera/notte
        ['dayStartTime','nightStartTime','sleepStartTime','dayBrightnessSlider','nightBrightnessSlider','sleepModeToggle'].forEach(function(id) {
            const el = document.getElementById(id);
            if (el) {
                el.addEventListener('input', markDayNightDirty);
                el.addEventListener('change', markDayNightDirty);
            }
        });

        // Sezioni collassabili con stato ricordato nel browser
        const sections = document.querySelectorAll('.container > .control-section');
        sections.forEach(function(sec, idx) {
            const head = sec.firstElementChild;
            if (!head) return;
            head.classList.add('sec-head');
            head.addEventListener('click', function(e) {
                // Non collassare cliccando su controlli dentro la testata (es. toggle scroll)
                if (e.target.closest('label') || e.target.closest('input') || e.target.closest('button')) return;
                sec.classList.toggle('collapsed');
                const collapsed = [];
                sections.forEach(function(s, i) { if (s.classList.contains('collapsed')) collapsed.push(i); });
                try { localStorage.setItem('oq2collapsed', JSON.stringify(collapsed)); } catch (err) {}
            });
        });
        try {
            JSON.parse(localStorage.getItem('oq2collapsed') || '[]').forEach(function(i) {
                if (sections[i]) sections[i].classList.add('collapsed');
            });
        } catch (err) {}
        
        // Event listener base
        document.getElementById('blinkToggle').addEventListener('change', toggleBlink);
        document.getElementById('digitalOverlayToggle').addEventListener('change', toggleDigitalOverlay);
        document.getElementById('powerToggle').addEventListener('change', togglePower);
        
        // Event listener per orari giorno/sera/notte
        document.getElementById('dayStartTime').addEventListener('focus', startEditingDayNight);
        document.getElementById('dayStartTime').addEventListener('change', keepEditingDayNight);
        document.getElementById('dayStartTime').addEventListener('blur', scheduleEndEditing);
        
        document.getElementById('nightStartTime').addEventListener('focus', startEditingDayNight);
        document.getElementById('nightStartTime').addEventListener('change', keepEditingDayNight);
        document.getElementById('nightStartTime').addEventListener('blur', scheduleEndEditing);

        document.getElementById('sleepStartTime').addEventListener('focus', startEditingDayNight);
        document.getElementById('sleepStartTime').addEventListener('change', keepEditingDayNight);
        document.getElementById('sleepStartTime').addEventListener('blur', scheduleEndEditing);

        document.getElementById('sleepModeToggle').addEventListener('change', keepEditingDayNight);

        // Event listener per slider giorno
        document.getElementById('dayBrightnessSlider').addEventListener('mousedown', startEditingDayNight);
        document.getElementById('dayBrightnessSlider').addEventListener('touchstart', startEditingDayNight);
        document.getElementById('dayBrightnessSlider').addEventListener('input', onDayBrightnessChange);
        document.getElementById('dayBrightnessSlider').addEventListener('mouseup', scheduleEndEditing);
        document.getElementById('dayBrightnessSlider').addEventListener('touchend', scheduleEndEditing);
        
        // Event listener per slider sera
        document.getElementById('nightBrightnessSlider').addEventListener('mousedown', startEditingDayNight);
        document.getElementById('nightBrightnessSlider').addEventListener('touchstart', startEditingDayNight);
        document.getElementById('nightBrightnessSlider').addEventListener('input', onNightBrightnessChange);
        document.getElementById('nightBrightnessSlider').addEventListener('mouseup', scheduleEndEditing);
        document.getElementById('nightBrightnessSlider').addEventListener('touchend', scheduleEndEditing);

        // Event listener per campo testo scorrevole
        document.getElementById('scrollTextInput').addEventListener('focus', startEditingScrollText);
        document.getElementById('scrollTextInput').addEventListener('input', keepEditingScrollText);
        document.getElementById('scrollTextInput').addEventListener('blur', scheduleEndEditingScrollText);

        // Event listener per campo pausa
        document.getElementById('scrollPause').addEventListener('focus', startEditingScrollPause);
        document.getElementById('scrollPause').addEventListener('input', keepEditingScrollPause);
        document.getElementById('scrollPause').addEventListener('blur', scheduleEndEditingScrollPause);

        // Event listener per slider velocità
        document.getElementById('scrollSpeedSlider').addEventListener('input', updateScrollSpeedLabel);

        console.log('Inizializzazione completata');
    });
    
    // Funzioni per gestire editing
    function startEditingDayNight() {
        userIsEditingDayNight = true;
        if (editingTimeout) {
            clearTimeout(editingTimeout);
            editingTimeout = null;
        }
        console.log('Utente sta modificando - polling disabilitato');
    }
    
    function keepEditingDayNight() {
        userIsEditingDayNight = true;
        if (editingTimeout) {
            clearTimeout(editingTimeout);
        }
        editingTimeout = setTimeout(endEditingDayNight, 3000);
    }
    
    function scheduleEndEditing() {
        if (editingTimeout) {
            clearTimeout(editingTimeout);
        }
        editingTimeout = setTimeout(endEditingDayNight, 2000);
    }
    
    function endEditingDayNight() {
        userIsEditingDayNight = false;
        editingTimeout = null;
        console.log('Editing terminato - polling riabilitato');
    }

    function startEditingDigital() {
        userIsEditingDigital = true;
        if (digitalEditingTimeout) {
            clearTimeout(digitalEditingTimeout);
            digitalEditingTimeout = null;
        }
    }

    function scheduleEndEditingDigital() {
        if (digitalEditingTimeout) {
            clearTimeout(digitalEditingTimeout);
        }
        digitalEditingTimeout = setTimeout(function() {
            userIsEditingDigital = false;
            digitalEditingTimeout = null;
        }, 5000);
    }

    // Funzioni per gestire editing testo scorrevole
    function startEditingScrollText() {
        userIsEditingScrollText = true;
        if (scrollTextEditingTimeout) {
            clearTimeout(scrollTextEditingTimeout);
            scrollTextEditingTimeout = null;
        }
    }

    function keepEditingScrollText() {
        userIsEditingScrollText = true;
        if (scrollTextEditingTimeout) {
            clearTimeout(scrollTextEditingTimeout);
        }
        scrollTextEditingTimeout = setTimeout(function() {
            userIsEditingScrollText = false;
            scrollTextEditingTimeout = null;
        }, 10000);
    }

    function scheduleEndEditingScrollText() {
        if (scrollTextEditingTimeout) {
            clearTimeout(scrollTextEditingTimeout);
        }
        scrollTextEditingTimeout = setTimeout(function() {
            userIsEditingScrollText = false;
            scrollTextEditingTimeout = null;
        }, 3000);
    }

    // Funzioni per gestire editing pausa
    function startEditingScrollPause() {
        userIsEditingScrollPause = true;
        if (scrollPauseEditingTimeout) {
            clearTimeout(scrollPauseEditingTimeout);
            scrollPauseEditingTimeout = null;
        }
    }

    function keepEditingScrollPause() {
        userIsEditingScrollPause = true;
        if (scrollPauseEditingTimeout) {
            clearTimeout(scrollPauseEditingTimeout);
        }
        scrollPauseEditingTimeout = setTimeout(function() {
            userIsEditingScrollPause = false;
            scrollPauseEditingTimeout = null;
        }, 10000);
    }

    function scheduleEndEditingScrollPause() {
        if (scrollPauseEditingTimeout) {
            clearTimeout(scrollPauseEditingTimeout);
        }
        scrollPauseEditingTimeout = setTimeout(function() {
            userIsEditingScrollPause = false;
            scrollPauseEditingTimeout = null;
        }, 3000);
    }

    function updateStatusIfNotEditing() {
        if (!userIsEditingDayNight && !userIsEditingScrollText && !userIsEditingScrollPause) {
            updateStatus();
        } else {
            console.log('Aggiornamento status saltato - utente sta modificando');
        }
    }
    
    // Funzione per rilevare fascia oraria
    function isCurrentlyDayTime() {
        const now = new Date();
        const currentHour = now.getHours();
        const currentMinute = now.getMinutes();
        const currentTimeMinutes = currentHour * 60 + currentMinute;
        
        const dayStart = document.getElementById('dayStartTime').value;
        const nightStart = document.getElementById('nightStartTime').value;
        
        if (!dayStart || !nightStart) return true;
        
        const dayParts = dayStart.split(':');
        const nightParts = nightStart.split(':');
        
        const dayHour = parseInt(dayParts[0]);
        const dayMin = parseInt(dayParts[1]);
        const nightHour = parseInt(nightParts[0]);
        const nightMin = parseInt(nightParts[1]);
        
        const dayStartMinutes = dayHour * 60 + dayMin;
        const nightStartMinutes = nightHour * 60 + nightMin;
        
        if (dayStartMinutes > nightStartMinutes) {
            return !((currentTimeMinutes >= nightStartMinutes) && (currentTimeMinutes < dayStartMinutes));
        } else {
            return !((currentTimeMinutes >= nightStartMinutes) || (currentTimeMinutes < dayStartMinutes));
        }
    }
    
    // Funzione per aggiornare luminosità in tempo reale
    function updateBrightnessRealtime(brightness) {
        fetch('/setBrightness', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: 'brightness=' + brightness
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            console.log('Luminosità aggiornata in tempo reale: ' + brightness + '%');
        })
        .catch(function(error) {
            console.error('Errore aggiornamento luminosità:', error);
        });
    }
    
    // Handler slider giorno
    function onDayBrightnessChange() {
        const slider = document.getElementById('dayBrightnessSlider');
        const brightness = slider.value;
        document.getElementById('dayBrightnessValue').textContent = brightness;
        
        keepEditingDayNight();
        
        if (isCurrentlyDayTime()) {
            if (brightnessUpdateTimeout) {
                clearTimeout(brightnessUpdateTimeout);
            }
            brightnessUpdateTimeout = setTimeout(function() {
                updateBrightnessRealtime(brightness);
            }, 100);
        } else {
            console.log('Fascia SERA - nessun aggiornamento in tempo reale');
        }
    }
    
    // Handler slider sera
    function onNightBrightnessChange() {
        const slider = document.getElementById('nightBrightnessSlider');
        const brightness = slider.value;
        document.getElementById('nightBrightnessValue').textContent = brightness;
        
        keepEditingDayNight();
        
        if (!isCurrentlyDayTime()) {
            if (brightnessUpdateTimeout) {
                clearTimeout(brightnessUpdateTimeout);
            }
            brightnessUpdateTimeout = setTimeout(function() {
                updateBrightnessRealtime(brightness);
            }, 100);
        } else {
            console.log('Fascia GIORNO - nessun aggiornamento in tempo reale');
        }
    }
    
    // Descrizioni brevi degli effetti, mostrate come tooltip sui bottoni
    const modeDescriptions = {
        13: 'Moto stile TRON con scie luminose',
        14: 'Onde concentriche che rivelano le scritte',
        15: 'Moto con scia arcobaleno che colora le scritte',
        16: 'Spari dal basso che colpiscono le lettere',
        17: 'Pacman insegue i fantasmi e colora le scritte',
        18: 'Orologio digitale a cifre',
        19: 'Pallina rimbalzante che rivela le lettere',
        20: 'Colori natalizi con lucine e sparkle',
        21: 'Neve che cade e si accumula, scritte ghiacciate',
        22: 'Raggi laser dal basso rivelano le lettere',
        23: 'Fiamme animate con orario in contrasto',
        24: 'Particelle risucchiate in un vortice centrale',
        25: 'Fuoco da camino realistico stile Fire2012',
        26: 'Fuochi artificiali con particelle esplosive',
        27: 'Blob colorati che fluttuano con scie morbide',
        28: 'Onde plasma colorate in continuo movimento',
        29: 'Aurora boreale con onde verdi e viola',
        30: 'Fascio radar rotante che rivela le lettere',
        31: 'Cielo stellato, orario come costellazione',
        32: 'Cuore pulsante con onde che rivelano le lettere',
        33: 'Blocchi colorati che compongono le scritte',
        34: 'Serpente che mangia le lettere e cresce',
        35: 'Sabbia che riempie le lettere dal basso',
        36: 'Pioggia e lampi che rivelano le scritte',
        37: 'Bolle che salgono e scoppiano sulle lettere',
        38: 'Pong autonomo, la pallina rivela le lettere',
        39: 'Lava lamp con blob che si fondono e separano'
    };

    function initializeModeButtons() {
        const container = document.getElementById('modeButtons');
        container.innerHTML = '';

        modeCategories.forEach(function(category) {
            // Crea il contenitore della categoria
            const categoryDiv = document.createElement('div');
            categoryDiv.className = 'mode-category';
            categoryDiv.style.cssText = 'margin-bottom:15px;';

            // Titolo categoria
            const categoryTitle = document.createElement('div');
            categoryTitle.className = 'category-title';
            categoryTitle.style.cssText = 'color:#b8b8b8;font-size:0.85rem;margin-bottom:8px;padding-left:5px;';
            categoryTitle.textContent = category.icon + ' ' + category.name;
            categoryDiv.appendChild(categoryTitle);

            // Griglia pulsanti
            const buttonsGrid = document.createElement('div');
            buttonsGrid.style.cssText = 'display:flex;flex-wrap:wrap;gap:8px;';

            category.modes.forEach(function(mode) {
                const button = document.createElement('button');
                button.className = 'mode-btn';
                button.textContent = mode.name;
                button.dataset.mode = mode.id;
                if (modeDescriptions[mode.id]) {
                    button.title = modeDescriptions[mode.id];
                }
                button.addEventListener('click', function() {
                    setMode(mode.id);
                });
                buttonsGrid.appendChild(button);
            });

            categoryDiv.appendChild(buttonsGrid);
            container.appendChild(categoryDiv);
        });
    }
    
    function initializeColorButtons() {
        const container = document.getElementById('colorButtons');
        container.innerHTML = '';

        colors.forEach(function(color) {
            const button = document.createElement('button');
            button.className = 'color-btn';
            button.title = color.name;
            button.dataset.value = color.isRainbow ? 'rainbow' : String(color.value).toUpperCase();

            if (color.isRainbow) {
                // Pulsante Rainbow con gradiente
                button.style.background = 'linear-gradient(90deg, red, orange, yellow, green, blue, violet)';
                button.addEventListener('click', function() {
                    setRainbow();
                });
            } else {
                button.style.backgroundColor = '#' + color.value;
                button.addEventListener('click', function() {
                    setColor(color.value);
                });
            }
            container.appendChild(button);
        });
    }
    
    function markDayNightDirty() {
        document.getElementById('dayNightSaveBtn').classList.add('btn-dirty');
    }

    function updateActiveBandBadges() {
        const day = isCurrentlyDayTime();
        document.getElementById('dayActiveBadge').style.display = day ? 'inline-block' : 'none';
        document.getElementById('nightActiveBadge').style.display = day ? 'none' : 'inline-block';
    }

    function updateTime() {
        const now = new Date();
        const timeString = now.toLocaleTimeString('it-IT');
        document.getElementById('currentTime').textContent = timeString;
    }
    
    let statusFailCount = 0;

    function updateStatus() {
        fetch('/status')
            .then(function(response) {
                return response.json();
            })
            .then(function(data) {
                console.log('Status ricevuto:', data);
                if (statusFailCount >= 3) {
                    showNotification('Connessione col dispositivo ripristinata', 'success');
                }
                statusFailCount = 0;
                document.getElementById('connDot').classList.add('online');
                
                document.getElementById('currentMode').textContent = data.modeName;
                document.getElementById('currentColor').textContent = data.colorName;
                document.getElementById('currentBlink').textContent = data.blink ? 'attivo' : 'disattivo';
                document.getElementById('currentBrightness').textContent = data.brightness + '%';
                document.getElementById('currentPower').textContent = data.power ? 'acceso' : 'spento';
                if (data.scrollEnabled !== undefined) {
                    document.getElementById('currentScroll').textContent = data.scrollEnabled ? 'attivo' : 'disattivo';
                    document.getElementById('scrollEnabledToggle').checked = data.scrollEnabled == 1;
                }
                document.getElementById('blinkToggle').checked = data.blink;
                document.getElementById('powerToggle').checked = data.power;
                if (data.digitalOverlayEnabled !== undefined) {
                    document.getElementById('digitalOverlayToggle').checked = data.digitalOverlayEnabled;
                }
                if (!userIsEditingDigital) {
                    if (data.digitalInterval !== undefined) {
                        document.getElementById('digitalInterval').value = data.digitalInterval;
                    }
                    if (data.digitalDuration !== undefined) {
                        document.getElementById('digitalDuration').value = data.digitalDuration;
                    }
                }
                
                if (!userIsEditingDayNight) {
                    if (data.dayStartTime) {
                        document.getElementById('dayStartTime').value = data.dayStartTime;
                    }
                    if (data.nightStartTime) {
                        document.getElementById('nightStartTime').value = data.nightStartTime;
                    }
                    if (data.sleepStartTime) {
                        document.getElementById('sleepStartTime').value = data.sleepStartTime;
                    }
                    if (data.sleepModeEnabled !== undefined) {
                        document.getElementById('sleepModeToggle').checked = data.sleepModeEnabled;
                    }
                    if (data.dayBrightness !== undefined) {
                        document.getElementById('dayBrightnessSlider').value = data.dayBrightness;
                        document.getElementById('dayBrightnessValue').textContent = data.dayBrightness;
                    }
                    if (data.nightBrightness !== undefined) {
                        document.getElementById('nightBrightnessSlider').value = data.nightBrightness;
                        document.getElementById('nightBrightnessValue').textContent = data.nightBrightness;
                    }
                    // Campi risincronizzati dal dispositivo: niente modifiche pendenti
                    document.getElementById('dayNightSaveBtn').classList.remove('btn-dirty');
                }
                updateActiveBandBadges();
                if (data.scrollPause !== undefined && !userIsEditingScrollPause) {
                    document.getElementById('scrollPause').value = data.scrollPause;
                }
                if (data.scrollManualSpeed !== undefined) {
                    document.getElementById('scrollSpeedSlider').value = data.scrollManualSpeed;
                    document.getElementById('scrollSpeedValue').textContent = data.scrollManualSpeed;
                }
                if (data.scrollShowText !== undefined) {
                    document.getElementById('scrollShowText').checked = data.scrollShowText;
                }
                if (data.scrollShowDate !== undefined) {
                    document.getElementById('scrollShowDate').checked = data.scrollShowDate;
                }
                if (data.scrollShowTime !== undefined) {
                    document.getElementById('scrollShowTime').checked = data.scrollShowTime;
                }
                if (data.scrollShowPreset !== undefined) {
                    document.getElementById('scrollShowPreset').checked = data.scrollShowPreset;
                }
                if (data.scrollTextualDateTime !== undefined) {
                    document.getElementById('scrollTextualDateTime').checked = data.scrollTextualDateTime;
                }
                if (data.scrollFontSize !== undefined) {
                    document.getElementById('scrollFontSize').value = data.scrollFontSize;
                    updateFontSizeLabel(data.scrollFontSize);
                }
                if (data.scrollText !== undefined && data.scrollText.length > 0 && !userIsEditingScrollText) {
                    document.getElementById('scrollTextInput').value = data.scrollText;
                    updateCharCount();
                }
                updateTotalCharCount();

                document.querySelectorAll('.mode-btn').forEach(function(btn) {
                    const isActive = parseInt(btn.dataset.mode) === data.mode;
                    if (isActive) {
                        btn.classList.add('active');
                    } else {
                        btn.classList.remove('active');
                    }
                });

                // Evidenzia il colore attualmente attivo
                if (data.currentColor !== undefined) {
                    document.querySelectorAll('.color-btn').forEach(function(btn) {
                        const v = btn.dataset.value || '';
                        const sel = data.rainbowMode ? (v === 'rainbow') : (v === String(data.currentColor).toUpperCase());
                        btn.classList.toggle('selected', sel);
                    });
                }
            })
            .catch(function(error) {
                console.error('Errore nel recupero dello stato:', error);
                statusFailCount++;
                document.getElementById('connDot').classList.remove('online');
                // Notifica solo dopo 3 errori consecutivi: evita spam su rete instabile
                if (statusFailCount === 3) {
                    showNotification('Dispositivo non raggiungibile', 'error');
                }
            });
    }
    
    function setMode(modeId) {
        fetch('/setMode', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: 'mode=' + modeId
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('Modalità cambiata con successo', 'success');
            updateStatus();
        })
        .catch(function(error) {
            console.error('Errore nel cambio modalità:', error);
            showNotification('Errore nel cambio modalità', 'error');
        });
    }
    
    function setColor(colorHex) {
        fetch('/setColor', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: 'color=' + colorHex
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('Colore cambiato con successo', 'success');
            updateStatus();
        })
        .catch(function(error) {
            console.error('Errore nel cambio colore:', error);
            showNotification('Errore nel cambio colore', 'error');
        });
    }

    function setRainbow() {
        fetch('/setRainbow', {
            method: 'POST'
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('Colore Rainbow attivato', 'success');
            updateStatus();
        })
        .catch(function(error) {
            console.error('Errore attivazione Rainbow:', error);
            showNotification('Errore attivazione Rainbow', 'error');
        });
    }

    function toggleBlink() {
        const blinkState = document.getElementById('blinkToggle').checked;
        
        fetch('/setBlink', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: 'blink=' + (blinkState ? '1' : '0')
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('Lampeggio ' + (blinkState ? 'attivato' : 'disattivato'), 'success');
            updateStatus();
        })
        .catch(function(error) {
            console.error('Errore nel cambio stato lampeggio:', error);
            showNotification('Errore nel cambio stato lampeggio', 'error');
        });
    }
    
    
    function toggleDigitalOverlay() {
        const enabled = document.getElementById('digitalOverlayToggle').checked;
        fetch('/setDigitalOverlay', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: 'enable=' + (enabled ? '1' : '0')
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('Digitale automatico ' + (enabled ? 'attivato' : 'disattivato'), 'success');
        })
        .catch(function(error) {
            console.error('Errore nel cambio stato digitale automatico:', error);
            showNotification('Errore nel cambio stato digitale automatico', 'error');
        });
    }

    function saveDigitalTiming() {
        const interval = document.getElementById('digitalInterval').value;
        const duration = document.getElementById('digitalDuration').value;

        fetch('/setDigitalTiming', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: 'interval=' + interval + '&duration=' + duration
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('Tempistica digitale salvata', 'success');
        })
        .catch(function(error) {
            console.error('Errore salvataggio tempistica:', error);
            showNotification('Errore salvataggio tempistica', 'error');
        });
    }

function applyScrollTextColor() {
    const picker = document.getElementById('scrollTextColorPicker');
    const hex = picker.value.replace('#', '');
    
    fetch('/setScrollTextColor', {
        method: 'POST',
        headers: {'Content-Type': 'application/x-www-form-urlencoded'},
        body: 'color=' + hex
    })
    .then(r => r.text())
    .then(() => showNotification('Colore testo applicato: #' + hex.toUpperCase(), 'success'))
    .catch(() => showNotification('Errore applicazione colore', 'error'));
}

function setScrollTextRainbow() {
    fetch('/setScrollTextRainbow', {method: 'POST'})
    .then(r => r.text())
    .then(() => showNotification('Rainbow testo attivato', 'success'))
    .catch(() => showNotification('Errore attivazione rainbow', 'error'));
}


function saveDayNightSettings() {
        const dayStart = document.getElementById('dayStartTime').value;
        const nightStart = document.getElementById('nightStartTime').value;
        const sleepStart = document.getElementById('sleepStartTime').value;
        const dayBright = document.getElementById('dayBrightnessSlider').value;
        const nightBright = document.getElementById('nightBrightnessSlider').value;
        const sleepEnabled = document.getElementById('sleepModeToggle').checked;
        
        fetch('/setDayNightSettings', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: 'dayStartTime=' + dayStart + 
                '&nightStartTime=' + nightStart + 
                '&sleepStartTime=' + sleepStart +
                '&dayBrightness=' + dayBright + 
                '&nightBrightness=' + nightBright +
                '&sleepModeEnabled=' + (sleepEnabled ? '1' : '0')
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('Impostazioni giorno/sera/notte salvate', 'success');
            document.getElementById('dayNightSaveBtn').classList.remove('btn-dirty');
            endEditingDayNight();
            setTimeout(function() {
                updateStatus();
            }, 500);
        })
        .catch(function(error) {
            showNotification('Errore nel salvataggio', 'error');
        });
    }    
    function togglePower() {
        const powerState = document.getElementById('powerToggle').checked;
        
        fetch('/power', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: 'power=' + (powerState ? '1' : '0')
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('Display ' + (powerState ? 'acceso' : 'spento'), 'success');
            updateStatus();
        })
        .catch(function(error) {
            console.error('Errore nel cambio stato display:', error);
            showNotification('Errore nel cambio stato display', 'error');
        });
    }
    
    function resetWiFi() {
        openConfirmModal("🔌 Reset WiFi",
            "Verranno cancellate le credenziali WiFi salvate e il dispositivo si riavvierà.\n\nDovrai riconfigurare la rete tramite access point ORAQUADRA2_AP. Continuare?",
            "Sì, resetta WiFi",
            doResetWiFi);
    }

    function doResetWiFi() {
        fetch('/resetWiFi', {
            method: 'POST'
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('WiFi resettato, riconnettiti al WiFi ORAQUADRA2_AP', 'success');
            setTimeout(function() {
                window.location.reload();
            }, 3000);
        })
        .catch(function(error) {
            console.error('Errore nel reset WiFi:', error);
            showNotification('Errore nel reset WiFi', 'error');
        });
    }

    function rebootDevice() {
        openConfirmModal("🔄 Riavvia Dispositivo",
            "Il dispositivo verrà riavviato e tornerà operativo in pochi secondi. Continuare?",
            "Sì, riavvia",
            doRebootDevice);
    }

    function doRebootDevice() {
        fetch('/reset', {
            method: 'POST'
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('Dispositivo in riavvio...', 'success');
            setTimeout(function() {
                window.location.reload();
            }, 3000);
        })
        .catch(function(error) {
            console.error('Errore nel riavvio:', error);
            showNotification('Errore nel riavvio', 'error');
        });
    }
    
    // ==== Modale di conferma generica / Generic confirmation modal ====
    let confirmModalCallback = null;

    function openConfirmModal(title, message, okLabel, callback) {
        document.getElementById('confirmModalTitle').textContent = title;
        document.getElementById('confirmModalText').textContent = message;
        document.getElementById('confirmModalOk').textContent = okLabel;
        confirmModalCallback = callback;
        document.getElementById('confirmModal').style.display = 'flex';
    }

    function closeConfirmModal() {
        document.getElementById('confirmModal').style.display = 'none';
        confirmModalCallback = null;
    }

    function confirmModalProceed() {
        const cb = confirmModalCallback;
        closeConfirmModal();
        if (cb) cb();
    }

    // ==== Reset di fabbrica / Factory reset ====
    function openFactoryResetModal() {
        openConfirmModal("⚠️ Reset di Fabbrica",
            "Questa operazione cancella TUTTE le impostazioni salvate (preset, colori, luminosità, testo scorrevole, fasce orarie) e le credenziali WiFi.\n\nIl dispositivo si riavvierà e dovrai riconfigurare la rete tramite access point ORAQUADRA2_AP.\n\nOperazione non reversibile. Continuare?",
            "Sì, resetta tutto",
            confirmFactoryReset);
    }

    function confirmFactoryReset() {
        fetch('/factoryReset', {
            method: 'POST'
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('Reset di fabbrica in corso... il dispositivo si riavvia. Riconnettiti al WiFi ORAQUADRA2_AP per riconfigurarlo.', 'success');
        })
        .catch(function(error) {
            console.error('Errore nel reset di fabbrica:', error);
            showNotification('Errore nel reset di fabbrica', 'error');
        });
    }

    function showNotification(message, type) {
        const notification = document.getElementById('notification');
        notification.textContent = message;
        notification.className = 'notification ' + type + ' show';
        
        setTimeout(function() {
            notification.classList.remove('show');
        }, 3000);
    }

function saveScrollText() {
    const text = document.getElementById('scrollTextInput').value;
    
    fetch('/setScrollText', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: 'text=' + encodeURIComponent(text)
    })
    .then(r => r.text())
    .then(() => {
        showNotification('✅ Testo scorrevole salvato! Verrà mostrato quando lo scroll è attivo.', 'success');
    })
    .catch(() => showNotification('Errore salvataggio testo', 'error'));
}

function handleScrollEnabledChange() {
    const enabled = document.getElementById('scrollEnabledToggle').checked;
    fetch('/setScrollEnabled?state=' + (enabled ? '1' : '0'))
        .then(r => r.json())
        .then(data => {
            if (data.scrollEnabled) {
                showNotification('✅ Testo scorrevole ATTIVATO', 'success');
            } else {
                showNotification('⏸️ Testo scorrevole DISATTIVATO - Mostra preset corrente', 'info');
            }
        })
        .catch(e => showNotification('Errore attivazione scroll', 'error'));
}

function saveScrollPause() {
    const pause = document.getElementById('scrollPause').value;
    fetch('/setScrollPause', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: 'pause=' + pause
    })
    .then(r => r.text())
    .then(resp => {
        showNotification("Pausa salvata: " + pause + " sec", "success");
    })
    .catch(err => showNotification("Errore salvataggio pausa", "error"));
}

function saveScrollSpeed() {
    const speed = document.getElementById('scrollSpeedSlider').value;
    fetch('/setScrollSpeed', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: 'speed=' + speed
    })
    .then(r => r.text())
    .then(resp => {
        showNotification("Velocità salvata: " + speed + " ms", "success");
    })
    .catch(err => showNotification("Errore salvataggio velocità", "error"));
}

function updateScrollSpeedLabel(value) {
    const slider = document.getElementById('scrollSpeedSlider');
    const displayValue = slider.value;
    document.getElementById('scrollSpeedValue').textContent = displayValue;
}

function saveScrollOptions() {
    const showText = document.getElementById('scrollShowText').checked ? 1 : 0;
    const showDate = document.getElementById('scrollShowDate').checked ? 1 : 0;
    const showTime = document.getElementById('scrollShowTime').checked ? 1 : 0;
    const pause = document.getElementById('scrollPause').value;
    fetch('/setScrollOptions', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: `showText=${showText}&showDate=${showDate}&showTime=${showTime}&pause=${pause}`  
    })
    .then(r => r.text())
    .then(resp => {
        let msg = [];
        if (showText) msg.push("Testo");
        if (showDate) msg.push("Data");
        if (showTime) msg.push("Ora");
        showNotification(msg.length > 0 ? "Attivato: " + msg.join(", ") : "Tutto disattivato", "success");
    })
    .catch(err => showNotification("Errore salvataggio opzioni", "error"));
}

function saveScrollShowPreset() {
    const showPreset = document.getElementById('scrollShowPreset').checked ? 1 : 0;
    fetch('/setScrollShowPreset', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: 'showPreset=' + showPreset
    })
    .then(r => r.text())
    .then(resp => {
        showNotification(showPreset ? "Preset attivo durante pausa" : "Preset disattivato durante pausa", "success");
    })
    .catch(err => showNotification("Errore salvataggio", "error"));
}

function saveScrollTextualDateTime() {
    const textual = document.getElementById('scrollTextualDateTime').checked ? 1 : 0;
    fetch('/setScrollTextualDateTime', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: 'textual=' + textual
    })
    .then(r => r.text())
    .then(resp => {
        showNotification(textual ? "Data/ora testuale attivata" : "Data/ora numerica attivata", "success");
        updateTotalCharCount();  // Ricalcola stima caratteri
    })
    .catch(err => showNotification("Errore salvataggio", "error"));
}

function updateFontSizeLabel(value) {
    const labels = ['', 'Piccolo', 'Medio', 'Grande'];
    document.getElementById('fontSizeLabel').textContent = labels[value] || 'Medio';
}

function updateCharCount() {
    const input = document.getElementById('scrollTextInput');
    const count = input.value.length;
    const charCountEl = document.getElementById('charCount');
    charCountEl.textContent = count;
    if (count >= 200) {
        charCountEl.style.color = '#ff4444';
        showNotification('Hai raggiunto il limite massimo di 200 caratteri!', 'error');
    } else if (count >= 180) {
        charCountEl.style.color = '#ffaa00';
    } else {
        charCountEl.style.color = '#888';
    }
    updateTotalCharCount();
}

function updateTotalCharCount() {
    let total = 0;
    let parts = 0;
    const showText = document.getElementById('scrollShowText').checked;
    const showDate = document.getElementById('scrollShowDate').checked;
    const showTime = document.getElementById('scrollShowTime').checked;
    const textual = document.getElementById('scrollTextualDateTime').checked;
    const textLen = document.getElementById('scrollTextInput').value.length;

    // Data
    if (showDate) {
        if (textual) {
            // Es: "MERCOLEDI UNO DICEMBRE DUEMILAVENTICINQUE" = ~45 caratteri
            total += 45;
        } else {
            // Es: "MERCOLEDI 01/12/2025" = ~21 caratteri
            total += 21;
        }
        parts++;
    }
    
    // Ora
    if (showTime) {
        if (textual) {
            // Es: "SONO LE OTTO E DODICI MINUTI" = ~30 caratteri (media)
            total += 30;
        } else {
            // "HH:MM" = 5 caratteri
            total += 5;
        }
        parts++;
    }
    
    if (showText && textLen > 0) {
        total += textLen;
        parts++;
    }
    
    // Separatori " - " (3 caratteri) tra le parti
    if (parts > 1) {
        total += (parts - 1) * 3;
    }

    const totalEl = document.getElementById('totalCharCount');
    totalEl.textContent = total;
    if (total > 250) {
        totalEl.style.color = '#ff4444';
    } else if (total > 200) {
        totalEl.style.color = '#ffaa00';
    } else {
        totalEl.style.color = '#4CAF50';
    }
}

function saveScrollFontSize(size) {
    fetch('/setScrollFontSize', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: 'fontSize=' + size
    })
    .then(r => r.text())
    .then(resp => {
        const labels = ['', 'Piccolo', 'Medio', 'Grande'];
        showNotification("Dimensione font: " + labels[size], "success");
    })
    .catch(err => showNotification("Errore salvataggio dimensione font", "error"));
}

// ==== Palette Colori Avanzata ====
function applyAdvancedColor() {
    const picker = document.getElementById('advancedColorPicker');
    if (!picker) return;
    const hex = picker.value.replace('#','');
    const formData = new FormData();
    formData.append('color', hex);
    fetch('/setColor', { method:'POST', body:formData })
        .then(()=>showNotification("Colore applicato: #"+hex.toUpperCase(),"success"))
        .catch(()=>showNotification("Errore applicazione colore","error"));
}

// --- gestione toggle testo scorrevole ---
function setScrollControlsEnabled(enabled) {
  // lista dei selector dei controlli di scroll (adatta i selettori ai tuoi input reali)
  const controls = document.querySelectorAll('.scroll-control'); // aggiungi classe .scroll-control a tutti gli input del pannello scroll
  controls.forEach(c => c.disabled = !enabled);
  document.getElementById('scrollEnabledToggle').checked = enabled;
}

// invoca /status per impostare lo stato iniziale
function refreshStatus() {
  fetch('/status').then(r => r.json()).then(data => {
    // ... altri aggiornamenti status ...
    if (typeof data.scrollEnabled !== 'undefined') {
      setScrollControlsEnabled(data.scrollEnabled == 1);
    }
  });
}

// Il toggle è gestito dall'attributo onchange (handleScrollEnabledChange):
// un secondo listener qui causava una doppia richiesta a ogni cambio

// al caricamento della pagina
document.addEventListener('DOMContentLoaded', (event) => {
  // assicurati che i controlli del pannello testo abbiano la classe .scroll-control
  refreshStatus();
});


</script>


</body>
</html>
)rawliteral";
