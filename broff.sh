#!/usr/bin/env bash

# Copyright (c) 1ukidev <leo.monteiro06@live.com>. Licensed under the GNU GPL v3.0 Licence.
# See the LICENCE file in the repository root for full licence text.

printHelp() {
    echo "Uso: broff [-h/--help] ARQUIVO_ENTRADA"
    echo "Implementação de uma linguagem de marcação simples. Utiliza o groff como back-end."
    echo "Mais informações em: https://github.com/1ukidev/broff"
    echo 
    echo "Opções:"
    echo "  -h, --help       Exibe esta ajuda."
}

if [ $# -lt 1 ]; then
    echo "Erro: Nenhum arquivo de entrada fornecido."
    echo
    printHelp
    exit 1
fi

inputFileName=""
outputFileName=""

for arg in "$@"; do
    if [ "$arg" = "-h" ] || [ "$arg" = "--help" ]; then
        printHelp
        exit 0
    elif [ "${arg:0:2}" = "--" ] || [ "${arg:0:1}" = "-" ]; then
        echo "Argumento inválido: $arg"
        exit 1
    else
        inputFileName="$arg"
    fi
done

if [ -z "$inputFileName" ]; then
    echo "Erro: Nenhum arquivo de entrada fornecido."
    echo
    printHelp
    exit 1
fi

if [ ! -f "$inputFileName" ]; then
    echo "Erro: Não foi possível abrir o arquivo de entrada."
    exit 1
fi

extensionIndex=$(expr index "$inputFileName" ".b")
if [ "$extensionIndex" -ne 0 ]; then
    outputFileName="${inputFileName:0:extensionIndex-1}.ms"
else
    outputFileName="$inputFileName.ms"
fi

if ! touch "$outputFileName"; then
    echo "Erro: Não foi possível abrir o arquivo de saída."
    exit 1
fi

sedCommands=""
sedCommands+="s/^\\.cabecalho/.NH/"
sedCommands+=";s/^\\.paragrafo/.PP/"
sedCommands+=";s/^\\.titulo/.TL/"
sedCommands+=";s/^\\.autor/.AU/"
sedCommands+=";s/^\\.instituicao/.AI/"
sedCommands+=";s/^\\.resumo/.AB no/"
sedCommands+=";s/^\\.fimresumo/.AE/"
sedCommands+=";s/^\\.lista/.IP/"
sedCommands+=";s/^\\.link/.pdfhref W -D/"
sedCommands+=";s/^\\.negrito/.B/"
sedCommands+=";s/^\\.italico/.I/"
sedCommands+=";s/^\\.negrito-e-italico/.BI/"
sedCommands+=";s/^\\.sublinhado/.UL/"
sedCommands+=";s/^\\.caixa/.BX/"

sed -E "$sedCommands" "$inputFileName" > "$outputFileName"

pdfFileName=""
if [ "$extensionIndex" -ne 0 ]; then
    pdfFileName="${inputFileName:0:extensionIndex-1}.pdf"
else
    pdfFileName="$inputFileName.pdf"
fi

groff -ms -K utf8 -T pdf $outputFileName > $pdfFileName

if [ $? -ne 0 ]; then
    echo
    echo "Erro ao gerar o PDF usando o groff."
    exit 1
fi

echo "PDF gerado com sucesso: $pdfFileName"
exit 0
