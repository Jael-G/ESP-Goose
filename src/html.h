#include <Arduino.h>

const char mainPage[] PROGMEM = R"rawliteral(
  <!DOCTYPE html>
  <html lang="en">

  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP-DUCKY</title>
    <style>
      body {
        background-color: #121212;
        color: #e0e0e0;
        font-family: Arial, sans-serif;
        margin: 0;
        padding: 2rem 1rem;
        text-align: center;
      }

      h1 {
        margin-bottom: 0.25rem;
        font-size: 2rem;
        color: #ffffff;
      }

      p {
        margin-top: 0;
        margin-bottom: 2rem;
        color: #b0b0b0;
        font-size: 1rem;
      }

      .section {
        margin-bottom: 2.5rem;
        max-width: 700px;
        margin-left: auto;
        margin-right: auto;
        text-align: left;
      }

      .section h2 {
        font-size: 1.3rem;
        color: #ffffff;
        margin-bottom: 0.25rem;
        border-bottom: 1px solid #2a2a2a;
        padding-bottom: 0.25rem;
      }

      .section p {
        font-size: 0.9rem;
        color: #9a9a9a;
        margin: 0 0 1rem;
      }

      textarea {
        width: 100%;
        min-height: 220px;
        background: #1b1b1b;
        color: #ffffff;
        border: 1px solid #2a2a2a;
        border-radius: 6px;
        padding: 1rem;
        resize: vertical;
        font-size: 1rem;
        margin-bottom: 1rem;
      }

      button,
      input[type="file"] {
        background: #2979ff;
        color: white;
        border: none;
        border-radius: 6px;
        padding: 0.75rem 1.5rem;
        font-size: 1rem;
        cursor: pointer;
        transition: background 0.3s;
      }

      button:hover,
      input[type="file"]:hover {
        background: #1c54b2;
      }

      input[type="file"] {
        padding: 0.5rem;
        background: #2a2a2a;
        border: 1px solid #3a3a3a;
        color: #e0e0e0;
        cursor: pointer;
      }

      /* Control Mouse Pad */
      #touchpad {
        background: #1e1e1e;
        border: 1px solid #2a2a2a;
        border-radius: 8px;
        width: 100%;
        height: 300px;
        touch-action: none;
        /* Prevent scrolling on touch */
        display: flex;
        align-items: center;
        justify-content: center;
        color: #b0b0b0;
        font-size: 1rem;
        user-select: none;
        position: relative;
      }

      #dxdyDisplay {
        position: absolute;
        bottom: 10px;
        right: 10px;
        background: rgba(40, 40, 40, 0.7);
        padding: 0.25rem 0.5rem;
        border-radius: 4px;
        font-size: 0.9rem;
      }

      /* Crosshair */
      #crosshair-vertical,
      #crosshair-horizontal {
        position: absolute;
        background: #2979ff;
        /* blue lines */
        pointer-events: none;
        /* do not block input */
      }

      #crosshair-vertical {
        width: 2px;
        height: 100%;
      }

      #crosshair-horizontal {
        height: 2px;
        width: 100%;
      }
    </style>
  </head>

  <body>
    <h1>ESP-DUCKY 🦆</h1>
    <p>Control your ESP32 Rubber Ducky clone with simple script editing, loading, and mouse control.</p>

    <!-- Script Editor -->
    <div class="section">
      <h2>✍️ Script Editor</h2>
      <p>Review, write, and edit your DuckyScript here before sending it to the ESP32 for execution.</p>
      <textarea id="scriptInput" placeholder="Write or load your script here..."></textarea>
      <br>
      <button onclick="sendScript()">🚀 Send Script</button>
      <div id="scriptMessage" style="
      top: 5px; 
      right: 10px; 
      color: #4caf50; 
      font-weight: bold; 
      display: none;">
      </div>
    </div>

    <!-- Script Loader -->
    <div class="section">
      <h2>📂 Script Loader</h2>
      <p>Load a premade script from a <code>.ed</code> or <code>.txt</code> file and insert it directly into the editor
        above.</p>
      <input type="file" id="fileInput" accept=".ed,.txt">
    </div>

    <!-- Mouse Controller -->
    <div class="section">
      <h2>🖱️ Mouse Controller</h2>
      <p>Click and hold with your mouse, or touch and hold on mobile, to move the mouse. The dx/dy values show movement
        and are sent to your ESP32 while dragging.</p>
      <div id="touchpad">
        <div id="crosshair-vertical"></div>
        <div id="crosshair-horizontal"></div>
        <div id="dxdyDisplay">dx: 0, dy: 0</div>
      </div>
      <div style="margin-top: 1rem; text-align: center;">
        <button onclick="mouseClick('left')" style="margin-right: 1rem;">Left Click</button>
        <button onclick="mouseClick('right')">Right Click</button>
      </div>
    </div>

    <script>
      // Script sending
      async function sendScript() {
        let script = document.getElementById("scriptInput").value;

        // Normalize line endings to \n
        script = script.replace(/\r\n/g, "\n").replace(/\r/g, "\n");

        const lines = script.split("\n").length;

        const formData = new URLSearchParams();
        formData.append("script", script);
        formData.append("lineCount", lines);

        const messageEl = document.getElementById("scriptMessage");

        try {
          const response = await fetch("/uploadScript", {
            method: "POST",
            headers: { "Content-Type": "application/x-www-form-urlencoded" },
            body: formData.toString()
          });

          if (response.ok) {
            showMessage("✅ Script sent successfully!", "green");
          } else {
            showMessage("❌ Failed to send script.", "red");
          }
        } catch (error) {
          showMessage("⚠️ Error connecting to ESP32.", "orange");
        }

        function showMessage(text, color) {
          messageEl.textContent = text;
          messageEl.style.color = color;
          messageEl.style.display = "block";
          setTimeout(() => {
            messageEl.style.display = "none";
          }, 5000); // disappear after 5 seconds
        }
      }
      // Script loader
      document.getElementById("fileInput").addEventListener("change", function (event) {
        const file = event.target.files[0];
        if (!file) return;
        const name = file.name;
        const ext = name.split('.').pop().toLowerCase();
        if (ext !== 'ed' && ext !== 'txt') {
          alert("⚠️ Please upload a .ed or .txt script file");
          return;
        }
        const reader = new FileReader();
        reader.onload = function (e) {
          document.getElementById("scriptInput").value = e.target.result.split(/\\r?\\n/).join("\\n");
        };
        reader.readAsText(file);
      });

      // Mouse Controller
      const touchpad = document.getElementById("touchpad");
      const dxdyDisplay = document.getElementById("dxdyDisplay");
      const crossV = document.getElementById("crosshair-vertical");
      const crossH = document.getElementById("crosshair-horizontal");

      let lastX = null, lastY = null;
      let dragging = false;

      function sendMouse(dx, dy) {
        const formData = new URLSearchParams();
        formData.append("dx", dx);
        formData.append("dy", dy);

        fetch("/controlMouse", {
          method: "POST",
          headers: { "Content-Type": "application/x-www-form-urlencoded" },
          body: formData.toString()
        });
      }

      function mouseClick(button) {
        let url = button === 'left' ? '/mouseLeftClick' : '/mouseRightClick';

        fetch(url, {
          method: 'POST',
          headers: { 'Content-Type': 'application/x-www-form-urlencoded' }
        }).catch(err => {
          console.warn('Error sending click:', err);
        });
      }



      function startDrag(x, y) {
        lastX = x;
        lastY = y;
        dragging = true;
        updateCrosshair(x, y);
      }

      function moveDrag(x, y) {
        if (!dragging) return;
        const dx = x - lastX;
        const dy = y - lastY;
        lastX = x;
        lastY = y;
        dxdyDisplay.textContent = `dx: ${dx}, dy: ${dy}`;
        sendMouse(dx, dy);

        // Keep crosshair inside box
        const rect = touchpad.getBoundingClientRect();
        const crossX = Math.max(0, Math.min(x, rect.width));
        const crossY = Math.max(0, Math.min(y, rect.height));
        updateCrosshair(crossX, crossY);
      }

      function endDrag() {
        dragging = false;
        dxdyDisplay.textContent = `dx: 0, dy: 0`;
      }

      function updateCrosshair(x, y) {
        crossV.style.left = x + "px";
        crossH.style.top = y + "px";
      }

      // Mouse events
      touchpad.addEventListener("mousedown", e => startDrag(e.offsetX, e.offsetY));
      document.addEventListener("mousemove", e => {
        if (!dragging) return;
        const rect = touchpad.getBoundingClientRect();
        const x = e.clientX - rect.left;
        const y = e.clientY - rect.top;
        moveDrag(x, y);
      });
      document.addEventListener("mouseup", endDrag);

      // Touch events
      touchpad.addEventListener("touchstart", e => {
        const t = e.touches[0];
        const rect = touchpad.getBoundingClientRect();
        startDrag(t.clientX - rect.left, t.clientY - rect.top);
      });
      touchpad.addEventListener("touchmove", e => {
        e.preventDefault();
        const t = e.touches[0];
        const rect = touchpad.getBoundingClientRect();
        moveDrag(t.clientX - rect.left, t.clientY - rect.top);
      }, { passive: false });
      touchpad.addEventListener("touchend", endDrag);
    </script>
  </body>

  </html>
)rawliteral";
