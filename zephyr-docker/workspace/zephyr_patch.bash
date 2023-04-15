#!/bin/bash
#if no arguments are passed or -c or --configure or -m or --make is passed, continue. Else, print error and exit
if [ "$#" -eq 0 ] || [ "$1" == "--configure" ] || [ "$1" == "-c" ] || [ "$1" == "--make" ] || [ "$1" == "-m" ]; then
        echo "Starting patch"
else
        echo "Error: invalid arguments"
        echo "Usage: ./zephyr_patch.bash [--configure|-c] [--make|-m]"
        echo "If no arguments are passed, only patching is done."
        exit 1
fi

# if --configure or -c is passed, configure newlib
if [ "$1" == "--configure" ] || [ "$1" == "-c" ]; then
        echo "Configuring newlib"
        # try configure, if failed print error and exit
        ./make clean; make distclean
        ./configure \
                --enable-newlib-reent-small \
                --disable-newlib-fvwrite-in-streamio \
                --disable-newlib-fseek-optimization \
                --disable-newlib-wide-orient \
                --enable-newlib-nano-malloc \
                --disable-newlib-unbuf-stream-opt \
                --enable-lite-exit \
                --enable-newlib-global-atexit \
                --enable-newlib-nano-formatted-io \
                --disable-nls --target=riscv64-zephyr-elf \
                || { echo "Error: configure failed"; exit 1; }

        echo "Done configuring"
        sleep 1
fi

# if --make or -m is passed, make newlib
if [ "$1" == "--make" ] || [ "$1" == "-m" ]; then
        echo "Making newlib"
        # try make, if failed print error and exit
        make || { echo "Error: make failed"; exit 1; }
        echo "Done making"
        sleep 1
fi

echo "Patching libc_nano.a 1/2"
# try cp, if failed print error and exit
cp ./newlib/libc/libc.a \
        /opt/toolchains/zephyr-sdk-0.15.1/riscv64-zephyr-elf/riscv64-zephyr-elf/lib/rv32imac_zicsr_zifencei/ilp32/libc_nano.a\
        || { echo "Error: cp failed"; } 

echo "Patching libc_nano.a 2/2"
# try cp, if failed print error and exit
cp ./newlib/libc/libc.a \
        /opt/toolchains/zephyr-sdk-0.15.1/riscv64-zephyr-elf/riscv64-zephyr-elf/lib/libc_nano.a\
        || { echo "Error: cp failed"; } 
echo "Patching done."#!/bin/bash
#if no arguments are passed or -c or --configure or -m or --make is passed, continue. Else, print error and exit
if [ "$#" -eq 0 ] || [ "$1" == "--configure" ] || [ "$1" == "-c" ] || [ "$1" == "--make" ] || [ "$1" == "-m" ]; then
        echo "Starting patch"
else
        echo "Error: invalid arguments"
        echo "Usage: ./zephyr_patch.bash [--configure|-c] [--make|-m]"
        echo "If no arguments are passed, only patching is done."
        exit 1
fi

# if --configure or -c is passed, configure newlib
if [ "$1" == "--configure" ] || [ "$1" == "-c" ]; then
        echo "Configuring newlib"
        # try configure, if failed print error and exit
        ./make clean; make distclean
        ./configure \
                --enable-newlib-reent-small \
                --disable-newlib-fvwrite-in-streamio \
                --disable-newlib-fseek-optimization \
                --disable-newlib-wide-orient \
                --enable-newlib-nano-malloc \
                --disable-newlib-unbuf-stream-opt \
                --enable-lite-exit \
                --enable-newlib-global-atexit \
                --enable-newlib-nano-formatted-io \
                --disable-nls --target=riscv64-zephyr-elf \
                || { echo "Error: configure failed"; exit 1; }

        echo "Done configuring"
        sleep 1
fi

# if --make or -m is passed, make newlib
if [ "$1" == "--make" ] || [ "$1" == "-m" ]; then
        echo "Making newlib"
        # try make, if failed print error and exit
        make || { echo "Error: make failed"; exit 1; }
        echo "Done making"
        sleep 1
fi

echo "Patching libc_nano.a 1/2"
# try cp, if failed print error and exit
cp ./newlib/libc/libc.a \
        /opt/toolchains/zephyr-sdk-0.15.1/riscv64-zephyr-elf/riscv64-zephyr-elf/lib/rv32imac_zicsr_zifencei/ilp32/libc_nano.a\
        || { echo "Error: cp failed"; } 

echo "Patching libc_nano.a 2/2"
# try cp, if failed print error and exit
cp ./newlib/libc/libc.a \
        /opt/toolchains/zephyr-sdk-0.15.1/riscv64-zephyr-elf/riscv64-zephyr-elf/lib/libc_nano.a\
        || { echo "Error: cp failed"; } 
echo "Patching done."