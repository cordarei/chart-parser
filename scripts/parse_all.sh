#!/bin/bash

for sample in "$@"
do
    ./parser <$sample
done
