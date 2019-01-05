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

#### Windows 7

Using Windows 7 SP 1 version 6.1 build 7601

- **x64-win7.bin**: Master Boot Record sector (Stage 1).
- **x64-win7-bootmgr**: Bootmgr (stage 2)

#### Windows 10

Using Windows 10 version 10.0.17134 build 17134.

- **x64-win10.bin**: Master Boot Record sector (Stage 1).
- **x64-win10-bootmgr**: Bootmgr (stage 2)
- **x64-win10-bootmgr.efi**: Bootmgr (stage 2) in EFI
- **x64-win10-memtest.efi**: 
- **x64-win10-winload**:
- **x64-win10-winload.efi**:
- **x64-win10-winresume**:
- **x64-win10-winresume.efi**: