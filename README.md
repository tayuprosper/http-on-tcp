# http-on-tcp

A **simple, lightweight, Unix-based HTTP server** implemented in C++.

> MIT Licensed. Minimal dependencies. Serves static files like `index.html` directly over TCP using a tiny HTTP/1.1 implementation.

---

## Table of Contents

- [About](#about)
- [Features](#features)
- [Requirements](#requirements)
- [Quickstart](#quickstart)
- [For C++ users: Build and run](#for-c-users-build-and-run)
- [For non-C++ users: Run without compiling](#for-non-c-users-run-without-compiling)
- [Usage & examples](#usage--examples)
- [Configuration](#configuration)
- [Project structure](#project-structure)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

---

## About

**http-on-tcp** is a teaching-friendly, no-frills HTTP server that speaks HTTP over plain TCP sockets. It’s great for learning sockets, HTTP parsing, and basic web serving without heavyweight frameworks.

---

## Features

- Pure C++ (no external HTTP libraries).
- Minimal HTTP/1.1 handling (GET routing to static files).
- Serves files from the working directory (e.g., `index.html`, `notfound.html`).
- Tiny codebase that’s easy to read and extend.
- Ships with a `makefile` for one-command builds.

---

## Requirements

- Unix-like OS (Linux, macOS, WSL on Windows).
- `g++` (or another C++ compiler).
- `make`.

> Tip: On Ubuntu/Debian, install build tools with:
> ```bash
> sudo apt update && sudo apt install -y build-essential
> ```

---

## Quickstart

```bash
# 1) Clone the repo
git clone https://github.com/tayuprosper/http-on-tcp.git
cd http-on-tcp

# 2) Build
make

# 3) Run (default 8080 unless you pass a port)
./server             # listens on 8080
# or
./server 3000        # listens on 3000


# http-on-tcp (C++ usage)

A **simple, lightweight, Unix-based HTTP server** implemented in C++.

> MIT Licensed. Minimal dependencies. Serves static files like `index.html` directly over TCP using a tiny HTTP/1.1 implementation.

---

## Requirements

- Unix-like OS (Linux, macOS, or WSL on Windows).  
- `g++` (or another C++ compiler).  
- `make`.

On Ubuntu/Debian, install with:

```bash
sudo apt update && sudo apt install -y build-essential
```

---

## Build and Run (C++ users)

### 1. Clone the repository

```bash
git clone https://github.com/tayuprosper/http-on-tcp.git
cd http-on-tcp
```

### 2. Build the project

```bash
make
```

This produces a `server` binary in the project root.

### 3. Run the server

```bash
./server
```

By default, it listens on port `8080`.

You can also specify a custom port:

```bash
./server 3000
```

Now the server is available at `http://localhost:3000/`.

### 4. Test the server

Open your browser at:

```
http://localhost:8080/
```

Or use `curl`:

```bash
curl -i http://localhost:8080/
```

For a non-existent route:

```bash
curl -i http://localhost:8080/does-not-exist
```

### 5. Clean build files (optional)

```bash
make clean
```

---

## Configuration

You can customize the served content by editing the files in the repository:

- `index.html` served when a page is missing (404).  
- `messages.txt` http.cpp           # HTTP parsing/response helpers
server.cpp         # TCP socket accept loop + request handling
notfound.html      # 404 page
makefile           # build rules (produces ./server)
README.md          # this file
```

---

## License

This project is available under the **MIT License**.  
See the [LICENSE](LICENSE) file for details.