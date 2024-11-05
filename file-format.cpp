#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <stdexcept>
#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
#include <cstring>

namespace fs = std::filesystem;

std::string openFileDialog() {
    OPENFILENAME ofn; 
    char szFile[260];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.lpstrFilter = "Todos os arquivos (*.*)\0*.*\0";
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrTitle = "Selecione um arquivo";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        return std::string(ofn.lpstrFile);
    }
    return std::string();
}

std::string openFolderDialog() {
    BROWSEINFO bi; 
    ZeroMemory(&bi, sizeof(bi));
    bi.lpszTitle = "Selecione uma pasta";
    
    LPITEMIDLIST pidlist = SHBrowseForFolder(&bi);
    if (pidlist != nullptr) {
        char path[MAX_PATH];
        if (SHGetPathFromIDList(pidlist, path)) {
            return std::string(path);
        }
    }
    return std::string(); 
}

void convertFile(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::string extension = fs::path(outputFilePath).extension().string();
    std::string command;

    if (extension == ".jpg" || extension == ".jpeg" || extension == ".png") {
        command = "ffmpeg -i \"" + inputFilePath + "\" -map_metadata -1 -metadata title=\"tysaiw\" \"" + outputFilePath + "\"";
    } else {
        command = "ffmpeg -i \"" + inputFilePath + "\" -map_metadata -1 -c:v copy -metadata title=\"tysaiw\" \"" + outputFilePath + "\"";
    }

    int result = system(command.c_str());
    if (result != 0) {
        throw std::runtime_error("Erro ao converter o arquivo.");
    }
}

int main(int argc, char* argv[]) {
    std::string outputFormat = "mp4";
    std::string inputFilePath;
    std::string outputDir;

    // Processa argumentos
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "--format") == 0 && (i + 1) < argc) {
            outputFormat = argv[i + 1]; 
            i++;
        } else if (std::strcmp(argv[i], "--input") == 0 && (i + 1) < argc) {
            inputFilePath = argv[i + 1];
            i++;
        } else if (std::strcmp(argv[i], "--output") == 0 && (i + 1) < argc) {
            outputDir = argv[i + 1];
            i++;
        }
    }


    if (inputFilePath.empty()) {
        inputFilePath = openFileDialog();
        if (inputFilePath.empty()) {
            std::cerr << "Nenhum arquivo selecionado." << std::endl;
            return 1;
        }
    }


    if (outputDir.empty()) {
        outputDir = openFolderDialog();
        if (outputDir.empty()) {
            std::cerr << "Nenhuma pasta selecionada." << std::endl;
            return 1;
        }
    }

    fs::path outputFilePath = fs::path(outputDir) / (fs::path(inputFilePath).stem().string() + "." + outputFormat);

    try {
        convertFile(inputFilePath, outputFilePath.string());
        std::cout << "Arquivo convertido com sucesso! Salvo em: " << outputFilePath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
