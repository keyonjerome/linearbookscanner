
name := linearbookscanner
source_dir := src

.PHONY: install
install:
	poetry install 
.PHONY: lint
lint:
	poetry run pylint $(source_dir)
	poetry run flake8 $(source_dir)
.PHONY: format
format:
	poetry run black $(source_dir) 