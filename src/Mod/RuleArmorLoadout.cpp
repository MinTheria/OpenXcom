/*
 * Copyright 2010-2016 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */
#include "RuleArmorLoadout.h"
#include "Armor.h"
#include "Mod.h"
#include "RuleInventory.h"

namespace OpenXcom
{

RuleArmorLoadout::RuleArmorLoadout(const std::string& type) : _type(type)
{
}

RuleArmorLoadout::~RuleArmorLoadout()
{
}

void RuleArmorLoadout::load(const YAML::YamlNodeReader& reader, const Mod* mod)
{
	_armor = mod->getArmor(_type, true);
	reader.tryRead("capacityOffset", _capacityOffset);
	_entries.clear();

	for (const auto& entryReader : reader["entries"].children())
	{
		Entry entry;
		entryReader.tryRead("slot", entry.slotName);
		if (!entry.slotName.empty())
		{
			entry.slot = mod->getInventory(entry.slotName, true);
		}
		if (entryReader["slotX"] || entryReader["slotY"])
		{
			entry.slotX = entryReader["slotX"].readVal(0);
			entry.slotY = entryReader["slotY"].readVal(0);
			entry.hasSlotPos = true;
		}

		int count = 0;
		if (entryReader.tryRead("count", count))
		{
			entry.count = count;
		}
		else if (entryReader.tryRead("max", count))
		{
			entry.count = count;
		}
		else if (entryReader.tryRead("min", count))
		{
			entry.count = count;
			entry.required = count > 0;
		}
		entryReader.tryRead("required", entry.required);
		entryReader.tryRead("overCapacityAllowance", entry.overCapacityAllowance);

		for (const auto& choiceReader : entryReader["choices"].children())
		{
			Choice choice;
			if (choiceReader.hasVal())
			{
				choice.item = mod->getItem(choiceReader.readVal<std::string>(), true);
			}
			else
			{
				choice.item = mod->getItem(choiceReader["item"].readVal<std::string>(), true);
				choiceReader.tryRead("spareAmmo", choice.spareAmmo);
				choiceReader.tryRead("allowUnloaded", choice.allowUnloaded);
				int ammoSlot = 0;
				for (const auto& ammoReader : choiceReader["ammo"].children())
				{
					if (ammoSlot >= RuleItem::AmmoSlotMax)
					{
						break;
					}
					if (ammoReader.hasVal())
					{
						choice.ammo[ammoSlot].push_back(mod->getItem(ammoReader.readVal<std::string>(), true));
					}
					else
					{
						for (const auto& ammoChoiceReader : ammoReader.children())
						{
							choice.ammo[ammoSlot].push_back(mod->getItem(ammoChoiceReader.readVal<std::string>(), true));
						}
					}
					++ammoSlot;
				}
			}
			entry.choices.push_back(choice);
		}

		if (entry.count > 0 && !entry.choices.empty())
		{
			_entries.push_back(entry);
		}
	}
}

}
