#!/bin/bash

PY="python3"
PIP="pip3"
VENV_DIR="venv"
ACTIVATE="source $VENV_DIR/bin/activate"
DEACTIVATE="deactivate"

# Check arguments
if [ ! -d $VENV_DIR ]; then
    # Virtual environment not initialized
    CMD="init"
elif [ $# -eq 0 ]; then
    # No arguments supplied
    CMD="run"
else
    # Argument supplied
    CMD="$1"
fi

# Clear the environment
if [ $CMD == "reset" ]; then
    rm -rf $VENV_DIR
    CMD="init"
fi

# Initialize virtual environment
if [ $CMD == "init" ]; then
    $PY -m venv $VENV_DIR
    $ACTIVATE
    $PIP install jupyter numpy scipy matplotlib pandas fortran-magic numba cython
    $DEACTIVATE
    CMD="run"
fi

# Run jupyter 
if [ $CMD == "run" ]; then
    $ACTIVATE
    $PY -m notebook --browser="chromium-browser"
    $DEACTIVATE
fi
