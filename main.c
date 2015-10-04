//Parts of the Shell
#include "apollo.h"
#include "appdrawer.h"
#include "language.h"
#include "calculator.h"
#include "clock.h"
#include "fm.h"
#include "game.h"
#include "home.h"
#include "lock.h"
#include "main.h"
#include "multi.h"
#include "mp3player.h"
#include "messenger.h"
#include "power_menu.h"
#include "recoverymenu.h"
#include "settingsmenu.h"

#include "screenshot.h"
#include "include/ram.h"
#include "include/utils.h"

PSP_MODULE_INFO("CyanogenPSP",  1, 5, 2);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU); 
PSP_HEAP_SIZE_KB(20*1024); //This line will be altered for slims. Right now us (20480) when building for PSP 1000s, and (53248) for testing on PPSSPP. Using heap_max breaks the browser so don't use it.

int initOSLib() //Initialize OsLib
{
    oslInit(0);
    oslInitGfx(OSL_PF_8888, 1);
	oslSetBilinearFilter(1);
    oslInitAudio();
    oslSetKeyAutorepeatInit(40);
    oslSetKeyAutorepeatInterval(10);
    return 0;
}

// Functions imported from prx:
int imposeGetVolume();
int imposeSetVolume();
int imposeSetBrightness(int value);
int imposeGetBrightness();
 
/* Exit callback */
int exit_callback(int arg1, int arg2, void *common)
 {
	sceKernelExitGame();
	return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp) 
{
	int cbid;
	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void) 
{
	int thid = 0;
	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, THREAD_ATTR_USER, 0);
	if(thid >= 0) sceKernelStartThread(thid, 0, 0);
	return thid;
}

void set_volume(int vol) {
	if(vol > 30)
		vol = 30;
	if(vol < 0)
		vol = 0;
		
	imposeSetVolume(vol);
}

void increase_volume(int n) {
	int v = imposeGetVolume();
	
	set_volume(v+n);
}

void decrease_volume(int n) {
	int v = imposeGetVolume();
	
	set_volume(v-n);
}

