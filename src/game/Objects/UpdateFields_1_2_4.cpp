// Auto generated file
// Patch: 1.2.4
// Build: 4222

static std::vector<UpdateFieldData> const g_updateFieldsData =
{
    // enum EObjectFields
    { TYPEMASK_OBJECT       , "OBJECT_FIELD_GUID"                               , 0x0  , 2  , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_OBJECT       , "OBJECT_FIELD_TYPE"                               , 0x2  , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_OBJECT       , "OBJECT_FIELD_ENTRY"                              , 0x3  , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_OBJECT       , "OBJECT_FIELD_SCALE_X"                            , 0x4  , 1  , UF_TYPE_FLOAT    , UF_FLAG_PUBLIC },
    { TYPEMASK_OBJECT       , "OBJECT_FIELD_PADDING"                            , 0x5  , 1  , UF_TYPE_INT      , UF_FLAG_NONE },
    { TYPEMASK_OBJECT       , "OBJECT_END"                                      , 0x6  , 0  , UF_TYPE_NONE     , UF_FLAG_NONE },
    // enum EItemFields
    { TYPEMASK_ITEM         , "ITEM_FIELD_OWNER"                                , 0x6  , 2  , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_ITEM         , "ITEM_FIELD_CONTAINED"                            , 0x8  , 2  , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_ITEM         , "ITEM_FIELD_CREATOR"                              , 0xA  , 2  , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_ITEM         , "ITEM_FIELD_GIFTCREATOR"                          , 0xC  , 2  , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_ITEM         , "ITEM_FIELD_STACK_COUNT"                          , 0xE  , 1  , UF_TYPE_INT      , UF_FLAG_OWNER_ONLY + UF_FLAG_UNK2 },
    { TYPEMASK_ITEM         , "ITEM_FIELD_DURATION"                             , 0xF  , 1  , UF_TYPE_INT      , UF_FLAG_OWNER_ONLY + UF_FLAG_UNK2 },
    { TYPEMASK_ITEM         , "ITEM_FIELD_SPELL_CHARGES"                        , 0x10 , 5  , UF_TYPE_INT      , UF_FLAG_OWNER_ONLY + UF_FLAG_UNK2 },
    { TYPEMASK_ITEM         , "ITEM_FIELD_FLAGS"                                , 0x15 , 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_ITEM         , "ITEM_FIELD_ENCHANTMENT"                          , 0x16 , 21 , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_ITEM         , "ITEM_FIELD_PROPERTY_SEED"                        , 0x2B , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_ITEM         , "ITEM_FIELD_RANDOM_PROPERTIES_ID"                 , 0x2C , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_ITEM         , "ITEM_FIELD_ITEM_TEXT_ID"                         , 0x2D , 1  , UF_TYPE_INT      , UF_FLAG_OWNER_ONLY },
    { TYPEMASK_ITEM         , "ITEM_FIELD_DURABILITY"                           , 0x2E , 1  , UF_TYPE_INT      , UF_FLAG_OWNER_ONLY + UF_FLAG_UNK2 },
    { TYPEMASK_ITEM         , "ITEM_FIELD_MAXDURABILITY"                        , 0x2F , 1  , UF_TYPE_INT      , UF_FLAG_OWNER_ONLY + UF_FLAG_UNK2 },
    { TYPEMASK_ITEM         , "ITEM_END"                                        , 0x30 , 0  , UF_TYPE_NONE     , UF_FLAG_NONE },
    // enum EContainerFields
    { TYPEMASK_CONTAINER    , "CONTAINER_FIELD_NUM_SLOTS"                       , 0x30 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_CONTAINER    , "CONTAINER_ALIGN_PAD"                             , 0x31 , 1  , UF_TYPE_BYTES    , UF_FLAG_NONE },
    { TYPEMASK_CONTAINER    , "CONTAINER_FIELD_SLOT_1"                          , 0x32 , 40 , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_CONTAINER    , "CONTAINER_END"                                   , 0x5A , 0  , UF_TYPE_NONE     , UF_FLAG_NONE },
    // enum EUnitFields
    { TYPEMASK_UNIT         , "UNIT_FIELD_CHARM"                                , 0x6  , 2  , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_SUMMON"                               , 0x8  , 2  , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_CHARMEDBY"                            , 0xA  , 2  , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_SUMMONEDBY"                           , 0xC  , 2  , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_CREATEDBY"                            , 0xE  , 2  , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_TARGET"                               , 0x10 , 2  , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_PERSUADED"                            , 0x12 , 2  , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_CHANNEL_OBJECT"                       , 0x14 , 2  , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_HEALTH"                               , 0x16 , 1  , UF_TYPE_INT      , UF_FLAG_DYNAMIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_POWER1"                               , 0x17 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_POWER2"                               , 0x18 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_POWER3"                               , 0x19 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_POWER4"                               , 0x1A , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_POWER5"                               , 0x1B , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_MAXHEALTH"                            , 0x1C , 1  , UF_TYPE_INT      , UF_FLAG_DYNAMIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_MAXPOWER1"                            , 0x1D , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_MAXPOWER2"                            , 0x1E , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_MAXPOWER3"                            , 0x1F , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_MAXPOWER4"                            , 0x20 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_MAXPOWER5"                            , 0x21 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_LEVEL"                                , 0x22 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_FACTIONTEMPLATE"                      , 0x23 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_BYTES_0"                              , 0x24 , 1  , UF_TYPE_BYTES    , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_VIRTUAL_ITEM_SLOT_DISPLAY"                  , 0x25 , 3  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_VIRTUAL_ITEM_INFO"                          , 0x28 , 6  , UF_TYPE_BYTES    , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_FLAGS"                                , 0x2E , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_AURA"                                 , 0x2F , 56 , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_AURALEVELS"                           , 0x67 , 10 , UF_TYPE_BYTES    , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_AURAAPPLICATIONS"                     , 0x71 , 10 , UF_TYPE_BYTES    , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_AURAFLAGS"                            , 0x7B , 7  , UF_TYPE_BYTES    , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_AURASTATE"                            , 0x82 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_BASEATTACKTIME"                       , 0x83 , 2  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_RANGEDATTACKTIME"                     , 0x85 , 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_UNIT         , "UNIT_FIELD_BOUNDINGRADIUS"                       , 0x86 , 1  , UF_TYPE_FLOAT    , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_COMBATREACH"                          , 0x87 , 1  , UF_TYPE_FLOAT    , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_DISPLAYID"                            , 0x88 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_NATIVEDISPLAYID"                      , 0x89 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_MOUNTDISPLAYID"                       , 0x8A , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_MINDAMAGE"                            , 0x8B , 1  , UF_TYPE_FLOAT    , UF_FLAG_PRIVATE + UF_FLAG_OWNER_ONLY + UF_FLAG_SPECIAL_INFO },
    { TYPEMASK_UNIT         , "UNIT_FIELD_MAXDAMAGE"                            , 0x8C , 1  , UF_TYPE_FLOAT    , UF_FLAG_PRIVATE + UF_FLAG_OWNER_ONLY + UF_FLAG_SPECIAL_INFO },
    { TYPEMASK_UNIT         , "UNIT_FIELD_MINOFFHANDDAMAGE"                     , 0x8D , 1  , UF_TYPE_FLOAT    , UF_FLAG_PRIVATE + UF_FLAG_OWNER_ONLY + UF_FLAG_SPECIAL_INFO },
    { TYPEMASK_UNIT         , "UNIT_FIELD_MAXOFFHANDDAMAGE"                     , 0x8E , 1  , UF_TYPE_FLOAT    , UF_FLAG_PRIVATE + UF_FLAG_OWNER_ONLY + UF_FLAG_SPECIAL_INFO },
    { TYPEMASK_UNIT         , "UNIT_FIELD_BYTES_1"                              , 0x8F , 1  , UF_TYPE_BYTES    , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_PETNUMBER"                            , 0x90 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_PET_NAME_TIMESTAMP"                   , 0x91 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_PETEXPERIENCE"                        , 0x92 , 1  , UF_TYPE_INT      , UF_FLAG_OWNER_ONLY },
    { TYPEMASK_UNIT         , "UNIT_FIELD_PETNEXTLEVELEXP"                      , 0x93 , 1  , UF_TYPE_INT      , UF_FLAG_OWNER_ONLY },
    { TYPEMASK_UNIT         , "UNIT_DYNAMIC_FLAGS"                              , 0x94 , 1  , UF_TYPE_INT      , UF_FLAG_DYNAMIC },
    { TYPEMASK_UNIT         , "UNIT_CHANNEL_SPELL"                              , 0x95 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_MOD_CAST_SPEED"                             , 0x96 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_CREATED_BY_SPELL"                           , 0x97 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_NPC_FLAGS"                                  , 0x98 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_NPC_EMOTESTATE"                             , 0x99 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_TRAINING_POINTS"                            , 0x9A , 1  , UF_TYPE_TWO_SHORT, UF_FLAG_OWNER_ONLY },
    { TYPEMASK_UNIT         , "UNIT_FIELD_STAT0"                                , 0x9B , 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE + UF_FLAG_OWNER_ONLY },
    { TYPEMASK_UNIT         , "UNIT_FIELD_STAT1"                                , 0x9C , 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE + UF_FLAG_OWNER_ONLY },
    { TYPEMASK_UNIT         , "UNIT_FIELD_STAT2"                                , 0x9D , 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE + UF_FLAG_OWNER_ONLY },
    { TYPEMASK_UNIT         , "UNIT_FIELD_STAT3"                                , 0x9E , 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE + UF_FLAG_OWNER_ONLY },
    { TYPEMASK_UNIT         , "UNIT_FIELD_STAT4"                                , 0x9F , 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE + UF_FLAG_OWNER_ONLY },
    { TYPEMASK_UNIT         , "UNIT_FIELD_RESISTANCES"                          , 0xA0 , 7  , UF_TYPE_INT      , UF_FLAG_PRIVATE + UF_FLAG_OWNER_ONLY + UF_FLAG_SPECIAL_INFO },
    { TYPEMASK_UNIT         , "UNIT_FIELD_ATTACKPOWER"                          , 0xA7 , 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE + UF_FLAG_OWNER_ONLY },
    { TYPEMASK_UNIT         , "UNIT_FIELD_BASE_MANA"                            , 0xA8 , 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE + UF_FLAG_OWNER_ONLY },
    { TYPEMASK_UNIT         , "UNIT_FIELD_ATTACK_POWER_MODS"                    , 0xA9 , 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PRIVATE + UF_FLAG_OWNER_ONLY },
    { TYPEMASK_UNIT         , "UNIT_FIELD_BYTES_2"                              , 0xAA , 1  , UF_TYPE_BYTES    , UF_FLAG_PUBLIC },
    { TYPEMASK_UNIT         , "UNIT_FIELD_RANGEDATTACKPOWER"                    , 0xAB , 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE + UF_FLAG_OWNER_ONLY },
    { TYPEMASK_UNIT         , "UNIT_FIELD_RANGED_ATTACK_POWER_MODS"             , 0xAC , 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PRIVATE + UF_FLAG_OWNER_ONLY },
    { TYPEMASK_UNIT         , "UNIT_FIELD_MINRANGEDDAMAGE"                      , 0xAD , 1  , UF_TYPE_FLOAT    , UF_FLAG_PRIVATE + UF_FLAG_OWNER_ONLY },
    { TYPEMASK_UNIT         , "UNIT_FIELD_MAXRANGEDDAMAGE"                      , 0xAE , 1  , UF_TYPE_FLOAT    , UF_FLAG_PRIVATE + UF_FLAG_OWNER_ONLY },
    { TYPEMASK_UNIT         , "UNIT_FIELD_PADDING"                              , 0xAF , 1  , UF_TYPE_INT      , UF_FLAG_NONE },
    { TYPEMASK_UNIT         , "UNIT_END"                                        , 0xB0 , 0  , UF_TYPE_NONE     , UF_FLAG_NONE },
    // enum EPlayerFields
    { TYPEMASK_PLAYER       , "PLAYER_SELECTION"                                , 0xB0 , 2  , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_DUEL_ARBITER"                             , 0xB2 , 2  , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_FLAGS"                                    , 0xB4 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_GUILDID"                                  , 0xB5 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_GUILDRANK"                                , 0xB6 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_BYTES"                                    , 0xB7 , 1  , UF_TYPE_BYTES    , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_BYTES_2"                                  , 0xB8 , 1  , UF_TYPE_BYTES    , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_BYTES_3"                                  , 0xB9 , 1  , UF_TYPE_BYTES    , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_DUEL_TEAM"                                , 0xBA , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_GUILD_TIMESTAMP"                          , 0xBB , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_1_0"                         , 0xBC , 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_1_PROPERTIES"                , 0xC4 , 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_2_0"                         , 0xC5 , 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_2_PROPERTIES"                , 0xCD , 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_3_0"                         , 0xCE , 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_3_PROPERTIES"                , 0xD6 , 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_4_0"                         , 0xD7 , 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_4_PROPERTIES"                , 0xDF , 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_5_0"                         , 0xE0 , 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_5_PROPERTIES"                , 0xE8 , 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_6_0"                         , 0xE9 , 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_6_PROPERTIES"                , 0xF1 , 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_7_0"                         , 0xF2 , 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_7_PROPERTIES"                , 0xFA , 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_8_0"                         , 0xFB , 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_8_PROPERTIES"                , 0x103, 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_9_0"                         , 0x104, 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_9_PROPERTIES"                , 0x10C, 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_10_0"                        , 0x10D, 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_10_PROPERTIES"               , 0x115, 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_11_0"                        , 0x116, 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_11_PROPERTIES"               , 0x11E, 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_12_0"                        , 0x11F, 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_12_PROPERTIES"               , 0x127, 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_13_0"                        , 0x128, 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_13_PROPERTIES"               , 0x130, 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_14_0"                        , 0x131, 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_14_PROPERTIES"               , 0x139, 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_15_0"                        , 0x13A, 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_15_PROPERTIES"               , 0x142, 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_16_0"                        , 0x143, 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_16_PROPERTIES"               , 0x14B, 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_17_0"                        , 0x14C, 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_17_PROPERTIES"               , 0x154, 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_18_0"                        , 0x155, 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_18_PROPERTIES"               , 0x15D, 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_19_0"                        , 0x15E, 8  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_VISIBLE_ITEM_19_PROPERTIES"               , 0x166, 1  , UF_TYPE_TWO_SHORT, UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_PAD_0"                              , 0x167, 1  , UF_TYPE_INT      , UF_FLAG_NONE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_INV_SLOT_HEAD"                      , 0x168, 46 , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_PACK_SLOT_1"                        , 0x196, 32 , UF_TYPE_GUID     , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_BANK_SLOT_1"                        , 0x1B6, 48 , UF_TYPE_GUID     , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_BANKBAG_SLOT_1"                     , 0x1E6, 12 , UF_TYPE_GUID     , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_VENDORBUYBACK_SLOT"                 , 0x1F2, 2  , UF_TYPE_GUID     , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FARSIGHT"                                 , 0x1F4, 2  , UF_TYPE_GUID     , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER__FIELD_COMBO_TARGET"                      , 0x1F6, 2  , UF_TYPE_GUID     , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_BUYBACK_NPC"                        , 0x1F8, 2  , UF_TYPE_GUID     , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_XP"                                       , 0x1FA, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_NEXT_LEVEL_XP"                            , 0x1FB, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_SKILL_INFO_1_1"                           , 0x1FC, 384, UF_TYPE_TWO_SHORT, UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_QUEST_LOG_1_1"                            , 0x37C, 60 , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_CHARACTER_POINTS1"                        , 0x3B8, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_CHARACTER_POINTS2"                        , 0x3B9, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_TRACK_CREATURES"                          , 0x3BA, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_TRACK_RESOURCES"                          , 0x3BB, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_CHAT_FILTERS"                             , 0x3BC, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_BLOCK_PERCENTAGE"                         , 0x3BD, 1  , UF_TYPE_FLOAT    , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_DODGE_PERCENTAGE"                         , 0x3BE, 1  , UF_TYPE_FLOAT    , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_PARRY_PERCENTAGE"                         , 0x3BF, 1  , UF_TYPE_FLOAT    , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_CRIT_PERCENTAGE"                          , 0x3C0, 1  , UF_TYPE_FLOAT    , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_EXPLORED_ZONES_1"                         , 0x3C1, 32 , UF_TYPE_BYTES    , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_REST_STATE_EXPERIENCE"                    , 0x3E1, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_COINAGE"                            , 0x3E2, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_POSSTAT0"                           , 0x3E3, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_POSSTAT1"                           , 0x3E4, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_POSSTAT2"                           , 0x3E5, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_POSSTAT3"                           , 0x3E6, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_POSSTAT4"                           , 0x3E7, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_NEGSTAT0"                           , 0x3E8, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_NEGSTAT1"                           , 0x3E9, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_NEGSTAT2"                           , 0x3EA, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_NEGSTAT3"                           , 0x3EB, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_NEGSTAT4"                           , 0x3EC, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_RESISTANCEBUFFMODSPOSITIVE"         , 0x3ED, 7  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_RESISTANCEBUFFMODSNEGATIVE"         , 0x3F4, 7  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_MOD_DAMAGE_DONE_POS"                , 0x3FB, 7  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_MOD_DAMAGE_DONE_NEG"                , 0x402, 7  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_MOD_DAMAGE_DONE_PCT"                , 0x409, 7  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_BYTES"                              , 0x410, 1  , UF_TYPE_BYTES    , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_AMMO_ID"                                  , 0x411, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_PVP_MEDALS"                         , 0x412, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_BUYBACK_ITEM_ID"                    , 0x413, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_BUYBACK_RANDOM_PROPERTIES_ID"       , 0x414, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_BUYBACK_SEED"                       , 0x415, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_BUYBACK_PRICE"                      , 0x416, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_BUYBACK_DURABILITY"                 , 0x417, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_BUYBACK_COUNT"                      , 0x418, 1  , UF_TYPE_INT      , UF_FLAG_PRIVATE },
    { TYPEMASK_PLAYER       , "PLAYER_FIELD_PADDING"                            , 0x419, 1  , UF_TYPE_INT      , UF_FLAG_NONE },
    { TYPEMASK_PLAYER       , "PLAYER_END"                                      , 0x41A, 0  , UF_TYPE_NONE     , UF_FLAG_NONE },
    // enum EGameObjectFields
    { TYPEMASK_GAMEOBJECT   , "GAMEOBJECT_DISPLAYID"                            , 0x6  , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_GAMEOBJECT   , "GAMEOBJECT_FLAGS"                                , 0x7  , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_GAMEOBJECT   , "GAMEOBJECT_ROTATION"                             , 0x8  , 4  , UF_TYPE_FLOAT    , UF_FLAG_PUBLIC },
    { TYPEMASK_GAMEOBJECT   , "GAMEOBJECT_STATE"                                , 0xC  , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_GAMEOBJECT   , "GAMEOBJECT_TIMESTAMP"                            , 0xD  , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_GAMEOBJECT   , "GAMEOBJECT_POS_X"                                , 0xE  , 1  , UF_TYPE_FLOAT    , UF_FLAG_PUBLIC },
    { TYPEMASK_GAMEOBJECT   , "GAMEOBJECT_POS_Y"                                , 0xF  , 1  , UF_TYPE_FLOAT    , UF_FLAG_PUBLIC },
    { TYPEMASK_GAMEOBJECT   , "GAMEOBJECT_POS_Z"                                , 0x10 , 1  , UF_TYPE_FLOAT    , UF_FLAG_PUBLIC },
    { TYPEMASK_GAMEOBJECT   , "GAMEOBJECT_FACING"                               , 0x11 , 1  , UF_TYPE_FLOAT    , UF_FLAG_PUBLIC },
    { TYPEMASK_GAMEOBJECT   , "GAMEOBJECT_DYN_FLAGS"                            , 0x12 , 1  , UF_TYPE_INT      , UF_FLAG_DYNAMIC },
    { TYPEMASK_GAMEOBJECT   , "GAMEOBJECT_FACTION"                              , 0x13 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_GAMEOBJECT   , "GAMEOBJECT_TYPE_ID"                              , 0x14 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_GAMEOBJECT   , "GAMEOBJECT_LEVEL"                                , 0x15 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_GAMEOBJECT   , "GAMEOBJECT_END"                                  , 0x16 , 0  , UF_TYPE_NONE     , UF_FLAG_NONE },
    // enum EDynamicObjectFields
    { TYPEMASK_DYNAMICOBJECT, "DYNAMICOBJECT_CASTER"                            , 0x6  , 2  , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_DYNAMICOBJECT, "DYNAMICOBJECT_BYTES"                             , 0x8  , 1  , UF_TYPE_BYTES    , UF_FLAG_PUBLIC },
    { TYPEMASK_DYNAMICOBJECT, "DYNAMICOBJECT_SPELLID"                           , 0x9  , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_DYNAMICOBJECT, "DYNAMICOBJECT_RADIUS"                            , 0xA  , 1  , UF_TYPE_FLOAT    , UF_FLAG_PUBLIC },
    { TYPEMASK_DYNAMICOBJECT, "DYNAMICOBJECT_POS_X"                             , 0xB  , 1  , UF_TYPE_FLOAT    , UF_FLAG_PUBLIC },
    { TYPEMASK_DYNAMICOBJECT, "DYNAMICOBJECT_POS_Y"                             , 0xC  , 1  , UF_TYPE_FLOAT    , UF_FLAG_PUBLIC },
    { TYPEMASK_DYNAMICOBJECT, "DYNAMICOBJECT_POS_Z"                             , 0xD  , 1  , UF_TYPE_FLOAT    , UF_FLAG_PUBLIC },
    { TYPEMASK_DYNAMICOBJECT, "DYNAMICOBJECT_FACING"                            , 0xE  , 1  , UF_TYPE_FLOAT    , UF_FLAG_PUBLIC },
    { TYPEMASK_DYNAMICOBJECT, "DYNAMICOBJECT_PAD"                               , 0xF  , 1  , UF_TYPE_BYTES    , UF_FLAG_PUBLIC },
    { TYPEMASK_DYNAMICOBJECT, "DYNAMICOBJECT_END"                               , 0x10 , 0  , UF_TYPE_NONE     , UF_FLAG_NONE },
    // enum ECorpseFields
    { TYPEMASK_CORPSE       , "CORPSE_FIELD_OWNER"                              , 0x6  , 2  , UF_TYPE_GUID     , UF_FLAG_PUBLIC },
    { TYPEMASK_CORPSE       , "CORPSE_FIELD_FACING"                             , 0x8  , 1  , UF_TYPE_FLOAT    , UF_FLAG_PUBLIC },
    { TYPEMASK_CORPSE       , "CORPSE_FIELD_POS_X"                              , 0x9  , 1  , UF_TYPE_FLOAT    , UF_FLAG_PUBLIC },
    { TYPEMASK_CORPSE       , "CORPSE_FIELD_POS_Y"                              , 0xA  , 1  , UF_TYPE_FLOAT    , UF_FLAG_PUBLIC },
    { TYPEMASK_CORPSE       , "CORPSE_FIELD_POS_Z"                              , 0xB  , 1  , UF_TYPE_FLOAT    , UF_FLAG_PUBLIC },
    { TYPEMASK_CORPSE       , "CORPSE_FIELD_DISPLAY_ID"                         , 0xC  , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_CORPSE       , "CORPSE_FIELD_ITEM"                               , 0xD  , 19 , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_CORPSE       , "CORPSE_FIELD_BYTES_1"                            , 0x20 , 1  , UF_TYPE_BYTES    , UF_FLAG_PUBLIC },
    { TYPEMASK_CORPSE       , "CORPSE_FIELD_BYTES_2"                            , 0x21 , 1  , UF_TYPE_BYTES    , UF_FLAG_PUBLIC },
    { TYPEMASK_CORPSE       , "CORPSE_FIELD_GUILD"                              , 0x22 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_CORPSE       , "CORPSE_FIELD_FLAGS"                              , 0x23 , 1  , UF_TYPE_INT      , UF_FLAG_PUBLIC },
    { TYPEMASK_CORPSE       , "CORPSE_END"                                      , 0x24 , 0  , UF_TYPE_NONE     , UF_FLAG_NONE },
};
