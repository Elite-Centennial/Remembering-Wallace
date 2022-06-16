// Elite Centennial

#pragma once

#include "NativeGameplayTags.h"

/**
 * Declare a gameplay tag to be used in C++
 *
 * This should be used in header files. It enables other files that include the header file to use this tag.
 */
#define DECLARE_GAMEPLAY_TAG(TagName) \
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TagName)

/**
 * Define a gameplay tag that was declared in a header file
 *
 * This must be used in non-header files. DevComment must use TEXT("...") macro.
 */
#define DEFINE_GAMEPLAY_TAG(TagName, Tag, DevComment) \
	FNativeGameplayTag TagName( \
		UE_PLUGIN_NAME, UE_MODULE_NAME, Tag, DevComment, ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD); \
	static_assert(UE::GameplayTags::Private::HasFileExtension(__FILE__, ".cpp"), \
		"UE_DEFINE_GAMEPLAY_TAG can only be used in .cpp files, if you're trying to share tags across modules, " \
		"use UE_DECLARE_GAMEPLAY_TAG_EXTERN in the public header, and UE_DEFINE_GAMEPLAY_TAG in the private .cpp");

/**
 * Declare and define a gameplay tag to be used in that file only
 *
 * This must be used in non-header files. DevComment must use TEXT("...") macro.
 */
#define DEFINE_INTERNAL_GAMEPLAY_TAG(TagName, Tag, DevComment) \
	static FNativeGameplayTag TagName( \
		UE_PLUGIN_NAME, UE_MODULE_NAME, Tag, DevComment, ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD); \
	static_assert(UE::GameplayTags::Private::HasFileExtension(__FILE__, ".cpp"), \
		"UE_DEFINE_GAMEPLAY_TAG_STATIC can only be used in .cpp files, if you're trying to share tags across modules," \
		" use UE_DECLARE_GAMEPLAY_TAG_EXTERN in the public header, and UE_DEFINE_GAMEPLAY_TAG in the private .cpp");