//First Boot Message
void firstBootInitiation()
{	
	int firstBoot;

	FILE * firstBootTxt;
	
	if (fileExists("system/firstBoot.txt"))
	{
		firstBootTxt = fopen("system/firstBoot.txt", "r");
		fscanf(firstBootTxt,"%d",&firstBoot);
		fclose(firstBootTxt);
	}
	else
	{
		firstBootTxt = fopen("system/firstBoot.txt", "w");
		fprintf(firstBootTxt, "1");
		fclose(firstBootTxt);
	}
	
	firstBootTxt = fopen("system/firstBoot.txt", "r");
	fscanf(firstBootTxt,"%d",&firstBoot);
	fclose(firstBootTxt);
	
	iconPackLoad();

	//Loads our images into memory
	loadIcons();
	background = oslLoadImageFile(backgroundPath, OSL_IN_RAM, OSL_PF_8888);
	cursor = oslLoadImageFilePNG(cursorPath, OSL_IN_VRAM, OSL_PF_8888);
	navbar2 = oslLoadImageFile("system/home/icons/nav2.png", OSL_IN_RAM, OSL_PF_8888);
	navbar = oslLoadImageFile("system/home/icons/nav.png", OSL_IN_VRAM, OSL_PF_8888);
	notif = oslLoadImageFile("system/home/menu/notif.png", OSL_IN_RAM, OSL_PF_8888);
	pointer = oslLoadImageFilePNG("system/home/icons/pointer.png", OSL_IN_RAM, OSL_PF_8888);
	pointer1 = oslLoadImageFilePNG("system/home/icons/pointer1.png", OSL_IN_RAM, OSL_PF_8888);
	backicon = oslLoadImageFilePNG(backiconPath, OSL_IN_RAM, OSL_PF_8888);
	homeicon = oslLoadImageFilePNG(homeiconPath, OSL_IN_RAM, OSL_PF_8888);
	multicon = oslLoadImageFilePNG(multiconPath, OSL_IN_RAM, OSL_PF_8888);
	backicon2 = oslLoadImageFilePNG(backicon2Path, OSL_IN_RAM, OSL_PF_8888);
	homeicon2 = oslLoadImageFilePNG(homeicon2Path, OSL_IN_RAM, OSL_PF_8888);
	multicon2 = oslLoadImageFilePNG(multicon2Path, OSL_IN_RAM, OSL_PF_8888);
	welcome = oslLoadImageFilePNG("system/home/icons/welcome.png", OSL_IN_RAM, OSL_PF_8888);
	transbackground = oslLoadImageFilePNG("system/home/icons/transbackground.png", OSL_IN_RAM, OSL_PF_8888);
	control = oslLoadImageFilePNG("system/home/menu/brightnesscontrol.png", OSL_IN_VRAM, OSL_PF_8888);
	ic_launcher_apollo = oslLoadImageFilePNG(apolloPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_browser = oslLoadImageFile(browserPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_calculator = oslLoadImageFilePNG(calcPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_filemanager = oslLoadImageFilePNG(fmPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_gallery = oslLoadImageFilePNG(galleryPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_game = oslLoadImageFilePNG(gamePath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_messenger = oslLoadImageFilePNG(messagesPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_settings = oslLoadImageFilePNG(settingsPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_umd = oslLoadImageFilePNG(umdPath, OSL_IN_RAM, OSL_PF_8888);
	layerA = oslLoadImageFilePNG("system/home/icons/layerA.png", OSL_IN_RAM, OSL_PF_8888);
	layerB = oslLoadImageFilePNG("system/home/icons/layerB.png", OSL_IN_RAM, OSL_PF_8888);
	usbdebug = oslLoadImageFilePNG("system/home/icons/usbdebug.png", OSL_IN_RAM, OSL_PF_8888);
	music = oslLoadImageFilePNG("system/home/icons/music.png", OSL_IN_RAM, OSL_PF_8888);
	
	
	//Debugger - Displays an error message if the following resources are missing.
	if (!background || !cursor || !ic_allapps || !ic_allapps_pressed || !navbar || !ic_launcher_apollo || !ic_launcher_settings || !ic_launcher_messenger || !ic_launcher_browser || !notif || !batt100 || !batt80 || !batt60 || !batt40 || !batt20 || !batt10 || !batt0 || !battcharge || !pointer || !pointer1 || !backicon || !multicon || !homeicon || !usbdebug  || !music)
		debugDisplay();
		
	//Sets the cursor's original position on the screen
	cursor->x = 240;
	cursor->y = 136;	
	
	if (firstBoot!= 0)
	{
		fclose(firstBootTxt);
	
		while (!osl_quit)
		{		
			oslStartDrawing();

			controls();

			oslDrawImage(background);		
			oslDrawImageXY(ic_launcher_apollo, 105, 190);
			oslDrawImageXY(ic_launcher_browser, 276, 190);
			oslDrawImageXY(ic_launcher_settings, 331, 190);
			oslDrawImageXY(ic_launcher_messenger, 160, 190);
			oslDrawImageXY(pointer, 230, 180);
			oslSetTransparentColor(RGB(0,0,0));
			appDrawerIcon();
			oslDisableTransparentColor();
			
			oslDrawImageXY(transbackground, 0, 0);
			oslDrawImageXY(welcome, 0, 0);
			
			oslIntraFontSetStyle(Roboto, 0.8f,BLACK,0,INTRAFONT_ALIGN_LEFT);
			oslDrawStringf(20,30, "%s", lang_welcome[language][0]);
			
			oslIntraFontSetStyle(Roboto, 0.6f,DARKGRAY,0,INTRAFONT_ALIGN_LEFT);
			oslDrawStringf(20,60, "%s", lang_welcome[language][1]);
			
			oslDrawStringf(20,80, "%s", lang_welcome[language][2]);
			
			oslIntraFontSetStyle(Roboto, 0.6f,WHITE,0,INTRAFONT_ALIGN_LEFT);
			oslDrawStringf(400,110, "%s", lang_welcome[language][3]);
		
			oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
			
			digitaltime(420,4,0,hrTime);
		
			battery(370,2,1);
			navbarButtons(0);
			
			oslDrawImage(cursor);
	
			if (cursor->x >= 388 && cursor->x <= 464 && cursor->y >= 98 && cursor->y <= 132 && osl_keys->pressed.cross)
			{ 
				firstBootTxt = fopen("system/firstBoot.txt", "w"); 
				fprintf(firstBootTxt, "0");
				fclose(firstBootTxt);
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(welcome);
				oslDeleteImage(transbackground);
				unloadIcons();
				home();
			}
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();	
		}
	}
		
	if (firstBoot == 0)
	{
		oslDeleteImage(welcome);
		oslDeleteImage(transbackground);
		unloadIcons();
		lockscreen();
		home();
	}
}

int bootAnimation()
{
	int currentFrame = 0, i = 0;
	
	FILE * bootAnimActivation;
	
	if (!(fileExists("system/boot/bootAnimActivator.txt")))
	{
		bootAnimActivation = fopen("system/boot/bootAnimActivator.txt", "w");
		fprintf(bootAnimActivation, "0");
		fclose(bootAnimActivation);
	}
	
	bootAnimActivation = fopen("system/boot/bootAnimActivator.txt", "r");
	fscanf(bootAnimActivation,"%d", &bootAnimActivator);
	fclose(bootAnimActivation);
	
	if (bootAnimActivator != 1)
		firstBootInitiation();
	
	bootAnim[0] = oslLoadImageFilePNG("system/boot/part1/boot0.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[1] = oslLoadImageFilePNG("system/boot/part1/boot1.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[2] = oslLoadImageFilePNG("system/boot/part1/boot2.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[3] = oslLoadImageFilePNG("system/boot/part1/boot3.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[4] = oslLoadImageFilePNG("system/boot/part1/boot4.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[5] = oslLoadImageFilePNG("system/boot/part1/boot5.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[6] = oslLoadImageFilePNG("system/boot/part1/boot6.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[7] = oslLoadImageFilePNG("system/boot/part1/boot7.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[8] = oslLoadImageFilePNG("system/boot/part1/boot8.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[9] = oslLoadImageFilePNG("system/boot/part1/boot9.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[10] = oslLoadImageFilePNG("system/boot/part1/boot10.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[11] = oslLoadImageFilePNG("system/boot/part1/boot11.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[12] = oslLoadImageFilePNG("system/boot/part1/boot12.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	
	while(!osl_quit)
	{
		oslStartDrawing();
		oslDrawImageXY(bootAnim[currentFrame], 80, -25); 
		sceDisplayWaitVblankStart();
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
		sceKernelDelayThread(10000 * 2);
   
		currentFrame++;
		if(currentFrame > 12)
		{
			currentFrame = 0;
			i++;
		}
		
		if (i > 21)
		{
			for (i = 0; i <=12; i++)
			{
				oslDeleteImage(bootAnim[i]);
			}
			firstBootInitiation();
		}
    }
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	return 0;
}

int main()
{
	SetupCallbacks(); //Setup callbacks
	initOSLib(); //Initializes OsLib
	oslIntraFontInit(INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8); //Initializes OSL fonts
	
	//Loads our tones
	camera_click = oslLoadSoundFile("system/media/audio/ui/camera_click.wav", OSL_FMT_NONE);
	KeypressStandard = oslLoadSoundFile("system/media/audio/ui/KeypressStandard.wav", OSL_FMT_NONE);
	Lock = oslLoadSoundFile("system/media/audio/ui/Lock.wav", OSL_FMT_NONE);
	Unlock = oslLoadSoundFile("system/media/audio/ui/Unlock.wav", OSL_FMT_NONE);

	FILE * backgroundPathTXT;
	
	if (!(fileExists("system/settings/background.txt")))
	{
		backgroundPathTXT = fopen("system/settings/background.txt", "w");
		fprintf(backgroundPathTXT, "system/framework/framework-res/res/background.png");
		fclose(backgroundPathTXT);
	}
	
	backgroundPathTXT = fopen("system/settings/background.txt", "r");
	fscanf(backgroundPathTXT,"%s",backgroundPath);
	fclose(backgroundPathTXT);
	
	FILE * fontPathTXT;
	
	if (!(fileExists("system/settings/fonts.txt")))
	{
		fontPathTXT = fopen("system/settings/fonts.txt", "w");
		fprintf(fontPathTXT, "system/fonts/Roboto.pgf");
		fclose(fontPathTXT);
	}
	
	fontPathTXT = fopen("system/settings/fonts.txt", "r");
	fscanf(fontPathTXT,"%s",fontPath);
	fclose(fontPathTXT);
	
	FILE * timeSetTxt;
	
	if (!(fileExists("system/app/clock/timeSet.txt")))
	{
		timeSetTxt = fopen("system/app/clock/timeSet.txt", "w");
		fprintf(timeSetTxt, "0");
		fclose(timeSetTxt);
	}
	
	timeSetTxt = fopen("system/app/clock/timeSet.txt", "r");
	fscanf(timeSetTxt,"%d", &hrTime);
	fclose(timeSetTxt);
	
	FILE * languageTxt;
	
	if (!(fileExists("system/settings/language.txt")))
	{
		languageTxt = fopen("system/settings/language.txt", "w");
		fprintf(languageTxt, "0");
		fclose(languageTxt);
	}
	
	languageTxt = fopen("system/settings/language.txt", "r");
	fscanf(languageTxt,"%d", &language);
	fclose(languageTxt);
	
	checkGBootActivation();
	createDirs();
	themesLoad();
	
	quickSettings = oslLoadImageFile(quickSettingsBgPath, OSL_IN_VRAM, OSL_PF_8888);
	batt100 = oslLoadImageFile("system/home/icons/100.png", OSL_IN_VRAM, OSL_PF_8888);
	batt80 = oslLoadImageFile("system/home/icons/80.png", OSL_IN_VRAM, OSL_PF_8888);
	batt60 = oslLoadImageFile("system/home/icons/60.png", OSL_IN_VRAM, OSL_PF_8888);
	batt40 = oslLoadImageFile("system/home/icons/40.png", OSL_IN_VRAM, OSL_PF_8888);
	batt20 = oslLoadImageFile("system/home/icons/20.png", OSL_IN_VRAM, OSL_PF_8888);
	batt10 = oslLoadImageFile("system/home/icons/10.png", OSL_IN_VRAM, OSL_PF_8888);
	batt0 = oslLoadImageFile("system/home/icons/0.png", OSL_IN_VRAM, OSL_PF_8888);
	battcharge = oslLoadImageFile("system/home/icons/charge.png", OSL_IN_VRAM, OSL_PF_8888);
	//volumeBar = oslLoadImageFilePNG("system/volumeBar.png", OSL_IN_RAM, OSL_PF_8888);
	//volumeControl = oslLoadImageFile("system/volumeControl.png", OSL_IN_RAM, OSL_PF_8888);
	
	Roboto = oslLoadIntraFontFile(fontPath, INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	oslSetFont(Roboto); //Load and set font
	
	SceUID modid;//, modid2;
	
	modid = pspSdkLoadStartModule("modules/display.prx", PSP_MEMORY_PARTITION_KERNEL);
	//modid2 = pspSdkLoadStartModule("modules/sound.prx", PSP_MEMORY_PARTITION_KERNEL);
	
	loadConfig();
	
	deleteUpdateFile(); //Delete update.zip
	
	setCpuBoot(); //Set default CPU or load pre-existing value
	
	getPSPNickname(); //Get PSP name before hand
	strcpy (pspname, nickname);
	
	//Main loop to run the program
	while (!osl_quit)
	{		
		bootAnimation();
		firstBootInitiation();
		
		//Ends Printing and Drawing
		oslEndDrawing(); 

		//End's Frame
        oslEndFrame(); 
		
		//Synchronizes the screen 
		oslSyncFrame();	
	}
	
	//Terminates/Ends the program
	oslQuit();
	return 0;
}

