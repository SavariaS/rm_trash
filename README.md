# rm-trash
**rm-trash** is an executable program for Linux that moves files to a trash can in accordance to the [FreeDesktop.org Trash specification](https://specifications.freedesktop.org/trash-spec/trashspec-1.0.html).  The options are the same as GNU's [rm](https://man7.org/linux/man-pages/man1/rm.1.html) for familiarity of use.

The man page in markdown can be found [here](https://github.com/SavariaS/rm_trash/blob/master/MAN.md).

## Installation

### From debian package
Download the .deb package from [Releases](https://github.com/SavariaS/rm_trash/releases).
```bash
# Install 
sudo apt install ./rm-trash_X.X.deb

# Uninstall
sudo apt remove rm-trash
```

### From source
```bash
# Install
make
sudo make install

# Uninstall
sudo make uninstall
```

## Generating man pages
```bash
cd doc
./generate.sh
```