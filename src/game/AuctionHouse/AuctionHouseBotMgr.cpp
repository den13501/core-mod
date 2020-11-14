#include "Database/DatabaseEnv.h"
#include "World.h"
#include "Log.h"
#include "ProgressBar.h"
#include "Policies/SingletonImp.h"
#include "Player.h"
#include "Item.h"
#include "AuctionHouseMgr.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "AuctionHouseBotMgr.h"
#include "Config/Config.h"
#include "Chat.h"

INSTANTIATE_SINGLETON_1(AuctionHouseBotMgr);

AuctionHouseBotMgr::~AuctionHouseBotMgr()
{
    m_items.clear();

    if (m_config)
        m_config.reset();
}

void AuctionHouseBotMgr::Load()
{
    /* 1 - DELETE */
    m_items.clear();
    m_loaded = false;

    if (m_config)
        m_config.reset();

    /*2 - LOAD */
    std::unique_ptr<QueryResult> result(WorldDatabase.Query("SELECT `item`, `stack`, `bid`, `buyout` FROM `auctionhousebot`"));

    if (!result)
    {
        BarGoLink bar(1);
        bar.step();

        sLog.outString();
        sLog.outString(">> Loaded 0 AuctionHouseBot items");
        return;
    }

    uint32 count = 0;
    BarGoLink bar(result->GetRowCount());

    Field* fields;
    do
    {
        bar.step();
        AuctionHouseBotEntry e;
        fields    = result->Fetch();
        e.item    = fields[0].GetUInt32();
        e.stack   = fields[1].GetUInt32();
        e.bid     = fields[2].GetUInt32();
        e.buyout  = fields[3].GetUInt32();

        m_items.push_back(e);

        ++count;
    }
    while (result->NextRow());

    sLog.outString();
    sLog.outString(">> Loaded %u AuctionHouseBot items", count);

    /* CONFIG */
    m_config                 = std::make_unique<AuctionHouseBotConfig>();
    m_config->enable         = sConfig.GetBoolDefault("AHBot.Enable", false);
    m_config->ahid           = sConfig.GetIntDefault("AHBot.ah.id", 7);
    m_config->botguid        = sConfig.GetIntDefault("AHBot.bot.guid", 1123);
    m_config->botaccount     = sConfig.GetIntDefault("AHBot.bot.account", 32377);
    m_config->ahfid          = sConfig.GetIntDefault("AHBot.ah.fid", 120);
    m_config->itemcount      = sConfig.GetIntDefault("AHBot.itemcount", 2);

    m_auctionHouseEntry = sAuctionMgr.GetAuctionHouseEntry(m_config->ahfid);
    if (!m_auctionHouseEntry)
    {
        sLog.outInfo("AHBot::Load() : No auction house for faction %u.", m_config->ahfid);
        return;
    }
    m_loaded = true;
}

void AuctionHouseBotMgr::Update(bool force /* = false */)
{
    if (!m_loaded)
        return;

    ASSERT(m_config);
    ASSERT(m_auctionHouseEntry);

    if (!(m_config->enable || force))
        return;

    if (m_items.empty() ||  /*m_config->botguid==0 ||*/ m_config->botaccount == 0)
    {
        sLog.outError("AHBot::Update() : Bad config or empty table.");
        return;
    }

    AuctionHouseObject* auctionHouse = sAuctionMgr.GetAuctionsMap(m_auctionHouseEntry);
    if (!auctionHouse)
    {
        sLog.outError("AHBot::Update() : No auction house for faction %u.", m_config->ahfid);
        return;
    }

    uint32 auctions     = auctionHouse->GetCount();
    uint32 items        = m_config->itemcount;
    uint32 entriesCount = m_items.size();

    while (auctions < items)
    {
        AuctionHouseBotEntry item = m_items[urand(0, entriesCount - 1)];
        // Makes single items and the most expensive appear at half the rate
        // of materials and consumables
        if ((item.stack == 1 || item.buyout >= 1000000 ) && urand(0, 1))
            continue;
        AddItem(item, auctionHouse);
        auctions++;
    }
}

