/*
 * This file is part of ResUtil.
 * http://github.com/fmuecke/resutil
 *
 * Copyright (c) 2016-2025 Florian Muecke.
 * All rights reserved.
 */
#include "pch.h"
#include "CppUnitTest.h"
#include "..\ResLib\ResTypes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace ResLib;

namespace ResUtilTest
{
	TEST_CLASS(ResUtilTest)
	{
	public:
		
		TEST_METHOD(ResTypeName_MapsToValidIds)
		{
			wstring w;
			Assert::IsTrue(Types::GetValue(Types::Strings::Bitmap, w) == RT_BITMAP);
		}

		TEST_METHOD(Quoted_ResTypeName_mapsTo_undefined_and_wide_string)
		{
			wstring w;
			Assert::AreEqual(Types::UNDEFINED_TYPE, Types::GetValue("custom type", w));
			Assert::AreEqual(L"custom type", w.c_str());
		}

		TEST_METHOD(ResTypeId_MapsToValidNames)
		{
			Assert::IsTrue(Types::GetName(RT_BITMAP) == Types::Strings::Bitmap);
		}

		TEST_METHOD(Undefined_ResTypeId_MapsTo_EmptyName)
		{
			Assert::IsTrue(Types::GetName(MAKEINTRESOURCE(12345)).empty());
		}

		TEST_METHOD(ParseResIdString_number_to_resId)
		{
			wstring w;
			auto result = Types::ParseResIdString("123", w);
			Assert::IsTrue(MAKEINTRESOURCEW(123) == result);
			Assert::IsTrue(w.empty());
		}

		TEST_METHOD(ParseResIdString_string_to_resId)
		{
			wstring w;
			auto result = Types::ParseResIdString("asd asd", w);
			Assert::AreEqual(L"asd asd", result);
			Assert::AreEqual(L"asd asd", w.c_str());
		}

		TEST_METHOD(GetValue_number_to_resTypeId)
		{
			wstring w;
			auto result = Types::GetValue(Types::Strings::Bitmap, w);
			Assert::IsTrue(RT_BITMAP == result);
			Assert::IsTrue(w.empty());
		}

		TEST_METHOD(GetValue_string_to_resTypeId)
		{
			wstring w;
			auto result = Types::GetValue("asd asd", w);
			Assert::IsTrue(Types::UNDEFINED_TYPE == result);
			Assert::AreEqual(L"asd asd", w.c_str());
		}

		TEST_METHOD(GetName_number_to_resTypeName)
		{
			auto result = Types::GetName(RT_BITMAP);
			Assert::AreEqual(Types::Strings::Bitmap, result.c_str());
		}

		TEST_METHOD(GetName_string_to_resTypeName)
		{
			wstring w{ L"Test Test" };
			auto result = Types::GetName(w.c_str());
			Assert::AreEqual("Test Test", result.c_str());
		}

	};
}
