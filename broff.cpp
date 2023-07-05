// Copyright (c) 1ukidev <leo.monteiro06@live.com>. Licensed under the GNU GPL v3.0 Licence.
// See the LICENCE file in the repository root for full licence text.

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

void
printHelp()
{
    using std::cout;

    cout << "Uso: broff [-h/--help] ARQUIVO_ENTRADA\n";
    cout << "Implementação de uma linguagem de marcação simples. Utiliza o groff como back-end. (WIP)\n";
    cout << "Mais informações em: https://github.com/1ukidev/broff\n\n";
    cout << "Opções:\n";
    cout << "  -h, --help       Exibe esta ajuda.\n";
}

int 
main(int argc, char* argv[])
{
    using namespace std;

    if (argc < 2) {
        cout << "Erro: Nenhum arquivo de entrada fornecido.\n\n";
        printHelp();
        return 1;
    }

    string inputFileName;
    string outputFileName;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-h" || arg ==  "--help") {
            printHelp();
            return 0;
        } else {
            inputFileName = arg;
        }
    }

    if (inputFileName.empty()) {
        cout << "Erro: Nenhum arquivo de entrada fornecido.\n\n";
        printHelp();
        return 1;
    }

    ifstream inputFile(inputFileName);
    if (!inputFile) {
        cout << "Erro: Não foi possível abrir o arquivo de entrada.\n";
        return 1;
    }

    size_t extensionIndex = inputFileName.rfind(".b");
    if (extensionIndex != string::npos) {
        outputFileName = inputFileName.substr(0, extensionIndex) + ".ms";
    } else {
        outputFileName = inputFileName + ".ms";
    }

    ofstream outputFile(outputFileName);
    if (!outputFile) {
        cout << "Erro: Não foi possível abrir o arquivo de saída.\n";
        return 1;
    }

    unordered_map<string, string> commands {
        { ".paragrafo", ".PP" },
        { ".negrito", ".B" }
    };

    string line;
    while (getline(inputFile, line)) {
        if (line.find(".") == 0) {
            string command = line.substr(0, line.find(' '));
            if (commands.count(command) > 0) {
                line.replace(0, command.length(), commands[command]);
            }
        }
        outputFile << line << "\n";
    }

    inputFile.close();
    outputFile.close();

    string pdfFileName;
    if (extensionIndex != string::npos) {
        pdfFileName = inputFileName.substr(0, extensionIndex) + ".pdf";
    } else {
        pdfFileName = inputFileName + ".pdf";
    }

    const string groffCommand = "groff -ms -K utf8 -T pdf " + outputFileName + " > " + pdfFileName;
    const int groffResult = system(groffCommand.c_str());

    if (groffResult != 0) {
        cout << "\nErro ao gerar o PDF usando o groff.\n";
        return 1;
    }

    cout << "PDF gerado com sucesso: " << pdfFileName << "\n";

    return 0;
}