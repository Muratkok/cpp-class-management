#ifndef CLASS_CREATE_HPP
#define CLASS_CREATE_HPP
#include <iostream>
#include <fstream>
#include <iomanip>
#include "nlohmann/json.hpp"

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

class classCreate
{
private:
    string name, lastName, className, myText, directoryPath, directoryPathInc, directoryPathSrc, fileNameInc, fileNameSrc;
    ofstream outfileInc, outfileSrc;
    ifstream infile;

public:
    classCreate(const std::string &name,
                const std::string &lastName,
                const std::string &className);
    ~classCreate();
    static std::shared_ptr<classCreate> classInit();
    bool assertClass(const std::string &str);
    void writeFile(string &fileName);
    void readFile(string &fileName);
    void writeJsonFile(string &fileName, json &j);
    void readJsonFile(string &fileName, json &j);
    void JsonFilesModify(fs::path &currentPath);
};

#endif // CLASS_CREATE_HPP