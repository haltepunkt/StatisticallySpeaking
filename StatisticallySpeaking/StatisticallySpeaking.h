#pragma comment(lib, "BakkesMod.lib")

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "utils/parser.h"

class StatisticallySpeaking : public BakkesMod::Plugin::BakkesModPlugin
{
public:
	void onLoad();
	void onUnload();

	void onEventMatchEnded(string eventName);

	string matchesFilePath = "./bakkesmod/data/StatisticallySpeaking-Matches.csv";
	ofstream matchesFile;
};
