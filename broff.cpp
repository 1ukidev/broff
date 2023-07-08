// Copyright (c) 1ukidev <leo.monteiro06@live.com>. Licensed under the GNU GPL v3.0 Licence.
// See the LICENCE file in the repository root for full licence text.

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

void printHelp() {
    std::cout << "Uso: broff [-h/--help] ARQUIVO_ENTRADA\n";
    std::cout << "Implementação de uma linguagem de marcação simples. Utiliza o groff como back-end. (WIP)\n";
    std::cout << "Mais informações em: https://github.com/1ukidev/broff\n\n";
    std::cout << "Opções:\n";
    std::cout << "  -h, --help       Exibe esta ajuda.\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Erro: Nenhum arquivo de entrada fornecido.\n\n";
        printHelp();
        return 1;
    }

    std::string inputFileName;
    std::string outputFileName;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printHelp();
            return 0;
        } else if (arg.substr(0, 2) == "--" || arg.substr(0, 1) == "-") {
            std::cerr << "Argumento inválido: " << arg << "\n";
            return 1;
        } else {
            inputFileName = arg;
        }
    }

    if (inputFileName.empty()) {
        std::cerr << "Erro: Nenhum arquivo de entrada fornecido.\n\n";
        printHelp();
        return 1;
    }

    std::ifstream inputFile(inputFileName);
    if (!inputFile) {
        std::cerr << "Erro: Não foi possível abrir o arquivo de entrada.\n";
        return 1;
    }

    const size_t extensionIndex = inputFileName.rfind(".b");
    if (extensionIndex != std::string::npos) {
        outputFileName = inputFileName.substr(0, extensionIndex) + ".ms";
    } else {
        outputFileName = inputFileName + ".ms";
    }

    std::ofstream outputFile(outputFileName);
    if (!outputFile) {
        std::cerr << "Erro: Não foi possível abrir o arquivo de saída.\n";
        return 1;
    }

    const std::unordered_map<std::string, std::string> commands {
        { ".cabecalho", ".NH" },
        { ".paragrafo", ".PP" },
        { ".titulo", ".TL" },
        { ".autor", ".AU" },
        { ".instituicao", ".AI" },
        { ".resumo", ".AB no" },
        { ".fimresumo", ".AE" },
        { ".lista", ".IP" },
        { ".link", ".pdfhref W -D" },
        { ".negrito", ".B" },
        { ".italico", ".I" },
        { ".negrito-e-italico", ".BI" },
        { ".sublinhado", ".UL" },
        { ".caixa", ".BX" }
    };

    std::string line;
    while (std::getline(inputFile, line)) {
        if (!line.empty() && line.find(".") == 0) {
            const std::string command = line.substr(0, line.find(' '));
            if (commands.count(command) > 0) {
                line.replace(0, command.length(), commands.at(command));
            }
        }
        outputFile << line << "\n";
    }

    inputFile.close();
    outputFile.close();

    std::string pdfFileName;
    if (extensionIndex != std::string::npos) {
        pdfFileName = inputFileName.substr(0, extensionIndex) + ".pdf";
    } else {
        pdfFileName = inputFileName + ".pdf";
    }

    const std::string groffCommand = "groff -ms -K utf8 -T pdf " + outputFileName + " > " + pdfFileName;
    const int groffResult = system(groffCommand.c_str());

    if (groffResult != 0) {
        std::cerr << "\nErro ao gerar o PDF usando o groff.\n";
        return 1;
    }

    std::cout << "PDF gerado com sucesso: " << pdfFileName << "\n";

    return 0;
}