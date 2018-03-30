# Demo project using GNU automake tools

## Prepare
First install GCC and automake autoconf:
```
sudo apt-get install gcc
sudo apt-get install automake autoconf
```

Then generate project configure scripts by calling autoreconf: 
```
autoreconf --install --symlink
```

## Build and install
```
./configure
make
sudo make install
```
If everything is ok, binary files will be installed into "/usr/local/bin/".

## Uninstall
```
sudo make uninstall
```
