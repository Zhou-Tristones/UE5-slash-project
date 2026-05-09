#pragma once

#define DRAW_DEBUG_SPHERE(Location) if(GetWorld()) \
	DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, true);

#define DRAW_SPHERE_COLOR(Location, Color) if(GetWorld()) \
	DrawDebugSphere(GetWorld(), Location, 8.f, 12, Color, false, 5.f);

#define DRAW_DEBUG_LINE(StartLocation, EndLocation) if(GetWorld()) \
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 2.f);

#define DRAW_DEBUG_POINT(Location) if(GetWorld()) \
	DrawDebugPoint(GetWorld(), Location, 10.f, FColor::Red, true);

#define DRAW_DEBUG_VECTOR(StartLocation, EndLocation) if(GetWorld()) \
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 2.f); \
	DrawDebugPoint(GetWorld(), EndLocation, 10.f, FColor::Red, true);

#define DRAW_DEBUG_BOX(Location, Extent) if(GetWorld()) \
	DrawDebugBox(GetWorld(), Location, Extent, FColor::Red, true);

#define DRAW_DEBUG_ARROW(StartLocation, EndLocation, Color) if(GetWorld()) \
	DrawDebugDirectionalArrow(GetWorld(), StartLocation, EndLocation, 8.f, Color, false, 5.f);

#define DRAW_DEBUG_SPHERE_SingleFrame(Location) if(GetWorld()) \
	DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, false, -1.f);

#define DRAW_DEBUG_POINT_SingleFrame(Location, Period) if(GetWorld()) \
	DrawDebugPoint(GetWorld(), Location, 10.f, FColor::Red, false, Period);

#define DRAW_DEBUG_VECTOR_SingleFrame(StartLocation, EndLocation) if(GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 2.f); \
		DrawDebugPoint(GetWorld(), EndLocation, 10.f, FColor::Red, false, -1.f); \
	}
