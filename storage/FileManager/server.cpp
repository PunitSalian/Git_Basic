#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <zlib.h>
#include <vector>

#define CHUNK 1024

// Function to decompress the zlib-compressed file
bool decompressFile(const std::string& inputFile) {
    // Open the input file
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening input file: " << inputFile << std::endl;
        return false;
    }

    // Initialize zlib stream structure
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    if (inflateInit(&strm) != Z_OK) {
        std::cerr << "zlib initialization failed!" << std::endl;
        return false;
    }

    std::vector<unsigned char> inBuffer(CHUNK);
    std::vector<unsigned char> outBuffer(CHUNK);

    // Decompress until the end of the input file
    int ret;
    do {
        // Read data into the input buffer
        inFile.read(reinterpret_cast<char*>(inBuffer.data()), CHUNK);
        strm.avail_in = inFile.gcount();
        strm.next_in = inBuffer.data();

        if (strm.avail_in == 0) break;
        std::string blob_data = "";
        uint64_t content_length = 0;
        // Decompress data
        do {
            strm.avail_out = CHUNK;
            strm.next_out = outBuffer.data();
            ret = inflate(&strm, Z_NO_FLUSH);

            if (ret == Z_ERRNO) {
                std::cerr << "Error during decompression!" << std::endl;
                inflateEnd(&strm);
                return false;
            }

            // Write decompressed data to the output file
            size_t have = CHUNK - strm.avail_out;
            if (blob_data == "") {
                blob_data = reinterpret_cast<char*>(outBuffer.data());
                //std::cout << blob_data;
                std::size_t pos = blob_data.find(' ');

                // Check if space is found
                if (pos != std::string::npos) {
                    // Extract the substring after the space
                    std::string number_str = blob_data.substr(pos + 1);

                    // Convert the extracted substring to an integer
                    content_length = std::stoi(number_str);
                } else {
                    std::cerr << "No space found in the input string." << std::endl;
                }
            
            }
            //std::cout << blob_data.size() << std::endl;
            for (int i = blob_data.size()+1; i < outBuffer.size() && content_length > 0; i++,content_length--)
                std::cout << outBuffer[i];

            blob_data = "";

        } while (strm.avail_out == 0);
    } while (ret != Z_STREAM_END);

    // Clean up and check for errors
    if (ret != Z_STREAM_END) {
        std::cerr << "Error: stream ended unexpectedly." << std::endl;
        inflateEnd(&strm);
        return false;
    }

    inflateEnd(&strm);
    return true;
}

int main(int argc, char *argv[])
{
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;
    
    if (argc < 2) {
        std::cerr << "No command provided.\n";
        return EXIT_FAILURE;
    }
    
    std::string command = argv[1];
    std::string flag;
    std::string data;
    if (argc > 2) {
       flag = argv[2];
    } 
    if (argc > 3) {
       data = argv[3];
    }
    if (command == "init") {
        std::cerr << "git init\n";
        try {
            std::filesystem::create_directory(".git");
            std::filesystem::create_directory(".git/objects");
            std::filesystem::create_directory(".git/refs");
    
            std::ofstream headFile(".git/HEAD");
            if (headFile.is_open()) {
                headFile << "ref: refs/heads/main\n";
                headFile.close();
            } else {
                std::cerr << "Failed to create .git/HEAD file.\n";
                return EXIT_FAILURE;
            }
    
            std::cout << "Initialized git directory\n";
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << e.what() << '\n';
            return EXIT_FAILURE;
        }
    } else if (command == "cat-file" && flag == "-p") {
        std::string obj_dir =  data.substr(0, 2);
        std::string zlib_file =  ".git/objects/" + obj_dir + "/" + data.substr(2);

        if (!decompressFile(zlib_file)) {
            std::cerr << "decompress zlib_file failed" << zlib_file << std::endl;
        }
    }

    
    return EXIT_SUCCESS;
}