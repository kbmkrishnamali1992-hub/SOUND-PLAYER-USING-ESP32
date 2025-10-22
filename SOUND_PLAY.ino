/*
  🎵 ESP32 PWM Audio Player
  -----------------------------------
  Developed by: KRISHNA B MALI

  ▶ DESCRIPTION:
  This sketch plays back 8-bit mono audio on an ESP32 using the PWM output.
  It reads audio data from a "sound.h" file generated from an MP3 file.

  -----------------------------------
  🧩 HOW TO CREATE "sound.h" FILE:
  1. Use the "MP3 → HEX Header Converter" tool developed earlier (Python GUI).
     - Input:  your .MP3 file
     - Output: "sound.h" containing 8-bit unsigned PCM array (8 kHz sample rate)

  2. The tool automatically converts your MP3 to:
       → HEX format (8-bit, 8kHz, mono)
       → sound.h file with:
          const uint8_t sound_audio[] PROGMEM = { ... };
          const unsigned int sound_audio_len = XXXX;

  3. Copy the generated "sound.h" file into the same folder as this sketch:
       📁 <Arduino_Sketch_Folder>
          ├── ESP32_PWM_Audio_Player.ino
          └── sound.h

  4. Then upload this sketch to your ESP32 board.

  -----------------------------------
  🧠 NOTES:
  • GPIO25 (default) is used for PWM audio output.
  • Connect GPIO25 → small speaker via an RC low-pass filter (1kΩ + 0.1µF).
  • Use SAMPLE_RATE = 8000 (matches converter output).
  • Audio automatically loops (you can disable in ISR).
*/

#include "sound.h"  // Include generated sound.h file with audio data

// PWM Configuration
const int AUDIO_PIN = 25;        // GPIO25 (use GPIO26 for stereo channel 2)
const int PWM_RESOLUTION = 8;    // 8-bit resolution (0-255)
const int PWM_FREQ = 78125;      // PWM frequency (~78 kHz for 8-bit)

// Audio playback variables
const int SAMPLE_RATE = 8000;   // Must match your .wav/.mp3 conversion rate
volatile uint32_t audioIndex = 0;
hw_timer_t *timer = NULL;

// Timer ISR to output audio samples
void IRAM_ATTR onTimer() {
  if (audioIndex < sizeof(sound_audio)) {
    ledcWrite(AUDIO_PIN, sound_audio[audioIndex]);
    audioIndex++;
  } else {
    audioIndex = 0;  // Loop audio (remove to play once)
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("🎧 ESP32 PWM Audio Player Starting...");
  
  // Configure LEDC PWM - NEW API for Core 3.x
  ledcAttach(AUDIO_PIN, PWM_FREQ, PWM_RESOLUTION);
  
  // Configure timer for sample rate timing - NEW API for Core 3.x
  timer = timerBegin(SAMPLE_RATE);  // Frequency in Hz
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1, true, 0);  // Interrupt every tick, auto-reload, unlimited count
  
  Serial.println("✅ Audio playback started!");
}

void loop() {
  // Main loop is free for other tasks
  delay(100);
}
