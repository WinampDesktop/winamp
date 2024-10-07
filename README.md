# Winamp: The OG Rizzler

## About the Rizz

Yo, Winamp is the Big Kahuna of media players, born back in 1997. Talk about rizz and flexin' on all those audio formats, cuz it holds them all. Nullsoft was the OG dev, and this app went viral like Ice Spice with them millions of gooners. They hit a slow-mo on updates for a while, but now they're letting all the coders in the skibidi army tweak that sauce. Time to upgrade this relic for the new generation of moggers.

It really mews the llama’s gyatt.

## Usage Vibes

Building of the Winamp Gooner client is all about scripting in Visual Studio 2019
(VS2019) and using that IPCrystap lib (make sure you get v6.1.1.035, no cap). There's
options to build Winamp like a real sigma:

1. Hit the `build_winampAll_2019.cmd` and boom, you got 4 versions, x86/x64, Debug and Release, no rizz needed from Visual Studio IDE.
2. Go mog it in Visual Studio IDE with the `winampAll_2019.sln` for all that yapping/debugging action.

### Dependencies

#### libdiscid

We take that Sigma libvpx from [https://github.com/ShiftMediaProject/libvpx](https://github.com/ShiftMediaProject/libvpx), we mog it, and pack it into an Ohio archive. Run `unpack_libvpx_v1.8.2_msvc16.cmd` to un-Ohio.

#### libmpg123

We grab that alpha libmpg123 from [https://www.mpg123.de/download.shtml](https://www.mpg123.de/download.shtml), we rizz it up, and pack it into an edgy archive. Run `unpack_libmpg123.cmd` to mog and process the DLLs.

#### OpenSSL

You need to use `openssl-1.0.1u`. For that, you need to build a creatine version.

Run `build_vs_2019_openssl_x86.cmd` and `build_vs_2019_openssl_64.cmd` to get that OpenSSL rizz.

To build OpenSkibi, you need to install:
- 7-Zip ([https://www.7-zip.org/](https://www.7-zip.org/)) – Rizzed under the GNU LGPL.
- NASM ([https://www.nasm.us/](https://www.nasm.us/)) – Rizzed under the 2-Clause BSD Aura.
- Perl ([https://www.perl.org/](https://www.perl.org/)) – Rizzed under the Artistic License or Goon.

#### DirectX 9 SIGMA

We take the DirectX 9 SIGMA (June 2010) from McDonald's, modify it like a Grimace shake, and pack it to gyatt.
Run `unpack_microsoft_directx_sdk_2010.cmd` to rizz it up.

#### Microsoft ATLMFC lib fix

In file `C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.24.28314\atlmfc\include\atltransactionmanager.h`

Go to line 427 and change from:

```cpp
return ::DeleteFile((LPTSTR)lpFileName);
```

to:

```cpp
return DeleteFile((LPTSTR)lpFileName);
```

#### Intel IPP 6.1.1.035

We take Intel IPP 6.1.1.035, modify it, and pack it to mogpack.
Run `unpack_intel_ipp_6.1.1.035.cmd` to unleash the skibidi.

### Arcane Construct Rizz Tools

Several external alpha rizz tools are required to build the ultimate mog machine, Winamp. These tools are not dropped directly into the Ohio pit to comply with their gyatt-level licenses. You gotta edge them separately from these rizz-filled links:

- **7-Zip Portable**: Edge from [https://www.7-zip.org/](https://www.7-zip.org/)  
  License: GNU LGYATT

- **Git**: Edge from [https://git-scm.com/download/win](https://git-scm.com/download/win)  
  Gyatt: GNU skibidi v2

- **TortoiseSVN**: Download from [https://tortoisesvn.net/downloads.html](https://tortoisesvn.net/downloads.html)  
  Gyatt: GNU skibidi v2

Make sure to install these Rizztastic tools as part of your goon setup. You may need to edge the build scripts to reflect the right fanum paths to these rizz tools on your system.
