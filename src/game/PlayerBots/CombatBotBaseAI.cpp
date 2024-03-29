#include "CombatBotBaseAI.h"
#include "Player.h"
#include "Group.h"
#include "PlayerBotMgr.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "Spell.h"
#include "SpellAuras.h"

void CombatBotBaseAI::AutoAssignRole()
{
    switch (me->GetClass())
    {
        case CLASS_WARRIOR:
        {
            if (me->HasSpell(SPELL_SHIELD_SLAM))
                m_role = ROLE_TANK;
            else
                m_role = ROLE_MELEE_DPS;
            return;
        }
        case CLASS_ROGUE:
        {
            m_role = ROLE_MELEE_DPS;
            return;
        }
        case CLASS_HUNTER:
        case CLASS_MAGE:
        case CLASS_WARLOCK:
        {
            m_role = ROLE_RANGE_DPS;
            return;
        }
        case CLASS_PALADIN:
        {
            if (me->HasSpell(SPELL_HOLY_SHIELD))
                m_role = ROLE_TANK;
            else if (me->HasSpell(SPELL_SANCTITY_AURA))
                m_role = ROLE_MELEE_DPS;
            else
                m_role = ROLE_HEALER;
            return;
        }
        case CLASS_PRIEST:
        {
            if (me->HasSpell(SPELL_SHADOWFORM))
                m_role = ROLE_RANGE_DPS;
            else
                m_role = ROLE_HEALER;
            return;
        }
        case CLASS_SHAMAN:
        {
            if (me->HasSpell(SPELL_ELEMENTAL_MASTERY))
                m_role = ROLE_RANGE_DPS;
            else if (me->HasSpell(SPELL_STORMSTRIKE))
                m_role = ROLE_MELEE_DPS;
            else
                m_role = ROLE_HEALER;
            return;
        }
        case CLASS_DRUID:
        {
            if (me->HasSpell(SPELL_MOONKIN_FORM))
                m_role = ROLE_RANGE_DPS;
            else if (me->HasSpell(SPELL_LEADER_OF_THE_PACK))
                m_role = ROLE_MELEE_DPS;
            else
                m_role = ROLE_HEALER;
            return;
        }
    }

    m_role = ROLE_MELEE_DPS;
}

void CombatBotBaseAI::ResetSpellData()
{
    for (auto& ptr : m_spells.raw.spells)
        ptr = nullptr;

    m_resurrectionSpell = nullptr;
    spellListDirectHeal.clear();
    spellListPeriodicHeal.clear();
    spellListTaunt.clear();
}

