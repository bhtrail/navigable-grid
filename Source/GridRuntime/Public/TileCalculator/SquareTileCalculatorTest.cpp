#include "SquareTileCalculator.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SquareTileCalculatorTest, "GridRuntime.Public.TileCalculator.SquareTileCalculatorTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter)

// TODO this needs to be moved to the private folder
bool SquareTileCalculatorTest::RunTest(const FString& Parameters)
{
	TStrongObjectPtr Calculator(NewObject<USquareTileCalculator>());

	FVector2D Result = Calculator->GridToRelative(FIntVector2(0, 0), 100);

	TestEqual("Calculator should return a half tile at (0,0)", Result, FVector2D(50, 50));
	
	// Make the test pass by returning true, or fail by returning false.
	return true;
}
