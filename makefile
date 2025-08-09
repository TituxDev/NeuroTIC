.PHONY: create compile install

REPO_NAME = NeuroTIC
REPO_URL = https://github.com/TituxDev/$(REPO_NAME).git

create:
	@bash scripts/new.sh


compile:
	@if [ -z "$(PROJECT_LOCATION)" ]; then \
		read -p "Enter project location (e.g., workspace, examples): " ploc; \
		export PROJECT_LOCATION=$$ploc; \
	else \
		export PROJECT_LOCATION=$(PROJECT_LOCATION); \
	fi; \
	if [ -z "$(PROJECT_NAME)" ]; then \
		read -p "Enter project name (filename without extension): " pname; \
		export PROJECT_NAME=$$pname; \
	else \
		export PROJECT_NAME=$(PROJECT_NAME); \
	fi; \
	if [ -z "$(PLATAFORM)" ]; then \
		read -p "Enter platform (e.g., CPU): " plat; \
		export PLATFORM=$$plat; \
	else \
		export PLATAFORM=$(PLATAFORM); \
	fi; \
	echo "Compiling project in location: $$PROJECT_LOCATION"; \
	echo "Project name: $$PROJECT_NAME"; \
	echo "Platform: $$PLATAFORM"; \
	./scripts/compile.sh "$$PROJECT_LOCATION" "$$PROJECT_NAME" "$$PLATAFORM"

install:
	@printf "NeuroTIC DOWNLOAD PROCESS STARTED...\n"; \
	printf "Checking if Git is installed: "; \
	if command -v git > /dev/null; then \
		printf "\033[032mGit found.\033[0m\n"; \
	else \
		printf "\033[031mGit not found\033[0m\n"; \
		printf "Do you want to install Git? (y/n): "; \
		read OPT; \
		if [ "$$OPT" = "y" ] || [ "$$OPT" = "Y" ]; then \
			printf "Installing Git...\n"; \
			sudo apt-get update && sudo apt-get install git -y; \
			printf "\033[032mGit installed successfully.\033[0m\n"; \
		else \
			printf "\033[031mGit is required to clone the repository. Exiting...\033[0m\n"; \
			exit 1; \
		fi \
	fi
	@printf "Checking if GCC is installed: "; \
	if command -v gcc > /dev/null; then \
		printf "\033[032mGCC found.\033[0m\n"; \
	else \
		printf "\033[031mGCC not found\033[0m\n"; \
		printf "Do you want to install GCC? (y/n): "; \
		read OPT; \
		if [ "$$OPT" = "y" ] || [ "$$OPT" = "Y" ]; then \
			printf "Installing GCC...\n"; \
			sudo apt-get update && sudo apt-get install build-essential -y; \
			printf "\033[032mGCC installed successfully.\033[0m\n"; \
		else \
			printf "\033[031mGCC is required to compile the project. Exiting...\033[0m\n"; \
			exit 1; \
		fi \
	fi
	@ORIGINAL_LOCATION=$$PWD; \
	LOCATION="$$PWD"; \
	printf "Current location: \033[32m$$LOCATION\033[0m\n"; \
	NEW_LOCATION=""; \
	ABORT=false; \
	while true; do \
		printf "Do you want to change the location? (y/n): "; \
		read OPT; \
		if [ "$$OPT" = "y" ] || [ "$$OPT" = "Y" ]; then \
			printf "Enter the new location: "; \
			read LOCATION; \
			TEMP_LOCATION=$$LOCATION; \
			while [ ! -d "$$TEMP_LOCATION" ]; do \
				NEW_LOCATION="$$TEMP_LOCATION"; \
				TEMP_LOCATION=$$(dirname "$$TEMP_LOCATION"); \
			done; \
			if [ ! -d "$$LOCATION" ]; then \
				mkdir -p "$$LOCATION"; \
			fi; \
			cd "$$LOCATION"; \
			LOCATION=$$PWD; \
			printf "Location changed to \033[32m$$LOCATION\033[0m\n"; \
		else \
			if [ $$ABORT = true ]; then \
				cd "$$ORIGINAL_LOCATION"; \
				if [ -n "$$NEW_LOCATION" ] && [ -d "$$NEW_LOCATION" ]; then \
					printf "\033[31mRemoving temporary directory: $$NEW_LOCATION\033[0m\n"; \
					rm -rf "$$NEW_LOCATION"; \
				fi; \
				printf "\033[31mAborting installation.\033[0m\n"; \
				exit 1; \
			fi \
		fi; \
		ABORT=true; \
		CHECK_PATH="$$LOCATION"; \
		while [ "$$CHECK_PATH" != "/" ]; do \
			if [ -d "$$CHECK_PATH/.git" ]; then \
				printf "\033[31mInvalid location: '$$LOCATION' is inside an existing Git repository.\033[0m\n"; \
				OPT="y"; \
				continue 2; \
			fi; \
			CHECK_PATH=$$(dirname "$$CHECK_PATH"); \
		done; \
		if [ -d $(REPO_NAME) ]; then \
			printf "\033[31mThere is an existing $(REPO_NAME) \033[0m\n"; \
			printf "Do you want to remove it? (y/n): "; \
			read OPT; \
			if [ "$$OPT" != "y" ] && [ "$$OPT" != "Y" ]; then \
				OPT="y"; \
				continue; \
			fi; \
			rm -rf $(REPO_NAME); \
		fi; \
		break; \
	done; \
	printf "Cloning the repository from $(REPO_URL)...\n"; \
	git clone $(REPO_URL); \
	if [ $$? -ne 0 ]; then \
		printf "\033[031mFailed to clone the repository.\033[0m\n"; \
		exit 1; \
	fi; \
	printf "\033[032mRepository cloned successfully.\033[0m\n"