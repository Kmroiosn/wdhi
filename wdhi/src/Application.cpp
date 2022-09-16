#include <iostream>
#include <string>
#include <filesystem>
#include <memory>
#include <queue>

using std::filesystem::path;
using std::filesystem::directory_iterator;

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

int wmain(int argc, wchar_t* argv[])
{
	using namespace std;
	cout << "�ڵ�ǰĿ¼�������ļ���..." << endl;

	vector<path> dirPath = GetDirectories("./");
	for (unsigned int x = 0; x < dirPath.size(); x++)
		cout << x+1 << ". " << dirPath[x].filename() << endl;

	if (dirPath.size() == 0)
		cout << "��ǰĿ¼�����ļ���" << endl << "�������ļ���·��: ";
	else 
		cout << "�������ļ�����Ż�·��: ";

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
		cout << "����: " << e.what() << endl;
		exit(-1);
	}
	
	cout << "�� " << *targetPath << " ���ҵ�" << targetSubDir.size() << "���ļ���" << endl;
	cout << "�������ļ�������: ";
	int req;
	cin >> req;
	cout << endl << "------------------------------------------" << endl;
	queue<path> InvalidDir;
	queue<path> ValidDir;
	for (const auto& path : targetSubDir) {
		int fileCount = 0;
		for (const auto& file : directory_iterator(path)) 
			fileCount++;

		cout << "���ļ��� " << path.filename() << " ���ҵ� " << fileCount << " ���ļ�" << endl;
		
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

	printf_s("\n\n%8s  |  %8s\n", "--�����--", "--δ���--");
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
		
		cout << setw(10) << ((valid.length()) ? valid : "")
			<< "  |  "
			<< ((invalid.length()) ? invalid : "") << endl;
	}
	cout << endl << "------------------------------------------" << endl;

	cout << endl << endl << "�� " << invalidSize << " ���������" << endl;

	system("pause");
}