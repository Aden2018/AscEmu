/*
 * AscEmu Framework based on ArcEmu MMORPG Server
 * Copyright (C) 2014-2016 AscEmu Team <http://www.ascemu.org/>
 * Copyright (C) 2008-2012 ArcEmu Team <http://www.ArcEmu.org/>
 * Copyright (C) 2005-2007 Ascent Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef SUMMONHANDLER_HPP
#define SUMMONHANDLER_HPP

#define SUMMON_SLOTS 6

class Unit;

#include "CommonTypes.hpp"
#include <array>
#include <vector>
#include <set>

//////////////////////////////////////////////////////////////////////////////////////////
/// Manages the summons for Units.
//////////////////////////////////////////////////////////////////////////////////////////
class SERVER_DECL SummonHandler
{
    public:

        SummonHandler();
        ~SummonHandler();

        //////////////////////////////////////////////////////////////////////////////////////////
        /// Adds a summon that doesn't have a summon slot
        /// \param Unit* summon  -  pointer to the summon to add    \return none
        //////////////////////////////////////////////////////////////////////////////////////////
        void AddSummon(Unit* summon);


        //////////////////////////////////////////////////////////////////////////////////////////
        /// Adds a summon to a summon slot. If something is already there, it's despawned.
        /// \param   Unit* summon  -  pointer to the summon to add
        /// \param   uint8 slot    -  slot number where we want to add the summon to
        /// \return none
        //////////////////////////////////////////////////////////////////////////////////////////
        void AddSummonToSlot(Unit* summon, uint8 slot);


        //////////////////////////////////////////////////////////////////////////////////////////
        /// Removes a summon that was not summoned to a slot
        /// \param   Unit* summon  -  summon to remove
        /// \return none
        //////////////////////////////////////////////////////////////////////////////////////////
        void RemoveSummon(Unit* summon);


        //////////////////////////////////////////////////////////////////////////////////////////
        /// Removes a summon that was summoned to a slot
        /// \param   uint8 slot  -  the slot to remove a summon from
        /// \param   bool del    -  decides if the summon should also be deleted
        /// \return none
        //////////////////////////////////////////////////////////////////////////////////////////
        void RemoveSummonFromSlot(uint8 slot, bool del = true);


        //////////////////////////////////////////////////////////////////////////////////////////
        /// Expires all summons that were summoned to a slot
        /// \param none
        /// \return none
        //////////////////////////////////////////////////////////////////////////////////////////
        void ExpireSummonsInSlot();


        //////////////////////////////////////////////////////////////////////////////////////////
        /// Removes and despawns all summons both from slots or otherwise
        /// \param none        \return none
        //////////////////////////////////////////////////////////////////////////////////////////
        void RemoveAllSummons();


        //////////////////////////////////////////////////////////////////////////////////////////
        /// Retrieves the spellids the summons in slots were created by
        /// \param std::vector<uint32> &spellids - reference to a vector where the spellids will be put
        /// \return none
        //////////////////////////////////////////////////////////////////////////////////////////
        void GetSummonSlotSpellIDs(std::vector< uint32 > &spellids);


        //////////////////////////////////////////////////////////////////////////////////////////
        /// Tells if there is a summon in the specified slot
        /// \param   uint8 slot  -  the slot we are querying
        /// \return Returns true if there is a summon in that slot false otherwise
        //////////////////////////////////////////////////////////////////////////////////////////
        bool HasSummonInSlot(uint8 slot);

        Unit* GetSummonInSlot(uint8 slot);
        Unit* GetSummonWithEntry(uint32 entry);

        //////////////////////////////////////////////////////////////////////////////////////////
        /// Flags all summons for PvP
        /// \param none         \return none
        //////////////////////////////////////////////////////////////////////////////////////////
        void SetPvPFlags();


        //////////////////////////////////////////////////////////////////////////////////////////
        /// Flags all summons for FFA PvP
        /// \param none         \return none
        //////////////////////////////////////////////////////////////////////////////////////////
        void SetFFAPvPFlags();


        //////////////////////////////////////////////////////////////////////////////////////////
        /// Sets the sanctuary flag on all summons
        /// \param none         \return none
        //////////////////////////////////////////////////////////////////////////////////////////
        void SetSanctuaryFlags();


        //////////////////////////////////////////////////////////////////////////////////////////
        /// Removes the PvP flag from all summons
        /// \param none         \return none
        //////////////////////////////////////////////////////////////////////////////////////////
        void RemovePvPFlags();


        //////////////////////////////////////////////////////////////////////////////////////////
        /// Removes the FFA PvP flag from all summons
        /// \param none         \return none
        //////////////////////////////////////////////////////////////////////////////////////////
        void RemoveFFAPvPFlags();


        //////////////////////////////////////////////////////////////////////////////////////////
        /// Removes the sanctuary flag from all summons
        /// \param none         \return none
        //////////////////////////////////////////////////////////////////////////////////////////
        void RemoveSanctuaryFlags();

    private:

        std::array<Unit* , SUMMON_SLOTS> summonslots;
        std::set<Unit*> guardians;
};

#endif      // SUMMONHANDLER_HPP