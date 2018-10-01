#!/bin/bash

#baixe o gcc 4.6.1 na pasta downloads

#descompactando o gcc 4.6.1
echo "#descompactando o gcc 4.6.1"
tar -xzf ~/Downloads/gcc-4.6.1.tar.gz
mv gcc-4.6.1 ~/Downloads/
echo " "

#exportando o compilador
echo "#exportando o compilador"
export PATH=$PATH:~/Downloads/gcc-4.6.1/bin
echo "$PATH"
echo " "

#verificando o compilador
echo "#verificando o compilador"
mips-elf-gcc -v
echo " "

#compilando os fontes do simulador do hf-risc
echo "#compilando os fontes do simulador do hf-risc"
cd usr/sim/hf_risc_sim
gcc -O2 hf_risc_sim.c -o hf_risc_sim
cd ../../../
echo " "

#compilando a aplicacao
echo "#compilando a aplicacao"
cd platform/single_core
make image
echo " "

#executando a aplicacao
#echo "#executando a aplicacao"
#../../usr/sim/hf_risc_sim/hf_risc_sim image.bin log.txt
#echo " "
