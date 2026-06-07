# ✨ Particle Network

A simple and fun particle simulation built in C++ with SDL2. Particles float around the screen and connect to each other with glowing lines when they get close enough — just a chill visual experiment.

---

## 👀 Preview

> Particles drifting across a black background, forming and breaking connections in real time.

---

## ✨ Features

- Up to **80 particles** bouncing around the screen
- Particles connect with **fading green lines** when within range
- Subtle **sine wave motion** gives each particle a smooth, organic feel
- Each particle has a limited number of connections, keeping the network clean
- Runs at a solid **~60 FPS** with SDL2 hardware acceleration

---

## 🛠️ Requirements

- C++17 or later
- [SDL2](https://www.libsdl.org/)

**Install SDL2 on Linux:**
```bash
sudo apt install libsdl2-dev
```

**Install SDL2 on Windows:**
> Download from the [SDL2 releases page](https://github.com/libsdl-org/SDL/releases) and link it to your project.

---

## 🚀 Build & Run

**Linux / macOS:**
```bash
g++ main.cpp -o particles -lSDL2 -lm
./particles
```

**Windows (MinGW):**
```bash
g++ main.cpp -o particles.exe -lSDL2 -lSDL2main -mwindows
```

---

## ⚙️ Tweaks

Want to mess around with it? These are the easiest things to change inside `main.cpp`:

| What | Where | Default |
|---|---|---|
| Max particles | `particles.size() < 80` | `80` |
| Connection range | `dist < 120` | `120px` |
| Particle speed | `1.5f + ... * 2.5f` | `1.5 – 4.0` |
| Wave intensity | `* 0.5` in the sine/cosine | `0.5` |
| Line fade strength | `50.0f * (1.0f - dist / 120.0f)` | `50 alpha` |

---

## 📄 License

MIT — built just for fun, feel free to do whatever with it.