void CombatBotBaseAI::PopulateSpellData()
{
    // Paladin Seals
    SpellEntry const* pSealOfRighteousness = nullptr;
    SpellEntry const* pSealOfCommand = nullptr;
    SpellEntry const* pSealOfFury = nullptr;

    // Paladin Blessings
    SpellEntry const* pBlessingOfLight = nullptr;
    SpellEntry const* pBlessingOfMight = nullptr;
    SpellEntry const* pBlessingOfWisdom = nullptr;
    SpellEntry const* pBlessingOfKings = nullptr;
    SpellEntry const* pBlessingOfSanctuary = nullptr;

    // Paladin Auras
    SpellEntry const* pDevotionAura = nullptr;
    SpellEntry const* pConcentrationAura = nullptr;
    SpellEntry const* pRetributionAura = nullptr;
    SpellEntry const* pSanctityAura = nullptr;
    SpellEntry const* pShadowResistanceAura = nullptr;
    SpellEntry const* pFrostResistanceAura = nullptr;
    SpellEntry const* pFireResistanceAura = nullptr;

    // Air Totems
    SpellEntry const* pGraceOfAirTotem = nullptr;
    SpellEntry const* pNatureResistanceTotem = nullptr;
    SpellEntry const* pWindfuryTotem = nullptr;
    SpellEntry const* pWindwallTotem = nullptr;
    SpellEntry const* pTranquilAirTotem = nullptr;

    // Earth Totems
    SpellEntry const* pEarthbindTotem = nullptr;
    SpellEntry const* pStoneclawtotem = nullptr;
    SpellEntry const* pStoneskinTotem = nullptr;
    SpellEntry const* pStrengthOfEarthTotem = nullptr;
    SpellEntry const* pTremorTotem = nullptr;

    // Fire Totems
    SpellEntry const* pFireNovaTotem = nullptr;
    SpellEntry const* pMagmaTotem = nullptr;
    SpellEntry const* pSearingTotem = nullptr;
    SpellEntry const* pFlametongueTotem = nullptr;
    SpellEntry const* pFrostResistanceTotem = nullptr;

    // Water Totems
    SpellEntry const* pFireResistanceTotem = nullptr;
    SpellEntry const* pDiseaseCleansingTotem = nullptr;
    SpellEntry const* pHealingStreamTotem = nullptr;
    SpellEntry const* pManaSpringTotem = nullptr;
    SpellEntry const* pPoisonCleansingTotem = nullptr;

    // Shaman Weapon Buffs
    SpellEntry const* pFrostbrandWeapon = nullptr;
    SpellEntry const* pRockbiterWeapon = nullptr;
    SpellEntry const* pWindfuryWeapon = nullptr;

    // Mage Polymorph
    SpellEntry const* pPolymorphSheep = nullptr;
    SpellEntry const* pPolymorphCow = nullptr;
    SpellEntry const* pPolymorphPig = nullptr;
    SpellEntry const* pPolymorphTurtle = nullptr;

    // Mage Frost Armor (to replace ice armor at low level)
    SpellEntry const* pFrostArmor = nullptr;

    bool hasDeadlyPoison = false;
    bool hasInstantPoison = false;
    bool hasCripplingPoison = false;
    bool hasWoundPoison = false;
    bool HasMindNumbingPoison = false;

    for (const auto& spell : me->GetSpellMap())
    {
        if (spell.second.disabled)
            continue;

        if (spell.second.state == PLAYERSPELL_REMOVED)
            continue;

        SpellEntry const* pSpellEntry = sSpellMgr.GetSpellEntry(spell.first);
        if (!pSpellEntry)
            continue;


        // Check Hidden Spells and Passive talents
        if (pSpellEntry->HasAttribute(SPELL_ATTR_PASSIVE))
        {
            switch (me->GetClass())
            {
                case CLASS_DRUID:
                {
                    if (pSpellEntry->SpellName[0].find("Furor") != std::string::npos)
                    {
                        if (!m_spells.druid.pFuror ||
                            m_spells.druid.pFuror->Rank < pSpellEntry->Rank)
                            m_spells.druid.pFuror = pSpellEntry;
                    }
                }
                case CLASS_MAGE:
                {
                    if (pSpellEntry->SpellName[0].find("Improved Arcane Explosion") != std::string::npos)
                    {
                        if (!m_spells.mage.pImprovedArcaneExplosion ||
                            m_spells.mage.pImprovedArcaneExplosion->Rank < pSpellEntry->Rank)
                            m_spells.mage.pImprovedArcaneExplosion = pSpellEntry;
                    }
                    else if (pSpellEntry->SpellName[0].find("Improved Flamestrike") != std::string::npos)
                    {
                        if (!m_spells.mage.pImprovedFlamestrike ||
                            m_spells.mage.pImprovedFlamestrike->Rank < pSpellEntry->Rank)
                            m_spells.mage.pImprovedFlamestrike = pSpellEntry;
                    }
                    else if (pSpellEntry->SpellName[0].find("Improved Fireball") != std::string::npos)
                    {
                        if (!m_spells.mage.pImprovedFireball ||
                            m_spells.mage.pImprovedFireball->Rank < pSpellEntry->Rank)
                            m_spells.mage.pImprovedFireball = pSpellEntry;
                    }
                    else if (pSpellEntry->SpellName[0].find("Improved Scorch") != std::string::npos)
                    {
                        if (!m_spells.mage.pImprovedScorch ||
                            m_spells.mage.pImprovedScorch->Rank < pSpellEntry->Rank)
                            m_spells.mage.pImprovedScorch = pSpellEntry;
                    }
                    else if (pSpellEntry->SpellName[0].find("Improved Arcane Missiles") != std::string::npos)
                    {
                        if (!m_spells.mage.pImprovedArcaneMissiles ||
                            m_spells.mage.pImprovedArcaneMissiles->Rank < pSpellEntry->Rank)
                            m_spells.mage.pImprovedArcaneMissiles = pSpellEntry;
                    }
                }
            }

            continue;
        }
            

        if (pSpellEntry->HasAttribute(SPELL_ATTR_HIDDEN_CLIENTSIDE))
            continue;

        switch (me->GetClass())
        {
            case CLASS_PALADIN:
            {
                if (pSpellEntry->SpellName[0].find("Seal of Righteousness") != std::string::npos)
                {
                    if (!pSealOfRighteousness ||
                        pSealOfRighteousness->Rank < pSpellEntry->Rank)
                        pSealOfRighteousness = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Seal of Command") != std::string::npos)
                {
                    if (!pSealOfCommand ||
                        pSealOfCommand->Rank < pSpellEntry->Rank)
                        pSealOfCommand = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Seal of Wisdom") != std::string::npos)
                {
                    if (!m_spells.paladin.pSealOfWisdom ||
                        m_spells.paladin.pSealOfWisdom->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pSealOfWisdom = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Seal of the Crusader") != std::string::npos)
                {
                    if (!m_spells.paladin.pSealOfCrusader ||
                        m_spells.paladin.pSealOfCrusader->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pSealOfCrusader = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Judgement") != std::string::npos)
                {
                    if (!m_spells.paladin.pJudgement ||
                        m_spells.paladin.pJudgement->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pJudgement = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Hammer of Justice") != std::string::npos)
                {
                    if (!m_spells.paladin.pHammerOfJustice ||
                        m_spells.paladin.pHammerOfJustice->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pHammerOfJustice = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Blessing of Sacrifice") != std::string::npos)
                {
                    if (!m_spells.paladin.pBlessingOfSacrifice ||
                        m_spells.paladin.pBlessingOfSacrifice->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pBlessingOfSacrifice = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Blessing of Freedom") != std::string::npos)
                {
                    if (!m_spells.paladin.pBlessingOfFreedom ||
                        m_spells.paladin.pBlessingOfFreedom->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pBlessingOfFreedom = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Blessing of Protection") != std::string::npos)
                {
                    if (!m_spells.paladin.pBlessingOfProtection ||
                        m_spells.paladin.pBlessingOfProtection->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pBlessingOfProtection = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Blessing of Sanctuary") != std::string::npos)
                {
                    if (!pBlessingOfSanctuary ||
                        pBlessingOfSanctuary->Rank < pSpellEntry->Rank)
                        pBlessingOfSanctuary = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Blessing of Kings") != std::string::npos)
                {
                    if (pSpellEntry->SpellName[0].find("Greater") != std::string::npos)
                        pBlessingOfKings = pSpellEntry;
                    else if (!pBlessingOfKings ||
                        (pBlessingOfKings->SpellName[0].find("Greater") == std::string::npos &&
                        pBlessingOfKings->Rank < pSpellEntry->Rank))
                        pBlessingOfKings = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Blessing of Wisdom") != std::string::npos)
                {
                    if(pSpellEntry->SpellName[0].find("Greater") != std::string::npos)
                        pBlessingOfWisdom = pSpellEntry;
                    else if (!pBlessingOfWisdom ||
                        (pBlessingOfWisdom->SpellName[0].find("Greater") == std::string::npos &&
                        pBlessingOfWisdom->Rank < pSpellEntry->Rank))
                        pBlessingOfWisdom = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Blessing of Might") != std::string::npos)
                {
                    if (pSpellEntry->SpellName[0].find("Greater") != std::string::npos)
                        pBlessingOfMight = pSpellEntry;
                    if (!pBlessingOfMight ||
                        (pBlessingOfMight->SpellName[0].find("Greater") == std::string::npos &&
                        pBlessingOfMight->Rank < pSpellEntry->Rank))
                        pBlessingOfMight = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Blessing of Light") != std::string::npos)
                {
                    if (!pBlessingOfLight ||
                        pBlessingOfLight->Rank < pSpellEntry->Rank)
                        pBlessingOfLight = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Devotion Aura") != std::string::npos)
                {
                    if (!pDevotionAura ||
                        pDevotionAura->Rank < pSpellEntry->Rank)
                        pDevotionAura = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Retribution Aura") != std::string::npos)
                {
                    if (!pRetributionAura ||
                        pRetributionAura->Rank < pSpellEntry->Rank)
                        pRetributionAura = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Concentration Aura") != std::string::npos)
                {
                    if (!pConcentrationAura ||
                        pConcentrationAura->Rank < pSpellEntry->Rank)
                        pConcentrationAura = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Sanctity Aura") != std::string::npos)
                {
                    if (!pSanctityAura ||
                        pSanctityAura->Rank < pSpellEntry->Rank)
                        pSanctityAura = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Shadow Resistance Aura") != std::string::npos)
                {
                    if (!pShadowResistanceAura ||
                        pShadowResistanceAura->Rank < pSpellEntry->Rank)
                        pShadowResistanceAura = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Frost Resistance Aura") != std::string::npos)
                {
                    if (!pFrostResistanceAura ||
                        pFrostResistanceAura->Rank < pSpellEntry->Rank)
                        pFrostResistanceAura = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Fire Resistance Aura") != std::string::npos)
                {
                    if (!pFireResistanceAura ||
                        pFireResistanceAura->Rank < pSpellEntry->Rank)
                        pFireResistanceAura = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Exorcism") != std::string::npos)
                {
                    if (!m_spells.paladin.pExorcism ||
                        m_spells.paladin.pExorcism->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pExorcism = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Consecration") != std::string::npos)
                {
                    if (!m_spells.paladin.pConsecration ||
                        m_spells.paladin.pConsecration->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pConsecration = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Hammer of Wrath") != std::string::npos)
                {
                    if (!m_spells.paladin.pHammerOfWrath ||
                        m_spells.paladin.pHammerOfWrath->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pHammerOfWrath = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Cleanse") != std::string::npos)
                {
                    if (!m_spells.paladin.pCleanse ||
                        m_spells.paladin.pCleanse->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pCleanse = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Divine Shield") != std::string::npos)
                {
                    if (!m_spells.paladin.pDivineShield ||
                        m_spells.paladin.pDivineShield->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pDivineShield = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Lay on Hands") != std::string::npos)
                {
                    if (!m_spells.paladin.pLayOnHands ||
                        m_spells.paladin.pLayOnHands->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pLayOnHands = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Righteous Fury") != std::string::npos) // post 1.9
                {
                    if (!m_spells.paladin.pRighteousFury ||
                        m_spells.paladin.pRighteousFury->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pRighteousFury = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Seal of Fury") != std::string::npos) // pre 1.9
                {
                    if (!pSealOfFury ||
                        pSealOfFury->Rank < pSpellEntry->Rank)
                        pSealOfFury = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Holy Shock") != std::string::npos)
                {
                    if (!m_spells.paladin.pHolyShock ||
                        m_spells.paladin.pHolyShock->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pHolyShock = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Divine Favor") != std::string::npos)
                {
                    if (!m_spells.paladin.pDivineFavor ||
                        m_spells.paladin.pDivineFavor->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pDivineFavor = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Holy Wrath") != std::string::npos)
                {
                    if (!m_spells.paladin.pHolyWrath ||
                        m_spells.paladin.pHolyWrath->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pHolyWrath = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Turn Evil") != std::string::npos)
                {
                    if (!m_spells.paladin.pTurnEvil ||
                        m_spells.paladin.pTurnEvil->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pTurnEvil = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Holy Shield") != std::string::npos)
                {
                    if (!m_spells.paladin.pHolyShield ||
                        m_spells.paladin.pHolyShield->Rank < pSpellEntry->Rank)
                        m_spells.paladin.pHolyShield = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Repentance") != std::string::npos)
                {
                if (!m_spells.paladin.pRepentance ||
                    m_spells.paladin.pRepentance->Rank < pSpellEntry->Rank)
                    m_spells.paladin.pRepentance = pSpellEntry;
                }

                break;
            }
            case CLASS_SHAMAN:
            {
                if (pSpellEntry->SpellName[0].find("Lightning Bolt") != std::string::npos)
                {
                    if (!m_spells.shaman.pLightningBolt ||
                        m_spells.shaman.pLightningBolt->Rank < pSpellEntry->Rank)
                        m_spells.shaman.pLightningBolt = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Chain Lightning") != std::string::npos)
                {
                    if (!m_spells.shaman.pChainLightning ||
                        m_spells.shaman.pChainLightning->Rank < pSpellEntry->Rank)
                        m_spells.shaman.pChainLightning = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Earth Shock") != std::string::npos)
                {
                    if (!m_spells.shaman.pEarthShock ||
                        m_spells.shaman.pEarthShock->Rank < pSpellEntry->Rank)
                        m_spells.shaman.pEarthShock = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Flame Shock") != std::string::npos)
                {
                    if (!m_spells.shaman.pFlameShock ||
                        m_spells.shaman.pFlameShock->Rank < pSpellEntry->Rank)
                        m_spells.shaman.pFlameShock = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Frost Shock") != std::string::npos)
                {
                    if (!m_spells.shaman.pFrostShock ||
                        m_spells.shaman.pFrostShock->Rank < pSpellEntry->Rank)
                        m_spells.shaman.pFrostShock = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Purge") != std::string::npos)
                {
                    if (!m_spells.shaman.pPurge ||
                        m_spells.shaman.pPurge->Rank < pSpellEntry->Rank)
                        m_spells.shaman.pPurge = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Stormstrike") != std::string::npos)
                {
                    if (!m_spells.shaman.pStormstrike ||
                        m_spells.shaman.pStormstrike->Rank < pSpellEntry->Rank)
                        m_spells.shaman.pStormstrike = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Elemental Mastery") != std::string::npos)
                {
                    if (!m_spells.shaman.pElementalMastery ||
                        m_spells.shaman.pElementalMastery->Rank < pSpellEntry->Rank)
                        m_spells.shaman.pElementalMastery = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Lightning Shield") != std::string::npos)
                {
                    if (!m_spells.shaman.pLightningShield ||
                        m_spells.shaman.pLightningShield->Rank < pSpellEntry->Rank)
                        m_spells.shaman.pLightningShield = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Ghost Wolf") != std::string::npos)
                {
                    if (!m_spells.shaman.pGhostWolf ||
                        m_spells.shaman.pGhostWolf->Rank < pSpellEntry->Rank)
                        m_spells.shaman.pGhostWolf = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Frostbrand Weapon") != std::string::npos)
                {
                    if (!pFrostbrandWeapon ||
                        pFrostbrandWeapon->Rank < pSpellEntry->Rank)
                        pFrostbrandWeapon = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Rockbiter Weapon") != std::string::npos)
                {
                    if (!pRockbiterWeapon ||
                        pRockbiterWeapon->Rank < pSpellEntry->Rank)
                        pRockbiterWeapon = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Windfury Weapon") != std::string::npos)
                {
                    if (!pWindfuryWeapon ||
                        pWindfuryWeapon->Id < pSpellEntry->Id)
                        pWindfuryWeapon = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Grace of Air Totem") != std::string::npos)
                {
                    if (!pGraceOfAirTotem ||
                        pGraceOfAirTotem->Rank < pSpellEntry->Rank)
                        pGraceOfAirTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Nature Resistance Totem") != std::string::npos)
                {
                    if (!pNatureResistanceTotem ||
                        pNatureResistanceTotem->Rank < pSpellEntry->Rank)
                        pNatureResistanceTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Windfury Totem") != std::string::npos)
                {
                    if (!pWindfuryTotem ||
                        pWindfuryTotem->Rank < pSpellEntry->Rank)
                        pWindfuryTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Windwall Totem") != std::string::npos)
                {
                    if (!pWindwallTotem ||
                        pWindwallTotem->Rank < pSpellEntry->Rank)
                        pWindwallTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Tranquil Air Totem") != std::string::npos)
                {
                    if (!pTranquilAirTotem ||
                        pTranquilAirTotem->Rank < pSpellEntry->Rank)
                        pTranquilAirTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Earthbind Totem") != std::string::npos)
                {
                    if (!pEarthbindTotem ||
                        pEarthbindTotem->Rank < pSpellEntry->Rank)
                        pEarthbindTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Stoneclaw Totem") != std::string::npos)
                {
                    if (!pStoneclawtotem ||
                        pStoneclawtotem->Rank < pSpellEntry->Rank)
                        pStoneclawtotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Stoneskin Totem") != std::string::npos)
                {
                    if (!pStoneskinTotem ||
                        pStoneskinTotem->Rank < pSpellEntry->Rank)
                        pStoneskinTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Strength of Earth Totem") != std::string::npos)
                {
                    if (!pStrengthOfEarthTotem ||
                        pStrengthOfEarthTotem->Rank < pSpellEntry->Rank)
                        pStrengthOfEarthTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Tremor Totem") != std::string::npos)
                {
                    if (!pTremorTotem ||
                        pTremorTotem->Rank < pSpellEntry->Rank)
                        pTremorTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Fire Nova Totem") != std::string::npos)
                {
                    if (!pFireNovaTotem ||
                        pFireNovaTotem->Rank < pSpellEntry->Rank)
                        pFireNovaTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Magma Totem") != std::string::npos)
                {
                    if (!pMagmaTotem ||
                        pMagmaTotem->Rank < pSpellEntry->Rank)
                        pMagmaTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Searing Totem") != std::string::npos)
                {
                    if (!pSearingTotem ||
                        pSearingTotem->Rank < pSpellEntry->Rank)
                        pSearingTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Flametongue Totem") != std::string::npos)
                {
                    if (!pFlametongueTotem ||
                        pFlametongueTotem->Rank < pSpellEntry->Rank)
                        pFlametongueTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Frost Resistance Totem") != std::string::npos)
                {
                    if (!pFrostResistanceTotem ||
                        pFrostResistanceTotem->Rank < pSpellEntry->Rank)
                        pFrostResistanceTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Fire Resistance Totem") != std::string::npos)
                {
                    if (!pFireResistanceTotem ||
                        pFireResistanceTotem->Rank < pSpellEntry->Rank)
                        pFireResistanceTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Disease Resistance Totem") != std::string::npos)
                {
                    if (!pDiseaseCleansingTotem ||
                        pDiseaseCleansingTotem->Rank < pSpellEntry->Rank)
                        pDiseaseCleansingTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Healing Stream Totem") != std::string::npos)
                {
                    if (!pHealingStreamTotem ||
                        pHealingStreamTotem->Rank < pSpellEntry->Rank)
                        pHealingStreamTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Mana Spring Totem") != std::string::npos)
                {
                    if (!pManaSpringTotem ||
                        pManaSpringTotem->Rank < pSpellEntry->Rank)
                        pManaSpringTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Poison Cleansing Totem") != std::string::npos)
                {
                    if (!pPoisonCleansingTotem ||
                        pPoisonCleansingTotem->Rank < pSpellEntry->Rank)
                        pPoisonCleansingTotem = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Mana Tide Totem") != std::string::npos)
                {
                    if (!m_spells.shaman.pManaTideTotem ||
                        m_spells.shaman.pManaTideTotem->Rank < pSpellEntry->Rank)
                        m_spells.shaman.pManaTideTotem = pSpellEntry;
                }
                break;
            }
            case CLASS_HUNTER:
            {
                if (pSpellEntry->SpellName[0].find("Aspect of the Cheetah") != std::string::npos)
                {
                    if (!m_spells.hunter.pAspectOfTheCheetah ||
                        m_spells.hunter.pAspectOfTheCheetah->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pAspectOfTheCheetah = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Aspect of the Hawk") != std::string::npos)
                {
                    if (!m_spells.hunter.pAspectOfTheHawk ||
                        m_spells.hunter.pAspectOfTheHawk->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pAspectOfTheHawk = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Aspect of the Monkey") != std::string::npos)
                {
                    if (!m_spells.hunter.pAspectOfTheMonkey ||
                        m_spells.hunter.pAspectOfTheMonkey->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pAspectOfTheMonkey = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Serpent Sting") != std::string::npos)
                {
                    if (!m_spells.hunter.pSerpentSting ||
                        m_spells.hunter.pSerpentSting->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pSerpentSting = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Viper Sting") != std::string::npos)
                {
                    if (!m_spells.hunter.pViperSting ||
                        m_spells.hunter.pViperSting->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pViperSting = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Arcane Shot") != std::string::npos)
                {
                    if (!m_spells.hunter.pArcaneShot ||
                        m_spells.hunter.pArcaneShot->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pArcaneShot = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Aimed Shot") != std::string::npos)
                {
                    if (!m_spells.hunter.pAimedShot ||
                        m_spells.hunter.pAimedShot->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pAimedShot = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Multi-Shot") != std::string::npos)
                {
                    if (!m_spells.hunter.pMultiShot ||
                        m_spells.hunter.pMultiShot->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pMultiShot = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Concussive Shot") != std::string::npos)
                {
                    if (!m_spells.hunter.pConcussiveShot ||
                        m_spells.hunter.pConcussiveShot->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pConcussiveShot = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Wing Clip") != std::string::npos)
                {
                    if (!m_spells.hunter.pWingClip ||
                        m_spells.hunter.pWingClip->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pWingClip = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Hunter's Mark") != std::string::npos)
                {
                    if (!m_spells.hunter.pHuntersMark ||
                        m_spells.hunter.pHuntersMark->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pHuntersMark = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Mongoose Bite") != std::string::npos)
                {
                    if (!m_spells.hunter.pMongooseBite ||
                        m_spells.hunter.pMongooseBite->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pMongooseBite = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Raptor Strike") != std::string::npos)
                {
                    if (!m_spells.hunter.pRaptorStrike ||
                        m_spells.hunter.pRaptorStrike->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pRaptorStrike = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Disengage") != std::string::npos)
                {
                    if (!m_spells.hunter.pDisengage ||
                        m_spells.hunter.pDisengage->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pDisengage = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Feign Death") != std::string::npos)
                {
                    if (!m_spells.hunter.pFeignDeath ||
                        m_spells.hunter.pFeignDeath->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pFeignDeath = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Scare Beast") != std::string::npos)
                {
                    if (!m_spells.hunter.pScareBeast ||
                        m_spells.hunter.pScareBeast->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pScareBeast = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Volley") != std::string::npos)
                {
                    if (!m_spells.hunter.pVolley ||
                        m_spells.hunter.pVolley->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pVolley = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Frost Trap") != std::string::npos)
                {
                    if (!m_spells.hunter.pFrosTrap ||
                        m_spells.hunter.pFrosTrap->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pFrosTrap = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Freezing Trap") != std::string::npos)
                {
                    if (!m_spells.hunter.pFreezingTrap ||
                        m_spells.hunter.pFreezingTrap->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pFreezingTrap = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Immolation Trap") != std::string::npos)
                {
                    if (!m_spells.hunter.pImmolationTrap ||
                        m_spells.hunter.pImmolationTrap->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pImmolationTrap = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Explosive Trap") != std::string::npos)
                {
                    if (!m_spells.hunter.pExplosiveTrap ||
                        m_spells.hunter.pExplosiveTrap->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pExplosiveTrap = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Deterrence") != std::string::npos)
                {
                    if (!m_spells.hunter.pDeterrence ||
                        m_spells.hunter.pDeterrence->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pDeterrence = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Counterattack") != std::string::npos)
                {
                    if (!m_spells.hunter.pCounterattack ||
                        m_spells.hunter.pCounterattack->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pCounterattack = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Intimidation") != std::string::npos)
                {
                    if (!m_spells.hunter.pIntimidation ||
                        m_spells.hunter.pIntimidation->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pIntimidation = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Bestial Wrath") != std::string::npos)
                {
                    if (!m_spells.hunter.pBestialWrath ||
                        m_spells.hunter.pBestialWrath->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pBestialWrath = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Tranquilizing Shot") != std::string::npos)
                {
                    if (!m_spells.hunter.pTranquilizingShot ||
                        m_spells.hunter.pTranquilizingShot->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pTranquilizingShot = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Rapid Fire") != std::string::npos)
                {
                    if (!m_spells.hunter.pRapidFire ||
                        m_spells.hunter.pRapidFire->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pRapidFire = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Trueshot Aura") != std::string::npos)
                {
                    if (!m_spells.hunter.pTrueshotAura ||
                        m_spells.hunter.pTrueshotAura->Rank < pSpellEntry->Rank)
                        m_spells.hunter.pTrueshotAura = pSpellEntry;
                }

                break;
            }
            case CLASS_MAGE:
            {
                if (pSpellEntry->SpellName[0].find("Ice Armor") != std::string::npos)
                {
                    if (!m_spells.mage.pIceArmor ||
                        m_spells.mage.pIceArmor->Rank < pSpellEntry->Rank)
                        m_spells.mage.pIceArmor = pSpellEntry;
                }
                if (pSpellEntry->SpellName[0].find("Frost Armor") != std::string::npos)
                {
                    if (!pFrostArmor ||
                        pFrostArmor->Rank < pSpellEntry->Rank)
                        pFrostArmor = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Ice Barrier") != std::string::npos)
                {
                    if (!m_spells.mage.pIceBarrier ||
                        m_spells.mage.pIceBarrier->Rank < pSpellEntry->Rank)
                        m_spells.mage.pIceBarrier = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Mana Shield") != std::string::npos)
                {
                    if (!m_spells.mage.pManaShield ||
                        m_spells.mage.pManaShield->Rank < pSpellEntry->Rank)
                        m_spells.mage.pManaShield = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Arcane Intellect") != std::string::npos)
                {
                    if (!m_spells.mage.pArcaneIntellect ||
                        m_spells.mage.pArcaneIntellect->Rank < pSpellEntry->Rank)
                        m_spells.mage.pArcaneIntellect = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Arcane Brilliance") != std::string::npos)
                {
                    if (!m_spells.mage.pArcaneBrilliance ||
                        m_spells.mage.pArcaneBrilliance->Rank < pSpellEntry->Rank)
                        m_spells.mage.pArcaneBrilliance = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Frostbolt") != std::string::npos)
                {
                    if (!m_spells.mage.pFrostbolt ||
                        m_spells.mage.pFrostbolt->Rank < pSpellEntry->Rank)
                        m_spells.mage.pFrostbolt = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Fire Blast") != std::string::npos)
                {
                    if (!m_spells.mage.pFireBlast ||
                        m_spells.mage.pFireBlast->Rank < pSpellEntry->Rank)
                        m_spells.mage.pFireBlast = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Fireball") != std::string::npos)
                {
                    if (!m_spells.mage.pFireball ||
                        m_spells.mage.pFireball->Rank < pSpellEntry->Rank)
                        m_spells.mage.pFireball = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Arcane Explosion") != std::string::npos)
                {
                    if (!m_spells.mage.pArcaneExplosion ||
                        m_spells.mage.pArcaneExplosion->Rank < pSpellEntry->Rank)
                        m_spells.mage.pArcaneExplosion = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Frost Nova") != std::string::npos)
                {
                    if (!m_spells.mage.pFrostNova ||
                        m_spells.mage.pFrostNova->Rank < pSpellEntry->Rank)
                        m_spells.mage.pFrostNova = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Cone of Cold") != std::string::npos)
                {
                    if (!m_spells.mage.pConeofCold ||
                        m_spells.mage.pConeofCold->Rank < pSpellEntry->Rank)
                        m_spells.mage.pConeofCold = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Blink") != std::string::npos)
                {
                    if (!m_spells.mage.pBlink ||
                        m_spells.mage.pBlink->Rank < pSpellEntry->Rank)
                        m_spells.mage.pBlink = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0] == "Polymorph") // Sheep
                {
                    if (!pPolymorphSheep ||
                        pPolymorphSheep->Rank < pSpellEntry->Rank)
                        pPolymorphSheep = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Polymorph: Cow") != std::string::npos)
                {
                    if (!pPolymorphCow ||
                        pPolymorphCow->Rank < pSpellEntry->Rank)
                        pPolymorphCow = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Polymorph: Pig") != std::string::npos)
                {
                    if (!pPolymorphPig ||
                        pPolymorphPig->Rank < pSpellEntry->Rank)
                        pPolymorphPig = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Polymorph: Turtle") != std::string::npos)
                {
                    if (!pPolymorphTurtle ||
                        pPolymorphTurtle->Rank < pSpellEntry->Rank)
                        pPolymorphTurtle = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Counterspell") != std::string::npos)
                {
                    if (!m_spells.mage.pCounterspell ||
                        m_spells.mage.pCounterspell->Rank < pSpellEntry->Rank)
                        m_spells.mage.pCounterspell = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Presence of Mind") != std::string::npos)
                {
                    if (!m_spells.mage.pPresenceOfMind ||
                        m_spells.mage.pPresenceOfMind->Rank < pSpellEntry->Rank)
                        m_spells.mage.pPresenceOfMind = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Arcane Power") != std::string::npos)
                {
                    if (!m_spells.mage.pArcanePower ||
                        m_spells.mage.pArcanePower->Rank < pSpellEntry->Rank)
                        m_spells.mage.pArcanePower = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Remove Lesser Curse") != std::string::npos)
                {
                    if (!m_spells.mage.pRemoveLesserCurse ||
                        m_spells.mage.pRemoveLesserCurse->Rank < pSpellEntry->Rank)
                        m_spells.mage.pRemoveLesserCurse = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Scorch") != std::string::npos)
                {
                    if (!m_spells.mage.pScorch ||
                        m_spells.mage.pScorch->Rank < pSpellEntry->Rank)
                        m_spells.mage.pScorch = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Pyroblast") != std::string::npos)
                {
                    if (!m_spells.mage.pPyroblast ||
                        m_spells.mage.pPyroblast->Rank < pSpellEntry->Rank)
                        m_spells.mage.pPyroblast = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Evocation") != std::string::npos)
                {
                    if (!m_spells.mage.pEvocation ||
                        m_spells.mage.pEvocation->Rank < pSpellEntry->Rank)
                        m_spells.mage.pEvocation = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Ice Block") != std::string::npos)
                {
                    if (!m_spells.mage.pIceBlock ||
                        m_spells.mage.pIceBlock->Rank < pSpellEntry->Rank)
                        m_spells.mage.pIceBlock = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Blizzard") != std::string::npos)
                {
                    if (!m_spells.mage.pBlizzard ||
                        m_spells.mage.pBlizzard->Rank < pSpellEntry->Rank)
                        m_spells.mage.pBlizzard = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Blast Wave") != std::string::npos)
                {
                    if (!m_spells.mage.pBlastWave ||
                        m_spells.mage.pBlastWave->Rank < pSpellEntry->Rank)
                        m_spells.mage.pBlastWave = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Combustion") != std::string::npos)
                {
                    if (!m_spells.mage.pCombustion ||
                        m_spells.mage.pCombustion->Rank < pSpellEntry->Rank)
                        m_spells.mage.pCombustion = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Flamestrike") != std::string::npos)
                {
                    if (!m_spells.mage.pFlamestrike ||
                        m_spells.mage.pFlamestrike->Rank < pSpellEntry->Rank)
                        m_spells.mage.pFlamestrike = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Arcane Missiles") != std::string::npos)
                {
                if (!m_spells.mage.pArcaneMissiles ||
                    m_spells.mage.pArcaneMissiles->Rank < pSpellEntry->Rank)
                    m_spells.mage.pArcaneMissiles = pSpellEntry;
                }
                break;
            }
            case CLASS_PRIEST:
            {
                if (pSpellEntry->SpellName[0].find("Power Word: Fortitude") != std::string::npos)
                {
                    if (!m_spells.priest.pPowerWordFortitude ||
                        m_spells.priest.pPowerWordFortitude->Rank < pSpellEntry->Rank)
                        m_spells.priest.pPowerWordFortitude = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Divine Spirit") != std::string::npos)
                {
                    if (!m_spells.priest.pDivineSpirit ||
                        m_spells.priest.pDivineSpirit->Rank < pSpellEntry->Rank)
                        m_spells.priest.pDivineSpirit = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Prayer of Spirit") != std::string::npos)
                {
                    if (!m_spells.priest.pPrayerofSpirit ||
                        m_spells.priest.pPrayerofSpirit->Rank < pSpellEntry->Rank)
                        m_spells.priest.pPrayerofSpirit = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Prayer of Fortitude") != std::string::npos)
                {
                    if (!m_spells.priest.pPrayerofFortitude ||
                        m_spells.priest.pPrayerofFortitude->Rank < pSpellEntry->Rank)
                        m_spells.priest.pPrayerofFortitude = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Prayer of Healing") != std::string::npos)
                {
                    if (!m_spells.priest.pPrayerofHealing ||
                        m_spells.priest.pPrayerofHealing->Rank < pSpellEntry->Rank)
                        m_spells.priest.pPrayerofHealing = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Inner Fire") != std::string::npos)
                {
                    if (!m_spells.priest.pInnerFire ||
                        m_spells.priest.pInnerFire->Rank < pSpellEntry->Rank)
                        m_spells.priest.pInnerFire = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Shadow Protection") != std::string::npos)
                {
                    if (!m_spells.priest.pShadowProtection ||
                        m_spells.priest.pShadowProtection->Rank < pSpellEntry->Rank)
                        m_spells.priest.pShadowProtection = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Power Word: Shield") != std::string::npos)
                {
                    if (!m_spells.priest.pPowerWordShield ||
                        m_spells.priest.pPowerWordShield->Rank < pSpellEntry->Rank)
                        m_spells.priest.pPowerWordShield = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Holy Nova") != std::string::npos)
                {
                    if (!m_spells.priest.pHolyNova ||
                        m_spells.priest.pHolyNova->Rank < pSpellEntry->Rank)
                        m_spells.priest.pHolyNova = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Holy Fire") != std::string::npos)
                {
                    if (!m_spells.priest.pHolyFire ||
                        m_spells.priest.pHolyFire->Rank < pSpellEntry->Rank)
                        m_spells.priest.pHolyFire = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Smite") != std::string::npos)
                {
                    if (!m_spells.priest.pSmite ||
                        m_spells.priest.pSmite->Rank < pSpellEntry->Rank)
                        m_spells.priest.pSmite = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Mind Blast") != std::string::npos)
                {
                    if (!m_spells.priest.pMindBlast ||
                        m_spells.priest.pMindBlast->Rank < pSpellEntry->Rank)
                        m_spells.priest.pMindBlast = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Mind Flay") != std::string::npos)
                {
                    if (!m_spells.priest.pMindFlay ||
                        m_spells.priest.pMindFlay->Rank < pSpellEntry->Rank)
                        m_spells.priest.pMindFlay = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Shadow Word: Pain") != std::string::npos)
                {
                    if (!m_spells.priest.pShadowWordPain ||
                        m_spells.priest.pShadowWordPain->Rank < pSpellEntry->Rank)
                        m_spells.priest.pShadowWordPain = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Inner Focus") != std::string::npos)
                {
                    if (!m_spells.priest.pInnerFocus ||
                        m_spells.priest.pInnerFocus->Rank < pSpellEntry->Rank)
                        m_spells.priest.pInnerFocus = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Abolish Disease") != std::string::npos)
                {
                    if (!m_spells.priest.pAbolishDisease ||
                        m_spells.priest.pAbolishDisease->Rank < pSpellEntry->Rank)
                        m_spells.priest.pAbolishDisease = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Dispel Magic") != std::string::npos)
                {
                    if (!m_spells.priest.pDispelMagic ||
                        m_spells.priest.pDispelMagic->Rank < pSpellEntry->Rank)
                        m_spells.priest.pDispelMagic = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Mana Burn") != std::string::npos)
                {
                    if (!m_spells.priest.pManaBurn ||
                        m_spells.priest.pManaBurn->Rank < pSpellEntry->Rank)
                        m_spells.priest.pManaBurn = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Devouring Plague") != std::string::npos)
                {
                    if (!m_spells.priest.pDevouringPlague ||
                        m_spells.priest.pDevouringPlague->Rank < pSpellEntry->Rank)
                        m_spells.priest.pDevouringPlague = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Psychic Scream") != std::string::npos)
                {
                    if (!m_spells.priest.pPsychicScream ||
                        m_spells.priest.pPsychicScream->Rank < pSpellEntry->Rank)
                        m_spells.priest.pPsychicScream = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Shadowform") != std::string::npos)
                {
                    if (!m_spells.priest.pShadowform ||
                        m_spells.priest.pShadowform->Rank < pSpellEntry->Rank)
                        m_spells.priest.pShadowform = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Vampiric Embrace") != std::string::npos)
                {
                    if (!m_spells.priest.pVampiricEmbrace ||
                        m_spells.priest.pVampiricEmbrace->Rank < pSpellEntry->Rank)
                        m_spells.priest.pVampiricEmbrace = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Silence") != std::string::npos)
                {
                    if (!m_spells.priest.pSilence ||
                        m_spells.priest.pSilence->Rank < pSpellEntry->Rank)
                        m_spells.priest.pSilence = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Fade") != std::string::npos)
                {
                    if (!m_spells.priest.pFade ||
                        m_spells.priest.pFade->Rank < pSpellEntry->Rank)
                        m_spells.priest.pFade = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Shackle Undead") != std::string::npos)
                {
                    if (!m_spells.priest.pShackleUndead ||
                        m_spells.priest.pShackleUndead->Rank < pSpellEntry->Rank)
                        m_spells.priest.pShackleUndead = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Touch of Weakness") != std::string::npos)
                {
                if (!m_spells.priest.pTouchOfWeakness ||
                    m_spells.priest.pTouchOfWeakness->Rank < pSpellEntry->Rank)
                    m_spells.priest.pTouchOfWeakness = pSpellEntry;
                }
                break;
            }
            case CLASS_WARLOCK:
            {
                if (pSpellEntry->SpellName[0].find("Demon Armor") != std::string::npos)
                {
                    if (!m_spells.warlock.pDemonArmor ||
                        m_spells.warlock.pDemonArmor->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pDemonArmor = pSpellEntry;
                }
                if (pSpellEntry->SpellName[0].find("Demon Skin") != std::string::npos)
                {
                    if (!m_spells.warlock.pDemonSkin ||
                        m_spells.warlock.pDemonSkin->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pDemonSkin = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Death Coil") != std::string::npos)
                {
                    if (!m_spells.warlock.pDeathCoil ||
                        m_spells.warlock.pDeathCoil->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pDeathCoil = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Detect Invisibility") != std::string::npos)
                {
                    if (!m_spells.warlock.pDetectInvisibility ||
                        m_spells.warlock.pDetectInvisibility->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pDetectInvisibility = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Shadow Ward") != std::string::npos)
                {
                    if (!m_spells.warlock.pShadowWard ||
                        m_spells.warlock.pShadowWard->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pShadowWard = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Shadow Bolt") != std::string::npos)
                {
                    if (!m_spells.warlock.pShadowBolt ||
                        m_spells.warlock.pShadowBolt->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pShadowBolt = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Corruption") != std::string::npos)
                {
                    if (!m_spells.warlock.pCorruption ||
                        m_spells.warlock.pCorruption->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pCorruption = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Conflagrate") != std::string::npos)
                {
                    if (!m_spells.warlock.pConflagrate ||
                        m_spells.warlock.pConflagrate->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pConflagrate = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Shadowburn") != std::string::npos)
                {
                    if (!m_spells.warlock.pShadowburn ||
                        m_spells.warlock.pShadowburn->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pShadowburn = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Searing Pain") != std::string::npos)
                {
                    if (!m_spells.warlock.pSearingPain ||
                        m_spells.warlock.pSearingPain->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pSearingPain = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Immolate") != std::string::npos)
                {
                    if (!m_spells.warlock.pImmolate ||
                        m_spells.warlock.pImmolate->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pImmolate = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Rain of Fire") != std::string::npos)
                {
                    if (!m_spells.warlock.pRainOfFire ||
                        m_spells.warlock.pRainOfFire->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pRainOfFire = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Demonic Sacrifice") != std::string::npos)
                {
                    if (!m_spells.warlock.pDemonicSacrifice ||
                        m_spells.warlock.pDemonicSacrifice->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pDemonicSacrifice = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Drain Life") != std::string::npos)
                {
                    if (!m_spells.warlock.pDrainLife ||
                        m_spells.warlock.pDrainLife->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pDrainLife = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Siphon Life") != std::string::npos)
                {
                    if (!m_spells.warlock.pSiphonLife ||
                        m_spells.warlock.pSiphonLife->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pSiphonLife = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Banish") != std::string::npos)
                {
                    if (!m_spells.warlock.pBanish ||
                        m_spells.warlock.pBanish->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pBanish = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Fear") != std::string::npos)
                {
                    if (!m_spells.warlock.pFear ||
                        m_spells.warlock.pFear->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pFear = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Howl of Terror") != std::string::npos)
                {
                    if (!m_spells.warlock.pHowlofTerror ||
                        m_spells.warlock.pHowlofTerror->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pHowlofTerror = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Curse of Agony") != std::string::npos)
                {
                    if (!m_spells.warlock.pCurseofAgony ||
                        m_spells.warlock.pCurseofAgony->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pCurseofAgony = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Curse of the Elements") != std::string::npos)
                {
                    if (!m_spells.warlock.pCurseoftheElements ||
                        m_spells.warlock.pCurseoftheElements->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pCurseoftheElements = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Curse of Shadow") != std::string::npos)
                {
                    if (!m_spells.warlock.pCurseofShadow ||
                        m_spells.warlock.pCurseofShadow->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pCurseofShadow = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Curse of Recklessness") != std::string::npos)
                {
                    if (!m_spells.warlock.pCurseofRecklessness ||
                        m_spells.warlock.pCurseofRecklessness->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pCurseofRecklessness = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Curse of Tongues") != std::string::npos)
                {
                    if (!m_spells.warlock.pCurseofTongues ||
                        m_spells.warlock.pCurseofTongues->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pCurseofTongues = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Life Tap") != std::string::npos)
                {
                    if (!m_spells.warlock.pLifeTap ||
                        m_spells.warlock.pLifeTap->Rank < pSpellEntry->Rank)
                        m_spells.warlock.pLifeTap = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Dark Pact") != std::string::npos)
                {
                if (!m_spells.warlock.pDarkPact ||
                    m_spells.warlock.pDarkPact->Rank < pSpellEntry->Rank)
                    m_spells.warlock.pDarkPact = pSpellEntry;
                }
                break;
            }
            case CLASS_WARRIOR:
            {
                if (pSpellEntry->SpellName[0].find("Battle Stance") != std::string::npos)
                {
                    if (!m_spells.warrior.pBattleStance ||
                        m_spells.warrior.pBattleStance->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pBattleStance = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Berserker Stance") != std::string::npos)
                {
                    if (!m_spells.warrior.pBerserkerStance ||
                        m_spells.warrior.pBerserkerStance->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pBerserkerStance = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Defensive Stance") != std::string::npos)
                {
                    if (!m_spells.warrior.pDefensiveStance ||
                        m_spells.warrior.pDefensiveStance->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pDefensiveStance = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Charge") != std::string::npos)
                {
                    if (!m_spells.warrior.pCharge ||
                        m_spells.warrior.pCharge->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pCharge = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Intercept") != std::string::npos)
                {
                    if (!m_spells.warrior.pIntercept ||
                        m_spells.warrior.pIntercept->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pIntercept = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Overpower") != std::string::npos)
                {
                    if (!m_spells.warrior.pOverpower ||
                        m_spells.warrior.pOverpower->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pOverpower = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Heroic Strike") != std::string::npos)
                {
                    if (!m_spells.warrior.pHeroicStrike ||
                        m_spells.warrior.pHeroicStrike->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pHeroicStrike = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Cleave") != std::string::npos)
                {
                    if (!m_spells.warrior.pCleave ||
                        m_spells.warrior.pCleave->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pCleave = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Execute") != std::string::npos)
                {
                    if (!m_spells.warrior.pExecute ||
                        m_spells.warrior.pExecute->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pExecute = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Mortal Strike") != std::string::npos)
                {
                    if (!m_spells.warrior.pMortalStrike ||
                        m_spells.warrior.pMortalStrike->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pMortalStrike = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Bloodthirst") != std::string::npos)
                {
                    if (!m_spells.warrior.pBloodthirst ||
                        m_spells.warrior.pBloodthirst->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pBloodthirst = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Bloodrage") != std::string::npos)
                {
                    if (!m_spells.warrior.pBloodrage ||
                        m_spells.warrior.pBloodrage->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pBloodrage = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Berserker Rage") != std::string::npos)
                {
                    if (!m_spells.warrior.pBerserkerRage ||
                        m_spells.warrior.pBerserkerRage->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pBerserkerRage = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Recklessness") != std::string::npos)
                {
                    if (!m_spells.warrior.pRecklessness ||
                        m_spells.warrior.pRecklessness->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pRecklessness = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Retaliation") != std::string::npos)
                {
                    if (!m_spells.warrior.pRetaliation ||
                        m_spells.warrior.pRetaliation->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pRetaliation = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Death Wish") != std::string::npos)
                {
                    if (!m_spells.warrior.pDeathWish ||
                        m_spells.warrior.pDeathWish->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pDeathWish = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Intimidating Shout") != std::string::npos)
                {
                    if (!m_spells.warrior.pIntimidatingShout ||
                        m_spells.warrior.pIntimidatingShout->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pIntimidatingShout = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Pummel") != std::string::npos)
                {
                    if (!m_spells.warrior.pPummel ||
                        m_spells.warrior.pPummel->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pPummel = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Rend") != std::string::npos)
                {
                    if (!m_spells.warrior.pRend ||
                        m_spells.warrior.pRend->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pRend = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Disarm") != std::string::npos)
                {
                    if (!m_spells.warrior.pDisarm ||
                        m_spells.warrior.pDisarm->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pDisarm = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Whirlwind") != std::string::npos)
                {
                    if (!m_spells.warrior.pWhirlwind ||
                        m_spells.warrior.pWhirlwind->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pWhirlwind = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Battle Shout") != std::string::npos)
                {
                    if (!m_spells.warrior.pBattleShout ||
                        m_spells.warrior.pBattleShout->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pBattleShout = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Demoralizing Shout") != std::string::npos)
                {
                    if (!m_spells.warrior.pDemoralizingShout ||
                        m_spells.warrior.pDemoralizingShout->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pDemoralizingShout = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Hamstring") != std::string::npos)
                {
                    if (!m_spells.warrior.pHamstring ||
                        m_spells.warrior.pHamstring->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pHamstring = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Thunder Clap") != std::string::npos)
                {
                    if (!m_spells.warrior.pThunderClap ||
                        m_spells.warrior.pThunderClap->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pThunderClap = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Sweeping Strikes") != std::string::npos)
                {
                    if (!m_spells.warrior.pSweepingStrikes ||
                        m_spells.warrior.pSweepingStrikes->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pSweepingStrikes = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Last Stand") != std::string::npos)
                {
                    if (!m_spells.warrior.pLastStand ||
                        m_spells.warrior.pLastStand->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pLastStand = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Shield Block") != std::string::npos)
                {
                    if (!m_spells.warrior.pShieldBlock ||
                        m_spells.warrior.pShieldBlock->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pShieldBlock = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Shield Wall") != std::string::npos)
                {
                    if (!m_spells.warrior.pShieldWall ||
                        m_spells.warrior.pShieldWall->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pShieldWall = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Shield Bash") != std::string::npos)
                {
                    if (!m_spells.warrior.pShieldBash ||
                        m_spells.warrior.pShieldBash->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pShieldBash = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Shield Slam") != std::string::npos)
                {
                    if (!m_spells.warrior.pShieldSlam ||
                        m_spells.warrior.pShieldSlam->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pShieldSlam = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Sunder Armor") != std::string::npos)
                {
                    if (!m_spells.warrior.pSunderArmor ||
                        m_spells.warrior.pSunderArmor->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pSunderArmor = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Concussion Blow") != std::string::npos)
                {
                    if (!m_spells.warrior.pConcussionBlow ||
                        m_spells.warrior.pConcussionBlow->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pConcussionBlow = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Piercing Howl") != std::string::npos)
                {
                    if (!m_spells.warrior.pPiercingHowl ||
                        m_spells.warrior.pPiercingHowl->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pPiercingHowl = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Revenge") != std::string::npos)
                {
                    if (!m_spells.warrior.pRevenge ||
                        m_spells.warrior.pRevenge->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pRevenge = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0]._Equal("Slam"))
                {
                    if (!m_spells.warrior.pSlam ||
                        m_spells.warrior.pSlam->Rank < pSpellEntry->Rank)
                        m_spells.warrior.pSlam = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Challenging Shout") != std::string::npos)
                {
                if (!m_spells.warrior.pChallengingShout ||
                    m_spells.warrior.pChallengingShout->Rank < pSpellEntry->Rank)
                    m_spells.warrior.pChallengingShout = pSpellEntry;
                }
                break;
            }
            case CLASS_ROGUE:
            {
                if (pSpellEntry->SpellName[0].find("Slice and Dice") != std::string::npos)
                {
                    if (!m_spells.rogue.pSliceAndDice ||
                        m_spells.rogue.pSliceAndDice->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pSliceAndDice = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Sinister Strike") != std::string::npos)
                {
                    if (!m_spells.rogue.pSinisterStrike ||
                        m_spells.rogue.pSinisterStrike->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pSinisterStrike = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Adrenaline Rush") != std::string::npos)
                {
                    if (!m_spells.rogue.pAdrenalineRush ||
                        m_spells.rogue.pAdrenalineRush->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pAdrenalineRush = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Eviscerate") != std::string::npos)
                {
                    if (!m_spells.rogue.pEviscerate ||
                        m_spells.rogue.pEviscerate->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pEviscerate = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Stealth") != std::string::npos)
                {
                    if (!m_spells.rogue.pStealth ||
                        m_spells.rogue.pStealth->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pStealth = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Garrote") != std::string::npos)
                {
                    if (!m_spells.rogue.pGarrote ||
                        m_spells.rogue.pGarrote->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pGarrote = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Ambush") != std::string::npos)
                {
                    if (!m_spells.rogue.pAmbush ||
                        m_spells.rogue.pAmbush->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pAmbush = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Cheap Shot") != std::string::npos)
                {
                    if (!m_spells.rogue.pCheapShot ||
                        m_spells.rogue.pCheapShot->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pCheapShot = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Premeditation") != std::string::npos)
                {
                    if (!m_spells.rogue.pPremeditation ||
                        m_spells.rogue.pPremeditation->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pPremeditation = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Backstab") != std::string::npos)
                {
                    if (!m_spells.rogue.pBackstab ||
                        m_spells.rogue.pBackstab->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pBackstab = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Hemorrhage") != std::string::npos)
                {
                    if (!m_spells.rogue.pHemorrhage ||
                        m_spells.rogue.pHemorrhage->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pHemorrhage = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Ghostly Strike") != std::string::npos)
                {
                    if (!m_spells.rogue.pGhostlyStrike ||
                        m_spells.rogue.pGhostlyStrike->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pGhostlyStrike = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Gouge") != std::string::npos)
                {
                    if (!m_spells.rogue.pGouge ||
                        m_spells.rogue.pGouge->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pGouge = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Rupture") != std::string::npos)
                {
                    if (!m_spells.rogue.pRupture ||
                        m_spells.rogue.pRupture->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pRupture = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Expose Armor") != std::string::npos)
                {
                    if (!m_spells.rogue.pExposeArmor ||
                        m_spells.rogue.pExposeArmor->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pExposeArmor = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Kidney Shot") != std::string::npos)
                {
                    if (!m_spells.rogue.pKidneyShot ||
                        m_spells.rogue.pKidneyShot->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pKidneyShot = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Cold Blood") != std::string::npos)
                {
                    if (!m_spells.rogue.pColdBlood ||
                        m_spells.rogue.pColdBlood->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pColdBlood = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Blade Flurry") != std::string::npos)
                {
                    if (!m_spells.rogue.pBladeFlurry ||
                        m_spells.rogue.pBladeFlurry->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pBladeFlurry = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Vanish") != std::string::npos)
                {
                    if (!m_spells.rogue.pVanish ||
                        m_spells.rogue.pVanish->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pVanish = pSpellEntry;
                }
                else if (pSpellEntry->IsFitToFamily<SPELLFAMILY_ROGUE, CF_ROGUE_BLIND>())
                {
                    if (!m_spells.rogue.pBlind ||
                        m_spells.rogue.pBlind->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pBlind = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Preparation") != std::string::npos)
                {
                    if (!m_spells.rogue.pPreparation ||
                        m_spells.rogue.pPreparation->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pPreparation = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Sap") != std::string::npos)
                {
                if (!m_spells.rogue.pSap ||
                    m_spells.rogue.pSap->Rank < pSpellEntry->Rank)
                    m_spells.rogue.pSap = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Evasion") != std::string::npos)
                {
                    if (!m_spells.rogue.pEvasion ||
                        m_spells.rogue.pEvasion->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pEvasion = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Riposte") != std::string::npos)
                {
                    if (!m_spells.rogue.pRiposte ||
                        m_spells.rogue.pRiposte->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pRiposte = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Kick") != std::string::npos)
                {
                    if (!m_spells.rogue.pKick ||
                        m_spells.rogue.pKick->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pKick = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Sprint") != std::string::npos)
                {
                    if (!m_spells.rogue.pSprint ||
                        m_spells.rogue.pSprint->Rank < pSpellEntry->Rank)
                        m_spells.rogue.pSprint = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Deadly Poison") != std::string::npos)
                {
                    hasDeadlyPoison = true;
                }
                else if (pSpellEntry->SpellName[0].find("Instant Poison") != std::string::npos)
                {
                    hasInstantPoison = true;
                }
                else if (pSpellEntry->SpellName[0].find("Crippling Poison") != std::string::npos)
                {
                    hasCripplingPoison = true;
                }
                else if (pSpellEntry->SpellName[0].find("Wound Poison") != std::string::npos)
                {
                    hasWoundPoison = true;
                }
                else if (pSpellEntry->SpellName[0].find("Mind-numbing Poison") != std::string::npos)
                {
                    HasMindNumbingPoison = true;
                }
                break;
            }
            case CLASS_DRUID:
            {
                if (pSpellEntry->SpellName[0].find("Bear Form") != std::string::npos)
                {
                    if (!m_spells.druid.pBearForm ||
                        m_spells.druid.pBearForm->Rank < pSpellEntry->Rank)
                        m_spells.druid.pBearForm = pSpellEntry;
                }
                else if (pSpellEntry->Id == (768)) // Cat Form
                {
                    if (!m_spells.druid.pCatForm ||
                        m_spells.druid.pCatForm->Rank < pSpellEntry->Rank)
                        m_spells.druid.pCatForm = pSpellEntry;
                }
                else if (pSpellEntry->Id == (783)) // Travel Form
                {
                    if (!m_spells.druid.pTravelForm ||
                        m_spells.druid.pTravelForm->Rank < pSpellEntry->Rank)
                        m_spells.druid.pTravelForm = pSpellEntry;
                }
                else if (pSpellEntry->Id == (1066)) // Aquatic Form
                {
                    if (!m_spells.druid.pAquaticForm ||
                        m_spells.druid.pAquaticForm->Rank < pSpellEntry->Rank)
                        m_spells.druid.pAquaticForm = pSpellEntry;
                }
                else if (pSpellEntry->Id == (24858)) // Moonkin Form
                {
                    if (!m_spells.druid.pMoonkinForm ||
                        m_spells.druid.pMoonkinForm->Rank < pSpellEntry->Rank)
                        m_spells.druid.pMoonkinForm = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Wrath") != std::string::npos)
                {
                    if (!m_spells.druid.pWrath ||
                        m_spells.druid.pWrath->Rank < pSpellEntry->Rank)
                        m_spells.druid.pWrath = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Moonfire") != std::string::npos)
                {
                    if (!m_spells.druid.pMoonfire ||
                        m_spells.druid.pMoonfire->Rank < pSpellEntry->Rank)
                        m_spells.druid.pMoonfire = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Starfire") != std::string::npos)
                {
                    if (!m_spells.druid.pStarfire ||
                        m_spells.druid.pStarfire->Rank < pSpellEntry->Rank)
                        m_spells.druid.pStarfire = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Hurricane") != std::string::npos)
                {
                    if (!m_spells.druid.pHurricane ||
                        m_spells.druid.pHurricane->Rank < pSpellEntry->Rank)
                        m_spells.druid.pHurricane = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Insect Swarm") != std::string::npos)
                {
                    if (!m_spells.druid.pInsectSwarm ||
                        m_spells.druid.pInsectSwarm->Rank < pSpellEntry->Rank)
                        m_spells.druid.pInsectSwarm = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Barkskin") != std::string::npos)
                {
                    if (!m_spells.druid.pBarkskin ||
                        m_spells.druid.pBarkskin->Rank < pSpellEntry->Rank)
                        m_spells.druid.pBarkskin = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Nature's Grasp") != std::string::npos)
                {
                    if (!m_spells.druid.pNaturesGrasp ||
                        m_spells.druid.pNaturesGrasp->Rank < pSpellEntry->Rank)
                        m_spells.druid.pNaturesGrasp = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Mark of the Wild") != std::string::npos)
                {
                    if (!m_spells.druid.pMarkoftheWild ||
                        m_spells.druid.pMarkoftheWild->Rank < pSpellEntry->Rank)
                        m_spells.druid.pMarkoftheWild = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Gift of the Wild") != std::string::npos)
                {
                    if (!m_spells.druid.pGiftoftheWild ||
                        m_spells.druid.pGiftoftheWild->Rank < pSpellEntry->Rank)
                        m_spells.druid.pGiftoftheWild = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Thorns") != std::string::npos)
                {
                    if (!m_spells.druid.pThorns ||
                        m_spells.druid.pThorns->Rank < pSpellEntry->Rank)
                        m_spells.druid.pThorns = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Cure Poison") != std::string::npos)
                {
                    if (!m_spells.druid.pCurePoison ||
                        m_spells.druid.pCurePoison->Rank < pSpellEntry->Rank)
                        m_spells.druid.pCurePoison = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Abolish Poison") != std::string::npos)
                {
                    if (!m_spells.druid.pAbolishPoison ||
                        m_spells.druid.pAbolishPoison->Rank < pSpellEntry->Rank)
                        m_spells.druid.pAbolishPoison = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Rebirth") != std::string::npos)
                {
                    if (!m_spells.druid.pRebirth ||
                        m_spells.druid.pRebirth->Rank < pSpellEntry->Rank)
                        m_spells.druid.pRebirth = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Innervate") != std::string::npos)
                {
                    if (!m_spells.druid.pInnervate ||
                        m_spells.druid.pInnervate->Rank < pSpellEntry->Rank)
                        m_spells.druid.pInnervate = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Nature's Swiftness") != std::string::npos)
                {
                    if (!m_spells.druid.pNaturesSwiftness ||
                        m_spells.druid.pNaturesSwiftness->Rank < pSpellEntry->Rank)
                        m_spells.druid.pNaturesSwiftness = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Entangling Roots") != std::string::npos)
                {
                    if (!m_spells.druid.pEntanglingRoots ||
                        m_spells.druid.pEntanglingRoots->Rank < pSpellEntry->Rank)
                        m_spells.druid.pEntanglingRoots = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Hibernate") != std::string::npos)
                {
                    if (!m_spells.druid.pHibernate ||
                        m_spells.druid.pHibernate->Rank < pSpellEntry->Rank)
                        m_spells.druid.pHibernate = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Pounce") != std::string::npos)
                {
                    if (!m_spells.druid.pPounce ||
                        m_spells.druid.pPounce->Rank < pSpellEntry->Rank)
                        m_spells.druid.pPounce = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Ravage") != std::string::npos)
                {
                    if (!m_spells.druid.pRavage ||
                        m_spells.druid.pRavage->Rank < pSpellEntry->Rank)
                        m_spells.druid.pRavage = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Claw") != std::string::npos)
                {
                    if (!m_spells.druid.pClaw ||
                        m_spells.druid.pClaw->Rank < pSpellEntry->Rank)
                        m_spells.druid.pClaw = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Shred") != std::string::npos)
                {
                    if (!m_spells.druid.pShred ||
                        m_spells.druid.pShred->Rank < pSpellEntry->Rank)
                        m_spells.druid.pShred = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Rake") != std::string::npos)
                {
                    if (!m_spells.druid.pRake ||
                        m_spells.druid.pRake->Rank < pSpellEntry->Rank)
                        m_spells.druid.pRake = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Rip") != std::string::npos)
                {
                    if (!m_spells.druid.pRip ||
                        m_spells.druid.pRip->Rank < pSpellEntry->Rank)
                        m_spells.druid.pRip = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Ferocious Bite") != std::string::npos)
                {
                    if (!m_spells.druid.pFerociousBite ||
                        m_spells.druid.pFerociousBite->Rank < pSpellEntry->Rank)
                        m_spells.druid.pFerociousBite = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Tiger's Fury") != std::string::npos)
                {
                    if (!m_spells.druid.pTigersFury ||
                        m_spells.druid.pTigersFury->Rank < pSpellEntry->Rank)
                        m_spells.druid.pTigersFury = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Dash") != std::string::npos)
                {
                    if (!m_spells.druid.pDash ||
                        m_spells.druid.pDash->Rank < pSpellEntry->Rank)
                        m_spells.druid.pDash = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Cower") != std::string::npos)
                {
                    if (!m_spells.druid.pCower ||
                        m_spells.druid.pCower->Rank < pSpellEntry->Rank)
                        m_spells.druid.pCower = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Faerie Fire (Feral)") != std::string::npos)
                {
                    if (!m_spells.druid.pFaerieFireFeral ||
                        m_spells.druid.pFaerieFireFeral->Rank < pSpellEntry->Rank)
                        m_spells.druid.pFaerieFireFeral = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Faerie Fire") != std::string::npos)
                {
                    if (!m_spells.druid.pFaerieFire ||
                        m_spells.druid.pFaerieFire->Rank < pSpellEntry->Rank)
                        m_spells.druid.pFaerieFire = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Growl") != std::string::npos)
                {
                    if (!m_spells.druid.pGrowl ||
                        m_spells.druid.pGrowl->Rank < pSpellEntry->Rank)
                        m_spells.druid.pGrowl = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Challenging Roar") != std::string::npos)
                {
                    if (!m_spells.druid.pChallengingRoar ||
                        m_spells.druid.pChallengingRoar->Rank < pSpellEntry->Rank)
                        m_spells.druid.pChallengingRoar = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Demoralizing Roar") != std::string::npos)
                {
                    if (!m_spells.druid.pDemoralizingRoar ||
                        m_spells.druid.pDemoralizingRoar->Rank < pSpellEntry->Rank)
                        m_spells.druid.pDemoralizingRoar = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Enrage") != std::string::npos)
                {
                    if (!m_spells.druid.pEnrage ||
                        m_spells.druid.pEnrage->Rank < pSpellEntry->Rank)
                        m_spells.druid.pEnrage = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Frenzied Regeneration") != std::string::npos)
                {
                    if (!m_spells.druid.pFrenziedRegeneration ||
                        m_spells.druid.pFrenziedRegeneration->Rank < pSpellEntry->Rank)
                        m_spells.druid.pFrenziedRegeneration = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Swipe") != std::string::npos)
                {
                    if (!m_spells.druid.pSwipe ||
                        m_spells.druid.pSwipe->Rank < pSpellEntry->Rank)
                        m_spells.druid.pSwipe = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Maul") != std::string::npos)
                {
                    if (!m_spells.druid.pMaul ||
                        m_spells.druid.pMaul->Rank < pSpellEntry->Rank)
                        m_spells.druid.pMaul = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Bash") != std::string::npos)
                {
                    if (!m_spells.druid.pBash ||
                        m_spells.druid.pBash->Rank < pSpellEntry->Rank)
                        m_spells.druid.pBash = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Feral Charge") != std::string::npos)
                {
                    if (!m_spells.druid.pFeralCharge ||
                        m_spells.druid.pFeralCharge->Rank < pSpellEntry->Rank)
                        m_spells.druid.pFeralCharge = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Prowl") != std::string::npos)
                {
                    if (!m_spells.druid.pProwl ||
                        m_spells.druid.pProwl->Rank < pSpellEntry->Rank)
                        m_spells.druid.pProwl = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Swiftmend") != std::string::npos)
                {
                    if (!m_spells.druid.pSwiftmend ||
                        m_spells.druid.pSwiftmend->Rank < pSpellEntry->Rank)
                        m_spells.druid.pSwiftmend = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Omen of Clarity") != std::string::npos)
                {
                    if (!m_spells.druid.pOmenOfClarity ||
                        m_spells.druid.pOmenOfClarity->Rank < pSpellEntry->Rank)
                        m_spells.druid.pOmenOfClarity = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Tranquility") != std::string::npos)
                {
                    if (!m_spells.druid.pTranquility ||
                        m_spells.druid.pTranquility->Rank < pSpellEntry->Rank)
                        m_spells.druid.pTranquility = pSpellEntry;
                }
                else if (pSpellEntry->SpellName[0].find("Remove Curse") != std::string::npos)
                {
                    if (!m_spells.druid.pRemoveCurse ||
                        m_spells.druid.pRemoveCurse->Rank < pSpellEntry->Rank)
                        m_spells.druid.pRemoveCurse = pSpellEntry;
                }
                break;
            }
        }

        for (uint32 i = 0; i < MAX_SPELL_EFFECTS; i++)
        {
            switch (pSpellEntry->Effect[i])
            {
                case SPELL_EFFECT_HEAL:
                    spellListDirectHeal.insert(pSpellEntry);
                    break;
                case SPELL_EFFECT_ATTACK_ME:
                    spellListTaunt.push_back(pSpellEntry);
                    break;
                case SPELL_EFFECT_RESURRECT:
                case SPELL_EFFECT_RESURRECT_NEW:
                    m_resurrectionSpell = pSpellEntry;
                    break;
                case SPELL_EFFECT_APPLY_AURA:
                {
                    switch (pSpellEntry->EffectApplyAuraName[i])
                    {
                        case SPELL_AURA_PERIODIC_HEAL:
                            spellListPeriodicHeal.insert(pSpellEntry);
                            break;
                        case SPELL_AURA_MOD_TAUNT:
                        {
                            if(pSpellEntry->Id != SPELL_CHALLENGING_SHOUT)
                                spellListTaunt.push_back(pSpellEntry);
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }

    switch (me->GetClass())
    {
        case CLASS_PALADIN:
        {
            if (pSealOfFury && m_role == ROLE_TANK)
                m_spells.paladin.pSeal = pSealOfFury;
            else if (pSealOfCommand)
                m_spells.paladin.pSeal = pSealOfCommand;
            else
                m_spells.paladin.pSeal = pSealOfRighteousness;

            if (pBlessingOfSanctuary && m_role == ROLE_TANK)
            {
                m_spells.paladin.pBlessingBuff = pBlessingOfSanctuary;
                m_spells.paladin.pBlessingBuffRanged = pBlessingOfSanctuary;
            }
            else if (pBlessingOfLight && m_role == ROLE_HEALER)
            {
                m_spells.paladin.pBlessingBuff = pBlessingOfLight;
                m_spells.paladin.pBlessingBuffRanged = pBlessingOfLight;
            }
            else
            {
                std::vector<SpellEntry const*> blessings;
                std::vector<SpellEntry const*> blessingsRanged;

                if (pBlessingOfMight)
                    blessings.push_back(pBlessingOfMight);
                if (pBlessingOfWisdom)
                    blessingsRanged.push_back(pBlessingOfWisdom);
                if (pBlessingOfKings)
                {
                    blessings.push_back(pBlessingOfKings);
                    blessingsRanged.push_back(pBlessingOfKings);
                }

                if (!blessings.empty())
                    m_spells.paladin.pBlessingBuff = SelectRandomContainerElement(blessings);
                if (!blessingsRanged.empty())
                    m_spells.paladin.pBlessingBuffRanged = SelectRandomContainerElement(blessingsRanged);
            }

            std::vector<SpellEntry const*> auras;
            if (pDevotionAura)
                auras.push_back(pDevotionAura);
            if (pConcentrationAura)
                auras.push_back(pConcentrationAura);
            if (pRetributionAura)
                auras.push_back(pRetributionAura);
            /*if (pSanctityAura)
                auras.push_back(pSanctityAura);
            if (pShadowResistanceAura)
                auras.push_back(pShadowResistanceAura);
            if (pFrostResistanceAura)
                auras.push_back(pFrostResistanceAura);
            if (pFireResistanceAura)
                auras.push_back(pFireResistanceAura);*/
            if (!auras.empty())
                m_spells.paladin.pAura = SelectRandomContainerElement(auras);

            break;
        }
        case CLASS_SHAMAN:
        {
            std::vector<SpellEntry const*> airTotems;
            if (pGraceOfAirTotem)
                airTotems.push_back(pGraceOfAirTotem);
            if (pNatureResistanceTotem)
                airTotems.push_back(pNatureResistanceTotem);
            if (pWindfuryTotem)
                airTotems.push_back(pWindfuryTotem);
            if (pWindwallTotem)
                airTotems.push_back(pWindwallTotem);
            if (pTranquilAirTotem)
                airTotems.push_back(pTranquilAirTotem);
            if (!airTotems.empty())
                m_spells.shaman.pAirTotem = SelectRandomContainerElement(airTotems);

            std::vector<SpellEntry const*> earthTotems;
            if (pEarthbindTotem)
                earthTotems.push_back(pEarthbindTotem);
            if (pStoneclawtotem)
                earthTotems.push_back(pStoneclawtotem);
            if (pStoneskinTotem)
                earthTotems.push_back(pStoneskinTotem);
            if (pStrengthOfEarthTotem)
                earthTotems.push_back(pStrengthOfEarthTotem);
            if (pTremorTotem)
                earthTotems.push_back(pTremorTotem);
            if (!earthTotems.empty())
                m_spells.shaman.pEarthTotem = SelectRandomContainerElement(earthTotems);

            std::vector<SpellEntry const*> fireTotems;
            if (pFireNovaTotem)
                fireTotems.push_back(pFireNovaTotem);
            if (pMagmaTotem)
                fireTotems.push_back(pMagmaTotem);
            if (pSearingTotem)
                fireTotems.push_back(pSearingTotem);
            if (pFlametongueTotem)
                fireTotems.push_back(pFlametongueTotem);
            if (pFrostResistanceTotem)
                fireTotems.push_back(pFrostResistanceTotem);
            if (!fireTotems.empty())
                m_spells.shaman.pFireTotem = SelectRandomContainerElement(fireTotems);

            std::vector<SpellEntry const*> waterTotems;
            if (pFireResistanceTotem)
                waterTotems.push_back(pFireResistanceTotem);
            if (pDiseaseCleansingTotem)
                waterTotems.push_back(pDiseaseCleansingTotem);
            if (pHealingStreamTotem)
                waterTotems.push_back(pHealingStreamTotem);
            if (pManaSpringTotem)
                waterTotems.push_back(pManaSpringTotem);
            if (pPoisonCleansingTotem)
                waterTotems.push_back(pPoisonCleansingTotem);
            if (!waterTotems.empty())
                m_spells.shaman.pWaterTotem = SelectRandomContainerElement(waterTotems);

            if (pWindfuryWeapon && m_role == ROLE_MELEE_DPS)
                m_spells.shaman.pWeaponBuff = pWindfuryWeapon;
            else
            {
                std::vector<SpellEntry const*> weaponBuffs;
                if (pWindfuryWeapon)
                    weaponBuffs.push_back(pWindfuryWeapon);
                if (pRockbiterWeapon)
                    weaponBuffs.push_back(pRockbiterWeapon);
                if (pFrostbrandWeapon)
                    weaponBuffs.push_back(pFrostbrandWeapon);
                if (!weaponBuffs.empty())
                    m_spells.shaman.pWeaponBuff = SelectRandomContainerElement(weaponBuffs);
            }

            break;
        }
        case CLASS_MAGE:
        {
            if (!m_spells.mage.pIceArmor && pFrostArmor)
                m_spells.mage.pIceArmor = pFrostArmor;

            std::vector<SpellEntry const*> polymorph;
            if (pPolymorphSheep)
                polymorph.push_back(pPolymorphSheep);
            if (pPolymorphCow)
                polymorph.push_back(pPolymorphCow);
            if (pPolymorphPig)
                polymorph.push_back(pPolymorphPig);
            if (pPolymorphTurtle)
                polymorph.push_back(pPolymorphTurtle);
            if (!polymorph.empty())
                m_spells.mage.pPolymorph = SelectRandomContainerElement(polymorph);

            break;
        }
        case CLASS_ROGUE:
        {
            // Rogues can only craft an item that applies the poison, they don't know the actual poison enchant.
            auto GetHighestRankOfPoisonByName = [](std::string name, uint32 level)
            {
                SpellEntry const* pHighestRank = nullptr;
                for (uint32 i = 0; i < sSpellMgr.GetMaxSpellId(); i++)
                {
                    if (SpellEntry const* pSpellEntry = sSpellMgr.GetSpellEntry(i))
                    {
                        if (pSpellEntry->Effect[0] == SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY &&
                            pSpellEntry->SpellName[0] == name && pSpellEntry->spellLevel <= level &&
                            (!pHighestRank || pHighestRank->spellLevel < pSpellEntry->spellLevel))
                        {
                             pHighestRank = pSpellEntry;
                        }
                    }
                }
                return pHighestRank;
            };

            SpellEntry const* pPoisonSpell = nullptr;
            std::vector<SpellEntry const*> vPoisons;
            if (hasDeadlyPoison && (pPoisonSpell = GetHighestRankOfPoisonByName("Deadly Poison", me->GetLevel())))
                vPoisons.push_back(pPoisonSpell);
            if (hasInstantPoison && (pPoisonSpell = GetHighestRankOfPoisonByName("Instant Poison", me->GetLevel())))
                vPoisons.push_back(pPoisonSpell);
            if (hasCripplingPoison && (pPoisonSpell = GetHighestRankOfPoisonByName("Crippling Poison", me->GetLevel())))
                vPoisons.push_back(pPoisonSpell);
            if (hasWoundPoison && (pPoisonSpell = GetHighestRankOfPoisonByName("Wound Poison", me->GetLevel())))
                vPoisons.push_back(pPoisonSpell);
            if (HasMindNumbingPoison && (pPoisonSpell = GetHighestRankOfPoisonByName("Mind-numbing Poison", me->GetLevel())))
                vPoisons.push_back(pPoisonSpell);

            if (!vPoisons.empty())
            {
                m_spells.rogue.pMainHandPoison = SelectRandomContainerElement(vPoisons);
                m_spells.rogue.pOffHandPoison = SelectRandomContainerElement(vPoisons);
            }

            break;
        }
        case CLASS_WARLOCK:
        {
            std::vector<SpellEntry const*> vCurses;
            if (m_spells.warlock.pCurseofShadow)
                vCurses.push_back(m_spells.warlock.pCurseofShadow);
            if(m_spells.warlock.pCurseoftheElements)
                vCurses.push_back(m_spells.warlock.pCurseoftheElements);

            if (!vCurses.empty())
                m_spells.warlock.pRaidCurse = SelectRandomContainerElement(vCurses);
        }
    }
}

void CombatBotBaseAI::AddAllSpellReagents()
{
    for (const auto& pSpell : m_spells.raw.spells)
    {
        if (pSpell)
        {
            for (const auto& reagent : pSpell->Reagent)
            {
                if (reagent && !me->HasItemCount(reagent, 1))
                    AddItemToInventory(reagent);
            }
            for (const auto& totem : pSpell->Totem)
            {
                if (totem && !me->HasItemCount(totem, 1))
                    AddItemToInventory(totem);
            }
        }
    }
}


bool CombatBotBaseAI::AreOthersOnSameTarget(ObjectGuid guid, bool checkMelee, bool checkSpells) const
{
    Group* pGroup = me->GetGroup();
    for (GroupReference* itr = pGroup->GetFirstMember(); itr != nullptr; itr = itr->next())
    {
        if (Player* pMember = itr->getSource())
        {
            // Not self.
            if (pMember == me)
                continue;

            // Not the target itself.
            if (pMember->GetObjectGuid() == guid)
                continue;

            if (pMember->GetTargetGuid() == guid)
            {
                if (checkMelee && pMember->HasUnitState(UNIT_STAT_MELEE_ATTACKING))
                    return true;

                if (checkSpells && pMember->IsNonMeleeSpellCasted())
                    return true;
            }
        }
    }
    return false;
}

bool CombatBotBaseAI::FindAndHealInjuredAlly(float minimumHealthPercent, float criticalHealthPercent)
{
    float directHealTreeshold = minimumHealthPercent - ( abs(minimumHealthPercent - criticalHealthPercent) / 2 );

    // Check critical targets first
    Unit* pTarget = SelectHealTarget(criticalHealthPercent, false);
    if (pTarget)
        return HealInjuredTargetDirect(pTarget);

    if (minimumHealthPercent > criticalHealthPercent)
    {
        // Now try periodic healing
        pTarget = SelectHealTarget(minimumHealthPercent, true);
        if (pTarget)
            return HealInjuredTargetPeriodic(pTarget);

        // Now direct healing - Mid point between HoT and critical
        pTarget = SelectHealTarget(directHealTreeshold, false);
        if (pTarget)
            return HealInjuredTargetDirect(pTarget);
    }

    return false;
}

template <class T>
SpellEntry const* CombatBotBaseAI::SelectMostEfficientHealingSpell(Unit const* pTarget, std::set<SpellEntry const*, T>& spellList) const
{
    SpellEntry const* pHealSpell = nullptr;
    int32 healthDiff = INT32_MAX;
    int32 const missingHealth = pTarget->GetMaxHealth() - pTarget->GetHealth();

    // Find most efficient healing spell.
    for (const auto pSpellEntry : spellList)
    {
        if (CanTryToCastSpell(pTarget, pSpellEntry))
        {
            int32 basePoints = 0;
            for (uint32 i = 0; i < MAX_SPELL_EFFECTS; i++)
            {
                switch (pSpellEntry->Effect[i])
                {
                    case SPELL_EFFECT_HEAL:
                        basePoints += pSpellEntry->EffectBasePoints[i];
                        break;
                    case SPELL_EFFECT_APPLY_AURA:
                    case SPELL_EFFECT_PERSISTENT_AREA_AURA:
                    case SPELL_EFFECT_APPLY_AREA_AURA_PARTY:
                        if (pSpellEntry->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_HEAL)
                            basePoints += ((pSpellEntry->GetDuration() / pSpellEntry->EffectAmplitude[i]) * pSpellEntry->EffectBasePoints[i]);
                        break;
                }
            }

            pSpellEntry->GetMaxDuration();
            int32 const diff = basePoints - missingHealth;
            if (std::abs(diff) < healthDiff)
            {
                healthDiff = diff;
                pHealSpell = pSpellEntry;
            }

            // Healing spells are sorted from strongest to weakest.
            if (diff < 0)
                break;
        }
    }

    return pHealSpell;
}

bool CombatBotBaseAI::HealInjuredTargetPeriodic(Unit* pTarget)
{
    if (SpellEntry const* pHealSpell = SelectMostEfficientHealingSpell(pTarget, spellListPeriodicHeal))
    {
        if (CanTryToCastSpell(pTarget, pHealSpell))
        {
            if (DoCastSpell(pTarget, pHealSpell) == SPELL_CAST_OK)
            {
                if (Player* pPlayer = pTarget->ToPlayer())
                {
                    pPlayer->SetHealTargetTimer(0, 1000);
                }
                return true;
            }
        }
    }

    return false;
}

bool CombatBotBaseAI::HealInjuredTargetDirect(Unit* pTarget)
{
    if (SpellEntry const* pHealSpell = SelectMostEfficientHealingSpell(pTarget, spellListDirectHeal))
        if (DoCastSpell(pTarget, pHealSpell) == SPELL_CAST_OK)
        {
            if (Player* pPlayer = pTarget->ToPlayer())
            {
                uint32 spellTimer = 1000;
                if (pHealSpell->GetCastTime())
                    spellTimer = (int)(0.5 * (pHealSpell->GetCastTime()));
                spellTimer = IsTank(pPlayer) ? 1.0f : spellTimer;
                pPlayer->SetHealTargetTimer(spellTimer, 0);
            }
            return true;
        }

    return false;
}

bool CombatBotBaseAI::IsValidHealTarget(Unit const* pTarget, float healthPercent) const
{
    return (pTarget->GetHealthPercent() < healthPercent) &&
            pTarget->IsAlive() &&
            me->IsWithinLOSInMap(pTarget) &&
            me->IsWithinDist(pTarget, 30.0f);
}

Unit* CombatBotBaseAI::SelectHealTarget(float healthPercent, bool periodic) const
{
    if (me->GetHealthPercent() < healthPercent &&
        !(periodic && me->HasAuraType(SPELL_AURA_PERIODIC_HEAL)))
        return me;

    std::vector<Unit*> vHighPriority;
    std::vector<Unit*> vMediumPriority;
    std::vector<Unit*> vLowPriority;

    Unit* pTarget = nullptr;

    if (Group* pGroup = me->GetGroup())
    {
        for (GroupReference* itr = pGroup->GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            if (Player* pMember = itr->getSource())
            {
                // We already checked self.
                if (pMember == me)
                    continue;

                // Check if healing is being cast by someone else
                if ((!periodic && pMember->GetHealDirectTargetTimer() > 0) || 
                    (periodic && pMember->GetHealPeriodicTargetTimer() > 0))
                    continue;

                // Critical Condition Target
                if (IsValidHealTarget(pMember, healthPercent) &&
                    !(periodic && pMember->HasAuraType(SPELL_AURA_PERIODIC_HEAL)))
                {
                    if (pMember->GetSubGroup() == me->GetSubGroup())
                        vHighPriority.push_back(pMember);
                    else
                        vMediumPriority.push_back(pMember);
                }

                // Also check party member pets - not raid pets
                if (pMember->GetSubGroup() == me->GetSubGroup())
                {
                    if (Unit* pPet = pMember->GetPet())
                    {
                        if (IsValidHealTarget(pPet, healthPercent) &&
                            !(periodic && pPet->HasAuraType(SPELL_AURA_PERIODIC_HEAL)))
                            vLowPriority.push_back(pPet);
                    }
                }
            }
        }
    }

    if (!vHighPriority.empty())
        pTarget = SelectRandomContainerElement(vHighPriority);
    else if (!vMediumPriority.empty())
        pTarget = SelectRandomContainerElement(vMediumPriority);
    else if(!vLowPriority.empty())
        pTarget = SelectRandomContainerElement(vLowPriority);

    return pTarget;
}

bool CombatBotBaseAI::IsValidHostileTarget(Unit const* pTarget) const
{
    return me->IsValidAttackTarget(pTarget) &&
           pTarget->IsVisibleForOrDetect(me, me, false) &&
           !pTarget->HasUnitState(UNIT_STAT_ISOLATED) &&
           !pTarget->HasBreakableByDamageCrowdControlAura();
}

bool CombatBotBaseAI::IsValidDispelTarget(Unit const* pTarget, SpellEntry const* pSpellEntry) const
{
    uint32 dispelMask = 0;
    bool bFoundOneDispell = false;
    // Compute Dispel Mask
    for (uint8 i = 0; i < MAX_EFFECT_INDEX; ++i)
    {
        if (pSpellEntry->Effect[i] != SPELL_EFFECT_DISPEL)
            continue;

        // Create dispel mask by dispel type
        uint32 dispel_type = pSpellEntry->EffectMiscValue[i];
        dispelMask |= Spells::GetDispellMask(DispelType(dispel_type));
    }
    bool friendly_dispel = pTarget && pTarget->IsFriendlyTo(me);

    if (pTarget &&
        // Check immune for offensive dispel
        (!pTarget->IsImmuneToSchoolMask(pSpellEntry->GetSpellSchoolMask()) ||
            friendly_dispel))
    {
        if (!friendly_dispel && !me->IsValidAttackTarget(pTarget))
            return false;

        auto const& auras = pTarget->GetSpellAuraHolderMap();
        for (const auto& aura : auras)
        {
            SpellAuraHolder* holder = aura.second;
            if ((1 << holder->GetSpellProto()->Dispel) & dispelMask)
            {
                if (holder->GetSpellProto()->Dispel == DISPEL_MAGIC ||
                    holder->GetSpellProto()->Dispel == DISPEL_DISEASE ||
                    holder->GetSpellProto()->Dispel == DISPEL_POISON)
                {
                    bool positive = holder->IsPositive();
                    // do not remove positive auras if friendly target
                    // do not remove negative auras if non-friendly target
                    // when removing charm auras ignore hostile reaction from the charm
                    if (!friendly_dispel && !positive && holder->GetSpellProto()->IsCharmSpell())
                        if (CharmInfo *charm = pTarget->GetCharmInfo())
                            if (FactionTemplateEntry const* ft = charm->GetOriginalFactionTemplate())
                                if (FactionTemplateEntry const* ft2 = me->GetFactionTemplateEntry())
                                    if (charm->GetOriginalFactionTemplate()->IsFriendlyTo(*ft2))
                                        bFoundOneDispell = true;
                    if (positive == friendly_dispel)
                        continue;
                }
                bFoundOneDispell = true;
                break;
            }
        }
    }

    if (!bFoundOneDispell)
        return false;

    return true;
}

uint8 CombatBotBaseAI::GetAttackersInRangeCount(float range) const
{
    uint8 count = 0;
    for (const auto& pTarget : me->GetAttackers())
    {
        if (me->GetCombatDistance(pTarget) <= range)
            count++;
    }

    return count;
}

uint8 CombatBotBaseAI::GetAlliesNeedingHealCount(float range, float healthPercent) const
{
    uint8 count = 0;

    if (Group* pGroup = me->GetGroup())
    {
        for (GroupReference* itr = pGroup->GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            if (Player* pMember = itr->getSource())
            {
                if (me->GetDistance(pMember) > range)
                    continue;
                if (pMember->GetHealDirectTargetTimer() > 0)
                    continue;
                // Check if we should heal party member.
                if (IsValidHealTarget(pMember, healthPercent))
                    count++;
                // Also check pets.
                if (Unit* pPet = pMember->GetPet())
                {
                    if (IsValidHealTarget(pPet, healthPercent))
                        count++;
                }
            }
        }
    }

    return count;
}

Unit* CombatBotBaseAI::SelectAttackerDifferentFrom(Unit const* pExcept) const
{
    for (const auto& pTarget : me->GetAttackers())
    {
        if (pTarget != pExcept)
            return pTarget;
    }

    return nullptr;
}

bool CombatBotBaseAI::IsValidBuffTarget(Unit const* pTarget, SpellEntry const* pSpellEntry) const
{
    std::list<uint32> morePowerfullSpells;
    sSpellMgr.ListMorePowerfullSpells(pSpellEntry->Id, morePowerfullSpells);

    for (const auto& i : pTarget->GetSpellAuraHolderMap())
    {
        if (i.first == pSpellEntry->Id)
            return false;

        if (sSpellMgr.IsRankSpellDueToSpell(pSpellEntry, i.first))
            return false;

        for (const auto& it : morePowerfullSpells)
            if (it == i.first)
                return false;
    }
        
    return true;
}

Player* CombatBotBaseAI::SelectBuffTarget(SpellEntry const* pSpellEntry) const
{
    if (IsValidBuffTarget(me, pSpellEntry))
        return me;

    std::vector<Player*> vBuffTargets;

    Player* pTarget = nullptr;

    Group* pGroup = me->GetGroup();
    if (pGroup)
    {
        for (GroupReference* itr = pGroup->GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            if (Player* pMember = itr->getSource())
            {
                if (pMember->IsAlive() &&
                    !pMember->IsGameMaster() &&
                    IsValidBuffTarget(pMember, pSpellEntry) &&
                    me->IsWithinLOSInMap(pMember) &&
                    me->IsWithinDist(pMember, 30.0f))
                    vBuffTargets.push_back(pMember);
            }
        }
    }

    if (!vBuffTargets.empty())
        pTarget = SelectRandomContainerElement(vBuffTargets);

    return pTarget;
}

Player* CombatBotBaseAI::SelectBuffTarget(SpellEntry const* pSpellEntryMeele, SpellEntry const* pSpellEntryRanged) const
{
    std::vector<Player*> vBuffTargets;

    Player* pTarget = nullptr;

    Group* pGroup = me->GetGroup();
    if (pGroup)
    {
        for (GroupReference* itr = pGroup->GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            if (Player* pMember = itr->getSource())
            {
                if (pMember->IsAlive() &&
                    !pMember->IsGameMaster() &&
                    me->IsWithinLOSInMap(pMember) &&
                    me->IsWithinDist(pMember, 30.0f))
                {
                    if ((IsMeleeWeaponClass(pMember->GetClass()) &&
                        IsValidBuffTarget(pMember, pSpellEntryMeele)) ||
                        (!IsMeleeWeaponClass(pMember->GetClass()) &&
                            IsValidBuffTarget(pMember, pSpellEntryRanged)))
                        vBuffTargets.push_back(pMember);
                }
            }
        }
    }

    if (!vBuffTargets.empty())
        pTarget = SelectRandomContainerElement(vBuffTargets);

    return pTarget;
}


Player* CombatBotBaseAI::SelectDispelTarget(SpellEntry const* pSpellEntry) const
{
    if(IsValidDispelTarget(me,pSpellEntry))
        return me;

    std::vector<Player*> vDispelTargets;

    Player* pTarget = nullptr;

    Group* pGroup = me->GetGroup();
    if (pGroup)
    {
        for (GroupReference* itr = pGroup->GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            if (Player* pMember = itr->getSource())
            {
                if (pMember->IsAlive() &&
                    !pMember->IsGameMaster() &&
                    IsValidDispelTarget(pMember, pSpellEntry) &&
                    me->IsWithinLOSInMap(pMember) &&
                    me->IsWithinDist(pMember, 30.0f))
                    vDispelTargets.push_back(pMember);
            }
        }
    }

    if (!vDispelTargets.empty())
        pTarget = SelectRandomContainerElement(vDispelTargets);

    return pTarget;
}

void CombatBotBaseAI::SummonPetIfNeeded()
{
    Pet* pPet = me->GetPet();

    if (me->GetClass() == CLASS_HUNTER)
    {
        if (me->GetPetGuid())
        {
            if (pPet && pPet->IsAlive())
            {
                if(pPet->GetReactState() != REACT_PASSIVE)
                    pPet->SetReactState(REACT_PASSIVE);
                if (!pPet->AllSpellsLearned())
                    pPet->LearnAllPetSpells();
                if (pPet->GetLoyaltyLevel() != BEST_FRIEND)
                {
                    pPet->SetPower(POWER_HAPPINESS, me->GetPet()->GetMaxPower(POWER_HAPPINESS));
                    pPet->SetLoyaltyLevel(BEST_FRIEND);
                }
                return;
            }
            else
            {
                if (me->HasSpell(SPELL_PET_REVIVE))
                {
                    me->CastSpell(me, SPELL_PET_REVIVE, false);
                    return;
                }
            }
        }

        if (me->GetLevel() < 10)
            return;

        if(!m_petId)
            m_petId = PickRandomValue( PET_WOLF, PET_CAT, PET_BEAR, PET_CRAB, PET_GORILLA, PET_BIRD,
                                        PET_BOAR, PET_BAT, PET_CROC, PET_SPIDER, PET_OWL, PET_STRIDER,
                                        PET_SCORPID, PET_SERPENT, PET_RAPTOR, PET_TURTLE, PET_HYENA,
                                        PET_FROSTSABER, PET_FROSTSABERSTK, PET_SHARDTOOTH,
                                        PET_HYENA, PET_HAKKAR, PET_BROKENTOOTH, PET_BLOODAXEWORG);

        if (Creature* pCreature = me->SummonCreature(m_petId,
            me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0.0f,
            TEMPSUMMON_TIMED_COMBAT_OR_DEAD_DESPAWN, 3000, false, 3000))
        {
            pCreature->SetLevel(me->GetLevel());
            me->CastSpell(pCreature, SPELL_TAME_BEAST, true);
        }
    }
    else if (me->GetClass() == CLASS_WARLOCK)
    {
        if (pPet && pPet->IsAlive())
        {
            if (pPet->GetReactState() != REACT_PASSIVE)
                pPet->SetReactState(REACT_PASSIVE);
            if (!pPet->AllSpellsLearned())
                pPet->LearnAllPetSpells();
            return;
        }

        if (!m_petId)
        {
            if (m_spells.warlock.pDemonicSacrifice)
                m_petId = SPELL_SUMMON_SUCCUBUS;
            else if(me->GetMap()->IsDungeon())
                m_petId = SPELL_SUMMON_IMP;
            else
            {
                std::vector<uint32> vSummons;
                if (me->HasSpell(SPELL_SUMMON_IMP))
                {
                    vSummons.push_back(SPELL_SUMMON_IMP);
                    vSummons.push_back(SPELL_SUMMON_IMP);
                    vSummons.push_back(SPELL_SUMMON_IMP);
                }
                // VoidWalker AI is broken for now. Wait for fix.
                //if (me->HasSpell(SPELL_SUMMON_VOIDWALKER))
                //   vSummons.push_back(SPELL_SUMMON_VOIDWALKER);
                if (me->HasSpell(SPELL_SUMMON_FELHUNTER))
                    vSummons.push_back(SPELL_SUMMON_FELHUNTER);
                if (me->HasSpell(SPELL_SUMMON_SUCCUBUS))
                    vSummons.push_back(SPELL_SUMMON_SUCCUBUS);
                if (!vSummons.empty())
                    m_petId = SelectRandomContainerElement(vSummons);

                if (!m_petId)
                    m_petId = SPELL_SUMMON_IMP;
            }
        }
        else
        {
            if(!m_spells.warlock.pDemonicSacrifice &&
                me->GetMap()->IsDungeon() &&
                m_petId != SPELL_SUMMON_IMP)
                m_petId = SPELL_SUMMON_IMP;
        }

        me->CastSpell(me, m_petId, false);
        
    }
}

bool CombatBotBaseAI::CanTryToCastSpell(Unit const* pTarget, SpellEntry const* pSpellEntry, uint32 maxStack) const
{
    if (!me->IsSpellReady(pSpellEntry->Id))
        return false;

    if (me->HasGCD(pSpellEntry))
        return false;

    if (pTarget->IsImmuneToSpell(pSpellEntry, false))
        return false;

    if (pSpellEntry->TargetAuraState &&
        !pTarget->HasAuraState(AuraState(pSpellEntry->TargetAuraState)))
        return false;

    if (pSpellEntry->DmgClass == SPELL_DAMAGE_CLASS_MAGIC)
    {
        if (pTarget->HasAuraType(SPELL_AURA_REFLECT_SPELLS))
            return false;

        Unit::AuraList const& mReflectSpellsSchool = pTarget->GetAurasByType(SPELL_AURA_REFLECT_SPELLS_SCHOOL);
        for (const auto i : mReflectSpellsSchool)
        {
            if (i->GetModifier()->m_miscvalue & pSpellEntry->GetSpellSchoolMask())
                return false;
        }
    }

    if (pSpellEntry->IsSpellAppliesAura())
    {
        if (SpellAuraHolder* pSpellAuraHolder = pTarget->GetSpellAuraHolder(pSpellEntry->Id))
        {
            if (pSpellAuraHolder->GetStackAmount() >= maxStack)
                return false;
        }
        else if (pTarget->HasAura(pSpellEntry->Id))
            return false;
    }

    SpellRangeEntry const* srange = sSpellRangeStore.LookupEntry(pSpellEntry->rangeIndex);
    if (me != pTarget && pSpellEntry->EffectImplicitTargetA[0] != TARGET_UNIT_CASTER)
    {
        float const dist = me->GetCombatDistance(pTarget);

        if (dist > srange->maxRange)
            return false;
        if (srange->minRange && dist < srange->minRange)
            return false;
    }

    return true;
}

bool CombatBotBaseAI::IsSpellReady(SpellEntry const* pSpellEntry) const
{
    if (!me->IsSpellReady(pSpellEntry->Id))
        return false;

    if (me->HasGCD(pSpellEntry))
        return false;

    if (pSpellEntry->CasterAuraState &&
        !me->HasAuraState(AuraState(pSpellEntry->CasterAuraState)))
        return false;

    if (pSpellEntry->GetErrorAtShapeshiftedCast(me->GetShapeshiftForm()) != SPELL_CAST_OK)
        return false;

    uint32 const powerCost = Spell::CalculatePowerCost(pSpellEntry, me);
    Powers const powerType = Powers(pSpellEntry->powerType);

    if (powerType == POWER_HEALTH)
    {
        if (me->GetHealth() <= powerCost)
            return false;
        return true;
    }

    if (me->GetPower(powerType) < powerCost)
        return false;

    return true;
}

SpellCastResult CombatBotBaseAI::DoCastSpell(Unit* pTarget, SpellEntry const* pSpellEntry)
{
    if (me != pTarget)
        me->SetFacingToObject(pTarget);

    if (me->IsMounted())
        me->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);

    me->SetTargetGuid(pTarget->GetObjectGuid());
    me->m_castingSpell = (me->GetClass() == CLASS_ROGUE) ? me->GetComboPoints() : pSpellEntry->Id;
    auto result = me->CastSpell(pTarget, pSpellEntry, false);

    //printf("cast %s result %u\n", pSpellEntry->SpellName[0].c_str(), result);

    if ((result == SPELL_FAILED_MOVING ||
        result == SPELL_CAST_OK) &&
        (pSpellEntry->GetCastTime() > 0) &&
        (me->IsMoving() || !me->IsStopped()))
        me->StopMoving();

    if ((result == SPELL_FAILED_NEED_AMMO_POUCH ||
        result == SPELL_FAILED_ITEM_NOT_READY) &&
        pSpellEntry->Reagent[0])
    {
        if (Item* pItem = me->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START))
            me->DestroyItem(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START, true);

        AddItemToInventory(pSpellEntry->Reagent[0]);
    }

    return result;
}

void CombatBotBaseAI::AddItemToInventory(uint32 itemId)
{
    ItemPosCountVec dest;
    uint8 msg = me->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, 1);
    if (msg == EQUIP_ERR_OK)
        me->StoreNewItem(dest, itemId, true, Item::GenerateItemRandomPropertyId(itemId));
}

void CombatBotBaseAI::EquipOrUseNewItem()
{
    for (int i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
    {
        Item* pItem = me->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
        if (pItem && !pItem->IsEquipped())
        {
            switch (pItem->GetProto()->Class)
            {
                case ITEM_CLASS_CONSUMABLE:
                {
                    SpellCastTargets targets;
                    targets.setUnitTarget(me);
                    me->CastItemUseSpell(pItem, targets);
                    break;
                }
                case ITEM_CLASS_WEAPON:
                case ITEM_CLASS_ARMOR:
                {
                    uint32 slot = me->FindEquipSlot(pItem->GetProto(), NULL_SLOT, true);
                    if (slot != NULL_SLOT)
                    {
                        if (Item* pItem2 = me->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                            me->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);

                        // Learn required proficiency
                        if (uint32 proficiencySpellId = pItem->GetProto()->GetProficiencySpell())
                            if (!me->HasSpell(proficiencySpellId))
                                me->LearnSpell(proficiencySpellId, false, false);

                        me->RemoveItem(INVENTORY_SLOT_BAG_0, i, false);
                        me->EquipItem(slot, pItem, true);
                    }
                    break;
                }
            }
        }
    }
}

bool CombatBotBaseAI::SummonShamanTotems()
{
    if (m_spells.shaman.pAirTotem &&
        !me->GetTotem(TOTEM_SLOT_AIR) &&
        CanTryToCastSpell(me, m_spells.shaman.pAirTotem))
    {
        if (DoCastSpell(me, m_spells.shaman.pAirTotem) == SPELL_CAST_OK)
            return true;
    }

    if (m_spells.shaman.pEarthTotem &&
        !me->GetTotem(TOTEM_SLOT_EARTH) &&
        CanTryToCastSpell(me, m_spells.shaman.pEarthTotem))
    {
        if (DoCastSpell(me, m_spells.shaman.pEarthTotem) == SPELL_CAST_OK)
            return true;
    }

    if (m_spells.shaman.pFireTotem &&
        !me->GetTotem(TOTEM_SLOT_FIRE) &&
        CanTryToCastSpell(me, m_spells.shaman.pFireTotem))
    {
        if (DoCastSpell(me, m_spells.shaman.pFireTotem) == SPELL_CAST_OK)
            return true;
    }

    if (m_spells.shaman.pWaterTotem &&
        !me->GetTotem(TOTEM_SLOT_WATER) &&
        CanTryToCastSpell(me, m_spells.shaman.pWaterTotem))
    {
        if (DoCastSpell(me, m_spells.shaman.pWaterTotem) == SPELL_CAST_OK)
            return true;
    }

    return false;
}

SpellCastResult CombatBotBaseAI::CastWeaponBuff(SpellEntry const* pSpellEntry, EquipmentSlots slot)
{
    Item* pWeapon = me->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
    if (!pWeapon)
        return SPELL_FAILED_ITEM_NOT_FOUND;
    if (pWeapon->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT))
        return SPELL_FAILED_ITEM_ALREADY_ENCHANTED;

    Spell* spell = new Spell(me, pSpellEntry, true, ObjectGuid(), nullptr, nullptr, nullptr);
    SpellCastTargets targets;
    targets.setItemTarget(pWeapon);
    return spell->prepare(std::move(targets), nullptr);
}

bool CombatBotBaseAI::IsWearingShield() const
{
    Item* pItem = me->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
    if (!pItem)
        return false;

    if (pItem->GetProto()->InventoryType == INVTYPE_SHIELD)
        return true;

    return false;
}

bool CombatBotBaseAI::IsDualWielding() const
{
    Item* pItem = me->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
    if (!pItem)
        return false;

    if (pItem->GetProto()->InventoryType == INVTYPE_WEAPON)
        return true;

    return false;
}

bool CombatBotBaseAI::IsTank(Player* pPlayer) const
{
    // Do not attack other tanks tagert
    if (pPlayer->AI())
    {
        if (CombatBotBaseAI* pAI = dynamic_cast<CombatBotBaseAI*>(pPlayer->AI()))
        {
            if (pAI->m_role == ROLE_TANK)
                return true;
        }
    }
    else if (pPlayer->HasSpell(SPELL_SHIELD_SLAM) ||
        pPlayer->HasSpell(SPELL_HOLY_SHIELD) ||
        pPlayer->GetShapeshiftForm() == FORM_BEAR ||
        pPlayer->GetShapeshiftForm() == FORM_DIREBEAR)
        return true;

    return false;
}

void CombatBotBaseAI::SendFakePacket(uint16 opcode)
{
    switch (opcode)
    {
        case MSG_MOVE_WORLDPORT_ACK:
        {
            me->GetSession()->HandleMoveWorldportAckOpcode();
            break;
        }
        case MSG_MOVE_TELEPORT_ACK:
        {
            WorldPacket data(MSG_MOVE_TELEPORT_ACK);
            data << me->GetObjectGuid();
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
            data << me->GetLastCounterForMovementChangeType(TELEPORT);
#endif
            data << uint32(time(nullptr));
            me->GetSession()->HandleMoveTeleportAckOpcode(data);
            break;
        }
        case CMSG_BATTLEFIELD_PORT:
        {
            for (uint32 i = BATTLEGROUND_QUEUE_AV; i <= BATTLEGROUND_QUEUE_AB; i++)
            {
                if (me->IsInvitedForBattleGroundQueueType(BattleGroundQueueTypeId(i)))
                {
                    WorldPacket data(CMSG_BATTLEFIELD_PORT);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
                    data << uint32(GetBattleGrounMapIdByTypeId(BattleGroundTypeId(i)));
#endif
                    data << uint8(1);
                    me->GetSession()->HandleBattleFieldPortOpcode(data);
                    break;
                }
            }
            break;
        }
        case CMSG_BEGIN_TRADE:
        {
            WorldPacket data(CMSG_BEGIN_TRADE);
            me->GetSession()->HandleBeginTradeOpcode(data);
            break;
        }
        case CMSG_ACCEPT_TRADE:
        {
            if (Item* pItem = me->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START))
                me->DestroyItem(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START, true);

            WorldPacket data(CMSG_ACCEPT_TRADE);
            data << uint32(1);
            me->GetSession()->HandleAcceptTradeOpcode(data);
            break;
        }
        case CMSG_RESURRECT_RESPONSE:
        {
            WorldPacket data(CMSG_RESURRECT_RESPONSE);
            data << me->GetResurrector();
            data << uint8(1);
            me->GetSession()->HandleResurrectResponseOpcode(data);
            break;
        }
    }
}

void CombatBotBaseAI::OnPacketReceived(WorldPacket const* packet)
{
    // Must always check "me" player pointer here!
    //printf("Bot received %s\n", LookupOpcodeName(packet->GetOpcode()));
    switch (packet->GetOpcode())
    {
        case SMSG_NEW_WORLD:
        {
            botEntry->m_pendingResponses.push_back(MSG_MOVE_WORLDPORT_ACK);
            break;
        }
        case MSG_MOVE_TELEPORT_ACK:
        {
            botEntry->m_pendingResponses.push_back(MSG_MOVE_TELEPORT_ACK);
            break;
        }
        case SMSG_TRADE_STATUS:
        {
            uint32 status = *((uint32*)(*packet).contents());
            if (status == TRADE_STATUS_BEGIN_TRADE)
            {
                botEntry->m_pendingResponses.push_back(CMSG_BEGIN_TRADE);
            }
            else if (status == TRADE_STATUS_TRADE_ACCEPT)
            {
                botEntry->m_pendingResponses.push_back(CMSG_ACCEPT_TRADE);
            }
            else if (status == TRADE_STATUS_TRADE_COMPLETE)
            {
                if (me)
                    EquipOrUseNewItem();
            }
            break;
        }
        case SMSG_RESURRECT_REQUEST:
        {
            botEntry->m_pendingResponses.push_back(CMSG_RESURRECT_RESPONSE);
            break;
        }
        case SMSG_BATTLEFIELD_STATUS:
        {
            if (!me)
                return;

            if (me->IsBeingTeleported() || me->InBattleGround())
                m_receivedBgInvite = false;
            else
            {
                for (uint32 i = BATTLEGROUND_QUEUE_AV; i <= BATTLEGROUND_QUEUE_AB; i++)
                {
                    if (me->IsInvitedForBattleGroundQueueType(BattleGroundQueueTypeId(i)))
                    {
                        m_receivedBgInvite = true;
                        break;
                    }
                }
            }
            return;
        }
    }
}
