# Binary Samples

Open repository of binary sample collections for binary analysis.

## NT Loader 

NTLDR - NT Loader.

The bootloader of Windows NT operating system. Used by windows OS up to Windows XP and Windows Server 2003.

#### Disk & Master Boot Record

When computer boots, BIOS transfer control to the first boot device (in this case disk). The first sector is called Master Boot Record (MBR) which has 512 bytes in size. MBR contains small piece of code (446 bytes) called _primary boot loader_ and the _partition table_ (64 bytes) describing primary and extended partitions.

By default MBR code looks for the partition marked as active, loads its boot sector into memory and passes control to it.

NTLDR replace the default MBR with its own code.

#### Boot Sequence

NTLDR requires two files in active boot partition:

- ntldr: the main bootloader.
- NTDETECT.COM: detect hardware information.

NTLDR works in stages with following sequence happen in booting process:

- **Stage 1**: located in MBR and points to _Stage 2_.

- **Stage 2**: NTLDR switch to protected mode, search for hibernation file (_hiberfil.sys_) if necessary. If booting normally it will read configuration file (_boot.ini_). It then use ntdetect.com to identifies hardware information before load the kernel.