# 🎵 ESP32 PWM Audio Player


An **ESP32 Arduino project** to play 8-bit mono audio using **PWM output**. Audio data is read from a `sound.h` file generated from an MP3 using the **MP3 → HEX Header Converter**.

---

## ⚡ Features

- Plays back 8-bit mono audio on ESP32 using PWM  
- Uses `sound.h` file generated from any MP3  
- Configurable PWM pin (default GPIO25)  
- Uses hardware timer ISR for accurate playback  
- Audio loops automatically (can be disabled)  
- Main loop free for other tasks  

---

## 📥 MP3 → HEX Header Converter

Use this Python GUI tool to generate the `sound.h` file required by this project:

[**Download MP3 → HEX Converter (Windows EXE)**](https://www.dropbox.com/scl/fi/bs5c3ucgepbh49r1aujnr/MP3_TO_HEX_V2.exe?rlkey=1tj3ru51kmptojo0gehau3a90&st=kr03bip0&dl=0)

**Conversion Details (FFmpeg used internally):**

```bash
ffmpeg -i input.mp3 -f u8 -ar 8000 -ac 1 -acodec pcm_u8 output.raw -y
````

* `-f u8` → 8-bit unsigned PCM
* `-ar 8000` → Sample rate 8 kHz
* `-ac 1` → Mono channel
* `-acodec pcm_u8` → PCM 8-bit
* `-y` → Overwrite output file

The tool generates a `sound.h` file:

```cpp
const uint8_t sound_audio[] PROGMEM = { 0x00, 0x12, 0x34, ... };
const unsigned int sound_audio_len = 12345;
```

---

## 🛠 Installation

1. **Generate `sound.h`**

   * Use the MP3 → HEX converter above to convert your MP3 file.
   * Copy the generated `sound.h` into the Arduino sketch folder:

```
ESP32_PWM_Audio_Player/
├── ESP32_PWM_Audio_Player.ino
└── sound.h
```

2. **Upload to ESP32**

   * Open `ESP32_PWM_Audio_Player.ino` in Arduino IDE
   * Select the correct ESP32 board and COM port
   * Upload the sketch

---

## 📝 ESP32 PWM Audio Player Setup

```cpp
#include "sound.h"

const int AUDIO_PIN = 25;        // GPIO25 for PWM output
const int PWM_RESOLUTION = 8;    // 8-bit
const int PWM_FREQ = 78125;      // ~78 kHz PWM frequency
const int SAMPLE_RATE = 8000;    // Must match your .raw file

volatile uint32_t audioIndex = 0;
hw_timer_t *timer = NULL;

void IRAM_ATTR onTimer() {
  if (audioIndex < sizeof(sound_audio)) {
    ledcWrite(AUDIO_PIN, sound_audio[audioIndex]);
    audioIndex++;
  } else {
    audioIndex = 0;  // Loop audio
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("🎧 ESP32 PWM Audio Player Starting...");

  ledcAttach(AUDIO_PIN, PWM_FREQ, PWM_RESOLUTION);

  timer = timerBegin(SAMPLE_RATE);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1, true, 0);  // ISR auto-reload
}

void loop() {
  delay(100); // Main loop free
}
```

**Notes:**

* Connect GPIO25 → speaker via RC low-pass filter (1kΩ + 0.1µF)
* Adjust `SAMPLE_RATE` if your MP3 → HEX conversion uses a different rate
* Audio loops automatically; remove loop in ISR to play once

---

## ⚠️ Requirements

* ESP32 development board
* Arduino IDE with ESP32 board support installed
* `sound.h` generated from MP3 file

---

## 📝 License

MIT License – Free to use, modify, and distribute.

---

## 👤 Author

**Krishna B Mali**

* GitHub: https://github.com/kbmkrishnamali1992-hub

```
