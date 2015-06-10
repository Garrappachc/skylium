# How to build Skylium #

In a terminal, type:
```
svn checkout http://skylium.googlecode.com/svn/trunk/ skylium
```

Ok, you've just downloaded sources into folder named "skylium". Go to this folder and compile:
```
cd skylium
make
```

Some lines of compilation. You're good! Now, from the root account, type (to install the lib):
```
make install
```

Ok, the Skylium framework is installed on your system.
If you wish to see a sample program, type:
```
make sample
./skylium
```

You can remove compiled files (.o) by typing:
```
make clean
```


To remove Skylium form your computer, type:
```
make uninstall
```