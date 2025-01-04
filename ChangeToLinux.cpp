#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

void executeCommand(const std::string &command)
{
    system(command.c_str());
}

void runAsAdmin(const std::string &command)
{
    SHELLEXECUTEINFO sei = {sizeof(sei)};
    sei.lpVerb = "runas";
    sei.lpFile = "cmd.exe";
    sei.lpParameters = ("/C " + command).c_str();
    sei.nShow = SW_HIDE;

    if (!ShellExecuteEx(&sei))
    {
        DWORD dwError = GetLastError();
        if (dwError == ERROR_CANCELLED)
        {
            std::cerr << "The user refused to allow privileges elevation." << std::endl;
        }
        else
        {
            std::cerr << "Failed to execute command with elevation." << std::endl;
        }
    }
}

void insertWordAfterKeyword(const std::string &filename, const std::string &keyword, const std::string &wordToInsert)
{
    std::ifstream inputFile(filename, std::ios::binary);
    if (!inputFile)
    {
        MessageBox(NULL, "Error opening file!", "Task Failed", MB_ICONERROR | MB_OK);
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    bool wordInserted = false;
    bool keywordFound = false;

    while (std::getline(inputFile, line))
    {
        size_t pos = line.find(keyword);
        if (pos != std::string::npos && !wordInserted)
        {
            keywordFound = true;
            if (line.find(wordToInsert) == std::string::npos)
            {
                line.insert(pos + keyword.length(), wordToInsert);
                wordInserted = true;
                MessageBox(NULL, "Task completed successfully!", "Task Success", MB_ICONINFORMATION | MB_OK);
            }
            else
            {
                MessageBox(NULL, "The line already contains the word.", "No Action Needed", MB_ICONINFORMATION | MB_OK);
            }
        }
        lines.push_back(line);
    }
    inputFile.close();

    if (!keywordFound)
    {
        MessageBox(NULL, "The keyword was not found in the file.", "No Action Needed", MB_ICONINFORMATION | MB_OK);
    }

    std::ofstream outputFile(filename, std::ios::binary | std::ios::trunc);
    for (const auto &l : lines)
    {
        outputFile << l << "\n"; // Using LF instead of CRLF
    }

    // Ensure file size is exactly 1024 bytes
    outputFile.seekp(0, std::ios::end);
    size_t fileSize = outputFile.tellp();
    if (fileSize < 1024)
    {
        outputFile.seekp(1024 - 1, std::ios::beg);
        outputFile.write("", 1);
    }
    else if (fileSize > 1024)
    {
        std::ifstream infile(filename, std::ios::binary);
        std::vector<char> buffer(1024);
        infile.read(buffer.data(), 1024);
        infile.close();

        std::ofstream outfile(filename, std::ios::binary | std::ios::trunc);
        outfile.write(buffer.data(), 1024);
    }
    outputFile.close();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Create the batch script
    std::string batchScript = "(echo sel disk 0 && echo sel part 1 && echo assign letter=z) | diskpart";

    // Run the batch script with admin privileges
    runAsAdmin(batchScript);

    // Wait for 3 seconds
    Sleep(3000);

    // Perform the word insertion task
    std::string filename = "Z:\\grub\\grubenv"; // Change the path to your desired location
    std::string keyword = "next_entry=";
    std::string wordToInsert = "0";
    insertWordAfterKeyword(filename, keyword, wordToInsert);

    return 0;
}
