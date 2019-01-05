# Binary Samples

Open repository of binary sample collections for binary analysis.

## Content

All executables will have `.macho**: ` extension. The name of each binary will follow this nomenclature:

```
$ARCH-$COMPILER.pe**: 
```

The `$ARCH` describe the family of processor or architecture. The most common one for PE are x86 and x64.

The `$COMPILER` describe the compiler used to produce the binary. Here is the list of the possible compilers:

- **clang**: LLVM Clang (part of LLVM infrastructure)
- **gcc**: GNU C Compiler (part of GNU Compiler Collection)


## List of PE Binary Sample

Thanks to [@masbog](https://github.com/masbog) for creating the binary.

Created with XCode 10.1.

#### iOS

Executables:

- **aarch64-clang.macho**: 
- **arm-arm6-clang.macho**: 
- **arm-arm7-clang.macho**: 
- **arm-arm7s-clang.macho**: 
- **arm-clang-fat.macho**: FAT binaries of 4 architectures (arm6, arm7, arm7s, aarch64)

#### Mac OS X

Executables:

- **x64-clang.macho**: 