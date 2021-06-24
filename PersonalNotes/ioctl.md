---
layout: default
title: ioctl and system calls
---

## What is `ioctl`?
Conventional OSs divide virtual memory into two components: the userspace and the kernel. The userspace is the memory area where application software and some drivers execute while the kernel space is strictly reserved for OS kernel and most device drivers.
[Seperation of userspace and kernel](https://en.wikipedia.org/wiki/User_space#:~:text=User-,mode,-User%20applications)

Userspace applications may make system calls to the kernel to communicate with external devices and make requests that it usually wouldn't have permission to run because the request may not be accessible on the user-level. System calls essentially provide an interface between processes and the Operating System.

However, is user code aims to directly communicate with a specific device, the facilities that the device may offer might not have been forseen by the kernel hence not fully accessible. The kernel is flexible to the addition of extra modules though, hence `ioctl` takes care of this. 

`ioctl` is an interface that provides the userspace access to device drivers on the kernel level and facilities possible operations through request keys. Through a single system call, the user can make a `ioctl` call to some open file descriptor/pointer given a requst code and some data if required:

```c++
#include <sys/ioctl.h>

int ioctl(int fd, unsigned long request, ...);
```

[man page](https://man7.org/linux/man-pages/man2/ioctl.2.html)


## Uses
- Hardware device configuration: control.make requests to hardware devices.
- Terminals: The Unix command-line interface is built on pseudo terminals (ptys), which emulate hardware text terminals such as VT100s.
