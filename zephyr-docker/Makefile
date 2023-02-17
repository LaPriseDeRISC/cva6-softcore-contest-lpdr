name="zephyr_build"
tag="zephyr-build:v1"
GID := $(shell id -g)
UID := $(shell id -u)
container_id=$(shell docker ps -aqf "name=$(name)")
tag_exists=$(shell docker images -q $(tag) 2> /dev/null) 
detach_keys="ctrl-d" # ctrl-d to detach from container, prevents accidental exit
# the following is for running as root in container
cont_user=user

# ANSI escape codes
RED=\033[0;31m
GREEN=\033[0;32m
YELLOW=\033[0;33m
BLUE=\033[0;34m
PURPLE=\033[0;35m
CYAN=\033[0;36m
BRWHITE=\033[0;97m
BGCYAN=\033[46m
NC=\033[0m

default: fattach

help:
	@printf "Usage:\
	\n\tmake\t\t\tForce attach to running docker image by running it first and building it if needed same as fattach\
	\n\tmake fattach\t\tForce attach to running docker image by running it first and building it if needed\
	\n\tmake attach\t\tAttach to running docker image\
	\n\tmake frun\t\tForce run docker image by building it if needed\
	\n\tmake run\t\tRun docker image\
	\n\tmake build\t\tBuild docker image\n"


build:
	@echo "Building $(UID) $(GID)"
	docker build -f Dockerfile --build-arg UID=$(UID) --build-arg GID=$(GID) -t zephyr-build:v1 .

frun:
ifeq ($(tag_exists),"" )
	@echo "$(YELLOW)Image not found, building it: $(NC)"
	make build
endif
	docker run -d -it --rm --user $(cont_user) --name $(name) --privileged -v $(PWD)/workspace:/workdir $(tag)
# container_id:=$(docker ps -l -q)

run:
ifeq ($(tag_exists),"" )
	@echo "$(RED)Image not found, run $(NC)$(BGCYAN) make build $(NC)$(RED) first or make frun to build on run if needed$(NC)"
else
ifneq ($(container_id),)
	@echo "$(YELLOW)Warning, container seems to be already running, run $(NC)$(BGCYAN) make stop $(NC)$(YELLOW) to stop it$(NC)"
endif
	docker run -d -it --rm  --user $(cont_user) --name $(name) --privileged -v $(PWD)/workspace:/workdir $(tag) 
endif
# container_id:=$(docker ps -l -q)
attach:
ifeq ($(container_id),)
	@echo "$(YELLOW)Container seems to be not running, run $(NC)$(BGCYAN) make run $(NC)$(YELLOW) to run it if it fails to attach$(NC)"
else
	@echo "$(GREEN)Attaching to $(container_id) $(NC)"
endif
#	-docker attach --detach-keys=$(detach_keys) --sig-proxy=false $(container_id) 
	-docker exec -it $(container_id) /bin/bash -c "cd /workdir && /bin/bash" 

fattach:
# container_id=$(docker ps -aqf "name=$(tag)")

ifeq ($(container_id),)
	@echo "$(YELLOW)Container not running, running it: $(NC)"
	make frun
# else
# # @echo "Container running, attaching $(shell docker ps -aqf "name=$(name)")"
# 	@echo "$(GREEN)Container running, attaching $(shell docker ps -aqf "name=$(name)") $(NC)"
endif
# clear
	make attach

stop:
	@echo "Stopping $(container_id)"
	docker rm -f $(shell docker ps -aqf "name=$(name)")
