#pragma once
/*
 * Copyright 2010-2016 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "../Engine/State.h"

namespace OpenXcom
{

class TextButton;
class Window;
class Text;
class TextList;
class Soldier;

/**
 * Final resource losses for a soldier-backed unit in a completed mission.
 */
struct PostMissionUnitStats
{
	Soldier *soldier;
	int healthMissing;
	int manaMissing;

	PostMissionUnitStats(Soldier *unitSoldier, int unitHealthMissing, int unitManaMissing) :
		soldier(unitSoldier), healthMissing(unitHealthMissing), manaMissing(unitManaMissing) { }
};

/**
 * Medals screen that displays new soldier medals.
 */
class CommendationState : public State
{
private:
	enum PostMissionPage
	{
		PAGE_COMMENDATIONS,
		PAGE_MISSION_STATS,
		PAGE_WOUNDS
	};

	TextButton *_btnOk, *_btnStats;
	Window *_window;
	Text *_txtTitle, *_txtKilled, *_txtStunned, *_txtHealth, *_txtMana;
	TextList *_lstSoldiers, *_lstMissionStats, *_lstWounds;
	std::vector<std::string> _commendationsNames;
	PostMissionPage _page;
	bool _hasCommendations, _hasMissionStats, _hasWounds;
	/// Sets the visibility according to the selected page.
	void applyVisibility();
public:
	/// Creates the Medals state.
	CommendationState(
		std::vector<Soldier*> soldiers,
		std::vector<PostMissionUnitStats> participants = std::vector<PostMissionUnitStats>(),
		int missionId = -1);
	/// Cleans up the Medals state.
	~CommendationState();
	/// Handler for clicking on a medal.
	void lstSoldiersMouseClick(Action *action);
	/// Handler for switching between post-mission pages.
	void btnStatsClick(Action *action);
	/// Handler for clicking the OK button.
	void btnOkClick(Action *action);
};

}
