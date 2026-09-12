# FIRST OF ALL!
MicroOS C and MicroOS ASM is NOT the same operating systems, their philosophy is DIFFERENT. However, they are in the same family.

# MicroOS C

A small 32-bit x86 operating system written primarily in C, with a minimal assembly bootstrap.

MicroOS C is the C-based evolution of the MicroOS project, focused on learning low-level systems programming, kernel development, memory management, hardware interaction, and operating-system fundamentals.

> **Status:** Experimental / Work in Progress

## Features

* 32-bit x86 kernel
* GRUB bootloader
* Multiboot-compatible boot process
* VGA text-mode output
* Basic keyboard input
* Simple command shell
* Basic user/password handling
* Freestanding C kernel
* Custom linker script
* QEMU support
* ISO generation with GRUB

## Architecture

MicroOS C currently targets:

```text
Architecture:  i386
Mode:          32-bit protected mode
Bootloader:    GRUB
Language:      C + Assembly
Firmware:      BIOS / Legacy boot through GRUB
Display:       VGA text mode
```

## Building

### Requirements

A Linux environment with the following tools:

* GCC
* GNU Binutils
* GRUB utilities
* `grub-mkrescue`
* xorriso
* QEMU

On Debian/Ubuntu-based systems, the required packages can generally be installed with:

```bash
sudo apt install gcc gcc-multilib binutils grub-pc-bin xorriso qemu-system-x86
```

### Build

Clone the repository and run:

```bash
make
```

This builds the kernel and creates the bootable ISO.

### Run

Start MicroOS C with Make:

```bash
make run
```


## Boot Process

The boot process is handled by GRUB.

The general flow is:

```text
BIOS
  ↓
GRUB
  ↓
Multiboot entry
  ↓
boot.S
  ↓
C kernel
  ↓
MicroOS shell
```

Assembly is kept minimal where possible, while the main kernel logic is implemented in C.

## Shell

MicroOS C includes a small kernel-level shell for interacting with the system.

The shell is intentionally simple and is expected to grow as more kernel functionality is implemented.

Future shell functionality may include:

* File system
* Process management
* Memory information
* Hardware information

## Development Goals

The project is mainly focused on implementing operating-system fundamentals from the ground up.

## Design Philosophy

MicroOS C is intentionally kept small and understandable.

The project prioritizes:

* Simple code
* Explicit low-level behavior
* Minimal dependencies
* Learning over abstraction
* Understandable kernel architecture

MicroOS C is not intended to compete with Linux, BSD, or other production operating systems.

It is a personal systems-programming project designed to explore how an operating system works underneath the abstraction layers.

## Related Project

MicroOS C is part of the broader **MicroOS** project.

The original MicroOS implementation explores 16-bit x86 real-mode development, while MicroOS C moves the project toward a more traditional 32-bit protected-mode kernel architecture.

## License

See the repository license for details.

## Disclaimer

MicroOS C is experimental software.

It is not intended for production use, real hardware deployment, or security-critical workloads.

Use QEMU or another emulator for development and testing.
