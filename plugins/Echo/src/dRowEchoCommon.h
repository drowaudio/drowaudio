#ifndef DROWECHOCOMMON_H
#define DROWECHOCOMMON_H

/*
 Use this file to confiure the plugin parameters (sliders), buttons and menu options
 It would be preferable to minimise the #defines but using the #defines for the name arrays
 means they can live right next to the enums which need to match except the enums should have a final
 item "Count". These #defined name arrays are assigned to the Names[] array in each nested class
 in the dRowEchoCommon.cpp file. 
 */

// make sure the #defines here match the enums in each of these nested classes

/**
 Some convenient enums and name arrays for paramteres, buttons etc.
 */
class EchoInterface
{
public:
	class Parameters
	{
	public:
		static char* Names[];	
		// these parameter names must be unique
		#define EchoInterfaceParameterNames { "Gain", "Rate", "Depth" }
		enum {
			Gain = 0,
			Rate,
			Depth,
			Count
		};
	};
	
//	class Meters
//	{
//	public:
//		static char* Names[];
//		#define EchoInterfaceMeterNames { "Out L", "Out R" }
//		enum {
//			OutL = 0,
//			OutR,
//			Count
//		};
//	};
	
//	class Buttons
//	{
//	public:
//		static char* Names[];
//		#define EchoInterfaceButtonNames { "Swap", "Centre", "Left", "Right" }
//		enum {
//			Swap = 0,
//			Centre,
//			Left,
//			Right,
//			Count
//		};
//	};
	
//	class MenuOptions
//	{
//	public:
//		static char* Label;
//		static char* Names[];
//		#define EchoInterfaceMenuOptionsLabel "Pan Law"
//		#define EchoInterfaceMenuOptionNames { "-6dB linear pan", "-3dB constant power pan" }
//		enum {
//			LinearPan = 0,		// this is offset by MENU_ID_OFFSET later
//			ConstantPowerPan,
//			Count
//		};
//	};
	
};

/**
 Some convenient enums for audio channels.
 */
class dRowChannels
{
public:
	enum InputChannels {
		InputLeft = 0, InputRight,
		
		Input0 = 0, Input1, Input2, Input3,
		Input4, Input5, Input6, Input7,
		Input8, Input9, Input10, Input11,
		Input12, Input13, Input14, Input15
	};
	
	enum OutputChannels {
		OutputLeft = 0, OutputRight,
		
		Output0 = 0, Output1, Output2, Output3,
		Output4, Output5, Output6, Output7,
		Output8, Output9, Output10, Output11,
		Output12, Output13, Output14, Output15
	};
};

/**
 Some utility functions.
 */
class dRowUtility
{
public:
	/**
	 Removes spaces and unsafe chars from a name for XML storage (for example).
	 */
	static String stringToSafeName(const String& input)
	{
		return input
			.replaceCharacter (' ', '_')
			.replaceCharacter ('.', '_')
			.retainCharacters (T("abcdefghijklmnopqrstuvwxyz_0123456789")); 
	}
	
};

#endif