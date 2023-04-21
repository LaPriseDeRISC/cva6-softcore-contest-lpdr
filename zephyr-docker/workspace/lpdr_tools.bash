#!/bin/bash


# if clean is passed as argument, remove the zephyr directory
if [ "$1" = "--clean" ]; then
	echo -e "- Cleaning zephyr directory"
	rm -rf .west tools zephyr modules bootloader
	echo -e "Cleaning zephyr directory done\n"

    echo -e "- Cleaning picolibc directory"
    # rm all except zephyr_build.sh 
    cd /workdir/picolibc/zephyr_build
    ./zephyr_build.bash clean

fi

# # if --path or -p is passed as argument, set the path to the zephyr directory
# if [ "$1" = "--path" -o "$1" = "-p" ]; then
#     ZEPHYR_PATH="/opt/toolchains/zephyr-sdk-0.15.1/riscv64-zephyr-elf/bin"
#     #if the directory does not exist, exit
#     if [ ! -d "$ZEPHYR_PATH" ]; then
#         echo -e "The directory $ZEPHYR_PATH does not exist !"
#         exit 1
#     fi

#     # echo -e "- Setting zephyr path to $ZEPHYR_PATH"
#     # PATH=$PATH:$ZEPHYR_PATH
#     echo "export PATH=$PATH:$ZEPHYR_PATH"
#     # echo -e "Setting zephyr path done"
#     exit 0
# fi

# if 

if [ "$1" = "--cs" -o "$1" = "--cheat" -o "$1" = "--cheat-sheet" ]; then
    echo -e "Cheat sheet :"
    echo -e "west build -p -b qemu_riscv32 /workdir/zephyr/samples/hello_world" 
    echo -e "west build -p -b cv32a6_zybo /workdir/ripe"
    echo -e "west build -t run"
	echo -e "libc location : /opt/toolchains/zephyr-sdk-0.15.1/riscv64-zephyr-elf/riscv64-zephyr-elf/lib/rv32imac_zicsr_zifencei/ilp32/libc_nano.a"
	echo -e "./configure --enable-newlib-reent-small --disable-newlib-fvwrite-in-streamio --disable-newlib-fseek-optimization --disable-newlib-wide-orient --enable-newlib-nano-malloc --disable-newlib-unbuf-stream-opt --enable-lite-exit --enable-newlib-global-atexit --enable-newlib-nano-formatted-io --disable-nls --target=riscv64-zephyr-elf"
    exit 0
fi

# if --setup or -s is passed as argument, setup the zephyr directory
if [ "$1" = "--zephyr-setup" -o "$1" = "-z" ]; then
	echo -e "- Setting up zephyr directory"
	NOT_LOGGED_STR="You are not logged"
	echo -e "- Checking if logged in to github"
	GH_STATUS=`gh auth status 2>&1`

	if [[ "$GH_STATUS" == *"$NOT_LOGGED_STR"* ]]; then
	echo -e "Not logged in to github"
	gh auth login
	else
	echo -e "Already logged in to github"
	fi

	echo -e "- west init :"
	west init -m https://github.com/LaPriseDeRISC/riscv-zephyr --mr main

	echo -e "- west update :"
	west update
	echo -e "Setting up zephyr directory done\n"
	exit 0
fi

# if --picolibc-setup or -p is passed as argument, setup the picolibc directory
if [ "$1" = "--picolibc-setup" -o "$1" = "-p" ]; then
    echo -e "- Setting up picolibc directory"
    NOT_LOGGED_STR="You are not logged"
    echo -e "- Checking if logged in to github"
    GH_STATUS=`gh auth status 2>&1`

    if [[ "$GH_STATUS" == *"$NOT_LOGGED_STR"* ]]; then
    echo -e "Not logged in to github"
    gh auth login
    else
    echo -e "Already logged in to github"
    fi

    echo -e "- picolibc init :"
    cd /workdir
    git clone https://github.com/LaPriseDeRISC/picolibc-lpdr.git picolibc
    cd picolibc/zephyr_build
    bash ./zephyr_build.bash configure
    echo -e "Setting up picolibc directory done\n"
    exit 0
fi

# if --picolibc-all or -pa is passed as argument, build and patch picolibc
if [ "$1" = "--picolibc-all" -o "$1" = "-pa" ]; then
    echo -e "- Building picolibc"
    cd /workdir/picolibc/zephyr_build
    bash ./zephyr_build.bash build
    echo -e "Building picolibc done\n"

    echo -e "- Patching picolibc"
    cd /workdir/picolibc/zephyr_build
    bash ./zephyr_build.bash patch
    echo -e "Patching picolibc done\n"
    exit 0
fi


# if [ "$1" = "--help" ]; then
echo -e "Usage: init.bash [OPTION]"
echo -e "Options:"
echo -e "   --help -h\tDisplay this help and exit"
echo -e "   --clean\tRemove the zephyr directory"
# echo -e "   --path -p\tSet the path to the zephyr directory"
echo -e "   --cheat -cs\tDisplay the cheat sheet"
echo -e "   --zephyr-setup -z\tSetup the zephyr directory"
echo -e "   --picolibc-setup -p\tSetup the picolibc directory"
echo -e "   --picolibc-all -pa\tBuild and patch picolibc"


exit 0
# fi