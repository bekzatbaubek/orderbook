#!/bin/sh

mkdir ./build

clang++ -std=c++17 -g -o ./build/orderbook orderbook.cpp
