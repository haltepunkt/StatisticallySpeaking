#pragma comment(lib, "PluginSDK.lib")

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "utils/parser.h"
using namespace std;

class StatisticallySpeaking : public BakkesMod::Plugin::BakkesModPlugin
{
public:
	void onLoad();
	void onUnload();

private:
	void onEventMatchEnded(string eventName);
	void getMMRAndSaveFile(int playlistId);
	void saveCSVFile(filesystem::path matchesFilePath, bool saveHeader = false);

	filesystem::path matchesFilePath;
	map<string, string> matchValues;
	ofstream matchesFile;
};
