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
#include "CommendationState.h"
#include <map>
#include <sstream>
#include "../Engine/Game.h"
#include "../Mod/Mod.h"
#include "../Engine/LocalizedText.h"
#include "../Interface/TextButton.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "../Interface/TextList.h"
#include "../Savegame/Soldier.h"
#include "../Savegame/SoldierDiary.h"
#include "../Savegame/BattleUnitStatistics.h"
#include "../Engine/Options.h"
#include "../Mod/RuleCommendations.h"
#include "../Ufopaedia/Ufopaedia.h"

namespace OpenXcom
{

/**
 * Initializes all the elements in the Medals screen.
 * @param soldiersMedalled List of soldiers with medals.
 * @param participants Final stats for soldiers tracked in the completed mission.
 * @param missionId ID used to select this mission's diary entries.
 */
CommendationState::CommendationState(std::vector<Soldier*> soldiersMedalled, std::vector<PostMissionUnitStats> participants, int missionId) :
	_page(soldiersMedalled.empty() && !participants.empty() && missionId >= 0 ? PAGE_MISSION_STATS : PAGE_COMMENDATIONS),
	_hasCommendations(!soldiersMedalled.empty()),
	_hasMissionStats(!participants.empty() && missionId >= 0),
	_hasWounds(!participants.empty())
{
	// Create object
	_window = new Window(this, 320, 200, 0, 0);
	_btnOk = new TextButton(participants.empty() ? 288 : 140, 16, 16, 176);
	_btnStats = new TextButton(140, 16, 164, 176);
	_txtTitle = new Text(300, 16, 10, 8);
	_txtKilled = new Text(42, 9, 224, 24);
	_txtStunned = new Text(30, 9, 266, 24);
	_txtHealth = new Text(42, 9, 224, 24);
	_txtMana = new Text(30, 9, 266, 24);
	_lstSoldiers = new TextList(288, 128, 8, 32);
	_lstMissionStats = new TextList(288, 136, 8, 32);
	_lstWounds = new TextList(288, 136, 8, 32);

	// Set palette
	setInterface("commendations");

	add(_window, "window", "commendations");
	add(_btnOk, "button", "commendations");
	add(_btnStats, "button", "commendations");
	add(_txtTitle, "heading", "commendations");
	add(_txtKilled, "heading", "commendations");
	add(_txtStunned, "heading", "commendations");
	add(_txtHealth, "heading", "commendations");
	add(_txtMana, "heading", "commendations");
	add(_lstSoldiers, "list", "commendations");
	add(_lstMissionStats, "list", "commendations");
	add(_lstWounds, "list", "commendations");

	centerAllSurfaces();

	// Set up object
	setWindowBackground(_window, "commendations");

	_btnOk->setText(tr("STR_OK"));
	_btnOk->onMouseClick((ActionHandler)&CommendationState::btnOkClick);
	_btnOk->onKeyboardPress((ActionHandler)&CommendationState::btnOkClick, Options::keyOk);
	_btnOk->onKeyboardPress((ActionHandler)&CommendationState::btnOkClick, Options::keyCancel);

	_btnStats->onMouseClick((ActionHandler)&CommendationState::btnStatsClick);

	_txtTitle->setAlign(ALIGN_CENTER);
	_txtTitle->setBig();
	_txtKilled->setText(tr("STR_KILLS_HEADER"));
	_txtKilled->setAlign(ALIGN_CENTER);
	_txtStunned->setText(tr("STR_STUNS_HEADER"));
	_txtStunned->setAlign(ALIGN_CENTER);
	_txtHealth->setText(tr("STR_HEALTH_ABBREVIATION"));
	_txtHealth->setAlign(ALIGN_CENTER);
	if (_game->getMod()->isManaFeatureEnabled())
	{
		_txtMana->setText(tr("STR_MANA_ABBREVIATION"));
	}
	_txtMana->setAlign(ALIGN_CENTER);

	_lstSoldiers->setColumns(2, 204, 84);
	_lstSoldiers->setSelectable(true);
	_lstSoldiers->setBackground(_window);
	_lstSoldiers->setMargin(8);
	_lstSoldiers->onMouseClick((ActionHandler)&CommendationState::lstSoldiersMouseClick);

	_lstMissionStats->setColumns(3, 208, 42, 30);
	_lstMissionStats->setAlign(ALIGN_RIGHT, 1);
	_lstMissionStats->setAlign(ALIGN_RIGHT, 2);
	_lstMissionStats->setSelectable(true);
	_lstMissionStats->setBackground(_window);
	_lstMissionStats->setMargin(8);

	_lstWounds->setColumns(3, 208, 42, 30);
	_lstWounds->setAlign(ALIGN_RIGHT, 1);
	_lstWounds->setAlign(ALIGN_RIGHT, 2);
	_lstWounds->setSelectable(true);
	_lstWounds->setBackground(_window);
	_lstWounds->setMargin(8);

	int row = 0;
	int titleRow = 0;
	const auto& commendationsList = _game->getMod()->getCommendationsList();
	bool modularCommendation;
	std::string noun;
	bool titleChosen = true;

	for (auto commIter = commendationsList.begin(); commIter != commendationsList.end();)
	{
		const auto& commType = (*commIter).first;
		const auto* commRule = (*commIter).second;

		modularCommendation = false;
		noun = "noNoun";
		if (titleChosen)
		{
			_lstSoldiers->addRow(2, "", ""); // Blank row, will be filled in later
			_commendationsNames.push_back("");
			row++;
		}
		titleChosen = false;
		titleRow = row - 1;

		for (auto* soldier : soldiersMedalled)
		{
			for (auto* soldierComm : *soldier->getDiary()->getSoldierCommendations())
			{
				if (soldierComm->getType() == commType && soldierComm->isNew() && noun == "noNoun")
				{
					soldierComm->makeOld();
					row++;

					if (soldierComm->getNoun() != "noNoun")
					{
						noun = soldierComm->getNoun();
						modularCommendation = true;
					}

					// Soldier name
					std::ostringstream wssName;
					wssName << "   ";
					wssName << soldier->getName();
					// Decoration level name
					int skipCounter = 0;
					int lastInt = -2;
					int thisInt = -1;
					int vectorIterator = 0;
					for (auto wtf = commRule->getCriteria()->begin()->second.begin(); wtf != commRule->getCriteria()->begin()->second.end(); ++wtf)
					{
						if (vectorIterator == soldierComm->getDecorationLevelInt() + 1)
						{
							break;
						}
						thisInt = *wtf;
						if (wtf != commRule->getCriteria()->begin()->second.begin())
						{
							--wtf;
							lastInt = *wtf;
							++wtf;
						}
						if (thisInt == lastInt)
						{
							skipCounter++;
						}
						vectorIterator++;
					}
					_lstSoldiers->addRow(2, wssName.str().c_str(), tr(soldierComm->getDecorationLevelName(skipCounter)).c_str());
					_commendationsNames.push_back("");
					break;
				}
			}
		}
		if (titleRow != row - 1)
		{
			// Medal name
			if (modularCommendation)
			{
				_lstSoldiers->setCellText(titleRow, 0, tr(commType).arg(tr(noun)));
			}
			else
			{
				_lstSoldiers->setCellText(titleRow, 0, tr(commType));
			}
			_lstSoldiers->setRowColor(titleRow, _lstSoldiers->getSecondaryColor());
			_commendationsNames[titleRow] = commType;
			titleChosen = true;
		}
		if (noun == "noNoun")
		{
			++commIter;
		}
	}

	for (const auto& participant : participants)
	{
		auto* soldier = participant.soldier;
		std::map<std::string, std::pair<int, int> > weaponStats;
		for (const auto* kill : soldier->getDiary()->getKills())
		{
			if (kill->mission != missionId || kill->faction != FACTION_HOSTILE)
			{
				continue;
			}
			std::string weapon = kill->weapon.empty() ? "STR_WEAPON_UNKNOWN" : kill->weapon;
			if (kill->status == STATUS_DEAD)
			{
				weaponStats[weapon].first++;
			}
			else if (kill->status == STATUS_UNCONSCIOUS)
			{
				weaponStats[weapon].second++;
			}
		}

		_lstMissionStats->addRow(3, soldier->getName().c_str(), "", "");
		_lstMissionStats->setRowColor(_lstMissionStats->getLastRowIndex(), _lstMissionStats->getSecondaryColor());
		for (const auto& stat : weaponStats)
		{
			std::ostringstream kills, stuns;
			if (stat.second.first)
			{
				kills << stat.second.first;
			}
			if (stat.second.second)
			{
				stuns << stat.second.second;
			}
			std::ostringstream weapon;
			weapon << "   " << tr(stat.first);
			_lstMissionStats->addRow(3, weapon.str().c_str(), kills.str().c_str(), stuns.str().c_str());
		}

		std::ostringstream healthMissing, manaMissing;
		healthMissing << participant.healthMissing;
		if (_game->getMod()->isManaFeatureEnabled())
		{
			manaMissing << participant.manaMissing;
		}
		_lstWounds->addRow(3, soldier->getName().c_str(), healthMissing.str().c_str(), manaMissing.str().c_str());
	}

	applyVisibility();
}

/**
 *
 */
CommendationState::~CommendationState()
{
}

/*
*
*/
void CommendationState::lstSoldiersMouseClick(Action *)
{
	Ufopaedia::openArticle(_game, _commendationsNames[_lstSoldiers->getSelectedRow()]);
}

/**
 * Switches to the next available post-mission page.
 */
void CommendationState::btnStatsClick(Action *)
{
	switch (_page)
	{
	case PAGE_COMMENDATIONS:
		_page = _hasMissionStats ? PAGE_MISSION_STATS : PAGE_WOUNDS;
		break;
	case PAGE_MISSION_STATS:
		_page = _hasWounds ? PAGE_WOUNDS : PAGE_COMMENDATIONS;
		break;
	case PAGE_WOUNDS:
		_page = _hasCommendations ? PAGE_COMMENDATIONS : PAGE_MISSION_STATS;
		break;
	}
	applyVisibility();
}

/**
 * Sets up the selected post-mission page.
 */
void CommendationState::applyVisibility()
{
	const bool showCommendations = _page == PAGE_COMMENDATIONS;
	const bool showMissionStats = _page == PAGE_MISSION_STATS;
	const bool showWounds = _page == PAGE_WOUNDS;

	if (showCommendations)
	{
		_txtTitle->setText(tr("STR_MEDALS"));
	}
	else if (showMissionStats)
	{
		_txtTitle->setText(tr("STR_MISSION_STATISTICS"));
	}
	else
	{
		_txtTitle->setText(tr("STR_WOUNDS"));
	}

	_lstSoldiers->setVisible(showCommendations);
	_txtKilled->setVisible(showMissionStats);
	_txtStunned->setVisible(showMissionStats);
	_lstMissionStats->setVisible(showMissionStats);
	_txtHealth->setVisible(showWounds);
	_txtMana->setVisible(showWounds && _game->getMod()->isManaFeatureEnabled());
	_lstWounds->setVisible(showWounds);

	int pageCount = (_hasCommendations ? 1 : 0) + (_hasMissionStats ? 1 : 0) + (_hasWounds ? 1 : 0);
	_btnStats->setVisible(pageCount > 1);
	if (showCommendations)
	{
		_btnStats->setText(tr(_hasMissionStats ? "STR_MISSION_STATISTICS" : "STR_WOUNDS"));
	}
	else if (showMissionStats)
	{
		_btnStats->setText(tr(_hasWounds ? "STR_WOUNDS" : "STR_MEDALS"));
	}
	else
	{
		_btnStats->setText(tr(_hasCommendations ? "STR_MEDALS" : "STR_MISSION_STATISTICS"));
	}
}

/**
 * Returns to the previous screen.
 * @param action Pointer to an action.
 */
void CommendationState::btnOkClick(Action *)
{
	_game->popState();
}

}
