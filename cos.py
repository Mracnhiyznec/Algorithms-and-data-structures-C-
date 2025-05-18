import pydub                  # для чтения MP3
import numpy as np           # для работы с массивами
import matplotlib.pyplot as plt
from scipy.signal.windows import hann
from collections import Counter

NOTE_NAMES = [
    "ДО", "ДО диез", "РЕ", "РЕ диез", "МИ", "ФА",
    "ФА диез", "СОЛЬ", "СОЛЬ диез", "ЛЯ", "ЛЯ диез", "СИ"
]
OCTAVE_NAMES = {
    0: "субконтроктава", 1: "контроктава", 2: "большая октава",
    3: "малая октава", 4: "первая октава", 5: "вторая октава",
    6: "третья октава", 7: "четвёртая октава", 8: "пятая октава"
}

def freq_to_note(freq):
    if freq <= 0:
        return None
    midi = int(round(69 + 12 * np.log2(freq / 440.0)))
    idx  = midi % 12
    octv = midi // 12 - 1
    if octv < 0 or octv > 8:
        return None
    return f"{NOTE_NAMES[idx]} ({OCTAVE_NAMES[octv]})"

def read(f, normalized=True):
    a = pydub.AudioSegment.from_mp3(f)
    y = np.array(a.get_array_of_samples(), dtype=np.int16)
    if a.channels == 2:
        y = y.reshape(-1, 2).mean(axis=1)
    y = y.astype(np.float32)
    if normalized:
        y /= 2**15
    return a.frame_rate, y

# --- основная часть ---
audio_file = '5.mp3'
sr, x = read(audio_file)

frame_size = 8192
hop_size   = 1024
window     = hann(frame_size)
freqs      = np.fft.rfftfreq(frame_size, d=1/sr)

# Для сбора статистики (если надо)
notes = []
# Печатаем сразу заголовок
print("Время (с)\tНота")
for start in range(0, len(x)-frame_size, hop_size):
    t = start / sr
    frame = x[start:start+frame_size] * window
    spectrum = np.abs(np.fft.rfft(frame))
    peak_idx = np.argmax(spectrum)
    if spectrum[peak_idx] < 0.01:
        note = "-"        # тишина/шум
    else:
        peak_freq = freqs[peak_idx]
        note = freq_to_note(peak_freq) or "-"
        notes.append(note)
    # Печать: время с тремя знаками после запятой и название ноты
    print(f"{t:.3f}\t\t{note}")

# Если хочется статистику после:
counts = Counter(notes)
print("\nСтатистика по нотам:")
for note, cnt in counts.most_common():
    print(f"  {note}: {cnt} раз")
