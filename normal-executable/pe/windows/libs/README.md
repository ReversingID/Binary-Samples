# Binary Samples

Open repository of binary sample collections for binary analysis.

## Windows Libraries

- **ntoskrnl.exe**: kernel
- **kernel32.dll**: 
- **ntdll.dll**:
- **user32.dll**:

## Windows Versions

- **nt-3.1**: Windows NT 3.1 (3.10.5098.1) for mips and x86.
- **nt-3.5**: Windows NT 3.5 (3.50.800.1) for alpha, mips, and x86.
- **nt-3.51**: Windows NT 3.51 (3.51.1057.1) for alpha, powerpc, mips, and x86. 
- **nt-4.0**: Windows NT 4.0 Enterprise Server (4.00.1381.1) for alpha and x86.
- **server-2000**: Windows Server 2000 (5.0.2195) for x86
- **server-2012r2**: Windows Server 2012 R2 for x64
- **95**: Windows 95 OSR2 for x86.
- **98**: Windows 98 SE for x86.
- **me**: Windows ME (4.90.3000) for x86
- **xp**: Windows XP SP 4 (build 2600) for x86.
- **7**: Windows 7 SP 1 (6.1 build 7601) for x64.
- **10**: Windows 10 (10.0.17134) for x64. 

## API Family

List of API family that provided (might vary for different Windows version).

- **Cc**: File system cache.
- **Csr**: communicate with the Win32 subsystem process (csrss.exe - client/server runtime subsystem).
- **Dbg**: debugging aid functions, ex: software breakpoint.
- **Ex**: windows executive, outer layer of _ntoskrnl.exe_.
- **Exp**: windows executive privates, routines within executive layer that are not exported for call outside of executive.
- **FsRtl**: Filesystem runtime library.
- **Io**: I/O manager.
- **Ke**: core kernel routines.
- **Ki**: internal routines that are not exported for call from outside the kernel.
- **Ks**: kernel streaming.
- **Ldr**: loader function for PE file handling.
- **Lpc**: Local Procedure Call. Undocumented, interprocess or user/kernel passing mechanism.
- **Lsa**: Local Security Authority.
- **Mi**: memory management routines not exported for call outside the memory manager.
- **Nls**: Native Language Support, similar to code pages.
- **Ob**: object manager.
- **Pfx**: prefix handling.
- **Po**: plug-and-play and power management.
- **Ps**: process and thread management.
- **Rtl**: Run-Time Library. Includes utility functions that can be used by native applications yet don't directly involve kernel support.
- **Se**: security.
- **Vf**: driver verifier.
- **Vi**: driver verifier routines not exported outside the driver verifier.
- **Zw**: system calls.