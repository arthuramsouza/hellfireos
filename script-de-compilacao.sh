#!/bin/bash

#descompactando o gcc 4.6.1
tar -xzf gcc-4.6.1.tar.gz

#exportando o compilador
export PATH=$PATH:gcc-4.6.1/bin

#verificando o compilador
mips-elf-gcc -v

#compilando os fontes do simulador do hf-risc
cd usr/sim/hf_risc_sim
gcc -O2 hf_risc_sim.c -o hf_risc_sim
cd ../../../

#compilando a aplicacao
cd platform/single_core
make image

#executando a aplicacao
../../usr/sim/hf_risc_sim/hf_risc_sim image.bin log.txt
