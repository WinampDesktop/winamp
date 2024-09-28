
# Winamp
| **#**  | **irl wlw kissing** |
|----|-------|
| **1**  | ![Image 1](https://github.com/user-attachments/assets/7ae51b2d-47e1-4ccc-8a10-fba74c0c6128) |
| **2**  | ![Image 2](https://github.com/user-attachments/assets/a8740ce8-4083-4ef0-a85d-21b0cf693746) |
| **3**  | ![Image 3](https://github.com/user-attachments/assets/d6f7a1b9-c662-482f-b478-da142b1b4a8e) |
| **4**  | ![Image 4](https://github.com/user-attachments/assets/e1251975-fbd5-42a7-b272-75ac7fc1895d) |
| **5**  | ![Image 5](https://github.com/user-attachments/assets/a1c2e88f-b318-4056-812b-edc153552b5c) |

| **#**  | **yuri kissing** |
|----|-------|
| **1**  | ![Image 1](https://github.com/user-attachments/assets/4d5e8c17-346f-4fd9-9bcc-01354a32fe7d) |
| **2**  | ![Image 2](https://github.com/user-attachments/assets/163753bb-4a6c-45a5-8032-84a8bd639838) |
| **3**  | ![Image 3](https://github.com/user-attachments/assets/91d88fc1-3dd7-46fe-908e-26c3298dab2f) |


Viewed
Original file line number 	Original file line 	Diff line number 	Diff line change
@@ -1,84 +1,10 @@
# Shikanoko Nokonoko Koshitantan
My Deer Friend Nokotan (Japanese: しかのこのこのここしたんたん, Hepburn: Shikanoko Nokonoko Koshitantan, lit. 'deer child, brazen, waiting for a chance') is a Japanese manga series written and illustrated by Oshioshio. It began serialization in Kodansha's shōnen manga magazine Shōnen Magazine Edge in November 2019. After the magazine's discontinuation in October 2023, it was transferred to Kodansha's Magazine Pocket manga website in December the same year. An anime television series adaptation produced by Wit Studio aired from July to September 2024. 


# Winamp
![image](https://github.com/user-attachments/assets/b3041bf6-b5b8-49ea-aa85-eb69abdf6506)


## About
![image](https://github.com/user-attachments/assets/40472456-5404-4038-b88d-b7474840252b)


Winamp is a multimedia player launched in 1997, iconic for its flexibility and wide compatibility with audio formats. Originally developed by Nullsoft, it gained massive popularity with still millions of users. Its development slowed down, but now, its source code was opened to the community, allowing developers to improve and modernize the player to meet current user needs.
![image](https://github.com/user-attachments/assets/0cad91a4-a0b8-471d-aef0-a5ecd3c555f7)


It really whips the llama's ass.
![image](https://github.com/user-attachments/assets/0b35e148-b7ca-4f34-813d-98469d5ca9c3)

## Usage

## About

Winamp is a multimedia player launched in 1997, iconic for its flexibility and wide compatibility with audio formats. Originally developed by Nullsoft, it gained massive popularity with still millions of users. Its development slowed down, but now, its source code was opened to the community, allowing developers to improve and modernize the player to meet current user needs.

It really whips the llama's ass.

## Usage

Building of the Winamp desktop client is currently based around Visual Studio 2019 (VS2019) and Intel IPP libs (You need to use exactly v6.1.1.035). There are different options of how to build Winamp:

1. Use the `build_winampAll_2019.cmd` script file that makes 4 versions x86/x64 (Debug and Release). In this case, Visual Studio IDE is not required.
2. Use the `winampAll_2019.sln` file to build and debug in Visual Studio IDE.

### Dependencies

#### libdiscid

We take libdiscid from https://github.com/metabrainz/libdiscid/tree/v0.6.2, copy it in /Src/external_dependencies/libdiscid-0.6.2/

#### libvpx

We take libvpx from [https://github.com/ShiftMediaProject/libvpx](https://github.com/ShiftMediaProject/libvpx), modify it, and pack it to archive.
Run `unpack_libvpx_v1.8.2_msvc16.cmd` to unpack.

#### libmpg123

We take libmpg123 from [https://www.mpg123.de/download.shtml](https://www.mpg123.de/download.shtml), modify it, and pack it to archive.
Run `unpack_libmpg123.cmd` to unpack and process the DLLs.

#### OpenSSL

You need to use `openssl-1.0.1u`. For that, you need to build a static version of these libs.
Run `build_vs_2019_openssl_x86.cmd` and `build_vs_2019_openssl_64.cmd`.

To build OpenSSL, you need to install:

- 7-Zip ([https://www.7-zip.org/](https://www.7-zip.org/)) – Licensed under the GNU LGPL.
- NASM ([https://www.nasm.us/](https://www.nasm.us/)) – Licensed under the 2-Clause BSD License.
- Perl ([https://www.perl.org/](https://www.perl.org/)) – Licensed under the Artistic License or GPL.

#### DirectX 9 SDK

We take DirectX 9 SDK (June 2010) from Microsoft, modify it, and pack it to archive.
Run `unpack_microsoft_directx_sdk_2010.cmd` to unpack it.

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

We take Intel IPP 6.1.1.035, modify it, and pack it to archive.

Run `unpack_intel_ipp_6.1.1.035.cmd` to unpack it.

### Build Tools

Several external build tools are required to build Winamp. These tools are not bundled directly into the repository to comply with their respective licenses. You will need to download them separately from the following links:

- **7-Zip Portable**: Download from [https://www.7-zip.org/](https://www.7-zip.org/)  
  License: GNU LGPL

- **Git**: Download from [https://git-scm.com/download/win](https://git-scm.com/download/win)  
  License: GNU GPL v2

- **TortoiseSVN**: Download from [https://tortoisesvn.net/downloads.html](https://tortoisesvn.net/downloads.html)  
  License: GNU GPL v2

Make sure to install these tools as part of your build environment. You may need to modify the build scripts to reflect the correct paths to these tools on your system.
