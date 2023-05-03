#pragma once
#include "TileCalculator/TileCalculator.h"

class FAStarSearch
{
public:
	FAStarSearch(FIntVector Start, FIntVector Goal, UTileCalculator* Calculator)
		: Succ(false), Reversed(false), Start(Start), Goal(Goal), Calculator(Calculator)
	{
		Comparer.FCost = &FCost;

		GCost.Add(Start, 0);

		FCost.Add(Start, Calculator->GetDistance(Start, Goal));

		OpenSet.HeapPush(Start, Comparer);
	}

	bool Step()
	{
		if (OpenSet.Num() == 0)
			return false;

		FIntVector Current;
		OpenSet.HeapPop(Current, Comparer);

		if (Current == Goal)
		{
			Succ = true;
			return true;
		}

		CloseSet.Add(Current);

		TArray<FIntVector> Neighbors;
		Calculator->GetNeighbours(Current, Neighbors);
		// TODO use populator to filter out invalid neighbours
		// TODO need someway to pick up vertical neighbours, not sure if it should be in the calculator
		// TODO maybe the neighbours should just come from the populator instead of getting from calculator
		// this is a good start for now though

		for (FIntVector Next : Neighbors)
		{

			// TODO reachability
			// if (CloseSet.Contains(Next) || !(Reversed ? PathFinder->IsReachable(Next, Current) : PathFinder->IsReachable(Current, Next)))
			if (CloseSet.Contains(Next))
				continue;

			int NewCost = GCost[Current] + (Reversed ? Calculator->GetDistance(Next, Current) : Calculator->GetDistance(Current, Next));
			if (!GCost.Contains(Next) || NewCost < GCost[Next])
			{
				GCost.Add(Next, NewCost);

				// TODO custom heuristic
				// FCost.Add(Next, NewCost + (Reversed ? PathFinder->Heuristic(Goal, Next) : PathFinder->Heuristic(Next, Goal)));
				FCost.Add(Next, NewCost + (Reversed ? Calculator->GetDistance(Goal, Next) : Calculator->GetDistance(Next, Goal)));

				if (!OpenSet.Contains(Next))
					OpenSet.HeapPush(Next, Comparer);

				CameFrom.Add(Next, Current);
			}
		}
		return true;
	}

	// UGrid* GetIntersect(const FAStar& R)
	// {
	// 	for (UGrid* Grid : CloseSet)
	// 	{
	// 		if (R.CloseSet.Contains(Grid))
	// 			return Grid;
	// 	}
	//
	// 	for (UGrid* Grid : R.CloseSet)
	// 	{
	// 		if (CloseSet.Contains(Grid))
	// 			return Grid;
	// 	}
	// 	return nullptr;
	// }

	void CollectPath(FIntVector Source, TArray<FIntVector>& Result)
	{
		FIntVector CurrentGrid = Goal;
		Result.AddUnique(CurrentGrid);

		while (CurrentGrid != Source)
		{
			CurrentGrid = CameFrom.FindChecked(CurrentGrid);
			Result.AddUnique(CurrentGrid);
		}
	}

	bool Succ;
private:
	struct FComparer
	{
		bool operator()(const FIntVector& L, const FIntVector& R) const
		{
			int32 LFCost = FCost->Contains(L) ? FCost->FindChecked(L) : TNumericLimits<int32>::Max();
			int32 RFCost = FCost->Contains(R) ? FCost->FindChecked(R) : TNumericLimits<int32>::Max();
			return LFCost < RFCost;
		}
		TMap<FIntVector, int32>* FCost;
	} Comparer;

	bool Reversed;
	TArray<FIntVector> OpenSet;
	TSet<FIntVector> CloseSet;
	TMap<FIntVector, int32> FCost;
	TMap<FIntVector, int32> GCost;
	TMap<FIntVector, FIntVector> CameFrom;
	FIntVector Start;
	FIntVector Goal;
	// UGridPathFinder* PathFinder;
	TWeakObjectPtr<UTileCalculator> Calculator;

	friend class FBidirectionalAStar;
};
