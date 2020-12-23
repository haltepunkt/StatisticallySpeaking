#include "StatisticallySpeaking.h"

BAKKESMOD_PLUGIN(StatisticallySpeaking, "Statistically Speaking", "1.2", 0)

void StatisticallySpeaking::onLoad(){
	auto playerId = gameWrapper->GetUniqueID();
	matchesFilePath = gameWrapper->GetDataFolder() / ("StatisticallySpeaking-Matches-" + playerId.str() + ".csv");
	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.EventMatchEnded", bind(&StatisticallySpeaking::onEventMatchEnded, this, placeholders::_1));
}

void StatisticallySpeaking::onUnload(){
}

void StatisticallySpeaking::onEventMatchEnded(string eventName){
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
							matchValues.clear();

							time_t now = time(NULL);
							tm nowInfo;
							localtime_s(&nowInfo, &now);

							char timestamp[32];
							strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &nowInfo);

							matchValues["Timestamp"] = string(timestamp);

							matchValues["Win"] = to_string((localPrimaryPlayer.GetTeamNum() == matchWinner.GetTeamNum()));

							int playlistId = playlist.GetPlaylistId();

							matchValues["Playlist ID"] = to_string(playlistId);
							matchValues["Playlist"] = playlist.GetTitle().ToString();
							matchValues["Ranked"] = to_string(playlist.GetbRanked());

							matchValues["Goals"] = to_string(localPrimaryPlayer.GetMatchGoals());
							matchValues["Assists"] = to_string(localPrimaryPlayer.GetMatchAssists());
							matchValues["Saves"] = to_string(localPrimaryPlayer.GetMatchSaves());
							matchValues["Demos"] = to_string(localPrimaryPlayer.GetMatchDemolishes());
							matchValues["Shots"] = to_string(localPrimaryPlayer.GetMatchShots());
							matchValues["Score"] = to_string(localPrimaryPlayer.GetMatchScore());
							matchValues["MVP"] = to_string(localPrimaryPlayer.GetbMatchMVP());

							gameWrapper->SetTimeout([this, playlistId](GameWrapper* gameWrapper) {
								getMMRAndSaveFile(playlistId);
								}, 6.0f);
						}
					}
				}
			}
		}
	}
}

void StatisticallySpeaking::getMMRAndSaveFile(int playlistId) {
	auto playerId = gameWrapper->GetUniqueID();

	if (gameWrapper->GetMMRWrapper().IsSynced(playerId, playlistId) &&
		!gameWrapper->GetMMRWrapper().IsSyncing(playerId)){
		matchValues["MMR"] = to_string((int)gameWrapper->GetMMRWrapper().GetPlayerMMR(playerId, playlistId));

		if (!ifstream(matchesFilePath)) {
			saveCSVFile(matchesFilePath, true);
		}

		saveCSVFile(matchesFilePath);
	}
	else {
		gameWrapper->SetTimeout([this, playlistId](GameWrapper* gameWrapper) {
			getMMRAndSaveFile(playlistId);
			}, 1.0f);
	}

}

void StatisticallySpeaking::saveCSVFile(filesystem::path matchesFilePath, bool saveHeader){
	matchesFile.open(matchesFilePath, ios_base::app);

	unsigned int idx = 1;

	for (const pair<string, string>& matchValue : matchValues) {
		matchesFile << (saveHeader ? matchValue.first : matchValue.second);

		if (idx < matchValues.size()) {
			matchesFile << ",";
		}

		idx++;
	}

	matchesFile << endl;

	matchesFile.close();
}
