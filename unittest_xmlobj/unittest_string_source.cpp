#include "gtest\gtest.h"
#include "..\xmlobj\string_source.h"

TEST(test_StringSource, test_AllMethod) {
	xml::StringSource ss("abcdefg\n123");
	EXPECT_EQ('a', ss.Peek());
	EXPECT_EQ(1, ss.Line());
	EXPECT_EQ(1, ss.Col());

	EXPECT_EQ('a', ss.Next());
	EXPECT_EQ(1, ss.Line());
	EXPECT_EQ(2, ss.Col());

	EXPECT_EQ('b', ss.Peek());
	EXPECT_EQ(1, ss.Line());
	EXPECT_EQ(2, ss.Col());

	EXPECT_EQ('b', ss.Next());
	EXPECT_EQ(1, ss.Line());
	EXPECT_EQ(3, ss.Col());

	ss.PushBack();
	EXPECT_EQ(1, ss.Line());
	EXPECT_EQ(2, ss.Col());

	EXPECT_EQ('b', ss.Peek());
	EXPECT_EQ(1, ss.Line());
	EXPECT_EQ(2, ss.Col());

	EXPECT_EQ(false, ss.Match("bbbb"));
	EXPECT_EQ(1, ss.Line());
	EXPECT_EQ(2, ss.Col());

	EXPECT_EQ(true, ss.Match("bcdef"));
	EXPECT_EQ(1, ss.Line());
	EXPECT_EQ(7, ss.Col());

	EXPECT_EQ('g', ss.Next());
	EXPECT_EQ(1, ss.Line());
	EXPECT_EQ(8, ss.Col());

	EXPECT_EQ('\n', ss.Peek());
	EXPECT_EQ(1, ss.Line());
	EXPECT_EQ(8, ss.Col());

	EXPECT_EQ('\n', ss.Next());
	EXPECT_EQ(2, ss.Line());
	EXPECT_EQ(1, ss.Col());	

	EXPECT_EQ(true, ss.Match("123"));
	EXPECT_EQ(2, ss.Line());
	EXPECT_EQ(4, ss.Col());	

	EXPECT_EQ('\0', ss.Peek());
	EXPECT_EQ('\0', ss.Next());
}

