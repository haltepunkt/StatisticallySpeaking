#pragma comment(lib, "BakkesMod.lib")

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "utils/parser.h"

class StatisticallySpeaking : public BakkesMod::Plugin::BakkesModPlugin
{
public:
	void onLoad();
	void onUnload();

	void onEventMatchEnded(string eventName);

	map<string, string> matchValues;
	ofstream matchesFile;
};
