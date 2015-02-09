# Rshell
Rshell is a basic unix shell with the purpose of exploring the different commands and capabilities of a typical unix shell.

### 
Development is currently in progress.
Successfully prints username and hostname.

#bin/ls
A recreation of GNU ls using c++ with the full functionality of the -a -l and -R flags.

###currently known bugs:
Formatting is not uniform among different flags.
Outputs of the ls executable are sorted by first [non characters][capital letters][lowercase letters] instead of being uniformly alphabetical.
.//// R or any variant of that does not remove the excess '////' (it is a valid path) and when printed, the path name of the current directory may have excessive '/' as well.
