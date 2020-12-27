// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "StealthHUD.h"

#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"


void AStealthHUD::DrawHUD()
{
	Super::DrawHUD();
	
	// Draw very simple crosshair
	if (CrosshairTex)
	{
		// find center of the Canvas
		const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

		// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
		const FVector2D CrosshairDrawPosition(Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f));

		// draw the crosshair
		FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem( TileItem );
	}
}
