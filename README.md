# simplest-fetch
The simplest fetch program you'll ever find, written in C. No seriously, this is what it looks like:
![image](https://github.com/an-prata/simplest-fetch/assets/91640048/6ff68ed2-c4e3-44e0-9837-74269bec03f5)

You can use the `-i` flag for base 1024 units (KiB, MiB, GiB, etc). By default the program uses base 1000 units like above (KB, MB, GB, etc).

## Installing
I maintain a package for `simplest-fetch` on the AUR:
```
yay -S simplest-fetch-git
```
Alternatively you can just clone and run the build script, an executable will appear in a new `build/` directory! For both of these the only platform I'm going to actively support is linux, chances are it'll work on BSD distros but I'm not sure, same with macOS.
