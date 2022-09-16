#include <iostream>
#include <string>
#include <filesystem>
#include <memory>
#include <queue>
#include <locale>

using std::filesystem::path;
using std::filesystem::directory_iterator;

std::vector<path> GetDirectories(path myPath);

enum Language { EN, ZH_CN, LanguageCount };
Language currentLang = EN;

#define LS(stringLabel, en, zh_cn) std::string stringLabel[LanguageCount] = {en, zh_cn}

LS(str_searching, "Searching directories in current folder...", "在当前目录中搜索文件夹...");
LS(str_noDirectoryInCurFolder, "No directory in current folder", "当前目录中无文件夹");
LS(str_inputPath, "Please input a directory path: ", "请输入文件夹路径: ");
LS(str_inputIndexOrPath, "Please input a directory index or path: ", "请输入文件夹序号或路径: ");
LS(str_error, "Error: ", "错误: ");
LS(str_foundDir1, "In direcotry ", "在 ");
LS(str_foundDir2, ", found ", " 中找到 ");
LS(str_foundDir3, " directories", " 个文件夹");
LS(str_foundFile3, " files", " 个文件");
LS(str_inputCount, "Please input how many files are needed: ", "请输入文件需求数: ");
LS(str_completed, "--Complete--", "--已完成--");
LS(str_incompleted, "--Incomplete--", "--未完成--");
LS(str_summary, " incomplete in total", " 项未完成");

int wmain(int argc, wchar_t* argv[])
{
	using namespace std;
	cout << str_searching[currentLang] << endl;

	vector<path> dirPath = GetDirectories("./");
	for (unsigned int x = 0; x < dirPath.size(); x++)
		cout << x+1 << ". " << dirPath[x].filename() << endl;

	if (dirPath.size() == 0)
		cout << str_noDirectoryInCurFolder[currentLang] << endl 
		<< str_inputPath[currentLang];
	else 
		cout << str_inputIndexOrPath[currentLang];

	std::string input;
	cin >> input;
	cout << endl << "-----------------------" << endl;

	unique_ptr<int> inputIndex;
	try {
		inputIndex = make_unique<int>(std::stoi(input));
	}
	catch (const std::exception e) {
	}

	unique_ptr<path> targetPath = make_unique<path>(inputIndex != nullptr ? dirPath[(*inputIndex) - 1] : input);
	
	vector<path> targetSubDir;
	try {
		targetSubDir = GetDirectories(*targetPath);
	}
	catch (exception e) {
		cout << str_error[currentLang] << e.what() << endl;
		exit(-1);
	}

	cout << str_foundDir1[currentLang] << *targetPath 
		<< str_foundDir2[currentLang] << targetSubDir.size() 
		<< str_foundDir3[currentLang] << endl;

	cout << str_inputCount[currentLang];
	int req;
	cin >> req;
	cout << endl << "------------------------------------------" << endl;
	queue<path> InvalidDir;
	queue<path> ValidDir;
	for (const auto& path : targetSubDir) {
		int fileCount = 0;
		for (const auto& file : directory_iterator(path)) 
			fileCount++;

		cout << str_foundDir1[currentLang] << path.filename() 
			<< str_foundDir2[currentLang] << fileCount 
			<< str_foundFile3[currentLang] << endl;
		
		if (fileCount < req) {
			InvalidDir.emplace(path);
		}
		else {
			ValidDir.emplace(path);
		}
	}
	cout << endl << "------------------------------------------" << endl;

	int validSize = (int)ValidDir.size();
	int invalidSize = (int)InvalidDir.size();

	printf_s("\n\n%12s  |  %s\n", str_completed[currentLang].c_str(), str_incompleted[currentLang].c_str());
	int size = targetSubDir.size();
	while (size) {
		string valid, invalid;
		if (!ValidDir.empty()) {
			valid = ValidDir.front().filename().string();
			ValidDir.pop();
			size--;
		}
		if (!InvalidDir.empty()) {
			invalid = InvalidDir.front().filename().string();
			InvalidDir.pop();
			size--;
		}
		
		cout << setw(12) << ((valid.length()) ? valid : "")
			<< "  |  "
			<< ((invalid.length()) ? invalid : "") << endl;
	}
	cout << endl << "------------------------------------------" << endl;

	cout << endl << endl << invalidSize << str_summary[currentLang] << endl;

	system("pause");
}

std::vector<path> GetDirectories(path myPath) {
	std::vector<std::filesystem::path> dirPath;
	for (const auto& file : directory_iterator(myPath))
	{
		if (file.is_directory())
		{
			dirPath.push_back(file);
		}
	}
	return dirPath;
}