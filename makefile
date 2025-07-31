.PHONY: install

REPO_URL = https://github.com/TituxDev/NeuroTIC.git

install:
	if [ ! -v git ]; then \
		sudo apt-get install git; \
	fi
	if [ ! -d NeuroTIC ]; then \
		git clone $(REPO_URL); \
	else \
		echo "Repository already exists."; \
	fi
	if [ -d NeuroTIC ]; then \
		cd NeuroTIC && git pull; \
		echo "Repository updated."; \
	else \
		echo "Failed to clone or update the repository."; \
		exit 1; \
	fi
	rm $0