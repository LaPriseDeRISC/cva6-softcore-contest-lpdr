#!/bin/sh
# if clean is passed as argument, remove the zephyr directory
if [ "$1" = "--clean" ]; then
	echo -e "- Cleaning zephyr directory"
	rm -rf .west tools zephyr modules bootloader
	echo -e "Cleaning zephyr directory done\n"
fi

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