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


#### Timing

- Timing attack, differential checking
- rdtsc

#### Parsing /proc

- cmdline
- status

#### Self Debug

- create new process and tracing itself with ptrace.

#### Misc

- TLS callback to detect before main.
