/*
 * AscEmu Framework based on ArcEmu MMORPG Server
 * Copyright (c) 2014-2017 AscEmu Team <http://www.ascemu.org/>
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

#include "StdAfx.h"


initialiseSingleton(SpellProcMgr);

bool SpellProc::CanProc(Unit* victim, SpellInfo* CastingSpell)
{
    return true;
}

bool SpellProc::CheckProcFlags(uint32 flag)
{
    if (mProcFlags & flag)
        return true;
    else
        return false;
}

bool SpellProc::CanDelete(uint32 spellId, uint64 casterGuid, uint64 misc)
{
    if (mSpell->Id == spellId && (casterGuid == 0 || mCaster == casterGuid) && !mDeleted)
        return true;

    return false;
}

bool SpellProc::CheckClassMask(Unit* victim, SpellInfo* CastingSpell)
{
    if ((mProcClassMask[0] == 0 && mProcClassMask[1] == 0 && mProcClassMask[2] == 0) ||
        mProcClassMask[0] & CastingSpell->SpellGroupType[0] ||
        mProcClassMask[1] & CastingSpell->SpellGroupType[1] ||
        mProcClassMask[2] & CastingSpell->SpellGroupType[2])
        return true;
    else
        return false;
}

bool SpellProc::DoEffect(Unit* victim, SpellInfo* CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int* dmg_overwrite, uint32 weapon_damage_type)
{
    return false;
}

void SpellProc::Init(Object* obj)
{
}

uint32 SpellProc::CalcProcChance(Unit* victim, SpellInfo* CastingSpell)
{
    // Check if proc chance is based on combo points
    if (mTarget->IsPlayer() && mOrigSpell && mOrigSpell->AttributesEx & ATTRIBUTESEX_REQ_COMBO_POINTS1 && mOrigSpell->AttributesExD & ATTRIBUTESEXD_PROCCHANCE_COMBOBASED)
        return float2int32(static_cast<Player*>(mTarget)->m_comboPoints * mOrigSpell->EffectPointsPerComboPoint[0]);
    else
        return mProcChance;
}

bool SpellProc::CanProcOnTriggered(Unit* victim, SpellInfo* CastingSpell)
{
    if (mOrigSpell != NULL && mOrigSpell->AttributesExC & ATTRIBUTESEXC_CAN_PROC_ON_TRIGGERED)
        return true;
    return false;
}

void SpellProc::CastSpell(Unit* victim, SpellInfo* CastingSpell, int* dmg_overwrite)
{
    SpellCastTargets targets;
    if (mProcFlags & PROC_TARGET_SELF)
        targets.m_unitTarget = mTarget->GetGUID();
    else
        targets.m_unitTarget = victim->GetGUID();

    Spell* spell = sSpellFactoryMgr.NewSpell(mTarget, mSpell, true, NULL);
    spell->forced_basepoints[0] = dmg_overwrite[0];
    spell->forced_basepoints[1] = dmg_overwrite[1];
    spell->forced_basepoints[2] = dmg_overwrite[2];
    spell->ProcedOnSpell = CastingSpell;

    if (mOrigSpell != NULL)
        spell->pSpellId = mOrigSpell->Id;

    spell->prepare(&targets);
}

SpellProc* SpellProcMgr::NewSpellProc(Unit* target, uint32 spell_id, uint32 orig_spell_id, uint64 caster, uint32 procChance, uint32 procFlags, uint32 procCharges, uint32* groupRelation, uint32* procClassMask, Object* obj)
{
    return NewSpellProc(target, sSpellCustomizations.GetSpellInfo(spell_id), sSpellCustomizations.GetSpellInfo(orig_spell_id), caster, procChance, procFlags, procCharges, groupRelation, procClassMask, obj);
}

SpellProc* SpellProcMgr::NewSpellProc(Unit* target, SpellInfo* spell, SpellInfo* orig_spell, uint64 caster, uint32 procChance, uint32 procFlags, uint32 procCharges, uint32* groupRelation, uint32* procClassMask, Object* obj)
{
    if (spell == NULL)
        return NULL;

    SpellProc* result;
    SpellProcMap::iterator itr;
    spell_proc_factory_function ptr = NULL;

    // Search for SpellProc in unordered_map
    itr = mSpellProcNameHash.find(spell->custom_NameHash);
    if (itr != mSpellProcNameHash.end())
        ptr = itr->second;
    else
    {
        itr = mSpellProc.find(spell->Id);
        if (itr != mSpellProc.end())
            ptr = itr->second;
    }

    if (ptr != NULL)
        result = (*ptr)();      // Found. Create a new object of this specific class
    else
        result = new SpellProc; // Not found. Create a new object of generic SpellProc

    result->mSpell = spell;
    result->mOrigSpell = orig_spell;
    result->mTarget = target;
    result->mCaster = caster;
    result->mProcChance = procChance;
    result->mProcFlags = procFlags;
    result->mProcCharges = procCharges;
    result->mLastTrigger = 0;
    result->mDeleted = false;

    if (groupRelation != NULL)
    {
        result->mGroupRelation[0] = groupRelation[0];
        result->mGroupRelation[1] = groupRelation[1];
        result->mGroupRelation[2] = groupRelation[2];
    }
    else
    {
        result->mGroupRelation[0] = 0;
        result->mGroupRelation[1] = 0;
        result->mGroupRelation[2] = 0;
    }
    if (procClassMask != NULL)
    {
        result->mProcClassMask[0] = procClassMask[0];
        result->mProcClassMask[1] = procClassMask[1];
        result->mProcClassMask[2] = procClassMask[2];
    }
    else
    {
        result->mProcClassMask[0] = 0;
        result->mProcClassMask[1] = 0;
        result->mProcClassMask[2] = 0;
    }

    result->Init(obj);

    return result;
}

void SpellProcMgr::Setup()
{
    SetupItems();
    SetupSpellProcClassScripts();
}
