#pragma once
#include "InventoryChanger.h"
#include <vector>
void AddItem(CMsgSOCacheSubscribed::SubscribedType* Object, int index, int itemDefIndex, int rarity, int paintKit, int seed, float wear, std::string name)
{
	CSOEconItem Skin;
	Skin.set_id(20000 + index);
	Skin.set_account_id(Interfaces::SteamUser()->GetSteamID().GetAccountID());
	Skin.set_def_index(itemDefIndex);
	Skin.set_inventory(20000 + index);
	Skin.set_origin(24);
	Skin.set_quantity(1);
	Skin.set_level(1);
	Skin.set_style(0);
	Skin.set_flags(0);
	Skin.set_in_use(false);
	Skin.set_original_id(0);
	Skin.set_rarity(rarity);
	Skin.set_quality(0);
	if (name.size() > 0)
		Skin.set_custom_name(name.data());
	// Paint Kit
	auto PaintKitAttribute = Skin.add_attribute();
	float PaintKitAttributeValue = (float)paintKit;
	PaintKitAttribute->set_def_index(6);
	PaintKitAttribute->set_value_bytes(&PaintKitAttributeValue, 4);
	// Paint Seed
	auto SeedAttribute = Skin.add_attribute();
	float SeedAttributeValue = (float)seed;
	SeedAttribute->set_def_index(7);
	SeedAttribute->set_value_bytes(&SeedAttributeValue, 4);
	// Paint Wear
	auto WearAttribute = Skin.add_attribute();
	float WearAttributeValue = wear;
	WearAttribute->set_def_index(8);
	WearAttribute->set_value_bytes(&WearAttributeValue, 4);
	// Stickers
	for (int j = 0; j < 4; j++)
	{
		// Sticker Kit
		CSOEconItemAttribute* StickerKitAttribute = Skin.add_attribute();
		uint32_t StickerKitAttributeValue = 0;
		StickerKitAttribute->set_def_index(113 + 4 * j);
		StickerKitAttribute->set_value_bytes(&StickerKitAttributeValue, 4);
		// Sticker Wear
		CSOEconItemAttribute* StickerWearAttribute = Skin.add_attribute();
		float StickerWearAttributeValue = 0.001f;
		StickerWearAttribute->set_def_index(114 + 4 * j);
		StickerWearAttribute->set_value_bytes(&StickerWearAttributeValue, 4);
		// Sticker Scale
		CSOEconItemAttribute* StickerScaleAttribute = Skin.add_attribute();
		float StickerScaleAttributeValue = 1.f;
		StickerScaleAttribute->set_def_index(115 + 4 * j);
		StickerScaleAttribute->set_value_bytes(&StickerScaleAttributeValue, 4);
		// Sticker Rotation
		CSOEconItemAttribute* StickerRotationAttribute = Skin.add_attribute();
		float StickerRotationAttributeValue = 0.f;
		StickerRotationAttribute->set_def_index(116 + 4 * j);
		StickerRotationAttribute->set_value_bytes(&StickerRotationAttributeValue, 4);
	}
	Object->add_object_data(Skin.SerializeAsString());
}

void CInventoryChanger::ApplyMedals(CMsgSOCacheSubscribed::SubscribedType* pInventoryCacheObject)
{
	CSOEconItem Medal;
	Medal.set_account_id(Interfaces::SteamUser()->GetSteamID().GetAccountID());
	Medal.set_origin(9);
	Medal.set_rarity(6);
	Medal.set_quantity(1);
	Medal.set_quality(4);
	Medal.set_level(1);
	CSOEconItemAttribute* TimeAcquiredAttribute = Medal.add_attribute();
	uint32_t TimeAcquiredAttributeValue = 0;
	TimeAcquiredAttribute->set_def_index(222);
	TimeAcquiredAttribute->set_value_bytes(&TimeAcquiredAttributeValue, 4);
	int i = 10000;
	for (uint32_t MedalIndex : { Settings::InventoryChanger::Coin[1], Settings::InventoryChanger::Coin[2], Settings::InventoryChanger::Coin[3], Settings::InventoryChanger::Coin[4], Settings::InventoryChanger::Coin[5], Settings::InventoryChanger::Coin[6], Settings::InventoryChanger::Coin[7], Settings::InventoryChanger::Coin[8], Settings::InventoryChanger::Coin[9], Settings::InventoryChanger::Coin[10] } /*Settings::InventoryChanger::medals*/)
	{
		Medal.set_def_index(MedalIndex);
		Medal.set_inventory(i);
		Medal.set_id(i);
		pInventoryCacheObject->add_object_data(Medal.SerializeAsString());
		i++;
	}
}

