#!/bin/bash

# Run natively
python3 -m jupyterlab

# Run the jupyter/datascience-notebook docker image.
# docker run -it --rm -p 8888:8888 jupyter/datascience-notebook jupyter lab

