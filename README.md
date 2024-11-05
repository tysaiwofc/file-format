# File Converter with FFmpeg

This is a C++ program that uses FFmpeg to convert video and image files. It allows conversion to different formats and removal of metadata.

## Features

- Select input files through a file selection dialog.
- Choose an output directory.
- Convert video and image files to the desired format.
- Remove metadata from images and videos (except for codec and duration for videos).
- Add a custom metadata (`title`).

## Requirements

- **C++ Compiler**: Ensure you have a C++ compiler installed, such as g++ or Visual Studio.
- **FFmpeg**: The program depends on FFmpeg for conversion. Make sure FFmpeg is installed and accessible from the command line (or add the path of the executable to your `PATH`).

## Compilation

To compile the program, use the following command (replace `source_file.cpp` with the name of your source file):

```bash
g++ -o cdc source_file.cpp -lcomdlg32 -lshell32
```

Or, if you are using Visual Studio, simply create a new project and add the source code to the project.

## Usage

You can run the program as follows:

1. Via Command Line:

```bash
./file-format [--format <format>] [--input <file path>] [--output <directory path>]
```

--format <format>: Specifies the output format (default: mp4).
--input <file path>: Specifies the input file path.
--output <directory path>: Specifies the output directory.

If the input file and output directory are not provided, the program will open file and folder selection dialogs.

2. Example: 

To convert a video file to MP4 format, you can use:
```bash
./file-format --input "path/to/your/video.avi" --output "path/to/directory/"
```

## Notes

The program removes all metadata from images (JPG, JPEG, PNG) and only retains codec and duration metadata for videos.
A custom metadata will be added to all converted files.

## Contributions
Feel free to submit pull requests or report issues.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.