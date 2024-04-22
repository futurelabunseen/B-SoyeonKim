#pragma once

// Character Action
#define CHARACTER_ACTION_ATTACK FGameplayTag::RequestGameplayTag(FName("Character.Action.Attack"))
#define CHARACTER_ACTION_NEXTATTACK FGameplayTag::RequestGameplayTag(FName("Character.Action.NextAttack"))

// Data
#define DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))

// Character State
#define CHARACTER_STATE_ISDEAD FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))
#define CHARACTER_STATE_INVISIBLE FGameplayTag::RequestGameplayTag(FName("Character.State.Invisible"))

// GameplayCue
#define GC_CHARACTER_ATTACKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.AttackHit"))