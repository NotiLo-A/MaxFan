# MaxFan

**MaxFan** is a lightweight utility for controlling the fan on ThinkPad laptops via `/proc/acpi/ibm/fan`.

It’s basically a **wrapper around cumbersome commands** like:

```bash
echo level full-speed | sudo tee /proc/acpi/ibm/fan
```

making them simpler and easier to use from a single command-line interface.

⚠️ **Note:** Works **only on ThinkPads** with `ibm-acpi` support. Requires `sudo`.

---

## Features

* Quickly set fan speed (`0-7`, `auto`, `disengaged`, `full-speed`)
* Enable or disable fan control mode
* Set watchdog timer (`0-120` seconds)
* Watch fan statistics in real time

All actions replace complex `echo` + `tee` commands with simple flags.

---

## Installation

Compile the program with:

```bash
g++ maxfan.cpp -o maxfan
```

Then run with `sudo`:

```bash
sudo ./maxfan --help
```

---

## Usage

```bash
sudo ./maxfan [OPTIONS]
```

Basic example:

```bash
sudo ./maxfan --level full-speed
sudo ./maxfan --mode disable
sudo ./maxfan --stats 2
```

Flags:

* `-h, --help` — show help
* `-s, --stats <seconds>` — update interval for stats (default: 2)
* `-l, --level <level>` — fan speed (default: auto)
* `-m, --mode <enable/disable>` — fan control mode (default: enable)
* `-w, --watchdog <value>` — watchdog timer (default: 0)

---

## Why use this?

Instead of writing awkward commands with `echo` and `tee` each time, **MaxFan gives you a simple, consistent interface** to control your ThinkPad’s fan.


*Honestly, it was easier for my lazy ass to write a program than to add a couple of arguments to a command I use like once a year.*
