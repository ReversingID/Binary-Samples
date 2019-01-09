# Binary Samples

Open repository of binary sample collections for binary analysis.

## Preliminary

Category of breakpoints:

- Software breakpoints
- Hardware breakpoints
- Memory breakpoints

## List of Anti-Debugging Techniques

#### Straight checks for debugger

- detection of 0xCC bytes
    - directly compare to 0xCC
    - comparison to XOR-ed value, ex: comparing to 0x99 (0xCC ^ 0x55)
- detection of hardware breakpoints
    - use GetThreadContext/SetThreadContext API to check for DRs
    - setup SEH, cause exception, and get direct access to debug registers in the handler as offset to context structure.
- detection of guard pages

#### API calls

- IsDebuggerPresent
- CheckRemoteDebuggerPresent
- FindWindow
- NtQueryObject
- NtQuerySystemInformation (ZwQuerySystemInformation)
- NtSetInformationThread (ZeSetInformationThread)
- NtContinue
- CloseHandle and NtClose
- GenerateConsoleCtrlEvent
- OutputDebugString

#### Interrupts

- Int 0x2C
- Int 0x2D
- Int 0x41
- Int 0xF1 (ICEBP)
- Trap Flag Check

#### Flags

- Trap flag
- IsDebugged
- NtGlobalFlag
- Heap flag

#### Timing

- Timestamp record (with GetTickCount, GetLocalTime, GetSystemTime, timeGetTime, NtQueryPerformanceCounter)
- rdtsc

#### Checksum

- calculating CRC32 (or other checksum algorithm) for certain blocks or whole binary and comparing to hardcoded values.

#### Self Debug

- create new process and call DebugActiveProcess(pid) on the parent process

#### Misc

- TLS callback to detect before main.