void AuctionHouseBotMgr::AddItem(AuctionHouseBotEntry e, AuctionHouseObject *auctionHouse)
{
    ASSERT(m_auctionHouseEntry);

    uint32 itemStack = 1;
    float priceFactor = 1.0f;
    uint32 itemBid = 100;
    uint32 itemBuyOut = 1000;

    ItemPrototype const* prototype = sObjectMgr.GetItemPrototype(e.item);
    if (prototype == nullptr)
    {
        sLog.outInfo("AHBot::AddItem() : Item %u does not exist.", e.item);
        return;
    }

    Item* item = Item::CreateItem(e.item, 1, nullptr);
    if (!item)
    {
        sLog.outInfo("AHBot::AddItem() : Cannot create item.");
        return;
    }

    sLog.outInfo("AHBot::AddItem() : Adding item %u.", e.item);

    uint32 randomPropertyId = Item::GenerateItemRandomPropertyId(e.item);
    if (randomPropertyId != 0)
        item->SetItemRandomProperties(randomPropertyId);

    uint32 etime = urand(1, 5);
    switch (etime)
    {
        case 1:
            etime = urand(7200,28800);
            break;
        case 2:
        case 3:
            etime = urand(28800, 57600);
            break;
        case 4:
        case 5:
            etime = 86400;
            break;
        default:
            etime = 86400;
            break;
    }

    if (e.stack > 1)
    {
        switch (urand(1, 5))
        {
            case 1:
                itemStack = urand(1, e.stack);
                break;
            case 2:
                itemStack = 1;
                break;
            case 3:
                itemStack = e.stack / 2;
                break;
            case 4:
            case 5:
                itemStack = e.stack;
                break;
            default:
                itemStack = 1;
                break;
        }
        priceFactor = (float)itemStack / (float)e.stack;
    }
    else
        itemStack = e.stack;
    
    itemBid = priceFactor * (urand(10,30) * 0.05f) * e.bid;
    itemBuyOut = priceFactor * (urand(16,30) * 0.05f) * e.buyout;
    if (!urand(0, 4))
        itemBuyOut--;
        
    if (itemBid > itemBuyOut)
        itemBid = itemBuyOut - 1;

    item->SetCount(itemStack);

    uint32 dep = sAuctionMgr.GetAuctionDeposit(m_auctionHouseEntry, etime, item);

    AuctionEntry* auctionEntry       = new AuctionEntry;
    auctionEntry->Id                 = sObjectMgr.GenerateAuctionID();
    auctionEntry->auctionHouseEntry  = m_auctionHouseEntry;
    auctionEntry->itemGuidLow        = item->GetGUIDLow();
    auctionEntry->itemTemplate       = item->GetEntry();
    auctionEntry->owner              = 0;
    auctionEntry->startbid           = itemBid;
    auctionEntry->buyout             = itemBuyOut;
    auctionEntry->bidder             = 0;
    auctionEntry->bid                = 0;
    auctionEntry->deposit            = dep;
    auctionEntry->depositTime        = time(nullptr);
    auctionEntry->expireTime         = (time_t) etime + time(nullptr);

    item->SaveToDB();

    sAuctionMgr.AddAItem(item);
    auctionHouse->AddAuction(auctionEntry);
    auctionEntry->SaveToDB();
}

bool ChatHandler::HandleAHBotUpdateCommand(char *args)
{
    sAuctionHouseBotMgr.Update(true);
    SendSysMessage("[AHBot] Update finished.");
    return true;
}

bool ChatHandler::HandleAHBotReloadCommand(char *args)
{
    sAuctionHouseBotMgr.Load();
    SendSysMessage("[AHBot] Reload finished.");
    return true;
}
