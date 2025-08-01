.PHONY: install

REPO_NAME = NeuroTIC
REPO_URL = https://github.com/TituxDev/$(REPO_NAME).git

install:
	@ABORT=false; \
	printf "NeuroTIC DOWNLOAD PROCESS STARTED...\n"; \
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
	fi; \
	printf "Checking if GCC is installed: "; \
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
	fi; \
	LOCATION="$$PWD"; \
	printf "Current location: \033[32m$$LOCATION\033[0m\n"; \
	while true; do \
		printf "Do you want to change the location? (y/n): "; \
		read OPT; \
		if [ "$$OPT" = "y" ] || [ "$$OPT" = "Y" ]; then \
			printf "Enter the new location: "; \
			read LOCATION; \
			if [ ! -d "$$LOCATION" ]; then \
				mkdir -p "$$LOCATION"; \
			fi; \
			cd "$$LOCATION"; \
			LOCATION=$$PWD; \
			printf "Location changed to \033[32m$$LOCATION\033[0m\n"; \
		else \
			if [ $$ABORT = true ]; then \
				printf "\033[31mAborting installation.\033[0m\n"; \
				exit 1; \
			fi; \
			ABORT=true; \
		fi; \
		if [ -d ".git" ]; then \
			printf "\033[31mThere is an existing git repository in this location.\033[0m\n"; \
			OPT="y"; \
			continue; \
		fi; \
		if [ -d $(REPO_NAME) ]; then \
			printf "\033[31mThere is an existing $(REPO_NAME) \033[0m\n"; \
			printf "Do you want to remove it? (y/n): "; \
			read OPT; \
			if [ "$$OPT" = "y" ] || [ "$$OPT" = "Y" ]; then \
				rm -rf $(REPO_NAME); \
			else \
				OPT="y"; \
				continue; \
			fi; \
		fi; \
		break; \
	done; \
	printf "Cloning the repository from $(REPO_URL)...\n"; \
	git clone $(REPO_URL); \
	printf "\033[032mRepository cloned successfully.\033[0m\n"
	@exit 0