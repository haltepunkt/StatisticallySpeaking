#include "StatisticallySpeaking.h"

BAKKESMOD_PLUGIN(StatisticallySpeaking, "Statistically Speaking", "1.0", 0)

void StatisticallySpeaking::onLoad()
{
	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.EventMatchEnded", bind(&StatisticallySpeaking::onEventMatchEnded, this, placeholders::_1));
}

void StatisticallySpeaking::onUnload()
{
}

void StatisticallySpeaking::onEventMatchEnded(string eventName)
{
	if (gameWrapper->IsInOnlineGame()) {
		ServerWrapper server = gameWrapper->GetOnlineGame();

		if (!server.IsNull()) {
			GameSettingPlaylistWrapper playlist = server.GetPlaylist();

			if (!playlist.IsLanMatch() && !playlist.IsPrivateMatch()) {
				PlayerControllerWrapper localPrimaryPlayerController = server.GetLocalPrimaryPlayer();

				if (!localPrimaryPlayerController.IsNull()) {
					PriWrapper localPrimaryPlayer = localPrimaryPlayerController.GetPRI();

					if (!localPrimaryPlayer.IsNull()) {
						TeamWrapper matchWinner = server.GetMatchWinner();

						if (!matchWinner.IsNull()) {
							map<string, string> matchValues;

							time_t now = time(NULL);
							tm nowInfo;
							localtime_s(&nowInfo, &now);

							char timestamp[32];
							strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &nowInfo);

							matchValues["Timestamp"] = string(timestamp);

							matchValues["Win"] = to_string((localPrimaryPlayer.GetTeamNum() == matchWinner.GetTeamNum()));

							matchValues["Playlist ID"] = to_string(playlist.GetPlaylistId());
							matchValues["Playlist"] = playlist.GetTitle().ToString();
							matchValues["Ranked"] = to_string(playlist.GetbRanked());

							matchValues["Goals"] = to_string(localPrimaryPlayer.GetMatchGoals());
							matchValues["Assists"] = to_string(localPrimaryPlayer.GetMatchAssists());
							matchValues["Saves"] = to_string(localPrimaryPlayer.GetMatchSaves());
							matchValues["Demos"] = to_string(localPrimaryPlayer.GetMatchDemolishes());
							matchValues["Shots"] = to_string(localPrimaryPlayer.GetMatchShots());
							matchValues["Score"] = to_string(localPrimaryPlayer.GetMatchScore());
							matchValues["MVP"] = to_string(localPrimaryPlayer.GetbMatchMVP());

							if (!ifstream(matchesFilePath)) {
								matchesFile.open(matchesFilePath, ios_base::app);

								unsigned int idx = 1;
								for (const pair<string, string>& matchValue : matchValues) {
									matchesFile << matchValue.first;

									if (idx < matchValues.size()) {
										matchesFile << ",";
									}

									idx++;
								}

								matchesFile << endl;

								matchesFile.close();
							}

							matchesFile.open(matchesFilePath, ios_base::app);

							unsigned int idx = 1;
							for (const pair<string, string>& matchValue : matchValues) {
								matchesFile << matchValue.second;

								if (idx < matchValues.size()) {
									matchesFile << ",";
								}

								idx++;
							}

							matchesFile << endl;

							matchesFile.close();
						}
					}
				}
			}
		}
	}
}
