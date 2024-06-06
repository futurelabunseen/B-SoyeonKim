#pragma once

// Character Action
#define WKTAG_CHARACTER_ACTION_ATTACK FGameplayTag::RequestGameplayTag(FName("Character.Action.Attack"))
#define WKTAG_CHARACTER_ACTION_NEXTATTACK FGameplayTag::RequestGameplayTag(FName("Character.Action.NextAttack"))
#define WKTAG_CHARACTER_ACTION_SKILL_AOE FGameplayTag::RequestGameplayTag(FName("Character.Action.Skill.AOE"))
#define WKTAG_CHARACTER_ACTION_SKILL_BLOCKATTACK FGameplayTag::RequestGameplayTag(FName("Character.Action.Skill.BlockAttack"))
#define WKTAG_CHARACTER_ACTION_SKILL_FLAMINGSWORD FGameplayTag::RequestGameplayTag(FName("Character.Action.Skill.FlamingSword"))
#define WKTAG_CHARACTER_ACTION_ATTACK_LASTCOMBO FGameplayTag::RequestGameplayTag(FName("Character.Action.Attack.LastCombo"))
#define WKTAG_CHARACTER_ACTION_HITREACT FGameplayTag::RequestGameplayTag(FName("Character.Action.HitReact"))

// Data
#define WKTAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))

// Character State
#define WKTAG_CHARACTER_STATE_ISDEAD FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))
#define WKTAG_CHARACTER_STATE_INVISIBLE FGameplayTag::RequestGameplayTag(FName("Character.State.Invisible"))
#define WKTAG_CHARACTER_STATE_ISFLAMING FGameplayTag::RequestGameplayTag(FName("Character.State.IsFlaming"))
#define WKTAG_CHARACTER_STATE_DEBUFF_STUN FGameplayTag::RequestGameplayTag(FName("Character.State.Debuff.Stun"))


// GameplayCue
#define WKTAG_GC_CHARACTER_ATTACKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.AttackHit"))
#define WKTAG_GC_SKILL_AOE FGameplayTag::RequestGameplayTag(FName("GameplayCue.Skill.AOE"))
#define WKTAG_GC_SKILL_BLOCKATTACK FGameplayTag::RequestGameplayTag(FName("GameplayCue.Skill.BlockAttack"))
#define WKTAG_GC_CHARACTER_BLOCKATTACK FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.BlockAttack"))
#define WKTAG_GC_CHARACTER_BURN FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.Burn"))

// Evnet
#define WKTAG_EVENT_CHARACTER_WEAPON_EQUIP FGameplayTag::RequestGameplayTag(FName("Event.Character.Weapon.Equip"))
#define WKTAG_EVENT_CHARACTER_WEAPON_UNEQUIP FGameplayTag::RequestGameplayTag(FName("Event.Character.Weapon.UnEquip"))
#define WKTAG_EVENT_CHARACTER_ACTION_BLOCKATTACK FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.BlockAttackCheck"))

//Game Control
#define WKTAG_GAME_CONTROL_BLUETEAMPLAYER FGameplayTag::RequestGameplayTag(FName("Game.Control.BlueTeamPlayer"))
#define WKTAG_GAME_CONTROL_REDTEAMPLAYER FGameplayTag::RequestGameplayTag(FName("Game.Control.RedTeamPlayer"))

#define WKTAG_GAME_CONTROL_DOMINATE_BLUETEAM FGameplayTag::RequestGameplayTag(FName("Game.Control.Dominate.BlueTeam"))
#define WKTAG_GAME_CONTROL_DOMINATE_REDTEAM FGameplayTag::RequestGameplayTag(FName("Game.Control.Dominate.RedTeam"))

#define WKTAG_GAME_CONTROL_BLUEGAUGEZERO FGameplayTag::RequestGameplayTag(FName("Game.Control.BlueGaugeZero"))
#define WKTAG_GAME_CONTROL_REDGAUGEZERO FGameplayTag::RequestGameplayTag(FName("Game.Control.RedGaugeZero"))

//Game Team
#define WKTAG_GAME_TEAM_BLUE FGameplayTag::RequestGameplayTag(FName("Game.Team.Blue"))
#define WKTAG_GAME_TEAM_RED FGameplayTag::RequestGameplayTag(FName("Game.Team.Red"))
#define WKTAG_GAME_TEAM_NONE FGameplayTag::RequestGameplayTag(FName("Game.Team.None"))

//Game State
#define WKTAG_GAME_STATE_INPROGRESS FGameplayTag::RequestGameplayTag(FName("Game.State.Inprogress"))
#define WKTAG_GAME_STATE_COOLDOWN FGameplayTag::RequestGameplayTag(FName("Game.State.Cooldown"))