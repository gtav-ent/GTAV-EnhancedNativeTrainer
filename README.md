# GTAV-EnhancedNativeTrainer

GTA V PC Enhanced Native Trainer, building on Alexander Blade's original sample.
Because of the MSXML parser, this project may now require the Windows SDK: https://www.microsoft.com/en-us/download/details.aspx?id=8279

The VS project currently expects you to have C:\Program Files\Microsoft SDKs\Windows\v6.0A\ and descendents. This may ultimately be unnecessary, I'm not sure.

##Added Features
#### Trainer Options
	* Freeze Wanted Level - Prevents Wanted Stars from disappearing and keeps them at set level.
	* Airbrake - Like with the GTAIV Native Trainer, you can use this to access hard 
	to reach areas. Can be used while in a vehicle. Has two movement speeds.
	* Seatbelt (Bike Only) - Prevents the Player from falling off bike. 
	  Still needs implementation for cars (to prevent shooting out the window).
	* Freeze Weather - Allows you to freeze the weather at a selected weather option
	  from the "Weather" sub-menu.
	* Invisibility - The player model can be made invisible using the option under 
	  the "Player" sub-menu.
#### New Hotkeys
	' - Move Through Door
	] - Never Wanted Enabled/Disabled
	F6 - Activate Airbrake - Allows you to move around the world using "W/S" to Increase/Decrease height,
	"A/D" to rotate movement and "NUM8/NUM5" to move forwards/backwards. "SHIFT" changes speed.
