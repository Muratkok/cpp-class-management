#include "classCreate.hpp"
#include <ctime>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;
using json = nlohmann::json;

bool controlJsonArg(json &args, string &stringFileName)
{
    bool exists = false;
    if (!args.is_array())
    {
        throw std::runtime_error("Inapprioparate json file.Please modify the json file");
    }

    for (const auto &arg : args)
    {
        if (arg.is_string() && arg.get<string>() == stringFileName)
        {
            exists = true;
            break;
        }
    }
    return exists;
}
bool classCreate::assertClass(const std::string &str)
{
    for (char c : str)
    {
        if (!std::isalpha(static_cast<unsigned char>(c)))
        {
            string str;
            if (this->name.empty())
            {
                cout << "Invalid Name Please Enter Your Name:" << endl;
            }
            else if (this->lastName.empty())
            {
                cout << "Invalid LastName Please Enter Your LastName:" << endl;
            }
            else
            {
                cout << "Invalid ClassName Please Enter Your ClassName:" << endl;
            }
            cin >> str;
            this->assertClass(str);
        }
    }
    return true;
}
classCreate::classCreate(const std::string &name,
                         const std::string &lastName,
                         const std::string &className)
{
    assertClass(name);
    assertClass(lastName);
    assertClass(className);
    this->name = name;
    this->lastName = lastName;
    this->className = className;
    fs::path currentPath = fs::current_path();
    this->directoryPath = currentPath.string() + string("/classes/");
    if (!fs::exists(this->directoryPath))
    {
        fs::create_directory(this->directoryPath);
    }
    this->directoryPathInc = this->directoryPath + string("inc");
    if (!fs::exists(this->directoryPathInc))
    {
        fs::create_directory(this->directoryPathInc);
    }
    this->directoryPathSrc = this->directoryPath + string("src");
    if (!fs::exists(this->directoryPathSrc))
    {
        fs::create_directory(this->directoryPathSrc);
    }
    this->fileNameInc = this->directoryPathInc + "/" + this->className + ".hpp";
    if (fs::exists(this->fileNameInc))
    {
        cerr << "File already exists!\n";
    }
    else
    {
        this->outfileInc.open(this->fileNameInc, ios::out);
        if (this->outfileInc.is_open())
        {
            time_t timestamp;
            time(&timestamp);

            this->outfileInc << "/*File is created by "
                             << name << " "
                             << lastName
                             << " at "
                             << ctime(&timestamp)
                             << "*/"
                             << endl;
            this->writeFile(this->fileNameInc);
            cout << "File created successfully\n";
        }
        else
        {
            cerr << "File could not be created\n";
        }
    }
    this->fileNameSrc = this->directoryPathSrc + "/" + this->className + ".cpp";
    if (fs::exists(this->fileNameSrc))
    {
        cerr << "File already exists!\n";
    }
    else
    {
        this->outfileSrc.open(this->fileNameSrc, ios::out);
        if (this->outfileSrc.is_open())
        {
            time_t timestamp;
            time(&timestamp);

            this->outfileSrc << "/*File is created by "
                             << name << " "
                             << lastName
                             << " at "
                             << ctime(&timestamp)
                             << "*/"
                             << endl;
            this->writeFile(this->fileNameSrc);
            cout << "File created successfully\n";
        }
        else
        {
            cerr << "File could not be created\n";
        }
        this->JsonFilesModify(currentPath);
    }
}
void classCreate::JsonFilesModify(fs::path &currentPath)
{
    json jsonValue;
    string jsonFile, fileIncNametoJsonFile, fileSrcNametoJsonFile;
    ////////////////////////
    jsonFile = currentPath.string() + "\\.vscode\\c_cpp_properties.json";
    fileIncNametoJsonFile = "${workspaceFolder}/classes/inc";
    this->readJsonFile(jsonFile, jsonValue);
    if (jsonValue.contains("configurations") && jsonValue["configurations"].is_array() && !jsonValue["configurations"].empty())
    {
        auto &includePath = jsonValue["configurations"][0];

        if (!includePath.contains("includePath") || !includePath["includePath"].is_array())
        {
            includePath["includePath"] = nlohmann::json::array();
        }

        auto &args = includePath;
        if (!controlJsonArg(args, fileIncNametoJsonFile))
            args.push_back(fileIncNametoJsonFile);
    }
    writeJsonFile(jsonFile, jsonValue);
    ////////////////////////
    jsonFile = currentPath.string() + "\\.vscode\\tasks.json";
    fileIncNametoJsonFile = "/I${workspaceFolder}\\classes\\Inc";
    fileSrcNametoJsonFile = "${workspaceFolder}\\classes\\src\\" + this->className + ".cpp";
    this->readJsonFile(jsonFile, jsonValue);
    if (jsonValue.contains("tasks") && jsonValue["tasks"].is_array() && !jsonValue["tasks"].empty())
    {
        auto &args = jsonValue["tasks"][0]["args"];
        if (!controlJsonArg(args, fileSrcNametoJsonFile))
            args.push_back(fileSrcNametoJsonFile);
        if (!controlJsonArg(args, fileIncNametoJsonFile))
            args.push_back(fileIncNametoJsonFile);
    }
    writeJsonFile(jsonFile, jsonValue);
    ////////////////////////
}
void classCreate::writeJsonFile(string &fileName, json &j)
{
    std::ofstream file(fileName);

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open config file");
    }

    file << j.dump(4);
    file.close();
}
void classCreate::readJsonFile(string &fileName, json &j)
{
    // JSON dosyasını oku
    ifstream inFile(fileName);
    if (!inFile.is_open())
    {
        throw std::runtime_error("Could not open config file");
    }

    inFile >> j;
    inFile.close();
}
void classCreate::writeFile(string &fileName)
{
    fs::path currentPath = fs::current_path();
    string currentPathStr = currentPath.string() + "/classCreate/";
    string tempfile;
    if (fileName.find(".hpp") != string::npos)
        tempfile = currentPathStr + "inc/temp.hpp";
    else if (fileName.find(".cpp") != string::npos)
        tempfile = currentPathStr + "src/temp.cpp";
    else
        throw runtime_error("Unsupported file type");

    ifstream inFile(tempfile);
    if (!inFile.is_open())
        throw std::runtime_error("Temp file not found: " + tempfile);

    ofstream outFile(fileName);
    if (!outFile.is_open())
        throw std::runtime_error("Target file could not be created: " + fileName);
    fs::path p(fileName);
    string baseName = p.stem().string();
    string uppBaseName = baseName;
    for (char &c : uppBaseName)
    {
        c = std::toupper(static_cast<unsigned char>(c));
    }
    string line;
    while (getline(inFile, line))
    {
        size_t pos = 0;

        while ((pos = line.find("temp", pos)) != string::npos)
        {
            line.replace(pos, 4, baseName);
            pos += baseName.length();
        }
        pos = 0;
        while ((pos = line.find("TEMP_HPP", pos)) != string::npos)
        {
            line.replace(pos, 8, uppBaseName + "_HPP");
            pos += uppBaseName.length() + 4;
        }

        outFile << line << '\n';
    }
    outFile.close();
}

void classCreate::readFile(string &fileName)
{
    this->infile.open(fileName, ios::in);
    if (!this->infile.is_open())
        cerr << "File is not found." << endl;
    while (getline(this->infile, this->myText))
    {
        std::cout << this->myText << std::endl;
    }
}

classCreate::~classCreate()
{
    this->outfileInc.close();
    this->outfileSrc.close();
    this->infile.close();
    cout << "Destructor is called." << endl;
}

shared_ptr<classCreate> classCreate::classInit()
{
    string name, lastName, className;

    std::cout << "Enter Name: ";
    std::cin >> name;

    std::cout << "Enter Last Name: ";
    std::cin >> lastName;
    std::cout << "Enter Class Name: ";
    std::cin >> className;
    return std::make_shared<classCreate>(name, lastName, className);
}