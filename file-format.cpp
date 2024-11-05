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

/**
 * @brief Opens a dialog for the user to select a file.
 * @return The path of the selected file or an empty string if no file was selected.
 */
std::string openFileDialog() {
    OPENFILENAME ofn;
    char szFile[260] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrTitle = "Select a file";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        return std::string(ofn.lpstrFile);
    }
    return std::string();
}

/**
 * @brief Opens a dialog for the user to select a folder.
 * @return The path of the selected folder or an empty string if no folder was selected.
 */
std::string openFolderDialog() {
    BROWSEINFO bi = {0};
    bi.lpszTitle = "Select a folder";
    
    LPITEMIDLIST pidlist = SHBrowseForFolder(&bi);
    if (pidlist != nullptr) {
        char path[MAX_PATH] = {0};
        if (SHGetPathFromIDList(pidlist, path)) {
            return std::string(path);
        }
    }
    return std::string(); 
}

/**
 * @brief Converts the input file to the specified output format using FFmpeg.
 * @param inputFilePath Path to the input file.
 * @param outputFilePath Path to save the output file.
 * @throws std::runtime_error If the conversion fails.
 */
void convertFile(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::string extension = fs::path(outputFilePath).extension().string();
    std::string command;

    if (extension == ".jpg" || extension == ".jpeg" || extension == ".png") {
        command = "ffmpeg -i \"" + inputFilePath + "\" -map_metadata -1 -metadata title=\"tysaiw\" \"" + outputFilePath + "\"";
    } else {
        command = "ffmpeg -i \"" + inputFilePath + "\" -map_metadata -1 -c:v copy -metadata title=\"tysaiw\" \"" + outputFilePath + "\"";
    }

    if (system(command.c_str()) != 0) {
        throw std::runtime_error("Error converting the file.");
    }
}

int main(int argc, char* argv[]) {
    std::string outputFormat = "mp4";
    std::string inputFilePath;
    std::string outputDir;

    // Process command-line arguments
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "--format") == 0 && (i + 1) < argc) {
            outputFormat = argv[++i]; 
        } else if (std::strcmp(argv[i], "--input") == 0 && (i + 1) < argc) {
            inputFilePath = argv[++i];
        } else if (std::strcmp(argv[i], "--output") == 0 && (i + 1) < argc) {
            outputDir = argv[++i];
        }
    }

    try {
        // If no input file provided via command line, open file dialog
        if (inputFilePath.empty()) {
            inputFilePath = openFileDialog();
            if (inputFilePath.empty()) {
                throw std::runtime_error("No file selected.");
            }
        }

        // If no output directory provided via command line, open folder dialog
        if (outputDir.empty()) {
            outputDir = openFolderDialog();
            if (outputDir.empty()) {
                throw std::runtime_error("No folder selected.");
            }
        }

        fs::path outputFilePath = fs::path(outputDir) / (fs::path(inputFilePath).stem().string() + "." + outputFormat);

        convertFile(inputFilePath, outputFilePath.string());
        std::cout << "File converted successfully! Saved to: " << outputFilePath << std::endl;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
