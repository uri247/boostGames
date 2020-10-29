# Games with Boost

## Install on linux

### Using APT
Ubuntu has boost library in repository. However, may be outdated. Ubuntu 18.04 has version 65 (missing beast). Install
specific libraries (In this example: thread, and date_time) with the following command:

    $ apt install libboost-thrad-dev libboost-datetime-dev libboost-log-dev
    
```libboost-all-dev``` is also available. That will put libs and includes in /usr/lib and /usr/include


### Build yourself

1. Download last version of boost from web site (~/Downloads/boost_1_72_0.gz)

2. Extract it (I keep all my projects in ~/src)
    ```sh
    $ cd ~/src
    $ tar -x -f ~/Downloads/boost_1_72_0.gz
    $ cd boost_1_72_0
    ```
   
3. boostrap.sh is a utility that configure the Boost Build system (b2). First, view all the available libraries:
    ```sh
    $ ./bootstrap.sh --show-libraries
    ```
    
4. Run bootstrap. I'm using /home/uri/.local as the destination.
    ```sh
    $ ./bootstrap.sh --with-python=/usr/bin/python3.8 --prefix=/home/uri/.local --with-libraries=date_time,chrono,thread
    ```

5. Run b2:
    ```sh
    $ ./b2
    $ ./b2 install
    ```
   
5. Verify boost libraries in `~/.local/lib`, and boost includes in `~/.local/include/boost`. Within CMake, find boost
macro will search .local first.
