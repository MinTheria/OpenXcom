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
 */
#include <array>
#include <string>
#include <vector>
#include "../Engine/Yaml.h"
#include "RuleItem.h"

namespace OpenXcom
{

class Armor;
class Mod;
class RuleInventory;

class RuleArmorLoadout
{
public:
	struct Choice
	{
		const RuleItem* item = nullptr;
		std::array<std::vector<const RuleItem*>, RuleItem::AmmoSlotMax> ammo;
		int spareAmmo = 0;
		bool allowUnloaded = false;
	};

	struct Entry
	{
		std::string slotName;
		const RuleInventory* slot = nullptr;
		int slotX = 0;
		int slotY = 0;
		bool hasSlotPos = false;
		int count = 1;
		bool required = false;
		int overCapacityAllowance = 0;
		std::vector<Choice> choices;
	};

private:
	std::string _type;
	const Armor* _armor = nullptr;
	int _capacityOffset = 0;
	std::vector<Entry> _entries;

public:
	RuleArmorLoadout(const std::string& type);
	~RuleArmorLoadout();

	void load(const YAML::YamlNodeReader& reader, const Mod* mod);

	const std::string& getType() const { return _type; }
	const Armor* getArmor() const { return _armor; }
	int getCapacityOffset() const { return _capacityOffset; }
	const std::vector<Entry>& getEntries() const { return _entries; }
};

}
