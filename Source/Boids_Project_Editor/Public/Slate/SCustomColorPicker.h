// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"

enum class ECustomColorSliderChannel
{
	None,
	Red,
	Green,
	Blue
};

struct FCustomColorPickerArgs
{
	/** Initial value of the color picker. */
	FLinearColor StartingColor = FLinearColor::White;

	/** Delegate invoked on any color value change. */
	FOnLinearColorValueChanged OnColorValueChanged;
	
	/** Delegate invoked on "Ok" button click. */
	FOnLinearColorValueChanged OnColorCommitted;
	
	/** Delegate invoked on "Cancel" button click. */
	FOnLinearColorValueChanged OnColorCancelled;
};

/**
 * Custom color picker used in BoidEditorUtilityWidget.
 * Allows picking color from color spectrum or RGB sliders.
 */
class BOIDS_PROJECT_EDITOR_API SCustomColorPicker : public SCompoundWidget
{
	
public:
	
	SLATE_BEGIN_ARGS(SCustomColorPicker)
		: _StartingColor(FLinearColor::White),
		_ParentWindow(nullptr),
		_OnColorValueChanged(),
		_OnColorCommitted(),
		_OnColorCancelled()
		{}

		/** Initial value of the color picker. */
		SLATE_ARGUMENT(FLinearColor, StartingColor);
		
		/** Reference to owning parent window. */
		SLATE_ARGUMENT(TSharedPtr<SWindow>, ParentWindow)
		
		/** Delegate invoked on any color value changes. */
		SLATE_EVENT(FOnLinearColorValueChanged, OnColorValueChanged)
		
		/** Delegate invoked on "Ok" button click. */
		SLATE_EVENT(FOnLinearColorValueChanged, OnColorCommitted)
		
		/** Delegate invoked on "Cancel" button click. */
		SLATE_EVENT(FOnLinearColorValueChanged, OnColorCancelled)
		
	SLATE_END_ARGS()

	/** Construct the widget from arguments. */
	void Construct(const FArguments& InArgs);
	
private:

	/** Construct necessary widgets. */
	TSharedRef<SWidget> ConstructColorSlider(ECustomColorSliderChannel Channel);
	TSharedRef<SWidget> ConstructColorSpectrum();
	
	/** Retrieves color channel (RGB) value. */
	float HandleColorSliderValue(ECustomColorSliderChannel Channel) const;
	
	/** Handler function for slider value change. */
	void HandleColorSliderValueChanged(float NewValue, ECustomColorSliderChannel Channel);
	
	/** Retrieves color value in HSV. */
	FLinearColor HandleColorSpectrumValue() const;
	
	/** Handler function for color spectrum value change. */
	void HandleColorSpectrumValueChanged(FLinearColor InColorValueHSV);
	
	/** Retrieves current color value in RGB. */
	FLinearColor HandleColorBlockValue() const;
	
	/** Retrieves starting color value in RGB. */
	FLinearColor HandleOldColorBlockValue() const;
	
	/** Updates CurrentColorHSV from CurrentColorRGB for color spectrum. */
	void UpdateCurrentColorHSV();

	/** Helper that invokes OnColorValueChanged delegate. */
	void HandleColorValueChanged();
	
	/** Ok Button Handler, which closes window and invokes OnColorCommitted. */
	FReply HandleOkButtonClicked();
	
	/** Cancel Button Handler, which closes window and invokes OnColorCancelled. */
	FReply HandleCancelButtonClicked();
	
	
	/** Reference to the parent window for closing it after pressing either "Ok" or "Cancel" buttons. */
	TWeakPtr<SWindow> ParentWindow;

	/** Invoked on color changes. */
	FOnLinearColorValueChanged OnColorValueChanged;
	
	/**  Invoked when pressing the "Ok" button. */
	FOnLinearColorValueChanged OnColorCommitted;
	
	/**  Invoked when pressing the "Canceled" button. */
	FOnLinearColorValueChanged OnColorCancelled;
	
	/** Initial and current color values. */
	FLinearColor StartingColor = FLinearColor::White;
	FLinearColor CurrentColorRGB = FLinearColor::White;
	FLinearColor CurrentColorHSV = FLinearColor::White;
	
	
	/** Constants */
	static const FVector2D DEFAULT_WINDOW_SIZE;
	static const FVector2D ADDITIONAL_WINDOW_SPAWN_PADDING;
	
	static constexpr float COLOR_SPECTRUM_WIDTH = 320.0f;
	static constexpr float COLOR_SPECTRUM_HEIGHT = 200.0f;

	static constexpr float COLOR_BLOCK_HEIGHT = 50.0f;

	static constexpr float BUTTONS_HEIGHT = 20.0f;
	static constexpr float BUTTONS_PADDING = 3.0f;
	
	static constexpr float PADDING = 10.0f;
	
	
public:
	
	/** Main function for calling color picker to spawn at cursor location. */
	static void OpenCustomColorPicker(const FCustomColorPickerArgs& InArgs);

private:
	
	void SetNewParentWindow(const TSharedRef<SWindow>& InParentWindow);
	void SetNewStartingColor(const FLinearColor InColor);
	
	/** Retrieves calculated spawn location taking into consideration its size and padding. */
	static FVector2D CalculateColorPickerConstructionLocation();

	/** Creates color picker and its window. */
	static TSharedRef<SCustomColorPicker> CreateCustomColorPicker(const FCustomColorPickerArgs& InArgs, 
	                                                              const TSharedRef<SWindow>& InWindow);
	static TSharedRef<SWindow> CreateCustomColorPickerWindow(const FVector2D InSpawnLocation);
	
	/** Initializes new starting values. */
	static void SetupNewStartingValues(const FCustomColorPickerArgs& InArgs, const TSharedRef<SWindow>& InWindow);
	
	/** Destroys leftover color picker if there is one. */
	static void TryDestroyOldWindow();
};
