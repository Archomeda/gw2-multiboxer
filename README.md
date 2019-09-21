# Guild Wars 2 Multiboxer
[![](https://img.shields.io/github/release/Archomeda/gw2-multiboxer.svg?include_prereleases&logo=github&logoColor=f0f0f0)](https://github.com/Archomeda/gw2-multiboxer/releases/latest)
[![](https://img.shields.io/github/downloads/Archomeda/gw2-multiboxer/total.svg?logo=github&logoColor=f0f0f0)](https://github.com/Archomeda/gw2-multiboxer/releases/latest)

***Note:** This Guild Wars 2 client modification has **not** been officially approved. Use at your own risk.*

A small add-on for Guild Wars 2 that makes using multiple accounts a lot easier without having to re-enter your account information all the time.

When Guild Wars 2 starts up, it opens a file called *Local.dat*, which can be found in either *%APPDATA%\Guild Wars 2* or *<your_documents_folder>\Guild Wars 2*.
This small add-on redirects that call to another file that you can customize by using an additional command line argument.

## Usage
Copy the *version.dll* file to the root folder of your Guild Wars 2 installation (**not** in the *bin* or *bin64* folder!), e.g. *C:\Program Files\Guild Wars 2*.
Start Guild Wars 2 with `Gw2-64.exe -ldat <local_dat_file>` where `<local_dat_file>` will be the new Local.dat filename.

To make this process easier, you can create shortcuts instead.
Refer to the [wiki](https://wiki.guildwars2.com/wiki/Command_line_arguments) if you don't know how to create shortcuts with command line arguments.

Please note that adding `-ldat <local_dat_file>` to the *settings.json* file doesn't work.

You can also run multiple Guild Wars 2 instances on the same computer.
Make sure you're using a unique Local.dat file (with `-ldat`), and you're adding `-shareArchive` to the command line parameters.
Keep in mind that you cannot update the game this way, and that any changes in the Guild Wars 2 settings do not persist (game limitation).
When updating is required, make sure you run the client normally.

## Compiling
This project uses Visual Studio 2019.

All dependencies are included in the project folders.
Please make sure to `checkout` all git submodules.

## Third-party software
This project uses third-party software.
Please refer to their GitHub repositories or websites for more information.

- [minhook 1.3.3](https://github.com/TsudaKageyu/minhook)

## Disclaimer
This is an **unofficial** library.
The author of this library is not associated with ArenaNet nor with any of its partners.
Modifying Guild Wars 2 through any third party software is not supported by ArenaNet nor by any of its partners.
By using this software, you agree that it is at your own risk and that you assume all responsibility.
There is no warranty for using this software.

If you do not agree with this, please do not use this software.
