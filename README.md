# NeonMix Academy

NeonMix Academy is a free, interactive DJ-learning app. It has no accounts,
subscriptions, payments, or server-side user data. Lesson progress is kept in
the learner's browser; audio files chosen for the workshop stay on that device.
The lightweight web server is written in C++17; lessons and audio exercises run
in the browser.
The app is free to use; the source code remains proprietary (see `LICENSE`).

## What learners can do

- Follow 20 lessons from beginner to advanced. Each lesson combines an
  explanation, a concrete example, a DJ tip, a practical challenge, and a quiz
  with immediate feedback.
- Earn XP and achievement badges by completing lessons, challenges, and the
  workshop.
- Use the tap-tempo exercise to estimate BPM by tapping along to a beat.
- Practice a guided two-track transition in the workshop, using demo audio or
  local audio files.
- Adjust BPM and the first beat, align phrases, use the crossfader and swap bass
  levels, then see a score and coaching feedback.
- Continue where they left off; progress is saved in local storage in that
  browser.

The BPM and musical-key analysis is approximate. Learners can correct values in
the workshop. Local progress does not sync across browsers or devices.

## Build on Windows

Install CMake, Visual Studio Build Tools with the C++ workload, and vcpkg. Then
configure and build from the project directory:

```powershell
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake"
cmake --build build --config Release
```

Run the app from the project directory so it can find `templates`:

```powershell
.\build\Release\neonmix.exe
```

Or double-click `start.bat`. Open http://127.0.0.1:8080. The server defaults to
localhost only; set `HOST=0.0.0.0` only when you intentionally want other
devices on the network or a tunnel to reach it.

## Configuration

| Variable | Purpose |
| --- | --- |
| `PORT` | HTTP port (default `8080`) |
| `HOST` | Bind address (default `127.0.0.1`) |
| `TEMPLATES_DIR` | Template directory (default `templates`) |

This is a learning project, not a hardened public production service. If you
expose it through a public tunnel, anyone with the link can use the Academy.
