BEGIN_ENUM(NetMessage, uint32_t)
	EV(NETMSG_HANDSHAKE, 1)
	EV(NETMSG_HOST_WELCOME, 2)
	EV(NETMSG_HOST_REFUSE, 3)
	EV(NETMSG_HOST_REFUSEPLAYER, 4)
	EV(NETMSG_CLIENT_JOINED, 8)
	EV(NETMSG_CLIENT_LEFT, 9)
	EV(NETMSG_HOST_LEFT, 5)
	EV(NETMSG_CLIENT_CONNECT, 6)
	EV(NETMSG_CLIENT_ACCEPT, 7)
	EV(NETMSG_PLAYER_CONNECT, 10)
	EV(NETMSG_PLAYER_DISCONNECT, 11)
	EV(NETMSG_PLAYER_JOINED, 13)
	EV(NETMSG_PLAYER_ACCEPT, 12)
	EV(NETMSG_PLAYER_LEFT, 14)
	EV(NETMSG_PLAYER_CHANGENAME, 15)
	EV(NETMSG_PLAYER_NAMECHANGED, 16)
	EV(NETMSG_ECS_REPLICATION, 17)
	EV(NETMSG_VOICEDATA, 18)
	EV(NETMSG_MIC_DISABLED, 20)
	EV(NETMSG_SKIPMOVIE_RESULT, 19)
	EV(NETMSG_CHAT, 185)
	EV(NETMSG_PEER_ACTIVATE, 118)
	EV(NETMSG_PEER_DEACTIVATE, 119)
	EV(NETMSG_MODULE_LOAD, 192)
	EV(NETMSG_MODULE_LOADED, 193)
	EV(NETMSG_SESSION_LOAD, 194)
	EV(NETMSG_SESSION_LOADED, 195)
	EV(NETMSG_SESSION_UNLOADED, 196)
	EV(NETMSG_LEVEL_LOAD, 197)
	EV(NETMSG_LEVEL_CREATED, 198)
	EV(NETMSG_LEVEL_LOADED, 199)
	EV(NETMSG_LOAD_START, 200)
	EV(NETMSG_LOAD_STARTED, 201)
	EV(NETMSG_LEVEL_INSTANTIATE_SWAP, 202)
	EV(NETMSG_LEVEL_SWAP_READY, 203)
	EV(NETMSG_LEVEL_SWAP_COMPLETE, 204)
	EV(NETMSG_LEVEL_START, 205)
	EV(NETMSG_CHARACTER_CREATE, 26)
	EV(NETMSG_CHARACTER_DESTROY, 27)
	EV(NETMSG_CHARACTER_CONFIRMATION, 28)
	EV(NETMSG_CHARACTER_ACTIVATE, 29)
	EV(NETMSG_CHARACTER_DEACTIVATE, 30)
	EV(NETMSG_CHARACTER_ASSIGN, 31)
	EV(NETMSG_CHARACTER_CHANGE_OWNERSHIP, 207)
	EV(NETMSG_CHARACTER_STEERING, 32)
	EV(NETMSG_CHARACTER_MOVEMENT_FALLING, 237)
	EV(NETMSG_CHARACTER_ACTION, 33)
	EV(NETMSG_CHARACTER_ACTION_DATA, 34)
	EV(NETMSG_CHARACTER_ACTION_REQUEST_RESULT, 35)
	EV(NETMSG_CHARACTER_STATUS, 36)
	EV(NETMSG_CHARACTER_STATUS_LIFETIME, 37)
	EV(NETMSG_CHARACTER_DIALOG, 40)
	EV(NETMSG_CHARACTER_USE_MOVEMENT, 43)
	EV(NETMSG_CHARACTER_MOVE_FAILED, 44)
	EV(NETMSG_CHARACTER_SELECTED_CLIMB_ON, 45)
	EV(NETMSG_FOLLOWER_CANT_USE_ITEM, 46)
	EV(NETMSG_CHARACTER_AOO, 47)
	EV(NETMSG_CHARACTER_UPDATE, 38)
	EV(NETMSG_CHARACTER_CONTROL, 39)
	EV(NETMSG_CHARACTER_TRANSFORM, 48)
	EV(NETMSG_CHARACTER_PICKPOCKET, 50)
	EV(NETMSG_CHARACTER_OFFSTAGE, 51)
	EV(NETMSG_CHARACTER_IN_DIALOG, 54)
	EV(NETMSG_CHARACTER_LOOT, 52)
	EV(NETMSG_CHARACTER_ITEM_USED, 53)
	EV(NETMSG_CHARACTER_UNSHEATHING, 41)
	EV(NETMSG_CHARACTER_END_REPOSE, 42)
	EV(NETMSG_CHARACTER_BEHAVIOR, 55)
	EV(NETMSG_PARTYGROUP, 60)
	EV(NETMSG_PARTYORDER, 61)
	EV(NETMSG_PARTY_BLOCKFOLLOW, 64)
	EV(NETMSG_PROJECTILE_CREATE, 92)
	EV(NETMSG_PROJECTILE_UPDATE, 93)
	EV(NETMSG_MULTIPLE_TARGET_OPERATION, 257)
	EV(NETMSG_TURNBASED_FINISHTEAM, 98)
	EV(NETMSG_TURNBASED_SETTEAM, 99)
	EV(NETMSG_TURNBASED_FLEECOMBATRESULT, 100)
	EV(NETMSG_TURNBASED_FLEE_REQUEST, 101)
	EV(NETMSG_TURNBASED_ENDTURN_REQUEST, 102)
	EV(NETMSG_TURNBASED_CANCEL_ENDTURN_REQUEST, 103)
	EV(NETMSG_TURNBASED_BEGIN_CANCEL_ENDTURN_REQUEST, 104)
	EV(NETMSG_TURNBASED_END_CANCEL_ENDTURN_REQUEST, 105)
	EV(NETMSG_TURNBASED_STARTTURN_CONFIRMATION, 106)
	EV(NETMSG_TURNBASED_SKIP_START_DELAY, 107)
	EV(NETMSG_UI_FORCETURNBASED_ENTERED, 108)
	EV(NETMSG_UI_FORCETURNBASED_LEFT, 109)
	EV(NETMSG_UI_FORCETURNBASED_TURN_STARTED, 110)
	EV(NETMSG_UI_FORCETURNBASED_TURN_ENDED, 111)
	EV(NETMSG_ITEM_CREATE, 67)
	EV(NETMSG_ITEM_DESTROY, 68)
	EV(NETMSG_ITEM_ACTIVATE, 69)
	EV(NETMSG_ITEM_DEACTIVATE, 70)
	EV(NETMSG_ITEM_DESTINATION, 71)
	EV(NETMSG_ITEM_UPDATE, 72)
	EV(NETMSG_ITEM_USE_REMOTELY, 73)
	EV(NETMSG_ITEM_MOVE_TO_WORLD, 74)
	EV(NETMSG_ITEM_ACTION, 75)
	EV(NETMSG_ITEM_STATUS, 76)
	EV(NETMSG_ITEM_STATUS_LIFETIME, 77)
	EV(NETMSG_ITEM_TRANSFORM, 78)
	EV(NETMSG_ITEM_CONFIRMATION, 79)
	EV(NETMSG_ITEM_OFFSTAGE, 80)
	EV(NETMSG_ITEM_MOVED_INFORM, 81)
	EV(NETMSG_INVENTORY_CREATE, 82)
	EV(NETMSG_INVENTORY_CREATE_AND_OPEN, 83)
	EV(NETMSG_INVENTORY_DESTROY, 84)
	EV(NETMSG_INVENTORY_VIEW_CREATE, 85)
	EV(NETMSG_INVENTORY_VIEW_DESTROY, 86)
	EV(NETMSG_INVENTORY_VIEW_UPDATE_ITEMS, 87)
	EV(NETMSG_INVENTORY_VIEW_UPDATE_PARENTS, 88)
	EV(NETMSG_INVENTORY_VIEW_SORT, 89)
	EV(NETMSG_INVENTORY_ITEM_UPDATE, 90)
	EV(NETMSG_INVENTORY_LOCKSTATE_SYNC, 91)
	EV(NETMSG_SURFACE_CREATE, 22)
	EV(NETMSG_SURFACE_META, 23)
	EV(NETMSG_DARKNESSTILE_UPDATE, 24)
	EV(NETMSG_GAMEACTION, 25)
	EV(NETMSG_GAMEOVER, 161)
	EV(NETMSG_ACT_OVER, 162)
	EV(NETMSG_SPELL_LEARN, 120)
	EV(NETMSG_SPELL_REMOVE_LEARNED, 121)
	EV(NETMSG_SPELL_PREPARE, 122)
	EV(NETMSG_SPELL_CANCEL, 123)
	EV(NETMSG_TADPOLE_POWER_ADD, 124)
	EV(NETMSG_TADPOLE_POWERS_REMOVE, 125)
	EV(NETMSG_GAMECONTROL_UPDATE_S2C, 129)
	EV(NETMSG_GAMECONTROL_UPDATE_C2S, 130)
	EV(NETMSG_GAMECONTROL_PRICETAG, 131)
	EV(NETMSG_CHANGE_RULESETS, 206)
	EV(NETMSG_REALTIME_MULTIPLAY, 226)
	EV(NETMSG_CHARACTER_ERROR, 133)
	EV(NETMSG_SHOW_ERROR, 254)
	EV(NETMSG_REQUESTAUTOSAVE, 188)
	EV(NETMSG_SAVEGAME, 189)
	EV(NETMSG_SAVEGAMEHANDSHAKE, 190)
	EV(NETMSG_SAVEGAMEHANDSHAKE_SCREENSHOT, 191)
	EV(NETMSG_EFFECT_CREATE, 126)
	EV(NETMSG_EFFECT_FORGET, 127)
	EV(NETMSG_EFFECT_DESTROY, 128)
	EV(NETMSG_CACHETEMPLATE, 134)
	EV(NETMSG_OVERHEADTEXT, 135)
	EV(NETMSG_COMBATLOG, 136)
	EV(NETMSG_COMBATLOGITEMINTERACTION, 137)
	EV(NETMSG_COMBATLOGENTRIES, 138)
	EV(NETMSG_SHOW_ENTER_REGION_UI_MESSAGE, 146)
	EV(NETMSG_CHARACTER_TELEPORT, 49)
	EV(NETMSG_SCREEN_FADE_FINISHED, 139)
	EV(NETMSG_OPEN_CUSTOM_BOOK_UI_MESSAGE, 140)
	EV(NETMSG_CLOSE_CUSTOM_BOOK_UI_MESSAGE, 141)
	EV(NETMSG_OPEN_MESSAGE_BOX_MESSAGE, 142)
	EV(NETMSG_CLOSED_MESSAGE_BOX_MESSAGE, 143)
	EV(NETMSG_OPEN_WAYPOINT_UI_MESSAGE, 144)
	EV(NETMSG_CLOSE_UI_MESSAGE, 145)
	EV(NETMSG_OPEN_CRAFT_UI_MESSAGE, 214)
	EV(NETMSG_JOURNAL_RESET, 147)
	EV(NETMSG_TELEPORT_WAYPOINT, 221)
	EV(NETMSG_QUEST_STEP, 148)
	EV(NETMSG_QUEST_CATEGORY_UPDATE, 149)
	EV(NETMSG_QUEST_PROGRESS, 150)
	EV(NETMSG_QUESTS_LOADED, 151)
	EV(NETMSG_JOURNALRECIPE_UPDATE, 153)
	EV(NETMSG_TROPHY_UPDATE, 152)
	EV(NETMSG_MARKER_UI_UPDATE, 155)
	EV(NETMSG_MARKER_UI_CREATE, 154)
	EV(NETMSG_QUEST_TRACK, 156)
	EV(NETMSG_JOURNALDIALOGLOG_UPDATE, 208)
	EV(NETMSG_UI_QUESTSELECTED, 157)
	EV(NETMSG_MYSTERY_ADVANCED, 158)
	EV(NETMSG_MYSTERY_DISABLED, 159)
	EV(NETMSG_NOTIFICATION, 160)
	EV(NETMSG_REGISTER_WAYPOINT, 163)
	EV(NETMSG_UNLOCK_WAYPOINT, 216)
	EV(NETMSG_LOCK_WAYPOINT, 218)
	EV(NETMSG_FLAG_UPDATE, 164)
	EV(NETMSG_LIGHTING_OVERRIDE, 183)
	EV(NETMSG_ATMOSPHERE_OVERRIDE, 184)
	EV(NETMSG_CAMERA_ACTIVATE, 180)
	EV(NETMSG_CAMERA_ROTATE, 181)
	EV(NETMSG_CAMERA_TARGET, 182)
	EV(NETMSG_SHROUD_UPDATE, 65)
	EV(NETMSG_PLAYMOVIE, 132)
	EV(NETMSG_TRADE_ACTION, 179)
	EV(NETMSG_ACHIEVEMENT_UNLOCKED_MESSAGE, 209)
	EV(NETMSG_SAVEGAME_LOAD_FAIL, 210)
	EV(NETMSG_SERVER_COMMAND, 211)
	EV(NETMSG_SERVER_NOTIFICATION, 212)
	EV(NETMSG_GAMETIME_SYNC, 186)
	EV(NETMSG_STORY_ELEMENT_UI, 213)
	EV(NETMSG_ACHIEVEMENT_PROGRESS_MESSAGE, 215)
	EV(NETMSG_TELEPORT_ACK, 234)
	EV(NETMSG_COMBINE_RESULT, 219)
	EV(NETMSG_UNLOCK_ITEM, 220)
	EV(NETMSG_DIALOG_STATE_MESSAGE, 168)
	EV(NETMSG_DIALOG_NODE_MESSAGE, 169)
	EV(NETMSG_DIALOG_ANSWER_MESSAGE, 170)
	EV(NETMSG_DIALOG_ANSWER_HIGHLIGHT_MESSAGE, 171)
	EV(NETMSG_DIALOG_HISTORY_MESSAGE, 172)
	EV(NETMSG_DIALOG_LISTEN, 227)
	EV(NETMSG_DIALOG_ACTORJOINS_MESSAGE, 173)
	EV(NETMSG_DIALOG_ACTORLEAVES_MESSAGE, 174)
	EV(NETMSG_DIALOG_REPLACESPEAKER_MESSAGE, 175)
	EV(NETMSG_DIALOG_INVALID_ANSWER, 228)
	EV(NETMSG_DIALOG_SUGGESTANSWER_MESSAGE, 176)
	EV(NETMSG_DIALOG_REQUEST_IN_DIALOGUE_ATTACK_MESSAGE, 177)
	EV(NETMSG_DIALOG_TIMELINE_UNLOADED_MESSAGE, 178)
	EV(NETMSG_DIALOG_INSTANCEDIALOGCHANGED, 330)
	EV(NETMSG_LOBBY_DATAUPDATE, 231)
	EV(NETMSG_LOBBY_USERUPDATE, 232)
	EV(NETMSG_LOBBY_STARTGAME, 233)
	EV(NETMSG_CAMERA_SPLINE, 236)
	EV(NETMSG_NET_ENTITY_CREATE, 116)
	EV(NETMSG_NET_ENTITY_DESTROY, 117)
	EV(NETMSG_MUSIC_EVENT, 230)
	EV(NETMSG_MUSIC_STATE, 229)
	EV(NETMSG_PAUSE, 239)
	EV(NETMSG_UNPAUSE, 240)
	EV(NETMSG_READYCHECK, 238)
	EV(NETMSG_DIPLOMACY, 241)
	EV(NETMSG_TRIGGER_CREATE, 242)
	EV(NETMSG_TRIGGER_DESTROY, 243)
	EV(NETMSG_TRIGGER_UPDATE, 244)
	EV(NETMSG_PING_REQUEST, 245)
	EV(NETMSG_CUSTOM_STATS_DEFINITION_CREATE, 246)
	EV(NETMSG_CUSTOM_STATS_DEFINITION_REMOVE, 247)
	EV(NETMSG_CUSTOM_STATS_DEFINITION_UPDATE, 248)
	EV(NETMSG_CUSTOM_STATS_CREATE, 249)
	EV(NETMSG_CUSTOM_STATS_UPDATE, 250)
	EV(NETMSG_GIVE_REWARD, 251)
	EV(NETMSG_LOAD_GAME_WITH_ADDONS, 252)
	EV(NETMSG_LOAD_GAME_WITH_ADDONS_FAIL, 253)
	EV(NETMSG_CLIENT_GAME_SETTINGS, 255)
	EV(NETMSG_DLC_UPDATE, 258)
	EV(NETMSG_TIMELINE_HANDSHAKE, 259)
	EV(NETMSG_TIMELINE_ACTOR_HANDSHAKE, 260)
	EV(NETMSG_TIMELINE_NODECOMPLETED, 261)
	EV(NETMSG_TIMELINE_PLAYER_WATCHING, 262)
	EV(NETMSG_TIMELINE_REQUEST_FASTFORWARD, 263)
	EV(NETMSG_TIMELINE_WORLD_CINEMATIC_COMPLETED, 264)
	EV(NETMSG_CLIENT_DIALOG_PRIVACY_CHANGED, 340)
	EV(NETMSG_CLIENT_DIALOG_JOIN_PRIVACY_FAILED, 341)
	EV(NETMSG_FORCE_TURN_BASED_END_PLAYER_TURN_REQUEST, 265)
	EV(NETMSG_FORCE_TURN_BASED_TOGGLE_REQUEST, 266)
	EV(NETMSG_CHARACTER_REQUEST_WEAPON_SET_SWITCH, 270)
	EV(NETMSG_CHARACTER_REQUEST_ARMOR_SET_SWITCH, 271)
	EV(NETMSG_REQUESTED_ROLL, 267)
	EV(NETMSG_PASSIVE_ROLL_SEQUENCE, 268)
	EV(NETMSG_CHARACTER_PATHING, 269)
	EV(NETMSG_CHARACTER_CREATION_ABORT, 272)
	EV(NETMSG_CHARACTER_CREATION_READY, 273)
	EV(NETMSG_CHARACTER_CREATION_UPDATE, 274)
	EV(NETMSG_CHARACTER_CREATION_PLAYORIGINTIMELINE, 275)
	EV(NETMSG_LEVEL_UP_UPDATE, 276)
	EV(NETMSG_RESPEC_UPDATE, 277)
	EV(NETMSG_FULL_RESPEC_UPDATE, 278)
	EV(NETMSG_CHARACTER_CREATION_LEVELUP, 279)
	EV(NETMSG_CHARACTER_CREATION_RESPEC, 280)
	EV(NETMSG_UI_INTERACTION_STOPPED, 282)
	EV(NETMSG_PASSIVE_TOGGLE, 288)
	EV(NETMSG_DUALWIELDING_TOGGLE, 289)
	EV(NETMSG_UI_COMBINE_OPEN, 281)
	EV(NETMSG_TUTORIAL_CLOSED, 283)
	EV(NETMSG_TUTORIAL_UI_EVENT, 284)
	EV(NETMSG_TUTORIAL_USER_ACTION, 285)
	EV(NETMSG_TUTORIALS_SHOWING, 286)
	EV(NETMSG_TUTORIALS_RESET, 287)
	EV(NETMSG_SNEAKING_CONES_VISIBLE_TOGGLE, 290)
	EV(NETMSG_HOTBAR_SLOT_SET, 291)
	EV(NETMSG_HOTBAR_SLOT_REMOVE_IS_NEW_FLAG, 292)
	EV(NETMSG_HOTBAR_COLUMN_SET, 293)
	EV(NETMSG_HOTBAR_ROWS_SET, 294)
	EV(NETMSG_HOTBAR_ROW_REMOVE, 295)
	EV(NETMSG_HOTBAR_LOCK_SET, 296)
	EV(NETMSG_HOTBAR_CUSTOM_DECK, 297)
	EV(NETMSG_HOTBAR_CURRENT_DECK_SET, 298)
	EV(NETMSG_PARTY_PRESET_SAVE, 299)
	EV(NETMSG_PARTY_PRESET_LOAD, 300)
	EV(NETMSG_PARTY_PRESET_LEVELUP, 301)
	EV(NETMSG_PARTY_PRESET_SPELL, 302)
	EV(NETMSG_STORY_FLAGS_LOAD, 303)
	EV(NETMSG_ACTIVE_ROLL_MODIFIERS, 307)
	EV(NETMSG_CAST_ALL, 223)
	EV(NETMSG_END_THE_DAY, 224)
	EV(NETMSG_ITEM_TOGGLE_IS_WARE, 304)
	EV(NETMSG_LONG_REST_DECISION, 225)
	EV(NETMSG_REQUEST_RELEASE_CONCENTRATION, 306)
	EV(NETMSG_ROLL_STREAM_ROLL_MODE_TYPE, 305)
	EV(NETMSG_SHORT_REST, 222)
	EV(NETMSG_CHARACTER_DROP_ENTITY, 308)
	EV(NETMSG_CHARACTER_CANCEL_DROP_ENTITY, 309)
	EV(NETMSG_CHARACTER_LOCKPICK, 310)
	EV(NETMSG_CHARACTER_TRAPDISARM, 311)
	EV(NETMSG_CHARACTER_IMPROV_WEAPON, 312)
	EV(NETMSG_CHARACTER_STOWED_VISIBILITY, 313)
	EV(NETMSG_DECLUTTERED_ITEMS, 314)
	EV(NETMSG_SPELL_CAST_START_PREVIEW, 315)
	EV(NETMSG_SPELL_CAST_UPDATE_PREVIEW, 316)
	EV(NETMSG_SPELL_CAST_CONFIRM, 317)
	EV(NETMSG_SPELL_CAST_CANCEL, 318)
	EV(NETMSG_SPELL_CAST_CAST, 319)
	EV(NETMSG_USE_ITEM_ACTION, 320)
	EV(NETMSG_INVENTORY_CREATE_NEW, 321)
	EV(NETMSG_INVENTORY_DESTROY_NEW, 322)
	EV(NETMSG_INVENTORY_ADD, 323)
	EV(NETMSG_INVENTORY_MOVETOSLOT, 324)
	EV(NETMSG_INVENTORY_TRANSFER, 325)
	EV(NETMSG_INVENTORY_SORT, 326)
	EV(NETMSG_INVENTORY_EQUIP, 327)
	EV(NETMSG_INVENTORY_UNEQUIP, 328)
	EV(NETMSG_INVENTORY_LOCK, 329)
	EV(NETMSG_CHARACTER_ORIGINAL_TEMPLATE_UPDATE, 331)
	EV(NETMSG_INTERRUPT_DECISION, 332)
	EV(NETMSG_INTERRUPT_SET_ASK_DATA, 333)
	EV(NETMSG_INTERRUPT_SET_ENABLED_DATA, 334)
	EV(NETMSG_SELECT_DICE, 335)
	EV(NETMSG_CAMERA_ARRIVED, 336)
	EV(NETMSG_CLEAR_RECIPE_NEW_FLAG, 337)
	EV(NETMSG_COMBINE_ITEMS, 338)
	EV(NETMSG_NEW_ITEMS_INSIDE, 339)

	EV(NETMSG_SCRIPT_EXTENDER, 400)
END_ENUM()