void CInventoryChanger::PostRetrieveMessage(uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
{
	if (Settings::InventoryChanger::EnableCoins)
	{
		uint32_t messageType = *punMsgType & 0x7FFFFFFF;

		if (messageType == k_EMsgGCCStrike15_v2_MatchmakingGC2ClientHello)
		{
			CMsgGCCStrike15_v2_MatchmakingGC2ClientHello Message;

			try
			{
				if (!Message.ParsePartialFromArray((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8))
					return;
			}
			catch (...)
			{
				return;
			}

			Message.set_player_level(int(Settings::InventoryChanger::Level));
			Message.set_player_cur_xp(int(Settings::InventoryChanger::XP));

			// TournamentPlayer

			PlayerRankingInfo* Ranking = Message.mutable_ranking();
			Ranking->set_account_id(Interfaces::SteamUser()->GetSteamID().GetAccountID());
			Ranking->set_rank_id(int(Settings::InventoryChanger::Rank));
			Ranking->set_wins(int(Settings::InventoryChanger::Wins));

			PlayerCommendationInfo* Commendation = Message.mutable_commendation();
			Commendation->set_cmd_friendly(int(Settings::InventoryChanger::Rec1));
			Commendation->set_cmd_leader(int(Settings::InventoryChanger::Rec2));
			Commendation->set_cmd_teaching(int(Settings::InventoryChanger::Rec3));

			if ((uint32_t)Message.ByteSize() <= cubDest - 8)
			{
				Message.SerializeToArray((void*)((DWORD)pubDest + 8), Message.ByteSize());

				*pcubMsgSize = Message.ByteSize() + 8;
			}
		}
	}

	if (Settings::InventoryChanger::enabled)
	{
		uint32_t MessageType = *punMsgType & 0x7FFFFFFF;
		if (MessageType != k_EMsgGCClientWelcome)
			return;

		CMsgClientWelcome Message;
		try
		{
			if (!Message.ParsePartialFromArray((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8))
				return;
		}
		catch (...)
		{
			return;
		}
		if (Message.outofdate_subscribed_caches_size() <= 0)
			return;
		CMsgSOCacheSubscribed* Cache = Message.mutable_outofdate_subscribed_caches(0);
		for (int i = 0; i < Cache->objects_size(); i++)
		{
			CMsgSOCacheSubscribed::SubscribedType* Object = Cache->mutable_objects(i);
			if (!Object->has_type_id())
				continue;
			if (Object->type_id() == 1) {
				for (int j = 0; j < Object->object_data_size(); j++)
				{
					std::string* ObjectData = Object->mutable_object_data(j);
					CSOEconItem Item;
					if (!Item.ParseFromArray((void*)const_cast<char*>(ObjectData->data()), ObjectData->size()))
						continue;
					if (Item.equipped_state_size() <= 0)
						continue;
				}

				for (int i = 0; i <= 33; i++)
				{

					if (g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackPaintKit > 0)
					{
						AddItem(Object, i, pWeaponItemIndexData[i], g_SkinChangerCfg[pWeaponItemIndexData[i]].Rarity, g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackPaintKit, 0, g_SkinChangerCfg[pWeaponItemIndexData[i]].flFallbackWear, "");
					}

					if (Settings::Skin::knf_ct_model == 1)
					{
						AddItem(Object, 55, WEAPON_KNIFE_BAYONET, 6, Settings::Skin::knf_ct_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}
					if (Settings::Skin::knf_ct_model == 2)
					{
						AddItem(Object, 56, WEAPON_KNIFE_FLIP, 6, Settings::Skin::knf_ct_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}
					if (Settings::Skin::knf_ct_model == 3)
					{
						AddItem(Object, 57, WEAPON_KNIFE_GUT, 6, Settings::Skin::knf_ct_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}
					if (Settings::Skin::knf_ct_model == 4)
					{
						AddItem(Object, 58, WEAPON_KNIFE_KARAMBIT, 6, Settings::Skin::knf_ct_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}
					if (Settings::Skin::knf_ct_model == 5)
					{
						AddItem(Object, 59, WEAPON_KNIFE_M9_BAYONET, 6, Settings::Skin::knf_ct_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}
					if (Settings::Skin::knf_ct_model == 6)
					{
						AddItem(Object, 60, WEAPON_KNIFE_TACTICAL, 6, Settings::Skin::knf_ct_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}
					if (Settings::Skin::knf_ct_model == 7)
					{
						AddItem(Object, 61, WEAPON_KNIFE_FALCHION, 6, Settings::Skin::knf_ct_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}
					if (Settings::Skin::knf_ct_model == 8)
					{
						AddItem(Object, 62, WEAPON_KNIFE_SURVIVAL_BOWIE, 6, Settings::Skin::knf_ct_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}
					if (Settings::Skin::knf_ct_model == 9)
					{
						AddItem(Object, 63, WEAPON_KNIFE_BUTTERFLY, 6, Settings::Skin::knf_ct_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}
					if (Settings::Skin::knf_ct_model == 10)
					{
						AddItem(Object, 64, WEAPON_KNIFE_PUSH, 6, Settings::Skin::knf_ct_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}


					if (Settings::Skin::knf_tt_model == 1)
					{
						AddItem(Object, 55, WEAPON_KNIFE_BAYONET, 6, Settings::Skin::knf_tt_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}
					if (Settings::Skin::knf_tt_model == 2)
					{
						AddItem(Object, 56, WEAPON_KNIFE_FLIP, 6, Settings::Skin::knf_tt_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}
					if (Settings::Skin::knf_tt_model == 3)
					{
						AddItem(Object, 57, WEAPON_KNIFE_GUT, 6, Settings::Skin::knf_tt_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}
					if (Settings::Skin::knf_tt_model == 4)
					{
						AddItem(Object, 58, WEAPON_KNIFE_KARAMBIT, 6, Settings::Skin::knf_tt_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}
					if (Settings::Skin::knf_tt_model == 5)
					{
						AddItem(Object, 59, WEAPON_KNIFE_M9_BAYONET, 6, Settings::Skin::knf_tt_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}
					if (Settings::Skin::knf_tt_model == 6)
					{
						AddItem(Object, 60, WEAPON_KNIFE_TACTICAL, 6, Settings::Skin::knf_tt_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}
					if (Settings::Skin::knf_tt_model == 7)
					{
						AddItem(Object, 61, WEAPON_KNIFE_FALCHION, 6, Settings::Skin::knf_tt_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}
					if (Settings::Skin::knf_tt_model == 8)
					{
						AddItem(Object, 62, WEAPON_KNIFE_SURVIVAL_BOWIE, 6, Settings::Skin::knf_tt_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}
					if (Settings::Skin::knf_tt_model == 9)
					{
						AddItem(Object, 63, WEAPON_KNIFE_BUTTERFLY, 6, Settings::Skin::knf_tt_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}
					if (Settings::Skin::knf_tt_model == 10)
					{
						AddItem(Object, 64, WEAPON_KNIFE_PUSH, 6, Settings::Skin::knf_tt_skin, 0, g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, "");
					}


					if (Settings::Skin::gloves_skin == 1)
					{
						AddItem(Object, 112, WEAPON_GLOVES_SPORT, 6, 10018, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 2)
					{
						AddItem(Object, 113, WEAPON_GLOVES_SPORT, 6, 10037, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 3)
					{
						AddItem(Object, 114, WEAPON_GLOVES_SPORT, 6, 10038, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 4)
					{
						AddItem(Object, 115, WEAPON_GLOVES_SPORT, 6, 10019, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 5)
					{
						AddItem(Object, 116, WEAPON_GLOVES_SPORT, 6, 10048, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 6)
					{
						AddItem(Object, 117, WEAPON_GLOVES_SPORT, 6, 10047, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 7)
					{
						AddItem(Object, 118, WEAPON_GLOVES_SPORT, 6, 10046, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 8)
					{
						AddItem(Object, 119, WEAPON_GLOVES_SPORT, 6, 10045, 0, 0.001f, "");
					}


					if (Settings::Skin::gloves_skin == 9)
					{
						AddItem(Object, 121, WEAPON_GLOVES_MOTO, 6, 10024, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 10)
					{
						AddItem(Object, 122, WEAPON_GLOVES_MOTO, 6, 10026, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 11)
					{
						AddItem(Object, 123, WEAPON_GLOVES_MOTO, 6, 10027, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 12)
					{
						AddItem(Object, 124, WEAPON_GLOVES_MOTO, 6, 10028, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 13)
					{
						AddItem(Object, 125, WEAPON_GLOVES_MOTO, 6, 10052, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 14)
					{
						AddItem(Object, 126, WEAPON_GLOVES_MOTO, 6, 10051, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 15)
					{
						AddItem(Object, 127, WEAPON_GLOVES_MOTO, 6, 10050, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 16)
					{
						AddItem(Object, 128, WEAPON_GLOVES_MOTO, 6, 10049, 0, 0.001f, "");
					}


					if (Settings::Skin::gloves_skin == 17)
					{
						AddItem(Object, 129, WEAPON_GLOVES_SPEC, 6, 10033, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 18)
					{
						AddItem(Object, 130, WEAPON_GLOVES_SPEC, 6, 10034, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 19)
					{
						AddItem(Object, 131, WEAPON_GLOVES_SPEC, 6, 10035, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 20)
					{
						AddItem(Object, 132, WEAPON_GLOVES_SPEC, 6, 10030, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 21)
					{
						AddItem(Object, 133, WEAPON_GLOVES_SPEC, 6, 10064, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 22)
					{
						AddItem(Object, 134, WEAPON_GLOVES_SPEC, 6, 10063, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 23)
					{
						AddItem(Object, 135, WEAPON_GLOVES_SPEC, 6, 10062, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 24)
					{
						AddItem(Object, 136, WEAPON_GLOVES_SPEC, 6, 10061, 0, 0.001f, "");
					}

					if (Settings::Skin::gloves_skin == 25)
					{
						AddItem(Object, 137, WEAPON_GLOVES_DRIVER, 6, 10013, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 26)
					{
						AddItem(Object, 138, WEAPON_GLOVES_DRIVER, 6, 10015, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 27)
					{
						AddItem(Object, 139, WEAPON_GLOVES_DRIVER, 6, 10016, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 28)
					{
						AddItem(Object, 140, WEAPON_GLOVES_DRIVER, 6, 10040, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 29)
					{
						AddItem(Object, 141, WEAPON_GLOVES_DRIVER, 6, 10044, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 30)
					{
						AddItem(Object, 142, WEAPON_GLOVES_DRIVER, 6, 10043, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 31)
					{
						AddItem(Object, 143, WEAPON_GLOVES_DRIVER, 6, 10042, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 32)
					{
						AddItem(Object, 144, WEAPON_GLOVES_DRIVER, 6, 10041, 0, 0.001f, "");
					}

					if (Settings::Skin::gloves_skin == 33)
					{
						AddItem(Object, 145, WEAPON_GLOVES_WARPS, 6, 10009, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 34)
					{
						AddItem(Object, 146, WEAPON_GLOVES_WARPS, 6, 10010, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 35)
					{
						AddItem(Object, 147, WEAPON_GLOVES_WARPS, 6, 10036, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 36)
					{
						AddItem(Object, 148, WEAPON_GLOVES_WARPS, 6, 10021, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 37)
					{
						AddItem(Object, 149, WEAPON_GLOVES_WARPS, 6, 10056, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 38)
					{
						AddItem(Object, 150, WEAPON_GLOVES_WARPS, 6, 10055, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 39)
					{
						AddItem(Object, 151, WEAPON_GLOVES_WARPS, 6, 10054, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 40)
					{
						AddItem(Object, 152, WEAPON_GLOVES_WARPS, 6, 10053, 0, 0.001f, "");
					}

					if (Settings::Skin::gloves_skin == 41)
					{
						AddItem(Object, 153, WEAPON_GLOVES_BLOODHOUND, 6, 10006, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 42)
					{
						AddItem(Object, 154, WEAPON_GLOVES_BLOODHOUND, 6, 10007, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 43)
					{
						AddItem(Object, 155, WEAPON_GLOVES_BLOODHOUND, 6, 10008, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 44)
					{
						AddItem(Object, 156, WEAPON_GLOVES_BLOODHOUND, 6, 10039, 0, 0.001f, "");
					}


					if (Settings::Skin::gloves_skin == 45)
					{
						AddItem(Object, 157, WEAPON_GLOVES_HYDRA, 6, 10060, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 46)
					{
						AddItem(Object, 158, WEAPON_GLOVES_HYDRA, 6, 10059, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 47)
					{
						AddItem(Object, 159, WEAPON_GLOVES_HYDRA, 6, 10058, 0, 0.001f, "");
					}
					if (Settings::Skin::gloves_skin == 48)
					{
						AddItem(Object, 160, WEAPON_GLOVES_HYDRA, 6, 10057, 0, 0.001f, "");
					}
				}

				ApplyMedals(Object);
			}
		}
		if ((uint32_t)Message.ByteSize() <= cubDest - 8)
		{
			Message.SerializeToArray((void*)((DWORD)pubDest + 8), Message.ByteSize());
			*pcubMsgSize = Message.ByteSize() + 8;
		}
	}
}
bool CInventoryChanger::PreSendMessage(uint32_t &unMsgType, void* pubData, uint32_t &cubData)
{
	if (!Settings::InventoryChanger::enabled)
		return true;
	uint32_t MessageType = unMsgType & 0x7FFFFFFF;
	return true;
}