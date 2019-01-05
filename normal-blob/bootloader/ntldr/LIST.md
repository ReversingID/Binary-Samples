# Binary Samples

Open repository of binary sample collections for binary analysis.

## Content

All stage 1 bootloaders are taken from first sector of disk. The name of each binary will follow this nomenclature:

```
$ARCH-$WINVER.bin
$ARCH-$WINVER-$NAME
```

The `$ARCH` indicate the family processor or architecture. Eventhough the bootloaders are 16 bits application, this $ARCH indicate where the binaries are taken from.

The `$WINVER` describe the Windows OS version as the binary source. 

#### Windows NT 3.51

Using Windows NT 3.51

- **x86-nt351.bin**: Master Boot Record sector (Stage 1).
- **x86-nt351-ntldr**: NTLDR (stage 2)
- **x86-nt351-ntdetect**: NTDETECT.COM

#### Windows XP

Using Windows XP SP4 build 2600.

- **x86-winxp.bin**: Master Boot Record sector (Stage 1).
- **x86-winxp-ntldr**: NTLDR (stage 2)
- **x86-winxp-ntdetect**: NTDETECT.COM
