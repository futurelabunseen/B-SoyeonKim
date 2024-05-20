#pragma once

template <typename EnumType>
static FORCEINLINE FString GetEnumName(const EnumType InValue)
{
	return StaticEnum<EnumType>()->GetNameStringByValue(static_cast<int64>(InValue));
}

template <typename EnumType>
static FORCEINLINE FString GetEnumDisplayName(const EnumType InValue)
{
	return StaticEnum<EnumType>()->GetDisplayNameTextByValue(static_cast<int64>(InValue)).ToString();
}